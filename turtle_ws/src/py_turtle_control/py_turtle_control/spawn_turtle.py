#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
节点：spawn_turtle —— 调用 turtlesim 自带的 /spawn 服务，创建第二只海龟

这是"客户端"角色的最小示例：
  1) 等服务上线
  2) 发请求（带参数：坐标、角度、名字）
  3) 等响应（拿到新海龟的名字）
"""
import rclpy
from rclpy.node import Node
from turtlesim.srv import Spawn      # turtlesim 自带的服务接口


class SpawnTurtle(Node):
    def __init__(self):
        super().__init__('spawn_turtle')

        # 声明参数：新海龟的出生位置和名字
        self.declare_parameter('x', 2.0)
        self.declare_parameter('y', 2.0)
        self.declare_parameter('theta', 0.0)
        self.declare_parameter('name', 'turtle2')

        x = self.get_parameter('x').value
        y = self.get_parameter('y').value
        theta = self.get_parameter('theta').value
        name = self.get_parameter('name').value

        # ---- 创建"客户端"：我要调用别人的服务 ----
        # 参数：服务类型、服务名
        self.client_ = self.create_client(Spawn, '/spawn')

        # ---- 等服务端上线（服务端没启动的话请求会失败）----
        while not self.client_.wait_for_service(timeout_sec=1.0):
            self.get_logger().info('等待 /spawn 服务上线...（请先启动 turtlesim）')

        # ---- 构造请求对象并填字段 ----
        request = Spawn.Request()
        request.x = x
        request.y = y
        request.theta = theta
        request.name = name

        # ---- 发送请求，拿到一个"未来结果" future ----
        self.future_ = self.client_.call_async(request)
        self.get_logger().info(f'已发送创建请求：{name} 在 ({x}, {y})')


def main(args=None):
    rclpy.init(args=args)
    node = SpawnTurtle()

    # ---- 等响应：不断 spin，直到 future 完成 ----
    while rclpy.ok():
        rclpy.spin_once(node)                 # 处理一次通信，让回调跑起来
        if node.future_.done():               # 服务端回应了
            try:
                response = node.future_.result()
                node.get_logger().info(f'✅ 创建成功，海龟名字：{response.name}')
            except Exception as e:
                node.get_logger().error(f'服务调用失败：{e}')
            break

    node.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()
