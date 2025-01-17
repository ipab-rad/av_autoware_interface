#pragma once

#include "rclcpp/rclcpp.hpp"

#include "geometry_msgs/msg/twist_stamped.hpp"
#include "std_msgs/msg/bool.hpp"
#include "std_msgs/msg/empty.hpp"

// Autoware inputs messages
#include "autoware_control_msgs/msg/control.hpp"
#include "autoware_vehicle_msgs/msg/engage.hpp"
// #include "autoware_vehicle_msgs/srv/control_mode_command.hpp"
#include "autoware_vehicle_msgs/msg/gear_command.hpp"
#include "autoware_vehicle_msgs/msg/hazard_lights_command.hpp"
#include "autoware_vehicle_msgs/msg/turn_indicators_command.hpp"
#include "tier4_vehicle_msgs/msg/actuation_command_stamped.hpp"
#include "tier4_vehicle_msgs/msg/vehicle_emergency_stamped.hpp"

// Dataspeed dbw output messages
// #include "dataspeed_ulc_msgs/msg/ulc_cmd.hpp"
#include "dbw_ford_msgs/msg/gear.hpp"
#include "dbw_ford_msgs/msg/gear_cmd.hpp"
#include "dbw_ford_msgs/msg/misc_cmd.hpp"
#include "dbw_ford_msgs/msg/steering_cmd.hpp"
#include "dbw_ford_msgs/msg/turn_signal.hpp"

// Dataspeed dbw input messages
#include "dbw_ford_msgs/msg/brake_report.hpp"
#include "dbw_ford_msgs/msg/gear_report.hpp"
#include "dbw_ford_msgs/msg/misc1_report.hpp"
#include "dbw_ford_msgs/msg/steering_report.hpp"
#include "dbw_ford_msgs/msg/throttle_report.hpp"
#include "dbw_ford_msgs/msg/wheel_speed_report.hpp"

// Autoware output messages
#include "autoware_vehicle_msgs/msg/control_mode_report.hpp"
#include "autoware_vehicle_msgs/msg/gear_report.hpp"
#include "autoware_vehicle_msgs/msg/hazard_lights_report.hpp"
#include "autoware_vehicle_msgs/msg/steering_report.hpp"
#include "autoware_vehicle_msgs/msg/turn_indicators_report.hpp"
#include "autoware_vehicle_msgs/msg/velocity_report.hpp"
#include "tier4_vehicle_msgs/msg/actuation_status_stamped.hpp"

namespace vehicle
{
// enum class InterfaceMode
// {
//     ULC_CMD_ONLY = 0U,
//     ULC_AND_RAW_STEERING_CMD = 1U
// };

/**
 * @class DataspeedAutowareInterface
 *
 * @brief Provide the interface between Autoware and Dataspeed dbw
 */
class DataspeedAutowareInterface : public rclcpp::Node
{
public:
  explicit DataspeedAutowareInterface(const rclcpp::NodeOptions & options);

  ~DataspeedAutowareInterface();

private:
  // // Subscriptions: From Autoware
  // rclcpp::Subscription<tier4_vehicle_msgs::msg::ActuationCommandStamped>::SharedPtr
  //     control_cmd_sub_;

  // rclcpp::Subscription<autoware_vehicle_msgs::msg::GearCommand>::SharedPtr gear_cmd_sub_;

  // rclcpp::Subscription<autoware_vehicle_msgs::msg::TurnIndicatorsCommand>::SharedPtr
  // turn_indciator_cmd_sub_;

  // rclcpp::Subscription<autoware_vehicle_msgs::msg::HazardLightsCommand>::SharedPtr
  // hazard_lights_cmd_sub_;

  // rclcpp::Subscription<autoware_vehicle_msgs::msg::Engage>::SharedPtr engage_cmd_sub_;

  // rclcpp::Subscription<tier4_vehicle_msgs::msg::VehicleEmergencyStamped>::SharedPtr
  // emergency_cmd_sub_;

  // Subscription: From Dataspeed DBW
  rclcpp::Subscription<dbw_ford_msgs::msg::SteeringReport>::SharedPtr steering_report_sub_;

  // rclcpp::Subscription<dbw_ford_msgs::msg::WheelSpeedReport>::SharedPtr wheelspeed_report_sub_;

  // rclcpp::Subscription<dbw_ford_msgs::msg::ThrottleReport>::SharedPtr throttle_report_sub_;

  // rclcpp::Subscription<dbw_ford_msgs::msg::BrakeReport>::SharedPtr brake_report_sub_;

  rclcpp::Subscription<geometry_msgs::msg::TwistStamped>::SharedPtr vehicle_twist_sub_;

  rclcpp::Subscription<dbw_ford_msgs::msg::Misc1Report>::SharedPtr misc1_report_sub_;

  rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr dbw_enabled_sub_;

  rclcpp::Subscription<dbw_ford_msgs::msg::GearReport>::SharedPtr gear_report_sub_;

  // // Publishers: To Dataspeed DBW
  // // rclcpp::Publisher<dataspeed_ulc_msgs::msg::UlcCmd>::SharedPtr ulc_cmd_pub_;

  // rclcpp::Publisher<dbw_ford_msgs::msg::SteeringCmd>::SharedPtr steering_wheel_cmd_pub_;

  // rclcpp::Publisher<dbw_ford_msgs::msg::GearCmd>::SharedPtr gear_cmd_pub_;

  // rclcpp::Publisher<dbw_ford_msgs::msg::MiscCmd>::SharedPtr misc1_cmd_pub_;

  // rclcpp::Publisher<std_msgs::msg::Empty>::SharedPtr enable_cmd_pub_;

  // rclcpp::Publisher<std_msgs::msg::Empty>::SharedPtr disable_cmd_pub_;

  // Publishers: To Autoware

  rclcpp::Publisher<autoware_vehicle_msgs::msg::SteeringReport>::SharedPtr steering_status_pub_;

  rclcpp::Publisher<autoware_vehicle_msgs::msg::VelocityReport>::SharedPtr velocity_status_pub_;

  rclcpp::Publisher<autoware_vehicle_msgs::msg::GearReport>::SharedPtr gear_status_pub_;

  rclcpp::Publisher<autoware_vehicle_msgs::msg::TurnIndicatorsReport>::SharedPtr
    turn_indicators_status_pub_;

  rclcpp::Publisher<autoware_vehicle_msgs::msg::HazardLightsReport>::SharedPtr
    hazard_light_status_pub_;

  rclcpp::Publisher<autoware_vehicle_msgs::msg::ControlModeReport>::SharedPtr control_mode_pub_;

  // rclcpp::Publisher<tier4_vehicle_msgs::msg::ActuationStatusStamped>::SharedPtr
  // actuation_status_pub_;

  // rclcpp::TimerBase::SharedPtr timer_;
  // rclcpp::TimerBase::SharedPtr signals_timer_;

  // // ULC parameters: Any value left as 0.0 will use the firmware defaults
  // double linear_acceleration_limit_ = 0.0;
  // double linear_deceleration_limit_ = 0.0;
  // double lateral_accleration_limit_ = 0.0;
  // double angular_acceleration_limit_ = 0.0;
  // double jerk_limit_throttle_ = 0.0;
  // double jerk_limit_brake_ = 0.0;

  // Vehicle Parameters
  float steering_wheel_to_tire_ratio_ = 0.0;
  // float wheelbase_ = 0.0;

  // Interface parameters
  double curr_vel_ = 0.0;

  // uint8_t interface_mode_ = 0U;
  // double min_velocity_cmd_ = 0.0;
  // double min_velocity_roundoff_ = 0.0;  // Velocity that considers vehilce is static
  // double vel_sign_tolerance_ = 0.0;
  // double fixed_steering_wheel_rate_value_ = 0.0;
  // bool use_control_steering_rate_ = false;

  // Flag var
  bool dbw_enabled_ = false;
  // bool is_emergency_ = false;

  // Msg received from Autoware
  // dbw_ford_msgs::msg::TurnSignal desired_turn_signal_;

  // Msgs received from the Datspeed DBW
  dbw_ford_msgs::msg::SteeringReport dbw_steering_report_;
  dbw_ford_msgs::msg::Misc1Report dbw_misc1_report_;
  // dbw_ford_msgs::msg::ThrottleReport dbw_throttle_report_;
  // dbw_ford_msgs::msg::BrakeReport    dbw_brake_report_;
  // dbw_ford_msgs::msg::WheelSpeedReport dbw_wheel_report_;
  // // dataspeed_ulc_msgs::msg::UlcCmd dbw_ulc_cmd_;
  // dbw_ford_msgs::msg::SteeringCmd dbw_steering_wheel_cmd_;
  // dbw_ford_msgs::msg::GearCmd dbw_gear_cmd_;
  // dbw_ford_msgs::msg::MiscCmd dbw_misc1_cmd_;

  // Msgs to send to Autoware
  autoware_vehicle_msgs::msg::VelocityReport velocity_report_msg_;
  autoware_vehicle_msgs::msg::SteeringReport steering_report_msg_;
  autoware_vehicle_msgs::msg::ControlModeReport control_mode_report_msg_;
  autoware_vehicle_msgs::msg::GearReport gear_report_msg_;
  // autoware_vehicle_msgs::msg::TurnIndicatorsReport turn_indicators_report_msg_;
  // autoware_vehicle_msgs::msg::HazardLightsReport hazard_lights_report_msg_;
  // tier4_vehicle_msgs::msg::ActuationStatusStamped actuation_stamped_status_msg_;

  // void onControlCommand(
  //     const autoware_auto_control_msgs::msg::AckermannControlCommand::ConstSharedPtr msg);
  // void onGearCommand(const autoware_vehicle_msgs::msg::GearCommand::ConstSharedPtr msg);
  // void onEngageCommand(const autoware_vehicle_msgs::msg::Engage::ConstSharedPtr msg);
  // void onTurnIndicatorCommand(const
  // autoware_vehicle_msgs::msg::TurnIndicatorsCommand::ConstSharedPtr msg); void
  // onTurnHazardCommand(const autoware_vehicle_msgs::msg::HazardLightsCommand::ConstSharedPtr msg);
  // void onEmergencyCommand(const tier4_vehicle_msgs::msg::VehicleEmergencyStamped::ConstSharedPtr
  // msg);

  void on_steering_report(const dbw_ford_msgs::msg::SteeringReport::ConstSharedPtr msg);
  void on_vehicle_twist(const geometry_msgs::msg::TwistStamped msg);
  void on_misc1_report(const dbw_ford_msgs::msg::Misc1Report::ConstSharedPtr msg);
  void on_dbw_enabled_report(const std_msgs::msg::Bool::ConstSharedPtr msg);
  void on_gear_report(const dbw_ford_msgs::msg::GearReport::ConstSharedPtr msg);
  // void onWheelSpeedReport(const dbw_ford_msgs::msg::WheelSpeedReport::ConstSharedPtr msg);
  // void onThrottleReport(const dbw_ford_msgs::msg::ThrottleReport::ConstSharedPtr msg);
  // void onBrakeReport(const dbw_ford_msgs::msg::BrakeReport::ConstSharedPtr msg);

  // void publishDbwCommands();
  // void publishSignalsCommand();
};

}  // namespace vehicle
