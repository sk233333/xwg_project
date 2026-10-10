// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from turtle_interfaces:msg/TurtleStatus.idl
// generated code does not contain a copyright notice

#ifndef TURTLE_INTERFACES__MSG__DETAIL__TURTLE_STATUS__BUILDER_HPP_
#define TURTLE_INTERFACES__MSG__DETAIL__TURTLE_STATUS__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "turtle_interfaces/msg/detail/turtle_status__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace turtle_interfaces
{

namespace msg
{

namespace builder
{

class Init_TurtleStatus_stamp
{
public:
  explicit Init_TurtleStatus_stamp(::turtle_interfaces::msg::TurtleStatus & msg)
  : msg_(msg)
  {}
  ::turtle_interfaces::msg::TurtleStatus stamp(::turtle_interfaces::msg::TurtleStatus::_stamp_type arg)
  {
    msg_.stamp = std::move(arg);
    return std::move(msg_);
  }

private:
  ::turtle_interfaces::msg::TurtleStatus msg_;
};

class Init_TurtleStatus_angular_speed
{
public:
  explicit Init_TurtleStatus_angular_speed(::turtle_interfaces::msg::TurtleStatus & msg)
  : msg_(msg)
  {}
  Init_TurtleStatus_stamp angular_speed(::turtle_interfaces::msg::TurtleStatus::_angular_speed_type arg)
  {
    msg_.angular_speed = std::move(arg);
    return Init_TurtleStatus_stamp(msg_);
  }

private:
  ::turtle_interfaces::msg::TurtleStatus msg_;
};

class Init_TurtleStatus_linear_speed
{
public:
  explicit Init_TurtleStatus_linear_speed(::turtle_interfaces::msg::TurtleStatus & msg)
  : msg_(msg)
  {}
  Init_TurtleStatus_angular_speed linear_speed(::turtle_interfaces::msg::TurtleStatus::_linear_speed_type arg)
  {
    msg_.linear_speed = std::move(arg);
    return Init_TurtleStatus_angular_speed(msg_);
  }

private:
  ::turtle_interfaces::msg::TurtleStatus msg_;
};

class Init_TurtleStatus_theta
{
public:
  explicit Init_TurtleStatus_theta(::turtle_interfaces::msg::TurtleStatus & msg)
  : msg_(msg)
  {}
  Init_TurtleStatus_linear_speed theta(::turtle_interfaces::msg::TurtleStatus::_theta_type arg)
  {
    msg_.theta = std::move(arg);
    return Init_TurtleStatus_linear_speed(msg_);
  }

private:
  ::turtle_interfaces::msg::TurtleStatus msg_;
};

class Init_TurtleStatus_y
{
public:
  explicit Init_TurtleStatus_y(::turtle_interfaces::msg::TurtleStatus & msg)
  : msg_(msg)
  {}
  Init_TurtleStatus_theta y(::turtle_interfaces::msg::TurtleStatus::_y_type arg)
  {
    msg_.y = std::move(arg);
    return Init_TurtleStatus_theta(msg_);
  }

private:
  ::turtle_interfaces::msg::TurtleStatus msg_;
};

class Init_TurtleStatus_x
{
public:
  explicit Init_TurtleStatus_x(::turtle_interfaces::msg::TurtleStatus & msg)
  : msg_(msg)
  {}
  Init_TurtleStatus_y x(::turtle_interfaces::msg::TurtleStatus::_x_type arg)
  {
    msg_.x = std::move(arg);
    return Init_TurtleStatus_y(msg_);
  }

private:
  ::turtle_interfaces::msg::TurtleStatus msg_;
};

class Init_TurtleStatus_turtle_name
{
public:
  Init_TurtleStatus_turtle_name()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_TurtleStatus_x turtle_name(::turtle_interfaces::msg::TurtleStatus::_turtle_name_type arg)
  {
    msg_.turtle_name = std::move(arg);
    return Init_TurtleStatus_x(msg_);
  }

private:
  ::turtle_interfaces::msg::TurtleStatus msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::turtle_interfaces::msg::TurtleStatus>()
{
  return turtle_interfaces::msg::builder::Init_TurtleStatus_turtle_name();
}

}  // namespace turtle_interfaces

#endif  // TURTLE_INTERFACES__MSG__DETAIL__TURTLE_STATUS__BUILDER_HPP_
