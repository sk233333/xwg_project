#[cfg(feature = "serde")]
use serde::{Deserialize, Serialize};



// Corresponds to turtle_interfaces__msg__TurtleStatus
/// 海龟状态报告（我们自己定义的数据格式）
/// 规则：文件名首字母大写驼峰 + .msg；一行一个字段；# 是注释
/// 这个文件会被 rosidl 自动翻译成 Python 类和 C++ 头文件，
/// 所以 Python 和 C++ 能"说同一种语言"

#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct TurtleStatus {
    /// 海龟名字
    pub turtle_name: std::string::String,

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
    pub stamp: builtin_interfaces::msg::Time,

}



impl Default for TurtleStatus {
  fn default() -> Self {
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::msg::rmw::TurtleStatus::default())
  }
}

impl rosidl_runtime_rs::Message for TurtleStatus {
  type RmwMsg = super::msg::rmw::TurtleStatus;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        turtle_name: msg.turtle_name.as_str().into(),
        x: msg.x,
        y: msg.y,
        theta: msg.theta,
        linear_speed: msg.linear_speed,
        angular_speed: msg.angular_speed,
        stamp: builtin_interfaces::msg::Time::into_rmw_message(std::borrow::Cow::Owned(msg.stamp)).into_owned(),
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        turtle_name: msg.turtle_name.as_str().into(),
      x: msg.x,
      y: msg.y,
      theta: msg.theta,
      linear_speed: msg.linear_speed,
      angular_speed: msg.angular_speed,
        stamp: builtin_interfaces::msg::Time::into_rmw_message(std::borrow::Cow::Borrowed(&msg.stamp)).into_owned(),
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      turtle_name: msg.turtle_name.to_string(),
      x: msg.x,
      y: msg.y,
      theta: msg.theta,
      linear_speed: msg.linear_speed,
      angular_speed: msg.angular_speed,
      stamp: builtin_interfaces::msg::Time::from_rmw_message(msg.stamp),
    }
  }
}


