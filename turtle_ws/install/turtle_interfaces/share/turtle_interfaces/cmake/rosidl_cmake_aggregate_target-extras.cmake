# generated from rosidl_cmake/cmake/rosidl_cmake_aggregate_target-extras.cmake.in

# Create a convenience aggregate target turtle_interfaces::turtle_interfaces
# that links all generated interface targets, so downstream packages can use
# a single modern CMake target name instead of ${turtle_interfaces_TARGETS}.
if(turtle_interfaces_TARGETS AND NOT TARGET turtle_interfaces::turtle_interfaces)
  add_library(turtle_interfaces::turtle_interfaces INTERFACE IMPORTED)
  set_target_properties(turtle_interfaces::turtle_interfaces PROPERTIES
    INTERFACE_LINK_LIBRARIES "${turtle_interfaces_TARGETS}")
endif()
