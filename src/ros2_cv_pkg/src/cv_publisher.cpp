#include <cv_bridge/cv_bridge.h>
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <opencv2/opencv.hpp>

using std::placeholders::_1;

class CvPublisher : public rclcpp::Node
{
public:
  CvPublisher() : Node("src_send")
  {
    image_publisher_ = this->create_publisher<sensor_msgs::msg::Image>("pub_topic", 10);
    publisher_timer_ =
        this->create_wall_timer(std::chrono::milliseconds(1), std::bind(&CvPublisher::image_publish, this));
  }

private:
  void image_publish()
  {
    cv::Mat image = cv::imread("/home/johan/png/aruco.png", cv::IMREAD_COLOR);
    if (!image.empty())
    {
      try
      {
        // cv::imshow("src_send", image);
        // cv::waitKey(0);
        // 将opnecv图像转换成ros图像消息
        cv_bridge::CvImage cv_img(std_msgs::msg::Header(), sensor_msgs::image_encodings::BGR8, image);
        sensor_msgs::msg::Image::SharedPtr image_msg = cv_img.toImageMsg();
        image_publisher_->publish(*image_msg);
      }
      catch (cv_bridge::Exception& e)
      {
        RCLCPP_ERROR(this->get_logger(), "cv_bridge exception: %s", e.what());
      }
    }
    else
    {
      RCLCPP_INFO(this->get_logger(), "imread image error !");
    }
  }
  rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr image_publisher_;
  rclcpp::TimerBase::SharedPtr publisher_timer_;
};

int main(int argc, char** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<CvPublisher>());
  rclcpp::shutdown();
  return 0;
}