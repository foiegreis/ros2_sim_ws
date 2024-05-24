#include <rclcpp/rclcpp.hpp>
#include <tf2_ros/transform_broadcaster.h>
#include <tf2/LinearMath/Quaternion.h>

class RobotTfBroadcaster : public rclcpp::Node
{
public:
  RobotTfBroadcaster()
  : Node("robot_tf_broadcaster")
  { 
    //Declare parameters
    this->declare_parameter<double>("theta1", 0.0);
    this->declare_parameter<double>("theta2", 0.0);
    this->declare_parameter<double>("theta3", 0.0);


    theta1_ = this->get_parameter("theta1").as_double();
    theta2_ = this->get_parameter("theta2").as_double();
    theta3_ = this->get_parameter("theta3").as_double();


    tf_broadcaster_ = std::make_unique<tf2_ros::TransformBroadcaster>(*this);

    // Publish transforms for each joint
    publishTransforms();
  }

private:
  void publishTransforms()
  {
    rclcpp::Rate rate(20); // Publish at 20 Hz

    while (rclcpp::ok()) {
      // Publish transforms for each joint
      publishJointTransform("base", "link1", 0.0, 0.0, 0.0, 0.0, 0.0, theta1_); // z
      publishJointTransform("link1", "sphere1", 0.0, 0.0, 1.05, 0.0, 0.0, 0.0); // fixed
      publishJointTransform("sphere1", "link2", 0.0, 0.0, 0.15, 0.0, theta2_, 0.0); // y
      publishJointTransform("link2", "sphere2", 0.0, 0.0, 1.05, 0.0, 0.0, 0.0); // fixed
      publishJointTransform("sphere2", "link3", 0.0, 0.0, 0.15, 0.0, theta3_, 0.0); // y
      publishJointTransform("link3", "gripper", 0.0, 0.0, 1.0, 0.0, -1.57, 0.0); // fixed

      rate.sleep();
    }
  }

  void publishJointTransform(
    const std::string& parent_frame,
    const std::string& child_frame,
    double x, double y, double z,
    double roll, double pitch, double yaw)
  {
    geometry_msgs::msg::TransformStamped transform_stamped;
    transform_stamped.header.stamp = this->now();
    transform_stamped.header.frame_id = parent_frame;
    transform_stamped.child_frame_id = child_frame;

    transform_stamped.transform.translation.x = x;
    transform_stamped.transform.translation.y = y;
    transform_stamped.transform.translation.z = z;

    tf2::Quaternion q;
    q.setRPY(roll, pitch, yaw);
    transform_stamped.transform.rotation.x = q.x();
    transform_stamped.transform.rotation.y = q.y();
    transform_stamped.transform.rotation.z = q.z();
    transform_stamped.transform.rotation.w = q.w();

    tf_broadcaster_->sendTransform(transform_stamped);
  }

  double theta1_;
  double theta2_;
  double theta3_;
  std::unique_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<RobotTfBroadcaster>());
  rclcpp::shutdown();
  return 0;
}