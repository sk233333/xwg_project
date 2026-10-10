// NOLINT: This file starts with a BOM since it contain non-ASCII characters
// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from turtle_interfaces:msg/TurtleStatus.idl
// generated code does not contain a copyright notice

#ifndef TURTLE_INTERFACES__MSG__DETAIL__TURTLE_STATUS__STRUCT_H_
#define TURTLE_INTERFACES__MSG__DETAIL__TURTLE_STATUS__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'turtle_name'
#include "rosidl_runtime_c/string.h"
// Member 'stamp'
#include "builtin_interfaces/msg/detail/time__struct.h"

/// Struct defined in msg/TurtleStatus in the package turtle_interfaces.
/**
  * 海龟状态报告（我们自己定义的数据格式）
  * 规则：文件名首字母大写驼峰 + .msg；一行一个字段；# 是注释
  * 这个文件会被 rosidl 自动翻译成 Python 类和 C++ 头文件，
  * 所以 Python 和 C++ 能"说同一种语言"
 */
typedef struct turtle_interfaces__msg__TurtleStatus
{
  /// 海龟名字
  rosidl_runtime_c__String turtle_name;
  /// x 坐标
  double x;
  /// y 坐标
  double y;
  /// 朝向
  double theta;
  /// 当前前进速度
  double linear_speed;
  /// 当前转弯速度
  double angular_speed;
  /// 时间戳
  builtin_interfaces__msg__Time stamp;
} turtle_interfaces__msg__TurtleStatus;

// Struct for a sequence of turtle_interfaces__msg__TurtleStatus.
typedef struct turtle_interfaces__msg__TurtleStatus__Sequence
{
  turtle_interfaces__msg__TurtleStatus * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} turtle_interfaces__msg__TurtleStatus__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // TURTLE_INTERFACES__MSG__DETAIL__TURTLE_STATUS__STRUCT_H_
