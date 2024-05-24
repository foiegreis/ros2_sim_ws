cmake_minimum_required(VERSION 3.8)
project(ros2_sim_tf)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)



# set dependencies
set(dependencies
rclcpp
tf2_ros
geometry_msgs
sensor_msgs
)

# add executables for scripts
add_executable(tf_publisher_node src/tf_publisher.cpp)
ament_target_dependencies(tf_publisher_node ${dependencies})

add_executable(tf_listener_node src/tf_listener.cpp)
ament_target_dependencies(tf_listener_node ${dependencies})

add_executable(tf_broadcaster_node src/tf_broadcaster.cpp)
ament_target_dependencies(tf_broadcaster_node ${dependencies})


# install the scripts
install(TARGETS
  tf_publisher_node
  tf_listener_node
  tf_broadcaster_node
  DESTINATION lib/${PROJECT_NAME})

# export dependencies
ament_export_dependencies(${dependencies})
ament_package()