import math
import rclpy
from rclpy.node import Node
from rclpy.qos import QoSProfile, QoSReliabilityPolicy, QoSHistoryPolicy  # ← 新增
from turtlesim.msg import Pose
from tf2_ros import TransformBroadcaster
from geometry_msgs.msg import TransformStamped


class TFBroadcaster(Node):
    def __init__(self):
        super().__init__('tf_broadcaster')

        self.declare_parameter('parent_frame', 'world')
        self.parent_frame_ = self.get_parameter('parent_frame').value

        self.br_ = TransformBroadcaster(self)

        # ★ 关键修复：用传感器级 QoS，和 turtlesim 的 /pose 匹配
        sensor_qos = QoSProfile(
            reliability=QoSReliabilityPolicy.BEST_EFFORT,   # turtlesim 用这个
            history=QoSHistoryPolicy.KEEP_LAST,
            depth=10,
        )

        # 两个订阅都用 sensor_qos，不要用默认的 10
        self.sub_t1_ = self.create_subscription(
            Pose, '/turtle1/pose',
            lambda msg: self.pose_callback(msg, 'turtle1'),
            sensor_qos)     # ← 改这里

        self.sub_t2_ = self.create_subscription(
            Pose, '/turtle2/pose',
            lambda msg: self.pose_callback(msg, 'turtle2'),
            sensor_qos)     # ← 改这里

        self.get_logger().info(
            f'TF 广播器已启动：{self.parent_frame_} -> turtle1 / turtle2')


    def pose_callback(self, msg: Pose, turtle_name: str):
        t = TransformStamped()
        t.header.stamp = self.get_clock().now().to_msg()
        t.header.frame_id = self.parent_frame_
        t.child_frame_id = turtle_name

        t.transform.translation.x = msg.x
        t.transform.translation.y = msg.y
        t.transform.translation.z = 0.0

        half = msg.theta / 2.0
        t.transform.rotation.x = 0.0
        t.transform.rotation.y = 0.0
        t.transform.rotation.z = math.sin(half)
        t.transform.rotation.w = math.cos(half)

        self.br_.sendTransform(t)


def main():
    rclpy.init()
    node = TFBroadcaster()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()