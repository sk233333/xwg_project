# turtle_ws —— ROS 2 话题通信入门实战（Python 发布 + C++ 订阅）

> 配套教材：《ROS 2 机器人开发：从入门到实践》第 3 章「订阅和发布——话题通信探索」
> 环境：Ubuntu 22.04 + ROS 2 Humble（Jazzy / Iron 用户把命令里的 `humble` 换成你的发行版名）
> 难度：零基础可跑通，每行代码都有中文注释

---

## 一、这个项目在做什么

一句话：**用 Python 节点让海龟转圈，同时把海龟状态打包成自定义消息转发出来，再用 C++ 节点订阅并显示。**

```
                        ┌────────────────────────┐
                        │  turtlesim（官方模拟器） │
                        └────────────────────────┘
                             ▲              │
             /turtle1/cmd_vel │              │ /turtle1/pose
              （下发速度指令）  │              │  （回传真实位置）
                             │              ▼
        ┌────────────────────────────────────────────────┐
        │  py_turtle_control（Python 节点）                │
        │  · 每 0.1 秒发一次速度 → 海龟画圆                │
        │  · 收听海龟位置 → 打包成 TurtleStatus 再发出去    │
        └────────────────────────────────────────────────┘
                             │ /turtle_status（自定义消息）
                             ▼
        ┌────────────────────────────────────────────────┐
        │  cpp_status_listener（C++ 节点）                 │
        │  · 订阅并打印海龟状态                            │
        └────────────────────────────────────────────────┘
```

**核心看点**：Python 写的节点和 C++ 写的节点，靠着同一份 `.msg` 接口文件无缝对话。

---

## 二、覆盖了教材哪些知识点

| 教材小节 | 在本项目中的位置 |
| --- | --- |
| 3.2.1 通过话题发布数据 | `timer_callback()` 发布 `Twist` 速度指令 |
| 3.2.2 订阅话题并处理 | `pose_callback()` 收听 `/turtle1/pose` |
| 3.3.1 发布速度控制海龟 | 持续下发 `linear.x` + `angular.z` 画圆 |
| 3.3.2 订阅 + 发布的回路 | 收位置 → 打包 → 再发出去 |
| 3.4.2 自定义通信接口 | `msg/TurtleStatus.msg`，Python 与 C++ 共用 |
| （未涉及）3.4.4 / 3.4.5 Qt | 图形界面属于进阶内容，本仓库未使用 |

---

## 三、目录结构

```
turtle_ws/
├── README.md
├── .gitignore
└── src/
    ├── turtle_interfaces/                 【包1】自定义接口（ament_cmake）
    │   ├── msg/TurtleStatus.msg           ← 消息格式定义，全项目的数据契约
    │   ├── CMakeLists.txt
    │   └── package.xml
    │
    ├── py_turtle_control/                 【包2】Python 控制节点（ament_python）
    │   ├── package.xml
    │   ├── setup.py                       ← entry_points 注册终端命令
    │   ├── setup.cfg
    │   ├── resource/py_turtle_control
    │   └── py_turtle_control/
    │       ├── __init__.py
    │       └── turtle_circle.py           ★ 主力节点：发布速度 + 转发状态
    │
    └── cpp_status_listener/               【包3】C++ 订阅节点（ament_cmake）
        ├── package.xml
        ├── CMakeLists.txt
        └── src/status_listener.cpp        ★ 订阅并打印 /turtle_status
```

---

## 四、快速开始

### 4.1 环境准备

```bash
# 确认 ROS 2 版本
ros2 --version
echo $ROS_DISTRO          # 记住输出，后面命令里的 humble 换成它

# 安装编译工具（已装可跳过）
sudo apt install python3-colcon-common-extensions -y

# 确认海龟模拟器存在
ros2 pkg list | grep turtlesim
# 没有就装：sudo apt install ros-humble-turtlesim -y
```

### 4.2 克隆并编译

```bash
cd ~
git clone https://github.com/<你的用户名>/<仓库名>.git turtle_ws
cd turtle_ws

source /opt/ros/humble/setup.bash
colcon build
```

看到 `Summary: 3 packages finished` 即编译成功。

### 4.3 验证自定义接口

```bash
source install/setup.bash
ros2 interface show turtle_interfaces/msg/TurtleStatus
```

能看到 7 个字段，说明接口生成成功。

### 4.4 运行（三个终端）

**终端 1 —— 启动海龟模拟器**

```bash
source /opt/ros/humble/setup.bash
ros2 run turtlesim turtlesim_node
```

**终端 2 —— 启动 Python 节点（海龟开始转圈）**

```bash
source /opt/ros/humble/setup.bash
source ~/turtle_ws/install/setup.bash
ros2 run py_turtle_control turtle_circle
```

**终端 3 —— 启动 C++ 订阅者**

```bash
source /opt/ros/humble/setup.bash
source ~/turtle_ws/install/setup.bash
ros2 run cpp_status_listener status_listener
```

终端 3 会周期性打印：

```
[C++收到 #30] turtle1 | 时间 1736493038.292527001 | 位置 x=5.53 y=5.48 朝向=1.87 | 速度 前=1.00 转=1.00
```

> **每个新终端都要 `source install/setup.bash`。** 想偷懒可以写进 `~/.bashrc`：
> `echo "source ~/turtle_ws/install/setup.bash" >> ~/.bashrc`

---

## 五、节点参数（不用改代码就能调）

### `turtle_circle`

| 参数 | 默认值 | 说明 |
| --- | --- | --- |
| `linear_speed` | `1.0` | 前进速度，越大跑越快 |
| `angular_speed` | `1.0` | 转弯速度，**越小圆越大** |
| `turtle_name` | `turtle1` | 填进状态消息里的名字 |

```bash
# 画大圆
ros2 run py_turtle_control turtle_circle --ros-args -p angular_speed:=0.3

# 画小圆
ros2 run py_turtle_control turtle_circle --ros-args -p angular_speed:=2.5

# 跑快点
ros2 run py_turtle_control turtle_circle --ros-args -p linear_speed:=3.0
```

### `status_listener`

| 参数 | 默认值 | 说明 |
| --- | --- | --- |
| `print_every` | `30` | 每收到几条打印一次（消息每秒几十条，防止刷屏） |

```bash
ros2 run cpp_status_listener status_listener --ros-args -p print_every:=5
```

---

## 六、调试命令（都试一遍，理解会深很多）

```bash
ros2 node list                  # 应看到 turtlesim / turtle_circle / status_listener
ros2 topic list                 # 应看到 /turtle1/cmd_vel、/turtle1/pose、/turtle_status
ros2 topic echo /turtle_status  # 实时看自定义消息内容
ros2 topic hz  /turtle_status   # 发布频率
ros2 topic info /turtle_status -v   # 看发布者/订阅者分别是谁
ros2 interface show turtle_interfaces/msg/TurtleStatus
rqt_graph                       # 图形化看节点与话题连线，强烈推荐
```

---

## 七、消息接口定义

`msg/TurtleStatus.msg`：

```
string turtle_name                  # 海龟名字
float64 x                           # x 坐标
float64 y                           # y 坐标
float64 theta                       # 朝向（弧度）
float64 linear_speed                # 当前前进速度
float64 angular_speed               # 当前转弯速度
builtin_interfaces/Time stamp       # 时间戳（sec + nanosec）
```

**为什么自己定义？** 因为 turtlesim 自带的 `Pose` 里没有"名字"字段。真实项目里现成接口不够用是常态，自己造一个就是 3.4.2 的全部意义。

同一份 `.msg` 会被 `rosidl` 翻译成两套代码：

```
msg/TurtleStatus.msg
   ├─→ install/.../turtle_interfaces/msg/turtle_status.hpp   （C++）
   └─→ install/.../turtle_interfaces/msg/_turtle_status.py   （Python）
```

可以用这两条命令亲眼确认：

```bash
find install -name "turtle_status.hpp"
find install -name "_turtle_status.py"
```

---

## 八、Python 与 C++ 写法对照

| 你要做的事 | Python (rclpy) | C++ (rclcpp) |
| --- | --- | --- |
| 引入库 | `import rclpy` | `#include "rclcpp/rclcpp.hpp"` |
| 导入自定义消息 | `from turtle_interfaces.msg import TurtleStatus` | `#include "turtle_interfaces/msg/turtle_status.hpp"` |
| 定义节点 | `class X(Node)` | `class X : public rclcpp::Node` |
| 建发布者 | `create_publisher(类型, 名, 10)` | `create_publisher<类型>("名", 10)` |
| 建订阅者 | `create_subscription(类型, 名, 回调, 10)` | `create_subscription<类型>("名", 10, std::bind(...))` |
| 建定时器 | `create_timer(0.1, 回调)` | `create_wall_timer(100ms, 回调)` |
| 打印日志 | `self.get_logger().info(...)` | `RCLCPP_INFO(this->get_logger(), ...)` |
| 保持运转 | `rclpy.spin(node)` | `rclcpp::spin(node)` |
| 注册可执行 | `setup.py` 的 `entry_points` | `CMakeLists.txt` 的 `add_executable` + `install` |

---

## 九、代码套路速记

**发布者 4 步**

```python
self.pub_ = self.create_publisher(Twist, '/turtle1/cmd_vel', 10)  # ① 建发布者
msg = Twist(); msg.linear.x = 1.0                                  # ② 造消息填字段
self.pub_.publish(msg)                                             # ③ 发布
self.timer_ = self.create_timer(0.1, self.timer_callback)          # ④ 定时器周期重复
rclpy.spin(node)                                                   #    没有 spin 会立刻退出
```

**订阅者 3 步**

```cpp
subscription_ = this->create_subscription<Msg>(                    // ① 建订阅者
    "/turtle_status", 10, std::bind(&X::callback, this, _1));
void callback(const Msg::SharedPtr msg) { ... }                    // ② 写回调
rclcpp::spin(node);                                                // ③ spin
```

**两个必须记住的坑**

1. **回调传函数本身，不加括号**。`self.pose_callback` ✅ / `self.pose_callback()` ❌
2. **速度指令要持续发**。海龟不会记住上一次的速度，停发它就停。`turtle_circle` 用 10Hz 定时器一直发，正是这个原因。

---

## 十、常见报错速查

| 现象 | 原因 | 解决 |
| --- | --- | --- |
| `Package 'xxx' not found` | 没 source，或该包没编译进 install | `source install/setup.bash`；`ls install` 看有没有这个包 |
| `ModuleNotFoundError: turtle_interfaces` | 接口包没编译成功 | `colcon build --packages-select turtle_interfaces` |
| `No such file: turtle_interfaces/msg/turtle_status.hpp` | CMakeLists / package.xml 漏了依赖 | 两个文件都要写 `turtle_interfaces` |
| `ros2 interface show` 找不到接口 | 漏了 `<member_of_group>rosidl_interface_packages</member_of_group>` | 补上后重新编译 |
| 改了 `setup.py` 但命令没变 | 没重新编译 | 改 `setup.py` 必须 `colcon build` |
| C++ 报 `expected ';' after class definition` | 类的结尾写成 `}` 了 | 必须是 `};` |
| C++ 报一堆 `was not declared in this scope` | 某个函数少写了 `}`，成员变量掉进函数里 | 检查括号配对：`grep -o '{' f \| wc -l` 应等于 `grep -o '}' f \| wc -l` |
| 海龟不动 | 节点没启动或话题名错 | `ros2 topic echo /turtle1/cmd_vel` 看有没有数据 |
| `not found: .../xxx/local_setup.bash` | 该包编译失败，没进 install | 单独编该包看真实报错 |

排查时**只看第一条 `error:`**，下面的 `gmake ... Error 1/2` 是结果不是原因。

---

## 十一、License

Apache-2.0
