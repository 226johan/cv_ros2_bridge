#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/twist.hpp>

class TurtlesimNode : public rclcpp::Node
{
public:
  TurtlesimNode() : Node("twist_publish")
  {
    turtle_publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("turtle1/cmd_vel", 10);
    linear_velocity_ = 1.0;
    angular_velocity_ = 2.0;
    publishMotion();
  }

private:
  void publishMotion()
  {
    rclcpp::Rate loop_rate(10);
    while (rclcpp::ok())
    {
      geometry_msgs::msg::Twist motion_msg;
      motion_msg.linear.x = linear_velocity_;
      motion_msg.angular.z = angular_velocity_;
      turtle_publisher_->publish(motion_msg);
      loop_rate.sleep();
    }
  }
  double linear_velocity_;
  double angular_velocity_;
  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr turtle_publisher_;
};

int main(int argc, char** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<TurtlesimNode>());
  rclcpp::shutdown();
  return 0;
}