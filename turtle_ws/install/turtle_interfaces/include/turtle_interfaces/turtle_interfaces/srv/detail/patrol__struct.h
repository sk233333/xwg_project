// NOLINT: This file starts with a BOM since it contain non-ASCII characters
// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from turtle_interfaces:srv/Patrol.idl
// generated code does not contain a copyright notice

#ifndef TURTLE_INTERFACES__SRV__DETAIL__PATROL__STRUCT_H_
#define TURTLE_INTERFACES__SRV__DETAIL__PATROL__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in srv/Patrol in the package turtle_interfaces.
typedef struct turtle_interfaces__srv__Patrol_Request
{
  /// ---- 请求：请巡逻到这个坐标 ----
  /// 目标 x
  double target_x;
  /// 目标 y
  double target_y;
} turtle_interfaces__srv__Patrol_Request;

// Struct for a sequence of turtle_interfaces__srv__Patrol_Request.
typedef struct turtle_interfaces__srv__Patrol_Request__Sequence
{
  turtle_interfaces__srv__Patrol_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} turtle_interfaces__srv__Patrol_Request__Sequence;


// Constants defined in the message

// Include directives for member types
// Member 'message'
#include "rosidl_runtime_c/string.h"

/// Struct defined in srv/Patrol in the package turtle_interfaces.
typedef struct turtle_interfaces__srv__Patrol_Response
{
  /// 是否成功接受
  bool success;
  /// 文字说明
  rosidl_runtime_c__String message;
} turtle_interfaces__srv__Patrol_Response;

// Struct for a sequence of turtle_interfaces__srv__Patrol_Response.
typedef struct turtle_interfaces__srv__Patrol_Response__Sequence
{
  turtle_interfaces__srv__Patrol_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} turtle_interfaces__srv__Patrol_Response__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // TURTLE_INTERFACES__SRV__DETAIL__PATROL__STRUCT_H_
