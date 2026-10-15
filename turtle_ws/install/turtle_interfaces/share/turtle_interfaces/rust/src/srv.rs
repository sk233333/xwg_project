#[cfg(feature = "serde")]
use serde::{Deserialize, Serialize};




// Corresponds to turtle_interfaces__srv__Patrol_Request

// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct Patrol_Request {
    /// ---- 请求：请巡逻到这个坐标 ----
    /// 目标 x
    pub target_x: f64,

    /// 目标 y
    pub target_y: f64,

}



impl Default for Patrol_Request {
  fn default() -> Self {
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::srv::rmw::Patrol_Request::default())
  }
}

impl rosidl_runtime_rs::Message for Patrol_Request {
  type RmwMsg = super::srv::rmw::Patrol_Request;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        target_x: msg.target_x,
        target_y: msg.target_y,
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
      target_x: msg.target_x,
      target_y: msg.target_y,
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      target_x: msg.target_x,
      target_y: msg.target_y,
    }
  }
}


// Corresponds to turtle_interfaces__srv__Patrol_Response

// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct Patrol_Response {
    /// 是否成功接受
    pub success: bool,

    /// 文字说明
    pub message: std::string::String,

}



impl Default for Patrol_Response {
  fn default() -> Self {
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::srv::rmw::Patrol_Response::default())
  }
}

impl rosidl_runtime_rs::Message for Patrol_Response {
  type RmwMsg = super::srv::rmw::Patrol_Response;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        success: msg.success,
        message: msg.message.as_str().into(),
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
      success: msg.success,
        message: msg.message.as_str().into(),
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      success: msg.success,
      message: msg.message.to_string(),
    }
  }
}






#[link(name = "turtle_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_service_type_support_handle__turtle_interfaces__srv__Patrol() -> *const std::ffi::c_void;
}

// Corresponds to turtle_interfaces__srv__Patrol
#[allow(missing_docs, non_camel_case_types)]
pub struct Patrol;

impl rosidl_runtime_rs::Service for Patrol {
    type Request = Patrol_Request;
    type Response = Patrol_Response;

    fn get_type_support() -> *const std::ffi::c_void {
        // SAFETY: No preconditions for this function.
        unsafe { rosidl_typesupport_c__get_service_type_support_handle__turtle_interfaces__srv__Patrol() }
    }
}


