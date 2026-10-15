#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
launch/turtles_patrol.launch.py
────────────────────────────────────────────
作用：一键拉起整个"双海龟"系统，等价于手动开 5 个终端分别 ros2 run

启动顺序（有依赖关系，不能随意调换）：
  ① turtlesim_node     模拟器（必须先起，提供服务 /spawn）
  ② spawn_turtle       生出海龟 turtle2（依赖 ① 的 /spawn 服务）
  ③ patrol_server      巡逻服务端（订阅 pose、发布 cmd_vel）
  ④ patrol_client      C++ 客户端（定时发随机目标点）
  ⑤ turtle_circle      turtle1 转圈

其中 ② 用 TimerAction 延迟 3 秒，保证 ① 的服务已经注册完成。
"""


# ============================================================
# 一、导入区
# ============================================================

# LaunchDescription：launch 文件的"根容器"，
# 所有要启动的节点、延时、事件处理器都装在这个对象里返回。
from launch import LaunchDescription

# TimerAction：延时动作。
# 作用：等 N 秒后再启动某个节点，用来解决"依赖的服务还没准备好"的问题。
from launch.actions import TimerAction

# Node：描述"一个要启动的 ROS2 节点"。
# 对应命令行里的：ros2 run <package> <executable>
from launch_ros.actions import Node


# ============================================================
# 二、固定函数名：generate_launch_description
# ============================================================
# 注意：函数名必须是这个，不能改！
# ros2 launch 会自动调用这个函数，拿到 LaunchDescription 对象。
# 如果改名，launch 会报 "generate_launch_description" not found。

def generate_launch_description():

    # =========================================================
    # 节点 ①：turtlesim 模拟器
    # =========================================================
    # 这是整个系统的"地基"，必须第一个启动。
    # 它起来之后才会提供：
    #   - /spawn 服务（生海龟）
    #   - /turtle1/pose、/turtle2/pose 话题
    #   - /turtle1/cmd_vel、/turtle2/cmd_vel 话题
    turtlesim_node = Node(
        package='turtlesim',         # 包名（ros2 pkg list 里看到的）
        executable='turtlesim_node', # 可执行文件名（包里 entry_point / install 里的名字）
        name='turtlesim',           # 节点名（ros2 node list 里显示的名字，可自定义）
        output='screen',            # 日志打印到屏幕（默认是 log 文件，看不到实时输出）
    )

    # =========================================================
    # 节点 ②：spawn_turtle —— 创建第二只海龟 turtle2
    # =========================================================
    # 依赖关系：必须等 ① 起来，否则 /spawn 服务不存在会失败。
    # 解决办法：不直接放进 LaunchDescription，
    #          而是用 TimerAction 包一层，延迟 3 秒再启动（见下方 return 部分）。
    spawn_turtle = Node(
        package='py_turtle_control',  # 你的 Python 包
        executable='spawn_turtle',    # setup.py 里 entry_points 注册的名字
        name='spawn_turtle',          # 节点名
        output='screen',              # 日志输出到屏幕

        # parameters：给节点传参数，等价于命令行 --ros-args -p x:=2.0
        # 这些参数在节点里用 declare_parameter() 接收。
        # 这里把海龟出生位置写死在 (2, 2)，面向 0 弧度，名字叫 turtle2。
        parameters=[{
            'x': 2.0,       # 出生 x 坐标
            'y': 2.0,       # 出生 y 坐标
            'theta': 0.0,   # 出生朝向（弧度）
            'name': 'turtle2',  # 海龟名字，也是后续话题名 /turtle2/pose 的来源
        }],
    )

    # =========================================================
    # 节点 ③：patrol_server —— 巡逻服务端（核心控制节点）
    # =========================================================
    # 它同时扮演三种角色：
    #   A. 服务端：提供 /patrol 服务，接收"去 (x,y)"的请求
    #   B. 订阅者：订阅 /turtle2/pose 获取海龟实时位置
    #   C. 发布者：发布 /turtle2/cmd_vel 让海龟动起来
    # 控制逻辑：收位置 → 算误差 → 发速度 → 循环，直到距离 < tolerance
    patrol_server = Node(
        package='py_turtle_control',
        executable='patrol_server',
        name='patrol_server',
        output='screen',
        parameters=[{
            'turtle_name': 'turtle2',  # 控制哪只海龟（决定订阅/发布哪个话题）
            'tolerance': 0.2,          # 到达阈值：距离 < 0.2 就算到了
            'max_linear': 2.0,         # 最大前进速度（防止撞墙）
            'max_angular': 2.0,        # 最大角速度（防止转太快）
        }],
    )

    # =========================================================
    # 节点 ④：patrol_client —— C++ 客户端（随机点生成器）
    # =========================================================
    # 行为：每隔 interval 秒，随机生成一个坐标 (x,y)，
    #       然后调用 /patrol 服务，让服务端把海龟送过去。
    # 它自己不动海龟，只负责"下命令"。
    patrol_client = Node(
        package='cpp_status_listener',  # 这个可执行文件在 C++ 包里
        executable='patrol_client',     # CMakeLists.txt 里 add_executable 的名字
        name='patrol_client',
        output='screen',
        parameters=[{
            'interval': 8.0,   # 每隔 8 秒发一次巡逻请求
        }],
    )

    # =========================================================
    # 节点 ⑤：turtle_circle —— turtle1 转圈
    # =========================================================
    # 就是你的第一个项目的节点，让 turtle1 绕圈。
    # 它和 turtle2 的巡逻互不干扰，各自发各自的 cmd_vel。
    turtle_circle = Node(
        package='py_turtle_control',
        executable='turtle_circle',
        name='turtle_circle',
        output='screen',
        # 这里没传参数，说明节点内部用了默认值（比如半径、线速度写死在代码里）
    )

    tf_broadcaster = Node(
    package='py_turtle_control',
    executable='tf_broadcaster',
    name='tf_broadcaster',
    output='screen',
    parameters=[{
        'parent_frame': 'world',
    }],
    )

    delayed_setup = TimerAction(
        period=3.0,
        actions=[spawn_turtle, tf_broadcaster],
    )



    # =========================================================
    # 三、组装 LaunchDescription 并返回
    # =========================================================
    # 列表里的顺序 ≠ 启动顺序！
    # ROS2 launch 会并发启动列表里所有"没有依赖约束"的节点。
    # 有依赖的节点必须用 TimerAction / RegisterEventHandler 显式约束。
    #
    # 这里的关键点：
    #   - turtlesim_node       立即启动（第一个）
    #   - spawn_turtle        被 TimerAction 包住，3 秒后才启动
    #                         这是为了让 /spawn 服务有时间注册完成
    #   - patrol_server       立即启动（它有 wait_for_service 重试保护）
    #   - patrol_client       立即启动（内部也有 wait_for_service 保护）
    #   - turtle_circle       立即启动（和 turtle2 无关）
    return LaunchDescription([

        # ① 立即启动模拟器
        turtlesim_node,

        # ② 延迟 3 秒再启动 spawn
        # TimerAction 的作用：
        #   period    —— 延迟时间（秒），这里给模拟器留足启动时间
        #   actions   —— 延迟后要执行的动作列表，可以放多个 Node
        # 为什么不用 OnProcessStart 事件？
        #   因为 OnProcessStart 触发太早（进程刚 fork，服务还没注册），
        #   用固定延时更稳妥、日志也更干净。
        delayed_setup,
        # ③ 巡逻服务端
        patrol_server,

        # ④ 巡逻客户端
        patrol_client,

        # ⑤ turtle1 转圈
        turtle_circle,
     
        
    ])