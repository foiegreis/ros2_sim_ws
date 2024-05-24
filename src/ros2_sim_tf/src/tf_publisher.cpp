#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/joint_state.hpp>
#include <tf2_ros/transform_listener.h>
#include <tf2_ros/buffer.h>
#include <geometry_msgs/msg/transform_stamped.hpp>

class JointPublisherNode : public rclcpp::Node
{
public:
    JointPublisherNode() : Node("tf_publisher_node")
    {   
        //Declare and get parameters
        this->declare_parameter<double>("theta1", 0.0);
        this->declare_parameter<double>("theta2", 0.0);
        this->declare_parameter<double>("theta3", 0.0);


        theta1_ = this->get_parameter("theta1").as_double();
        theta2_ = this->get_parameter("theta2").as_double();
        theta3_ = this->get_parameter("theta3").as_double();

        // Initialize the joint state publisher
        joint_state_publisher_ = this->create_publisher<sensor_msgs::msg::JointState>("joint_states", 10);

        // Initialize the transform buffer and listener
        tf_buffer_ = std::make_shared<tf2_ros::Buffer>(this->get_clock());
        tf_listener_ = std::make_shared<tf2_ros::TransformListener>(*tf_buffer_);

        RCLCPP_INFO(this->get_logger(), "Joint states (%f %f %f) were sent to the system.", theta1_, theta2_, theta3_);


        // Set up a timer to periodically check for the transform and broadcast it
        timer_ = this->create_wall_timer(
            std::chrono::milliseconds(100),
            std::bind(&JointPublisherNode::broadcastJointState, this));
    }

private:
    void broadcastJointState()
    {
        // Create a JointState message
        auto joint_state = sensor_msgs::msg::JointState();
        joint_state.header.stamp = this->get_clock()->now();
        joint_state.name = {"base_link1_joint", "link1_sphere1_joint", "link2_sphere2_joint"};
        joint_state.position = {theta1_, theta2_, theta3_};
        
        // Publish the joint state
        joint_state_publisher_->publish(joint_state);


    }

    double theta1_;
    double theta2_;
    double theta3_;
    rclcpp::Publisher<sensor_msgs::msg::JointState>::SharedPtr joint_state_publisher_;
    std::shared_ptr<tf2_ros::Buffer> tf_buffer_;
    std::shared_ptr<tf2_ros::TransformListener> tf_listener_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char **argv)
{

    rclcpp::init(argc, argv);
    auto node = std::make_shared<JointPublisherNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
