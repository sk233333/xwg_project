#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
节点：turtle_circle（Python，本项目主力节点）

它同时干两件事：
  1)每隔 0.1 秒发一次速度指令，让海龟不停地转圈（发布者）
  2)收听海龟的真实位置，打包成自定义消息转发给C++节点（订阅者 + 发布者）
用到的 ROS2 套路：
  发布者 4 步：create_publisher → 造消息 → publish → 定时器周期重复
  订阅者 3 步：create_subscription → 写回调 → spin
"""
import rclpy
from rclpy.node import Node
from geometry_msgs.msg import Twist              # 速度指令（ROS2 自带）
from turtlesim.msg import Pose                   # 海龟位置（turtlesim 自带）
from turtle_interfaces.msg import TurtleStatus   # 自定义状态（我们自己写的）

class TurtleCircle(Node):
    """节点类：必须继承 Node"""

    def __init__(self):
        # 注册节点，名字叫 turtle_circle，把普通 Python 对象注册成 ROS2 节点的动作。
        # 不写这句，后面的 create_publisher、create_timer 全部失效
        # 因为 ROS2 根本不知道有你这个节点。
        super().__init__('turtle_circle')

        # ---------- 参数：运行时可以改，不用动代码 ----------
        #declare_parameter()=告诉 ROS2“我这个节点有一个参数，名字X，默认值Y，类型是Z”
        self.declare_parameter('linear_speed', 1.0)    # 前进速度
        self.declare_parameter('angular_speed', 1.0)   # 转弯速度
        self.declare_parameter('turtle_name', 'turtle1')

        #get_parameter()=告诉 ROS2“我这个节点要用一个参数，名字X，类型是Z”
        #linear_speed_ → 带尾下划线，和 ROS 2 参数名区分（命名约定）,下面用法同这个
        #get_parameter() → 从 ROS2参数系统取 Parameter 对象
        #.value → 从对象里取出真正的数字/字符串/布尔值，如self.linear_speed_ = 1.0
        self.linear_speed_ = self.get_parameter('linear_speed').value
        self.angular_speed_ = self.get_parameter('angular_speed').value
        self.turtle_name_ = self.get_parameter('turtle_name').value

        # ---------- 创建发布者：把速度指令发给海龟 ----------
        #第一个参数Twist是告诉 ROS2：我要发的是 geometry_msgs/msg/Twist 中Twist类型的消息
        #第二个参数'/turtle1/cmd_vel'是告诉ROS2：我要发给这个话题，是乌龟仿真器默认订阅这个
        #第三个参数10是告诉 ROS2：我这个发布者的队列长度是10，防止消息发得太快被丢掉
        self.cmd_pub_ = self.create_publisher(Twist, '/turtle1/cmd_vel', 10)

        # ---------- 创建发布者：把状态发给 C++ 节点 ----------
        #第一个参数TurtleStatus是告诉 ROS2：我要发的是 TurtleStatus 类型的消息
        #第二个参数'/turtle_status'是告诉ROS2：我要发给这个话题，是C++节点默认订阅这个
        #第三个参数10是告诉ROS2：我这个发布者的队列长度是10，防止消息发得太快被丢掉
        self.status_pub_ = self.create_publisher(TurtleStatus, '/turtle_status', 10)

        # ---------- 创建订阅者：收听海龟真实位置 ----------
        #create_subscription()=告诉 ROS2“我要订阅这个话题，收到消息后请调用这个回调函数”
        #第一个参数Pose是告诉ROS2：我要收的是turtlesim/msg/Pose中Pose类型的消息
        #第二个参数'/turtle1/pose'是告诉ROS2：我要收这个话题，是乌龟仿真器默认发布这个
        #第三个参数self.pose_callback是告诉ROS2：收到消息后请调用这个，下面定义了这个函数
        #第四个参数10是告诉ROS2：我这个订阅者的队列长度是10，防止消息来得太快被丢掉
        self.pose_sub_ = self.create_subscription(
            Pose, '/turtle1/pose', self.pose_callback, 10)

        # ---------- 定时器：持续下发速度指令 ----------
        #create_timer()是Node类的一个方法，返回一个Timer对象，定时器会每隔指定时间调用指定回调函数
        #告诉ROS2“请每隔 0.1 秒调用一次这个回调函数，由rclpy.spin()统一调度，到期自动调用你的回调函数”
        #create_timer() 返回一个 rclpy.timer.Timer 对象，它内部持有 C 层面的定时资源。
        #Python 的垃圾回收规则是"没人引用的对象就回收"，如果不存到 self.timer_，
        #Timer 对象引用计数为 0，被 GC 回收后 C 层面的定时器被销毁，spin() 不再调度它，回调永远不触发。
        #存到 self.xxx_ = 手动保持引用 = 告诉 Python"这个对象我还用着，别回收"。
        #self.timer_callback：下面定义了这个函数
        self.timer_ = self.create_timer(0.1, self.timer_callback)

        self.pose_ = None    # 保存最近一次收到的位置
        self.count_ = 0      # 计数器，用来减少日志刷屏
        #self.get_logger().info('消息')=用ROS2的日志系统以INFO级别输出一条带时间戳、带节点名的日志
        #替代 print()，支持级别过滤、远程查看、多节点区分。
        self.get_logger().info('海龟转圈节点已启动！')

    def timer_callback(self):
        """定时器回调：每隔 0.1 秒发一次速度指令"""
        msg = Twist()
        msg.linear.x = self.linear_speed_     # 往前走
        msg.angular.z = self.angular_speed_   # 同时不停转弯
        #发布者：把速度指令发给海龟 ，调用上面发布者指令
        self.cmd_pub_.publish(msg)
        # 一边往前一边拐弯，合起来就是画圆

    def pose_callback(self, msg):
        """订阅回调：收到海龟位置，转发成自定义消息给 C++"""
        #pose_callback 里的 msg 不是你创建的，也不是你调用的。
        #这个函数是创建订阅者时注册的回调，由ROS2在收到/turtle1/pose消息时自动调用塞给你的
        self.pose_ = msg

        status = TurtleStatus()
        status.turtle_name = self.turtle_name_
        status.x = msg.x
        status.y = msg.y
        status.theta = msg.theta
        status.linear_speed = msg.linear_velocity
        status.angular_speed = msg.angular_velocity
        #前面是把 Pose 里的数据搬到 TurtleStatus 里，下面是时间戳
        #Node.get_clock()→拿到Clock →.now()拿到当前Time→.to_msg()转成ROS2标准时间消息
        status.stamp = self.get_clock().now().to_msg()

        self.status_pub_.publish(status)

        # 海龟位置来得非常快（每秒几十次），全打印会刷屏，所以每 30 条打一次
        self.count_ += 1
        if self.count_ % 30 == 0:
            #代替print()
            self.get_logger().info(
                # f-string 格式化字符串，{变量:.2f} 表示浮点数保留两位
                f'已转发 {self.count_} 条状态：x={msg.x:.2f} y={msg.y:.2f}')


def main(args=None):
    """程序入口 + 收尾"""
    rclpy.init(args=args)          # 初始化 ROS2
    node = TurtleCircle()          # 创建节点
    try:
        rclpy.spin(node)           # 持续运转，等待定时器和消息
    except KeyboardInterrupt:
        pass                       # 用户按 Ctrl+C，正常退出
    finally:
        node.get_logger().info('正在停止海龟...') #代替print()
        node.cmd_pub_.publish(Twist())   # 发全零速度 = 让海龟停下来
        node.destroy_node()              # 释放资源
        rclpy.shutdown()                 # 关闭 rclpy


if __name__ == '__main__':
    main()
