
---

# turtle_ws —— ROS 2 话题 + 服务 + TF 入门实战

> 配套教材：《ROS 2 机器人开发：从入门到实践》第 3 章「订阅和发布——话题通信探索」、第 4 章「服务与参数」
> 环境：Ubuntu 22.04 + ROS 2 Humble（Jazzy / Iron 用户把命令里的 `humble` 换成你的发行版名）
> 难度：零基础可跑通，每行代码都有中文注释
## 这个项目的前半部分话题的详细解读教程请看这个博客：[xwg_readme_8](https://sk233333.github.io/posts/my-first-post/)
## 后半部分服务+tf工具的详细解读教程请看这个博客：[xwg_readme_9](https://sk233333.github.io/posts/xwg_service_launch/)
---

## 这个仓库在做什么

**前半部分（话题）**：用 Python 节点让 turtle1 转圈，同时把海龟状态打包成自定义消息转发，再用 C++ 节点订阅并打印。
**后半部分（服务 + TF）**：创建 turtle2，用「服务」让它随机巡逻，用 TF 广播让坐标系在 RViz2 里跟着动。

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
        │  · 每 0.1 秒发速度 → 海龟画圆                    │
        │  · 收听位置 → 打包成 TurtleStatus 再发出去        │
        └────────────────────────────────────────────────┘
                             │ /turtle_status（自定义消息）
                             ▼
        ┌────────────────────────────────────────────────┐
        │  cpp_status_listener（C++ 节点）                 │
        │  · 订阅并打印海龟状态                            │
        └────────────────────────────────────────────────┘

        ─ ─ ─ ─ ─ ─ ─ 以下是后半部分（服务 + TF）─ ─ ─ ─ ─ ─ ─

        spawn_turtle ──/spawn──▶ 创建 turtle2
        patrol_client ──/patrol──▶ patrol_server ──/turtle2/cmd_vel──▶ turtle2 走路
        tf_broadcaster ──/tf──▶ RViz2 中显示 world → turtle1 / turtle2 坐标系
```

**核心看点**：Python 节点和 C++ 节点靠着同一份 `.msg` / `.srv` 接口文件无缝对话。

---

## 目录

- [一、覆盖教材哪些知识点](#一覆盖教材哪些知识点)
- [二、目录结构](#二目录结构)
- [三、快速开始](#三快速开始)
- [四、话题部分：海龟转圈与状态转发](#四话题部分海龟转圈与状态转发)
- [五、服务部分：turtle2 随机巡逻](#五服务部分turtle2-随机巡逻)
- [六、TF 部分：让坐标系在 RViz2 里动起来](#六tf-部分让坐标系在-rviz2-里动起来)
- [七、一键拉起：launch 文件](#七一键拉起launch-文件)
- [八、节点参数（不用改代码就能调）](#八节点参数不用改代码就能调)
- [九、调试命令速查](#九调试命令速查)
- [十、消息 / 服务接口定义](#十消息--服务接口定义)
- [十一、Python 与 C++ 写法对照](#十一python-与-c-写法对照)
- [十二、代码套路速记](#十二代码套路速记)
- [十三、常见报错速查](#十三常见报错速查)

---

## 一、覆盖教材哪些知识点

| 教材小节 | 在本项目中的位置 |
| --- | --- |
| 3.2.1 通过话题发布数据 | `timer_callback()` 发布 `Twist` 速度指令 |
| 3.2.2 订阅话题并处理 | `pose_callback()` 收听 `/turtle1/pose` |
| 3.3.1 发布速度控制海龟 | 持续下发 `linear.x` + `angular.z` 画圆 |
| 3.3.2 订阅 + 发布的回路 | 收位置 → 打包 → 再发出去 |
| 3.4.2 自定义通信接口 | `msg/TurtleStatus.msg`，Python 与 C++ 共用 |
| 第 4 章 服务通信 | `srv/Patrol.srv` + `patrol_server` / `patrol_client` |
| 坐标变换 TF | `tf_broadcaster` 广播 `world → turtle` |
| （未涉及）3.4.4 / 3.4.5 Qt | 图形界面属于进阶内容，本仓库未使用 |

---

## 二、目录结构

```
turtle_ws/
├── README.md
├── .gitignore
└── src/
    ├── turtle_interfaces/                 【包1】自定义接口（ament_cmake）
    │   ├── msg/TurtleStatus.msg           ← 话题用的消息
    │   ├── srv/Patrol.srv                 ← 服务用的接口
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
    │       ├── turtle_circle.py           ★ 发布速度 + 转发状态
    │       ├── spawn_turtle.py            ★ 调用 /spawn 创建 turtle2
    │       ├── patrol_server.py           ★ 巡逻服务端（闭环控制）
    │       └── tf_broadcaster.py          ★ TF 广播器
    │
    └── cpp_status_listener/               【包3】C++ 节点（ament_cmake）
        ├── package.xml
        ├── CMakeLists.txt
        └── src/
            ├── status_listener.cpp        ★ 订阅并打印 /turtle_status
            └── patrol_client.cpp          ★ C++ 巡逻客户端（异步）
```

---

## 三、快速开始

### 3.1 环境准备

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

### 3.2 克隆并编译

```bash
cd ~
git clone https://github.com/<你的用户名>/<仓库名>.git turtle_ws
cd turtle_ws

source /opt/ros/humble/setup.bash
colcon build
```

看到 `Summary: 3 packages finished` 即编译成功。

### 3.3 验证接口

```bash
source install/setup.bash
ros2 interface show turtle_interfaces/msg/TurtleStatus    # 应看到 7 个字段
ros2 interface show turtle_interfaces/srv/Patrol          # 应看到请求 + 响应
```

### 3.4 运行（四个终端）

**终端 1 —— 启动海龟模拟器**

```bash
source /opt/ros/humble/setup.bash
ros2 run turtlesim turtlesim_node
```

**终端 2 —— Python 节点（turtle1 开始转圈 + 转发状态）**

```bash
source /opt/ros/humble/setup.bash
source ~/turtle_ws/install/setup.bash
ros2 run py_turtle_control turtle_circle
```

**终端 3 —— C++ 订阅者（打印海龟状态）**

```bash
source /opt/ros/humble/setup.bash
source ~/turtle_ws/install/setup.bash
ros2 run cpp_status_listener status_listener
```

**终端 4 —— 一键拉起服务 + TF（推荐，替代手动起下面三个）**

```bash
source /opt/ros/humble/setup.bash
source ~/turtle_ws/install/setup.bash
ros2 launch py_turtle_control turtles_patrol.launch.py
```

> **每个新终端都要 `source install/setup.bash`。** 想偷懒写进 `~/.bashrc`：
> `echo "source ~/turtle_ws/install/setup.bash" >> ~/.bashrc`

---

## 四、话题部分：海龟转圈与状态转发

`py_turtle_control/turtle_circle.py` 一个节点干两件事：

1. **发布者**：每 0.1 秒向 `/turtle1/cmd_vel` 发 `Twist`，让海龟画圆
2. **订阅者**：收听 `/turtle1/pose`，把位置 + 速度打包成 `TurtleStatus` 发出去

`cpp_status_listener/status_listener.cpp` 订阅 `/turtle_status` 并打印：

```
[C++收到 #30] turtle1 | 时间 1736493038.292527001 | 位置 x=5.53 y=5.48 朝向=1.87 | 速度 前=1.00 转=1.00
```

**关键认知**：海龟不会记住上一次的速度，**速度指令必须持续发**。所以要用定时器周期发送，而不是发一次就完事。

---

## 五、服务部分：turtle2 随机巡逻

> 详细原理讲解见博客：[ROS2 服务+TF 项目详解](https://sk233333.github.io/posts/xwg_service_launch/)

### 5.1 为什么"巡逻到某个点"要用服务

巡逻的需求是：**我告诉你一个目标点，你告诉我到没到**。这种"一问一答、必须有回音"的场景，用服务（Service）最合适；而话题是单向广播，没有返回值。

| | 话题 Topic | 服务 Service |
| --- | --- | --- |
| 通信模式 | 单向广播，多对多 | 一问一答，一对一 |
| 是否有返回 | 无 | 有（Response） |
| 适合场景 | 传感器数据、速度指令流 | 一次性任务：生成、导航、开关 |

### 5.2 服务接口：Patrol.srv

`turtle_interfaces/srv/Patrol.srv`：

```
# --- 以上是请求（客户端 → 服务端）---
float64 target_x        # 目标 x
float64 target_y        # 目标 y
---
# --- 以下是响应（服务端 → 客户端）---
bool success            # 是否成功接受
string message          # 文字说明
```

`---` 是分隔符：**上面是请求，下面是响应**。这也是服务能"有回音"的原因。

### 5.3 四个节点各司其职

| 节点 | 语言 | 角色 |
| --- | --- | --- |
| `spawn_turtle` | Python | 调用 `/spawn` 创建 turtle2 |
| `patrol_server` | Python | 服务端：接收目标点，闭环控制 turtle2 走过去 |
| `patrol_client` | C++ | 客户端：每隔 N 秒生成随机点，调用 `/patrol` |
| `turtle_circle` | Python | 让 turtle1 继续转圈（与服务无关，并行运行） |

### 5.4 ⭐ 最重要的设计：服务回调必须立刻返回

这是整个服务部分的灵魂，也是最容易踩的坑。

`rclpy.spin(node)` 是**单线程**的：它在执行你的回调时，无法同时处理其他回调。

```python
# ❌ 错误写法：在服务回调里死等海龟到达
def handle_patrol(request, response):
    while not reached(request.target_x, request.target_y):
        time.sleep(0.1)      # 海龟永远到不了！
    response.success = True
    return response
```

**为什么会死锁？**

```
服务回调里 while 死等
  → spin 被卡在 service callback 里
  → pose_callback 根本没机会执行
  → 海龟收不到速度指令
  → 永远到不了目标点
  → while 永远不结束  →  死锁
```

**✅ 正确做法**：服务回调只「接任务、立刻回复」，真正的移动交给 `pose_callback` 异步做。

```python
def handle_patrol(self, request, response):
    # 只做两件事：记下目标点 + 立刻回复
    self.target_x_ = request.target_x
    self.target_y_ = request.target_y
    self.is_patrolling_ = True
    response.success = True
    response.message = '已接受巡逻任务'
    return response
    # ← 回调到这里就返回了，spin 恢复，pose_callback 终于能执行

def pose_callback(self, msg):
    if not self.is_patrolling_:
        return
    # 这里才是真正控制海龟走路的地方
    error = math.hypot(self.target_x_ - msg.x, self.target_y_ - msg.y)
    if error < self.tolerance_:
        self.is_patrolling_ = False   # 到了，停
    else:
        self.cmd_pub_.publish(...)     # 没到，继续发速度
```

**一句话记住**：服务回调 = 接单前台（快进快出），`pose_callback` = 后厨（慢慢做）。

### 5.5 C++ 客户端：异步调用与 SharedFuture

`patrol_client.cpp` 用 `async_send_request` 异步发请求，用 `waiting_` 标志位防止请求堆积：

```cpp
// 如果上一次请求还没完成，就跳过（避免堆积）
if (waiting_) {
    RCLCPP_INFO(this->get_logger(), "上一次巡逻还没结束，跳过本次请求。");
    return;
}

waiting_ = true;
client_->async_send_request(
    request,
    [this](rclcpp::Client<turtle_interfaces::srv::Patrol>::SharedFuture result) {
        // 响应到达时自动执行
        auto response = result.get();
        RCLCPP_INFO(this->get_logger(),
            "收到响应：success=%s | %s",
            response->success ? "true" : "false",
            response->message.c_str());
        waiting_ = false;   // 重置标志位，允许下一次请求
    });
```

**`SharedFuture` 去哪了？** `async_send_request` 返回的 `SharedFuture` 有两个去向：

- **返回值**：本代码没接住，直接丢弃
- **lambda 参数 `result`**：ROS2 在响应到达时，把装着响应的 `SharedFuture` 作为参数传入回调

**注意**：漏写 `waiting_ = false` 会导致客户端永久卡死在第一个请求上。

---

## 六、TF 部分：让坐标系在 RViz2 里动起来

> 详细原理讲解见博客：[ROS2 服务+TF 项目详解](https://sk233333.github.io/posts/xwg_service_launch/)

### 6.1 /pose 是数据，/tf 是坐标关系

turtlesim 只发 `/pose`（位置数据），**不会**自动把它声明成坐标系。这个"把 pose 翻译成 TF"的桥接工作要自己写：

```
/pose  ──数值：x=5.5, y=3.2, theta=1.87──▶  tf_broadcaster  ──▶  /tf
                                                                  world
                                                                   ├── turtle1
                                                                   └── turtle2
```

### 6.2 ⭐ 最容易踩的坑：QoS 不匹配导致静默失败

```python
# turtlesim 发布 /pose 用的是 BEST_EFFORT（宁可丢帧也要实时）
# 订阅者默认用 RELIABLE（必须收到每一条）
# 两者不匹配 → 订阅者连不上，但 ROS2 不报错！
sensor_qos = QoSProfile(
    reliability=QoSReliabilityPolicy.BEST_EFFORT,
    history=QoSHistoryPolicy.KEEP_LAST,
    depth=10,
)

self.sub_t1_ = self.create_subscription(
    Pose, '/turtle1/pose',
    lambda msg: self.pose_callback(msg, 'turtle1'),   # lambda 把海龟名字绑进回调
    sensor_qos)
```

**现象**：`ros2 topic echo /turtle1/pose` 能看到数据，但 `/tf` 里永远缺 turtle1，怎么查都查不出问题。

**结论**：**订阅 `/pose` 必须显式指定 `BEST_EFFORT`**。

### 6.3 一个节点广播两只海龟

用 lambda 把海龟名字绑进回调，同一个回调函数靠 `turtle_name` 参数区分是谁的 pose：

```python
# lambda msg: self.pose_callback(msg, 'turtle1')
# 含义：收到消息后调用 pose_callback，并把 'turtle1' 作为第二个参数传进去
self.sub_t1_ = self.create_subscription(
    Pose, '/turtle1/pose',
    lambda msg: self.pose_callback(msg, 'turtle1'), sensor_qos)
self.sub_t2_ = self.create_subscription(
    Pose, '/turtle2/pose',
    lambda msg: self.pose_callback(msg, 'turtle2'), sensor_qos)
```

### 6.4 朝向角转四元数

TF 要求旋转用四元数表示，而 turtlesim 给的是绕 Z 轴转的角 theta：

```python
half = msg.theta / 2.0
t.transform.rotation.x = 0.0
t.transform.rotation.y = 0.0
t.transform.rotation.z = math.sin(half)    # 绕 Z 轴
t.transform.rotation.w = math.cos(half)    # 编码"转了多少"
```

**半角公式**：`q.z = sin(θ/2)`，`q.w = cos(θ/2)`。几何含义——`w` 编码转了多少，`(x,y,z)` 编码绕哪根轴转。

### 6.5 在 RViz2 里看效果

```bash
rviz2
# 然后做三件事：
# 1. Fixed Frame 改为 world
# 2. 左下角 Add → TF
# 3. 勾选 "Show Names"，就能看到 world / turtle1 / turtle2 三根坐标轴跟着海龟动
```

---

## 七、一键拉起：launch 文件

`launch/turtles_patrol.launch.py` 一键拉起 6 个节点，用 `TimerAction` 解决依赖时序问题：

```python
return LaunchDescription([

    turtlesim_node,    # ① 立即启动模拟器（地基）

    # ② + ⑥：3 秒后一起启动
    # spawn 依赖 /spawn 服务，tf 依赖 /pose 话题，都需要等 turtlesim 起来
    TimerAction(period=3.0, actions=[spawn_turtle, tf_broadcaster]),

    patrol_server,     # ③ 巡逻服务端
    patrol_client,     # ④ 巡逻客户端
    turtle_circle,     # ⑤ turtle1 转圈
])
```

**三条铁律**：

1. **每个 Node 变量只能出现一次** —— 重复加入会报 `executed more than once`
2. **列表顺序 ≠ 启动顺序** —— 有依赖的节点必须用 `TimerAction` 显式约束
3. **为什么用 `TimerAction` 而不是事件触发** —— `OnProcessStart` 触发太早（进程刚 fork，服务还没注册），固定延时更稳妥

### 7.1 setup.py 的 data_files：哪行是 launch 加的

```python
data_files=[
    ('share/ament_index/resource_index/packages',
     ['resource/' + package_name]),                    # ← ros2 pkg create 自带

    ('share/' + package_name, ['package.xml']),         # ← ros2 pkg create 自带

    ('share/' + package_name + '/launch',               # ← ⭐ launch 新增
     [os.path.join('launch', f)
      for f in os.listdir('launch')
      if f.endswith('.launch.py')]),
],
```

`data_files` 每一行都是 **`(装到哪, 装什么)`** 的配对。`colcon build` 后文件会被复制到 `install/`：

```
install/py_turtle_control/share/py_turtle_control/
├── package.xml
└── launch/
    └── turtles_patrol.launch.py    ← 第 3 行装的
```

**为什么用列表推导式自动扫描？** 以后在 `launch/` 下新增任何 `xxx.launch.py` 都会被自动装进去，不用改 `setup.py`。（顺便提醒：`data_files` 在整个 `setup.py` 里**只能定义一次**，重复定义会编译报错。）

### 7.2 C++ 包每新增一个节点要加三行

```cmake
# 第二个可执行文件：巡逻客户端（新增）
add_executable(patrol_client src/patrol_client.cpp)
ament_target_dependencies(patrol_client rclcpp turtle_interfaces)
install(TARGETS patrol_client DESTINATION lib/${PROJECT_NAME})
```

| 行 | 作用 |
| --- | --- |
| `add_executable` | 把 `.cpp` 编译成可执行程序 |
| `ament_target_dependencies` | 挂上依赖（头文件 + 库） |
| `install` | 把 exe 复制到 `install/包名/lib/包名/` |

> Python 只要在 `setup.py` 的 `entry_points` 里注册就行，**C++ 必须手动在 `CMakeLists.txt` 里声明**。

---

## 八、节点参数（不用改代码就能调）

### `turtle_circle`

| 参数 | 默认值 | 说明 |
| --- | --- | --- |
| `linear_speed` | `1.0` | 前进速度，越大跑越快 |
| `angular_speed` | `1.0` | 转弯速度，**越小圆越大** |
| `turtle_name` | `turtle1` | 填进状态消息里的名字 |

```bash
ros2 run py_turtle_control turtle_circle --ros-args -p angular_speed:=0.3   # 大圆
ros2 run py_turtle_control turtle_circle --ros-args -p angular_speed:=2.5   # 小圆
ros2 run py_turtle_control turtle_circle --ros-args -p linear_speed:=3.0    # 跑快点
```

### `status_listener`

| 参数 | 默认值 | 说明 |
| --- | --- | --- |
| `print_every` | `30` | 每收到几条打印一次（防刷屏） |

```bash
ros2 run cpp_status_listener status_listener --ros-args -p print_every:=5
```

### `patrol_server`

| 参数 | 默认值 | 说明 |
| --- | --- | --- |
| `turtle_name` | `turtle2` | 控制哪只海龟 |
| `tolerance` | `0.2` | 距离小于此值算到达 |
| `max_linear` | `2.0` | 最大前进速度（防撞墙） |
| `max_angular` | `2.0` | 最大角速度（防转太快） |

### `patrol_client`

| 参数 | 默认值 | 说明 |
| --- | --- | --- |
| `interval` | `8.0` | 每隔几秒发一次巡逻请求 |

### `tf_broadcaster`

| 参数 | 默认值 | 说明 |
| --- | --- | --- |
| `parent_frame` | `world` | TF 树的根帧名字 |

---

## 九、调试命令速查

```bash
ros2 node list                  # 应看到 turtlesim / turtle_circle / status_listener / patrol_server / patrol_client / tf_broadcaster
ros2 topic list                 # 应看到 /turtle1/cmd_vel、/turtle1/pose、/turtle_status、/turtle2/cmd_vel、/turtle2/pose、/tf
ros2 topic echo /turtle_status  # 实时看自定义消息内容
ros2 topic echo /tf             # 看广播出去的坐标变换
ros2 topic hz  /turtle_status   # 发布频率
ros2 topic info /turtle_status -v   # 看发布者/订阅者分别是谁
ros2 interface show turtle_interfaces/msg/TurtleStatus
ros2 interface show turtle_interfaces/srv/Patrol
ros2 service list               # 看有哪些服务（应有 /patrol、/spawn）
ros2 service call /spawn turtlesim/srv/Spawn "{x: 2.0, y: 2.0, theta: 0.0, name: 'turtle2'}"
rqt_graph                       # 图形化看节点与话题连线，强烈推荐
```

---

## 十、消息 / 服务接口定义

### TurtleStatus.msg

```
string turtle_name                  # 海龟名字
float64 x                           # x 坐标
float64 y                           # y 坐标
float64 theta                       # 朝向（弧度）
float64 linear_speed                # 当前前进速度
float64 angular_speed               # 当前转弯速度
builtin_interfaces/Time stamp       # 时间戳（sec + nanosec）
```

**为什么自己定义？** turtlesim 自带的 `Pose` 里没有"名字"字段。真实项目里现成接口不够用是常态，自己造一个就是 3.4.2 的全部意义。

### Patrol.srv

```
float64 target_x
float64 target_y
---
bool success
string message
```

同一份 `.msg` / `.srv` 会被 `rosidl` 翻译成两套代码：

```
msg/TurtleStatus.msg  ──▶  turtle_status.hpp  +  _turtle_status.py
srv/Patrol.srv        ──▶  patrol.hpp        +  _patrol.py
```

可以用这两条命令亲眼确认：

```bash
find install -name "turtle_status.hpp"
find install -name "_turtle_status.py"
```

---

## 十一、Python 与 C++ 写法对照

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

## 十二、代码套路速记

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

## 十三、常见报错速查

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
| **TF 里缺 turtle1，但不报错** | 订阅 `/pose` 用了默认 RELIABLE，与发布者 BEST_EFFORT 不匹配 | 显式指定 `BEST_EFFORT` QoS |
| **launch 报 `executed more than once`** | 同一个 Node 变量在 LaunchDescription 里出现了两次 | 每个 Node 变量只出现一次，复用就包进 TimerAction |
| **客户端卡死在第一个请求** | 漏写 `waiting_ = false` | 在响应回调里重置标志位 |
| **服务回调里 while 死等导致海龟不动** | 单线程 spin 被卡住，pose_callback 无法执行 | 服务回调只接任务立刻返回，移动交给 pose_callback |
| **colcon build 报 `data_files` 重复** | `setup.py` 里定义了两个 `data_files` | 合并成一个 |

**排查原则**：只看**第一条 `error:`**，下面的 `gmake ... Error 1/2` 是结果不是原因。

---

## 十四、话题 vs 服务，到底怎么选

| 场景 | 用话题 | 用服务 |
| --- | --- | --- |
| 传感器数据流 | ✅ 持续、高频 | ❌ |
| 速度指令流 | ✅ 持续下发 | ❌ |
| 生成/销毁对象 | ❌ | ✅ `/spawn` |
| 导航到某点 | ❌ | ✅ `/patrol` |
| 需要返回结果 | ❌ | ✅ 有 Response |
| 需要实时反馈进度 | ⚠️ 用话题另开一路 | 用 Action（进阶） |

---

## License

Apache-2.0
