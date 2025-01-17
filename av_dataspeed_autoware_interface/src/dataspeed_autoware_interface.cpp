#include "av_dataspeed_autoware_interface/dataspeed_autoware_interface.hpp"

#include <cstdio>

namespace vehicle
{

DataspeedAutowareInterface::DataspeedAutowareInterface(const rclcpp::NodeOptions & options)
: Node("dataspeed_autoware_interface", options)
{
  using std::placeholders::_1;

  // Subscriptions from Datspeed DBW
  steering_report_sub_ = this->create_subscription<dbw_ford_msgs::msg::SteeringReport>(
    "/vehicle/steering_report", 1,
    std::bind(&DataspeedAutowareInterface::on_steering_report, this, _1));

  vehicle_twist_sub_ = this->create_subscription<geometry_msgs::msg::TwistStamped>(
    "/vehicle/twist", 1, std::bind(&DataspeedAutowareInterface::on_vehicle_twist, this, _1));

  misc1_report_sub_ = this->create_subscription<dbw_ford_msgs::msg::Misc1Report>(
    "/vehicle/misc_1_report", 1, std::bind(&DataspeedAutowareInterface::on_misc1_report, this, _1));

  dbw_enabled_sub_ = this->create_subscription<std_msgs::msg::Bool>(
    "/vehicle/dbw_enabled", 1,
    std::bind(&DataspeedAutowareInterface::on_dbw_enabled_report, this, _1));

  gear_report_sub_ = this->create_subscription<dbw_ford_msgs::msg::GearReport>(
    "/vehicle/gear_report", 1, std::bind(&DataspeedAutowareInterface::on_gear_report, this, _1));

  // Publishers to Autoware
  steering_status_pub_ = create_publisher<autoware_vehicle_msgs::msg::SteeringReport>(
    "/vehicle/status/steering_status", rclcpp::QoS{1});

  velocity_status_pub_ = create_publisher<autoware_vehicle_msgs::msg::VelocityReport>(
    "/vehicle/status/velocity_status", rclcpp::QoS{1});

  gear_status_pub_ = create_publisher<autoware_vehicle_msgs::msg::GearReport>(
    "/vehicle/status/gear_status", rclcpp::QoS{1});

  turn_indicators_status_pub_ = create_publisher<autoware_vehicle_msgs::msg::TurnIndicatorsReport>(
    "/vehicle/status/turn_indicators_status", rclcpp::QoS{1});

  hazard_light_status_pub_ = create_publisher<autoware_vehicle_msgs::msg::HazardLightsReport>(
    "/vehicle/status/hazard_lights_status", rclcpp::QoS{1});

  control_mode_pub_ = create_publisher<autoware_vehicle_msgs::msg::ControlModeReport>(
    "/vehicle/status/control_mode", rclcpp::QoS{1});

  // Subscriptions from Autoware (TODO)

  // Publishers to Dataspeed DBW (TODO)

  // Parameters
  steering_wheel_to_tire_ratio_ = this->declare_parameter("steering_wheel_to_tire_ratio", 14.8);

  RCLCPP_INFO(this->get_logger(), "Datspeed Autoware Interface Initialised!");
}

DataspeedAutowareInterface::~DataspeedAutowareInterface()
{
  RCLCPP_INFO(this->get_logger(), "Dataspeed Autoware Interface Destroyed!");
}

void DataspeedAutowareInterface::on_steering_report(
  const dbw_ford_msgs::msg::SteeringReport::ConstSharedPtr msg)
{
  // Get current time
  auto current_time = this->now();

  // Publish steering status
  dbw_steering_report_ = *msg;
  steering_report_msg_.stamp = dbw_steering_report_.header.stamp;
  steering_report_msg_.steering_tire_angle =
    dbw_steering_report_.steering_wheel_angle / steering_wheel_to_tire_ratio_;
  steering_status_pub_->publish(steering_report_msg_);

  // Publish velocity status
  velocity_status_pub_->publish(velocity_report_msg_);

  // Publish gear report
  gear_status_pub_->publish(gear_report_msg_);

  // Publish control mode report
  if (dbw_enabled_) {
    control_mode_report_msg_.mode = control_mode_report_msg_.AUTONOMOUS;
  } else {
    control_mode_report_msg_.mode = control_mode_report_msg_.MANUAL;
  }

  control_mode_report_msg_.stamp = current_time;
  control_mode_pub_->publish(control_mode_report_msg_);

  // Publish turn indicator and hazard light status
  {
    autoware_vehicle_msgs::msg::TurnIndicatorsReport turn_indicators_report;
    autoware_vehicle_msgs::msg::HazardLightsReport hazard_light_report;
    if (dbw_misc1_report_.turn_signal.value == dbw_ford_msgs::msg::TurnSignal::LEFT) {
      turn_indicators_report.report = turn_indicators_report.ENABLE_LEFT;
      hazard_light_report.report = hazard_light_report.DISABLE;
    } else if (dbw_misc1_report_.turn_signal.value == dbw_ford_msgs::msg::TurnSignal::RIGHT) {
      turn_indicators_report.report = turn_indicators_report.ENABLE_RIGHT;
      hazard_light_report.report = hazard_light_report.DISABLE;
    } else if (dbw_misc1_report_.turn_signal.value == dbw_ford_msgs::msg::TurnSignal::HAZARD) {
      hazard_light_report.report = hazard_light_report.ENABLE;
      turn_indicators_report.report = turn_indicators_report.DISABLE;
    }

    turn_indicators_report.stamp = dbw_misc1_report_.header.stamp;
    turn_indicators_status_pub_->publish(turn_indicators_report);

    hazard_light_report.stamp = dbw_misc1_report_.header.stamp;
    hazard_light_status_pub_->publish(hazard_light_report);
  }
}

void DataspeedAutowareInterface::on_vehicle_twist(const geometry_msgs::msg::TwistStamped msg)
{
  velocity_report_msg_.header.frame_id = "base_link";
  velocity_report_msg_.header.stamp = msg.header.stamp;
  curr_vel_ = msg.twist.linear.x;

  // if (gear_report_msg_.report == gear_report_msg_.REVERSE) {
  //   velocity_report_msg_.longitudinal_velocity = msg.twist.linear.x * -1.0;
  // } else {
  //   velocity_report_msg_.longitudinal_velocity = msg.twist.linear.x;
  // }

  velocity_report_msg_.longitudinal_velocity = msg.twist.linear.x;
  velocity_report_msg_.lateral_velocity = msg.twist.linear.y;
  velocity_report_msg_.heading_rate = msg.twist.angular.z;
}

void DataspeedAutowareInterface::on_misc1_report(
  const dbw_ford_msgs::msg::Misc1Report::ConstSharedPtr msg)
{
  // If 'res_inc' button is pressed and it was not pressed before, send enable
  if (msg->btn_cc_res_inc && !dbw_misc1_report_.btn_cc_res_inc) {
    RCLCPP_INFO(this->get_logger(), "\n~~~~~~~ Engaging DBW ~~~~~~~~");
    // enable_cmd_pub_->publish(std_msgs::msg::Empty());
  }

  // If 'set_dec' button is pressed and it was not pressed before, send disable
  if (msg->btn_cc_set_dec && !dbw_misc1_report_.btn_cc_set_dec) {
    RCLCPP_INFO(this->get_logger(), "\n~~~~~~~ Disabling DBW ~~~~~~~~");
    // disable_cmd_pub_->publish(std_msgs::msg::Empty());
  }

  dbw_misc1_report_ = *msg;
}

void DataspeedAutowareInterface::on_dbw_enabled_report(
  const std_msgs::msg::Bool::ConstSharedPtr msg)
{
  dbw_enabled_ = msg->data;
}

void DataspeedAutowareInterface::on_gear_report(
  const dbw_ford_msgs::msg::GearReport::ConstSharedPtr msg)
{
  using dbw_ford_msgs::msg::Gear;

  if (msg->state.gear == Gear::PARK) {
    gear_report_msg_.report = gear_report_msg_.PARK;
  } else if (msg->state.gear == Gear::REVERSE) {
    gear_report_msg_.report = gear_report_msg_.REVERSE;
  } else if (msg->state.gear == Gear::NEUTRAL) {
    gear_report_msg_.report = gear_report_msg_.NEUTRAL;
  } else if (msg->state.gear == Gear::DRIVE) {
    gear_report_msg_.report = gear_report_msg_.DRIVE;
  }

  gear_report_msg_.stamp = msg->header.stamp;
}

}  // namespace vehicle

#include "rclcpp_components/register_node_macro.hpp"
RCLCPP_COMPONENTS_REGISTER_NODE(vehicle::DataspeedAutowareInterface)
