cmake_minimum_required(VERSION 3.8)
project(turtle_interfaces)
#声明 CMake 最低版本和项目名称（必须保留）

find_package(ament_cmake REQUIRED)
find_package(rosidl_default_generators REQUIRED)  # 接口翻译器
find_package(builtin_interfaces REQUIRED)         # 因为 msg 里用了 Time
#find_package：告诉 CMake：“请帮我找到 XXX 这个库/包，并把它的头文件、库文件、CMake 工具都准备好，让我后面能用。
#REQUIRED = 这个包必须找到，找不到就直接报错停止编译，不要继续了。如果不加 只给一个警告，编译继续，后面用到时莫名其妙失败
#ament_cmake：ROS2基于 CMake 封装的一套"构建系统框架"，它给普通 CMake 加了 ROS2专属的能力。
#rosidl_default_generators：消息生成器。它负责读取你的 .msg 文件，自动生成 C++ 头文件和 Python 类（即你之前笔记里的“翻译器”）。
#builtin_interfaces：关键依赖。因为你上一步在 TurtleStatus.msg 最后一行写了 builtin_interfaces/Time stamp，编译时必须找到这个内置时间类型，否则会报错。

rosidl_generate_interfaces(${PROJECT_NAME}
  "msg/TurtleStatus.msg"
  "srv/Patrol.srv"
  DEPENDENCIES builtin_interfaces
)
#这是整个文件的灵魂。每新增一个 .msg 都要在这里登记，上面find_package里 rosidl_default_generators包里的用法。
#"msg/TurtleStatus.msg"：告诉生成器去编译这个文件。
#DEPENDENCIES builtin_interfaces：“声明我用了这个用法，编先确保 builtin_interfaces 的接口代码已经生成完毕，再生成我的消息代码。”

ament_export_dependencies(rosidl_default_runtime)
ament_package()
#ament_export_dependencies：ament_cmake 提供的函数，导出运行时的依赖。意思是：以后别的包（比如你的海龟控制节点）依赖 turtle_interfaces 时，自动连带依赖消息运行时（rosidl_default_runtime）。
#rosidl_default_runtime 是 ament_export_dependencies() 里填的一个“运行时依赖包名，因为你的 turtle_interfaces包生成的是消息，而消息在运行时需要这些（底层库C++消息类： rosidl_runtime_cpp， Python 消息类：rosidl_runtime_py， 底层 C类型支持：rosidl_runtime_c，rosidl_default_runtim，和上面ament_export_dependencies一起使用把
#ament_package()：ROS2构建系统的标准结尾。
