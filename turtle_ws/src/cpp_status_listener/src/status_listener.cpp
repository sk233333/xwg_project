// =====================================================================
// 节点：status_listener —— C++ 编写的【订阅者】
// 只干一件事：收听 /turtle_status，把收到的内容打印出来。
//
// 它收到的消息是 Python 节点发来的，而消息格式由 .msg 文件定义，
// 所以 Python 和 C++ 虽然语言不同，却能无缝对话 —— 这就是 ROS2 的魅力。
//
// C++ 订阅者套路（4 步）：
//   第1步 继承 rclcpp::Node
//   第2步 create_subscription<消息类型>("话题名", 队列长度, 回调)
//   第3步 写回调函数
//   第4步 main 里 rclcpp::spin(节点)
// =====================================================================

#include <memory>                 // std::make_shared 智能指针
#include <string>                 // std::string
#include "rclcpp/rclcpp.hpp"      // ROS2 的 C++ 库，等于 Python 里的 rclpy
                                  // msg/TurtleStatus.msg → "包名/msg/turtle_status.hpp"
#include "turtle_interfaces/msg/turtle_status.hpp"

using std::placeholders::_1;
//C++ 标准库早就定义了 std::placeholders::_1
//后面你写 _1，编译器自动替换成全名，_1 就是 std::bind 的"占位坑"，标记"调用者传的第一个参数放这里"。
//它不存数据、不运行、不阻塞，纯粹是绑定阶段的标记。消息到了，ROS2往_1那个坑里一塞，你的回调就拿到了msg。

// 【第1步】继承 rclcpp::Node
class StatusListener : public rclcpp::Node
{
public:
  StatusListener()
  : Node("status_listener")       // 节点名，不能和 Python 节点重名
  {
    // 参数：每收到几条打印一次（防止刷屏）
    this->declare_parameter<int>("print_every", 30);
    print_every_ = this->get_parameter("print_every").as_int();
    // print_every_在private里定义了，默认值是30，用户可以在启动节点时通过参数覆盖它。
    //as_int()是ROS2里rclcpp::Parameter类的一个成员函数，专门用来把参数值转成int类型

    // 【第2步】创建订阅者
    // 注意：C++ 的成员函数不能直接当回调，必须用 std::bind 包一层：
    //   std::bind(&类名::函数名, this, _1)
    //   _1 是占位符，表示"将来收到的那条消息"
    subscription_ = this->create_subscription<turtle_interfaces::msg::TurtleStatus>(
      "/turtle_status", 10,
      std::bind(&StatusListener::status_callback, this, _1));
      //python版订阅：self.subscription = self.create_subscription(
      //  TurtleStatus,turtle_status',self.status_callback,10 )

    RCLCPP_INFO(this->get_logger(), "C++ 订阅者已启动，正在收听 /turtle_status ...");
  }
  //这行是 ROS 2 的日志输出（打日志），相当于你 Python 里的 self.get_logger().info(...)

private:
  // 【第3步】回调函数：每收到一条消息，ROS2 自动调用一次
  // ::SharedPtr 是 ROS2 推荐写法（智能指针，自动管内存，不用手动 delete）
  //msg和python里pose接口那说过的一样，这个msg是接收到的消息，不是自己创建的
  void status_callback(const turtle_interfaces::msg::TurtleStatus::SharedPtr msg)
  {
    count_++;

    // 每 print_every_ 条打印一次，避免刷屏
    if (count_ % print_every_ != 0) {
      return;
    }

    // %s 对应字符串，%.2f 对应保留两位小数的浮点数
    // C++ 的 std::string 传给 %s 必须加 .c_str()
    // %d 对应整数，%09u 表示不足 9 位前面补 0（纳秒是 9 位数）
    RCLCPP_INFO(this->get_logger(),
      "[C++收到 #%d] %s | 时间 %d.%09u | 位置 x=%.2f y=%.2f 朝向=%.2f | 速度 前=%.2f 转=%.2f",
      count_, msg->turtle_name.c_str(),
      msg->stamp.sec, msg->stamp.nanosec,
      msg->x, msg->y, msg->theta,
      msg->linear_speed, msg->angular_speed);
  }
  // ===== 成员变量 =====
  rclcpp::Subscription<turtle_interfaces::msg::TurtleStatus>::SharedPtr subscription_;
  //rclcpp::Subscription是ROS2里订阅者的类模板，<>里是消息类型
  //turtle_interfaces::msg::TurtleStatus=turtle_interfaces包的msg子空间里的TurtleStatus类
  //SharedPtr 是智能指针，自动管内存，不用手动 delete。

  int count_ = 0;          // 收到多少条
  int print_every_ = 30;   // 每几条打印一次
};

// 【第4步】main 函数
int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);                          // 1) 初始化 ROS2
  auto node = std::make_shared<StatusListener>();    // 2) 创建节点
  rclcpp::spin(node);                                // 3) 持续运转，等待消息
  rclcpp::shutdown();                                // 4) Ctrl+C 后收尾
  return 0;
}
