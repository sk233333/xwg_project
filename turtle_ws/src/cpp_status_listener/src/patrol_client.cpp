// =====================================================================
// 节点：patrol_client —— C++ 编写的【客户端】
// 干什么：每隔几秒随机生成一个坐标，调用 /patrol 服务让 turtle2 走过去，
//         然后等待服务端响应并打印结果。
//
// C++ 客户端套路（5 步）：
//   第1步 继承 rclcpp::Node
//   第2步 create_client<服务类型>("服务名")
//   第3步 wait_for_service() 等服务端上线
//   第4步 构造 Request、async_send_request() 发送
//   第5步 spin / 等待 future 完成，拿 Response
// =====================================================================

#include <chrono>
#include <cstdlib>                // rand() / srand()
#include <ctime>                  // time()
#include <memory>
#include <random>

#include "rclcpp/rclcpp.hpp"
#include "turtle_interfaces/srv/patrol.hpp"    // 自定义服务头文件
                                               // srv/Patrol.srv → "包名/srv/patrol.hpp"
#include "turtle_interfaces/msg/turtle_status.hpp"

using std::placeholders::_1;
using namespace std::chrono_literals; //允许我用 500ms、1s 这种人类写法写 ROS2 时间参数”
//std::chrono：C++ 标准库里专门处理时间的模块（毫秒、秒、时钟等）。
//chrono_literals：时间字面量（Literal）的专属命名空间。
//using namespace ...：把里面的东西“引入当前作用域”，省得每次都加前缀。


class PatrolClient : public rclcpp::Node
{                                                   // 【1开】类
public:
  PatrolClient()
  : Node("patrol_client")
  {                                                 // 【2开】构造函数
    this->declare_parameter<double>("interval", 8.0);   // 每隔几秒请求一次
    interval_ = this->get_parameter("interval").as_double();

    // 随机数：用当前时间做种子，保证每次运行轨迹不同，srand 是"上发条"，只拧一次；
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // 【第2步】创建客户端,在当前节点（this）中，创建一个服务客户端，专门用来连接名为 /patrol 的服务。
    client_ = this->create_client<turtle_interfaces::srv::Patrol>("/patrol");

    // 顺便订阅一下状态，看看海龟走到哪了
    status_sub_ = this->create_subscription<turtle_interfaces::msg::TurtleStatus>(
      "/turtle_status", 10,
      std::bind(&PatrolClient::status_callback, this, _1));

    // 定时器：每隔 interval_ 秒触发一次巡逻请求
    timer_ = this->create_wall_timer(
      std::chrono::milliseconds(static_cast<int>(interval_ * 1000)),
      std::bind(&PatrolClient::timer_callback, this));

    RCLCPP_INFO(this->get_logger(), "C++ 巡逻客户端已启动，等待服务端...");
  }                                                 // 【2闭】

private:
  // ===================================================================
  // 定时器回调：产生随机点并发起服务请求
  // ===================================================================
  void timer_callback()
  {                                                 // 【3开】
    // 如果上一次请求还没完成，就跳过这次（避免堆积）
    if (waiting_) {
      RCLCPP_INFO(this->get_logger(), "上一次巡逻还没结束，跳过本次请求。");
      return;
    }

    // 【第3步】等服务端上线
    if (!client_->wait_for_service(1s)) {
    //wait_for_service()这是 ROS2 客户端对象内置的方法
      RCLCPP_WARN(this->get_logger(), "/patrol 服务未上线，请先启动 patrol_server！");
      return;
    }

    // ---- 生成随机目标点（ turtlesim 画布是 11x11 ）----
    double x = 1.0 + static_cast<double>(std::rand()) / RAND_MAX * 9.0;
    double y = 1.0 + static_cast<double>(std::rand()) / RAND_MAX * 9.0;

    // 【第4步】构造请求
    auto request = std::make_shared<turtle_interfaces::srv::Patrol::Request>();
    //和python里patrol_callback(self, request, response)用法相似
    request->target_x = x;
    request->target_y = y;

    RCLCPP_INFO(this->get_logger(), "发起巡逻请求：目标 (%.2f, %.2f)", x, y);

    // 异步发送，返回一个 future
    // 第二个参数是"收到响应时的回调函数"（lambda 表达式）
    waiting_ = true;
    auto future = client_->async_send_request(
      request,
      [this](rclcpp::Client<turtle_interfaces::srv::Patrol>::SharedFuture result) {
        // 【第5步】这个 lambda 在收到响应时自动执行
        auto response = result.get();       // 拿到响应对象
        RCLCPP_INFO(this->get_logger(),
          "收到响应：success=%s | %s",
          response->success ? "true" : "false",
          response->message.c_str());
        waiting_ = false;                    // 允许下一次请求
      });
  }                                                 // 【3闭】

  // 订阅回调：看看海龟当前状态
  void status_callback(const turtle_interfaces::msg::TurtleStatus::SharedPtr msg)
  {                                                 // 【4开】
    count_++;
    if (count_ % 40 == 0) {
      RCLCPP_INFO(this->get_logger(),
        "[状态 #%d] %s 位置 x=%.2f y=%.2f 速度 前=%.2f 转=%.2f",
        count_, msg->turtle_name.c_str(),
        msg->x, msg->y,
        msg->linear_speed, msg->angular_speed);
    }
  }                                                 // 【4闭】

  // ===== 成员变量 =====
  rclcpp::Client<turtle_interfaces::srv::Patrol>::SharedPtr client_;
  rclcpp::Subscription<turtle_interfaces::msg::TurtleStatus>::SharedPtr status_sub_;
  rclcpp::TimerBase::SharedPtr timer_;
  bool waiting_ = false;        // 是否有请求在等待响应
  int count_ = 0;
  double interval_ = 8.0;
};                                                  // 【1闭】注意分号

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<PatrolClient>();
  rclcpp::spin(node);                 // 持续运转：定时器 + 服务响应 + 话题
  rclcpp::shutdown();
  return 0;
}
