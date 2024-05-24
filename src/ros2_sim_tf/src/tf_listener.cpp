#include <rclcpp/rclcpp.hpp>
#include <tf2_ros/transform_listener.h>
#include <tf2_ros/buffer.h>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2/LinearMath/Matrix3x3.h>
#include <geometry_msgs/msg/transform_stamped.hpp>

class RobotTfListener : public rclcpp::Node
{
public:
  RobotTfListener()
  : Node("robot_tf_listener")
  {
    tf_buffer_ = std::make_unique<tf2_ros::Buffer>(this->get_clock());
    tf_listener_ = std::make_shared<tf2_ros::TransformListener>(*tf_buffer_);

    // Get the transform between the sphere1 and link2 frames
    getTransform("link1", "base");
  }

private:
  void getTransform(const std::string& target_frame, const std::string& source_frame)
  {
    rclcpp::Rate rate(10); // Check for transforms at 10 Hz

    while (rclcpp::ok()) {
      try {
        geometry_msgs::msg::TransformStamped transform_stamped = tf_buffer_->lookupTransform(
          target_frame, source_frame, tf2::TimePointZero);

        // Print the transform
        RCLCPP_INFO(this->get_logger(), "Transform from %s to %s:", target_frame.c_str(), source_frame.c_str());
        RCLCPP_INFO(this->get_logger(), "Translation: [%f, %f, %f]",
          transform_stamped.transform.translation.x,
          transform_stamped.transform.translation.y,
          transform_stamped.transform.translation.z);

        tf2::Quaternion q(
          transform_stamped.transform.rotation.x,
          transform_stamped.transform.rotation.y,
          transform_stamped.transform.rotation.z,
          transform_stamped.transform.rotation.w);


        tf2::Matrix3x3 matrix(q);

        double roll, pitch, yaw;

        matrix.getRPY(roll, pitch, yaw);

        RCLCPP_INFO(this->get_logger(), "Rotation: [%f, %f, %f]", roll, pitch, yaw);
        break;
      }
      catch (tf2::TransformException& ex) {
        RCLCPP_WARN(this->get_logger(), "Could not get transform: %s", ex.what());
        rate.sleep();
      }
    }
  }

  std::unique_ptr<tf2_ros::Buffer> tf_buffer_;
  std::shared_ptr<tf2_ros::TransformListener> tf_listener_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<RobotTfListener>());
  rclcpp::shutdown();
  return 0;
}