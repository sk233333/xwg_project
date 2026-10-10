#[cfg(feature = "serde")]
use serde::{Deserialize, Serialize};


#[link(name = "turtle_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__turtle_interfaces__msg__TurtleStatus() -> *const std::ffi::c_void;
}

#[link(name = "turtle_interfaces__rosidl_generator_c")]
extern "C" {
    fn turtle_interfaces__msg__TurtleStatus__init(msg: *mut TurtleStatus) -> bool;
    fn turtle_interfaces__msg__TurtleStatus__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<TurtleStatus>, size: usize) -> bool;
    fn turtle_interfaces__msg__TurtleStatus__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<TurtleStatus>);
    fn turtle_interfaces__msg__TurtleStatus__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<TurtleStatus>, out_seq: *mut rosidl_runtime_rs::Sequence<TurtleStatus>) -> bool;
}

// Corresponds to turtle_interfaces__msg__TurtleStatus
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]

/// 海龟状态报告（我们自己定义的数据格式）
/// 规则：文件名首字母大写驼峰 + .msg；一行一个字段；# 是注释
/// 这个文件会被 rosidl 自动翻译成 Python 类和 C++ 头文件，
/// 所以 Python 和 C++ 能"说同一种语言"

#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct TurtleStatus {
    /// 海龟名字
    pub turtle_name: rosidl_runtime_rs::String,

    /// x 坐标
    pub x: f64,

    /// y 坐标
    pub y: f64,

    /// 朝向
    pub theta: f64,

    /// 当前前进速度
    pub linear_speed: f64,

    /// 当前转弯速度
    pub angular_speed: f64,

    /// 时间戳
    pub stamp: builtin_interfaces::msg::rmw::Time,

}



impl Default for TurtleStatus {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !turtle_interfaces__msg__TurtleStatus__init(&mut msg as *mut _) {
        panic!("Call to turtle_interfaces__msg__TurtleStatus__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for TurtleStatus {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { turtle_interfaces__msg__TurtleStatus__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { turtle_interfaces__msg__TurtleStatus__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { turtle_interfaces__msg__TurtleStatus__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for TurtleStatus {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for TurtleStatus where Self: Sized {
  const TYPE_NAME: &'static str = "turtle_interfaces/msg/TurtleStatus";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__turtle_interfaces__msg__TurtleStatus() }
  }
}


