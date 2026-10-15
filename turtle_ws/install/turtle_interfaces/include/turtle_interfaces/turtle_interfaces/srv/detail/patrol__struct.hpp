// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from turtle_interfaces:srv/Patrol.idl
// generated code does not contain a copyright notice

#ifndef TURTLE_INTERFACES__SRV__DETAIL__PATROL__STRUCT_HPP_
#define TURTLE_INTERFACES__SRV__DETAIL__PATROL__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__turtle_interfaces__srv__Patrol_Request __attribute__((deprecated))
#else
# define DEPRECATED__turtle_interfaces__srv__Patrol_Request __declspec(deprecated)
#endif

namespace turtle_interfaces
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct Patrol_Request_
{
  using Type = Patrol_Request_<ContainerAllocator>;

  explicit Patrol_Request_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->target_x = 0.0;
      this->target_y = 0.0;
    }
  }

  explicit Patrol_Request_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->target_x = 0.0;
      this->target_y = 0.0;
    }
  }

  // field types and members
  using _target_x_type =
    double;
  _target_x_type target_x;
  using _target_y_type =
    double;
  _target_y_type target_y;

  // setters for named parameter idiom
  Type & set__target_x(
    const double & _arg)
  {
    this->target_x = _arg;
    return *this;
  }
  Type & set__target_y(
    const double & _arg)
  {
    this->target_y = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    turtle_interfaces::srv::Patrol_Request_<ContainerAllocator> *;
  using ConstRawPtr =
    const turtle_interfaces::srv::Patrol_Request_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<turtle_interfaces::srv::Patrol_Request_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<turtle_interfaces::srv::Patrol_Request_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      turtle_interfaces::srv::Patrol_Request_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<turtle_interfaces::srv::Patrol_Request_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      turtle_interfaces::srv::Patrol_Request_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<turtle_interfaces::srv::Patrol_Request_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<turtle_interfaces::srv::Patrol_Request_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<turtle_interfaces::srv::Patrol_Request_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__turtle_interfaces__srv__Patrol_Request
    std::shared_ptr<turtle_interfaces::srv::Patrol_Request_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__turtle_interfaces__srv__Patrol_Request
    std::shared_ptr<turtle_interfaces::srv::Patrol_Request_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const Patrol_Request_ & other) const
  {
    if (this->target_x != other.target_x) {
      return false;
    }
    if (this->target_y != other.target_y) {
      return false;
    }
    return true;
  }
  bool operator!=(const Patrol_Request_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct Patrol_Request_

// alias to use template instance with default allocator
using Patrol_Request =
  turtle_interfaces::srv::Patrol_Request_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace turtle_interfaces


#ifndef _WIN32
# define DEPRECATED__turtle_interfaces__srv__Patrol_Response __attribute__((deprecated))
#else
# define DEPRECATED__turtle_interfaces__srv__Patrol_Response __declspec(deprecated)
#endif

namespace turtle_interfaces
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct Patrol_Response_
{
  using Type = Patrol_Response_<ContainerAllocator>;

  explicit Patrol_Response_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->success = false;
      this->message = "";
    }
  }

  explicit Patrol_Response_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : message(_alloc)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->success = false;
      this->message = "";
    }
  }

  // field types and members
  using _success_type =
    bool;
  _success_type success;
  using _message_type =
    std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>;
  _message_type message;

  // setters for named parameter idiom
  Type & set__success(
    const bool & _arg)
  {
    this->success = _arg;
    return *this;
  }
  Type & set__message(
    const std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>> & _arg)
  {
    this->message = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    turtle_interfaces::srv::Patrol_Response_<ContainerAllocator> *;
  using ConstRawPtr =
    const turtle_interfaces::srv::Patrol_Response_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<turtle_interfaces::srv::Patrol_Response_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<turtle_interfaces::srv::Patrol_Response_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      turtle_interfaces::srv::Patrol_Response_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<turtle_interfaces::srv::Patrol_Response_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      turtle_interfaces::srv::Patrol_Response_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<turtle_interfaces::srv::Patrol_Response_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<turtle_interfaces::srv::Patrol_Response_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<turtle_interfaces::srv::Patrol_Response_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__turtle_interfaces__srv__Patrol_Response
    std::shared_ptr<turtle_interfaces::srv::Patrol_Response_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__turtle_interfaces__srv__Patrol_Response
    std::shared_ptr<turtle_interfaces::srv::Patrol_Response_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const Patrol_Response_ & other) const
  {
    if (this->success != other.success) {
      return false;
    }
    if (this->message != other.message) {
      return false;
    }
    return true;
  }
  bool operator!=(const Patrol_Response_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct Patrol_Response_

// alias to use template instance with default allocator
using Patrol_Response =
  turtle_interfaces::srv::Patrol_Response_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace turtle_interfaces

namespace turtle_interfaces
{

namespace srv
{

struct Patrol
{
  using Request = turtle_interfaces::srv::Patrol_Request;
  using Response = turtle_interfaces::srv::Patrol_Response;
};

}  // namespace srv

}  // namespace turtle_interfaces

#endif  // TURTLE_INTERFACES__SRV__DETAIL__PATROL__STRUCT_HPP_
