cmake_minimum_required(VERSION 3.8)
project(av_dataspeed_autoware_interface)

# Set C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_components REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(autoware_control_msgs REQUIRED)
find_package(autoware_vehicle_info_utils REQUIRED)
find_package(autoware_vehicle_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(tier4_api_msgs REQUIRED)
find_package(tier4_debug_msgs REQUIRED)
find_package(tier4_external_api_msgs REQUIRED)
find_package(tier4_vehicle_msgs REQUIRED)
find_package(dbw_ford_msgs REQUIRED)

include_directories(include)

add_library(dataspeed_autoware_interface_component SHARED
            src/dataspeed_autoware_interface.cpp)

# target_include_directories(dataspeed_autoware_interface_component PUBLIC
# $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
# $<INSTALL_INTERFACE:include>)
# target_compile_features(dataspeed_autoware_interface_component PUBLIC c_std_99
# cxx_std_17)  # Require C99 and C++17

ament_target_dependencies(
  dataspeed_autoware_interface_component
  "rclcpp"
  "rclcpp_components"
  "std_msgs"
  "geometry_msgs"
  "autoware_control_msgs"
  "autoware_vehicle_info_utils"
  "autoware_vehicle_msgs"
  "nav_msgs"
  "tier4_api_msgs"
  "tier4_debug_msgs"
  "tier4_external_api_msgs"
  "tier4_vehicle_msgs"
  "dbw_ford_msgs")

rclcpp_components_register_node(
  dataspeed_autoware_interface_component PLUGIN
  "vehicle::DataspeedAutowareInterface" EXECUTABLE dataspeed_autoware_interface)

ament_export_targets(export_dataspeed_autoware_interface_component)

install(
  TARGETS dataspeed_autoware_interface_component
  EXPORT export_dataspeed_autoware_interface_component
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin)

# Install directories.
install(DIRECTORY launch DESTINATION share/${PROJECT_NAME})

install(DIRECTORY config DESTINATION share/${PROJECT_NAME})

ament_package()
