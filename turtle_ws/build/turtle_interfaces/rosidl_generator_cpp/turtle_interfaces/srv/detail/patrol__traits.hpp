// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from turtle_interfaces:srv/Patrol.idl
// generated code does not contain a copyright notice

#ifndef TURTLE_INTERFACES__SRV__DETAIL__PATROL__TRAITS_HPP_
#define TURTLE_INTERFACES__SRV__DETAIL__PATROL__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "turtle_interfaces/srv/detail/patrol__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace turtle_interfaces
{

namespace srv
{

inline void to_flow_style_yaml(
  const Patrol_Request & msg,
  std::ostream & out)
{
  out << "{";
  // member: target_x
  {
    out << "target_x: ";
    rosidl_generator_traits::value_to_yaml(msg.target_x, out);
    out << ", ";
  }

  // member: target_y
  {
    out << "target_y: ";
    rosidl_generator_traits::value_to_yaml(msg.target_y, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const Patrol_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: target_x
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "target_x: ";
    rosidl_generator_traits::value_to_yaml(msg.target_x, out);
    out << "\n";
  }

  // member: target_y
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "target_y: ";
    rosidl_generator_traits::value_to_yaml(msg.target_y, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const Patrol_Request & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace srv

}  // namespace turtle_interfaces

namespace rosidl_generator_traits
{

[[deprecated("use turtle_interfaces::srv::to_block_style_yaml() instead")]]
inline void to_yaml(
  const turtle_interfaces::srv::Patrol_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  turtle_interfaces::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use turtle_interfaces::srv::to_yaml() instead")]]
inline std::string to_yaml(const turtle_interfaces::srv::Patrol_Request & msg)
{
  return turtle_interfaces::srv::to_yaml(msg);
}

template<>
inline const char * data_type<turtle_interfaces::srv::Patrol_Request>()
{
  return "turtle_interfaces::srv::Patrol_Request";
}

template<>
inline const char * name<turtle_interfaces::srv::Patrol_Request>()
{
  return "turtle_interfaces/srv/Patrol_Request";
}

template<>
struct has_fixed_size<turtle_interfaces::srv::Patrol_Request>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<turtle_interfaces::srv::Patrol_Request>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<turtle_interfaces::srv::Patrol_Request>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace turtle_interfaces
{

namespace srv
{

inline void to_flow_style_yaml(
  const Patrol_Response & msg,
  std::ostream & out)
{
  out << "{";
  // member: success
  {
    out << "success: ";
    rosidl_generator_traits::value_to_yaml(msg.success, out);
    out << ", ";
  }

  // member: message
  {
    out << "message: ";
    rosidl_generator_traits::value_to_yaml(msg.message, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const Patrol_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: success
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "success: ";
    rosidl_generator_traits::value_to_yaml(msg.success, out);
    out << "\n";
  }

  // member: message
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "message: ";
    rosidl_generator_traits::value_to_yaml(msg.message, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const Patrol_Response & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace srv

}  // namespace turtle_interfaces

namespace rosidl_generator_traits
{

[[deprecated("use turtle_interfaces::srv::to_block_style_yaml() instead")]]
inline void to_yaml(
  const turtle_interfaces::srv::Patrol_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  turtle_interfaces::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use turtle_interfaces::srv::to_yaml() instead")]]
inline std::string to_yaml(const turtle_interfaces::srv::Patrol_Response & msg)
{
  return turtle_interfaces::srv::to_yaml(msg);
}

template<>
inline const char * data_type<turtle_interfaces::srv::Patrol_Response>()
{
  return "turtle_interfaces::srv::Patrol_Response";
}

template<>
inline const char * name<turtle_interfaces::srv::Patrol_Response>()
{
  return "turtle_interfaces/srv/Patrol_Response";
}

template<>
struct has_fixed_size<turtle_interfaces::srv::Patrol_Response>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<turtle_interfaces::srv::Patrol_Response>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<turtle_interfaces::srv::Patrol_Response>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<turtle_interfaces::srv::Patrol>()
{
  return "turtle_interfaces::srv::Patrol";
}

template<>
inline const char * name<turtle_interfaces::srv::Patrol>()
{
  return "turtle_interfaces/srv/Patrol";
}

template<>
struct has_fixed_size<turtle_interfaces::srv::Patrol>
  : std::integral_constant<
    bool,
    has_fixed_size<turtle_interfaces::srv::Patrol_Request>::value &&
    has_fixed_size<turtle_interfaces::srv::Patrol_Response>::value
  >
{
};

template<>
struct has_bounded_size<turtle_interfaces::srv::Patrol>
  : std::integral_constant<
    bool,
    has_bounded_size<turtle_interfaces::srv::Patrol_Request>::value &&
    has_bounded_size<turtle_interfaces::srv::Patrol_Response>::value
  >
{
};

template<>
struct is_service<turtle_interfaces::srv::Patrol>
  : std::true_type
{
};

template<>
struct is_service_request<turtle_interfaces::srv::Patrol_Request>
  : std::true_type
{
};

template<>
struct is_service_response<turtle_interfaces::srv::Patrol_Response>
  : std::true_type
{
};

}  // namespace rosidl_generator_traits

#endif  // TURTLE_INTERFACES__SRV__DETAIL__PATROL__TRAITS_HPP_
