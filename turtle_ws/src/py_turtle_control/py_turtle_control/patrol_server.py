#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
节点：patrol_server —— 巡逻服务的【服务端】

它同时扮演三种角色：
  1) 服务端：接收 /patrol 请求（客户端给个坐标）
  2) 订阅者：收听 /turtle2/pose（海龟真实位置）
  3) 发布者：发 /turtle2/cmd_vel（速度指令，让海龟动）

闭环控制逻辑（第 3 章 3.3.2 的核心）：
  收位置 → 算误差 → 发速度 → 再看位置 …… 直到到达
"""
import math
import rclpy
from rclpy.node import Node
from geometry_msgs.msg import Twist
from turtlesim.msg import Pose
from turtle_interfaces.srv import Patrol        # 自定义服务
from turtle_interfaces.msg import TurtleStatus  # 自定义状态


class PatrolServer(Node):
    def __init__(self):
        super().__init__('patrol_server')

        # 参数
        self.declare_parameter('turtle_name', 'turtle2')
        self.declare_parameter('tolerance', 0.2)      # 距离小于这个值就算到达
        self.declare_parameter('max_linear', 2.0)     # 最大前进速度
        self.declare_parameter('max_angular', 2.0)    # 最大转弯速度

        self.turtle_name_ = self.get_parameter('turtle_name').value
        self.tolerance_ = self.get_parameter('tolerance').value
        self.max_linear_ = self.get_parameter('max_linear').value
        self.max_angular_ = self.get_parameter('max_angular').value

        # ---- 服务端：接收巡逻请求 ----
        self.service_ = self.create_service(Patrol, '/patrol', self.patrol_callback)

        # ---- 订阅者：收听 turtle2 位置 ----
        self.pose_sub_ = self.create_subscription(
            Pose, f'/{self.turtle_name_}/pose', self.pose_callback, 10)

        # ---- 发布者：给 turtle2 发速度 ----
        self.cmd_pub_ = self.create_publisher(
            Twist, f'/{self.turtle_name_}/cmd_vel', 10)

        # ---- 发布者：发状态（让 C++ 那边能看到）----
        self.status_pub_ = self.create_publisher(TurtleStatus, '/turtle_status', 10)

        # 巡逻状态
        self.pose_ = None            # 最近一次收到的位置
        self.target_x_ = None        # 目标点
        self.target_y_ = None
        self.patrolling_ = False     # 是否正在巡逻
        self.arrived_ = False        # 是否已到达
        self.count_ = 0

        self.get_logger().info(
            f'巡逻服务端已启动，控制 {self.turtle_name_}，等待 /patrol 请求...')

    # =================================================================
    # 服务回调：客户端请求巡逻时被调用
    #   参数：request（客户端填的请求对象）、response（要返回的响应对象）
    #   必须 return response
    # =================================================================
    def patrol_callback(self, request, response):
        self.target_x_ = request.target_x
        self.target_y_ = request.target_y
        self.patrolling_ = True
        self.arrived_ = False

        self.get_logger().info(
            f'收到巡逻请求：目标 ({request.target_x:.2f}, {request.target_y:.2f})')

        # 重要设计：服务回调【立刻返回】，不在这里阻塞等待到达
        # 原因：回调里死等会卡住 spin，海龟就动不了了（死锁）
        # 真正的移动交给 pose_callback 异步完成
        response.success = True
        response.message = f'已接收巡逻任务，正在前往 ({request.target_x:.2f}, {request.target_y:.2f})'
        return response

    # =================================================================
    # 订阅回调：海龟位置来了，做闭环控制
    # =================================================================
    def pose_callback(self, msg):
        self.pose_ = msg

        # 没在巡逻就发零速度待命
        if not self.patrolling_:
            self.cmd_pub_.publish(Twist())
            return

        # ---- 算误差：目标点 - 当前位置 ----
        dx = self.target_x_ - msg.x
        dy = self.target_y_ - msg.y
        distance = math.hypot(dx, dy)      # 直线距离

        cmd = Twist()

        if distance <= self.tolerance_:    # 到达了
            self.patrolling_ = False
            self.arrived_ = True
            self.cmd_pub_.publish(Twist())  # 发零速度刹车
            self.get_logger().info(f'✅ 已到达目标点，误差 {distance:.3f}')
            return

        # ---- P 控制：误差越大，动作越大 ----
        # 线速度：距离越远走得越快，但不超过上限
        cmd.linear.x = min(self.max_linear_, 1.0 * distance)

        # 角速度：先转向目标，再走
        target_angle = math.atan2(dy, dx)              # 目标方向角
        angle_error = target_angle - msg.theta          # 角度误差
        # 把角度误差归一化到 [-π, π]，避免转远路
        angle_error = math.atan2(math.sin(angle_error), math.cos(angle_error))
        cmd.angular.z = max(-self.max_angular_,
                            min(self.max_angular_, 2.0 * angle_error))

        # 角度误差太大时先原地转，别一边转一边撞墙
        if abs(angle_error) > 0.5:
            cmd.linear.x = 0.0

        self.cmd_pub_.publish(cmd)

        # ---- 定期发布状态 ----
        self.count_ += 1
        if self.count_ % 20 == 0:
            status = TurtleStatus()
            status.turtle_name = self.turtle_name_
            status.x = msg.x
            status.y = msg.y
            status.theta = msg.theta
            status.linear_speed = cmd.linear.x
            status.angular_speed = cmd.angular.z
            status.stamp = self.get_clock().now().to_msg()
            self.status_pub_.publish(status)
            self.get_logger().info(
                f'巡逻中：距目标 {distance:.2f}，角度误差 {angle_error:.2f}')


def main(args=None):
    rclpy.init(args=args)
    node = PatrolServer()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.cmd_pub_.publish(Twist())   # 退出前刹车
        node.destroy_node()
        rclpy.shutdown()


if __name__ == '__main__':
    main()
