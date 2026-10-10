cmake_minimum_required(VERSION 3.8)
#告诉 CMake至少需要 3.8 版本才能正确解析这个 CMakeLists.txt

project(cpp_status_listener)
#工程名定义，必须和 package.xml 里的 <name> 一致

#编译器警告选项，if条件检测当前编译器是不是 GCC或 Clang（Linux下基本都是）,不影响编译成功与否，但帮你抓 bug​
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
  #Wall-打开所有常用警告（没初始化变量、未使用变量等）
  #Wextra-额外警告（空循环体、签名不匹配等）
  #Wpedantic-严格遵循 C++ 标准，拒绝非标准扩展
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)                # rclcpp/rclcpp.hpp
find_package(turtle_interfaces REQUIRED)     # 自定义消息头文件
#find_package()告诉 CMake：去系统里找这些 ROS2包，找到它们的头文件路径、库文件路径、编译选项
#REQUIRED找不到就报错
#ament_cmake ROS 2 C++ 构建系统基础,不直接 include，但必须有
#rclcppC++客户端库, 对应：#include "rclcpp/rclcpp.hpp"
#turtle_interfaces，自定义消息 TurtleStatus对应：#include "turtle_interfaces/msg/turtle_status.hpp"

add_executable(status_listener src/status_listener.cpp)
#add_executable是 CMake的内置命令，作用是"声明把哪些.cpp文件编成一个可执行程序
# 把.cpp编译成可执行文件（第一个参数就是ros2 run的命令名，后面是原代码路径）

ament_target_dependencies(status_listener rclcpp turtle_interfaces)
#ament_target_dependencies(目标名 包1 包2 ...)= 把已经 find_package找到的 ROS2包，一键把头文件+库+传递依赖，挂到我的节点上,自动帮你递归展开所有传递依赖

install(TARGETS status_listener DESTINATION lib/${PROJECT_NAME})
#TARGETS status_listener 安装上面 add_executable 生成的可执行文件
#DESTINATION lib/${PROJECT_NAME} 安装到 install/cpp_status_listener/lib/cpp_status_listener/

ament_package()
#结束声明
