/*
 * commander_node.cpp
 *
 * Node "cerveau" du projet : calcule la commande de deplacement de la tortue
 * et la publie sur /trajectory_cmd. Le node "turtle_executor" (Python) se
 * charge de transformer cette commande en Twist pour turtlesim.
 */

#include <chrono>
#include <functional>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/bool.hpp"
#include "trajectory_interfaces/msg/trajectory_command.hpp"

using namespace std::chrono_literals;

class CommanderNode : public rclcpp::Node
{
public:
  CommanderNode()
  : Node("commander_node")
  {
    trajectory_publisher_ =
      this->create_publisher<trajectory_interfaces::msg::TrajectoryCommand>(
        "/trajectory_cmd", 10);

    trajectory_timer_ = this->create_wall_timer(
      100ms, std::bind(&CommanderNode::publish_trajectory, this));

    obstacle_subscription_ = this->create_subscription<std_msgs::msg::Bool>(
      "/obstacle_alert", 10,
      std::bind(&CommanderNode::obstacle_callback, this, std::placeholders::_1));

    phase_started_at_ = this->now();

    RCLCPP_INFO(this->get_logger(), "commander_node demarre.");
  }

private:
  enum class TrajectoryPhase
  {
    MOVE_RIGHT,
    TURN_UP,
    CIRCLE,
    BACK_UP,
    STOPPED
  };

  void obstacle_callback(const std_msgs::msg::Bool::SharedPtr message)
  {
    // Traiter la premiere alerte true. Les alertes suivantes ne relancent
    // pas le recul et la tortue ne repart pas apres l'arret.
    if (message->data &&
        phase_ != TrajectoryPhase::BACK_UP &&
        phase_ != TrajectoryPhase::STOPPED)
    {
      phase_ = TrajectoryPhase::BACK_UP;
      phase_started_at_ = this->now();
      RCLCPP_INFO(this->get_logger(), "Obstacle detecte : recul pendant 1 seconde.");
    }
  }

  void publish_trajectory()
  {
    constexpr double linear_speed = 1.0;
    constexpr double circle_radius = 2.75;
    constexpr double turn_speed = 1.0;
    constexpr double pi = 3.14159265358979323846;
    constexpr double backup_duration = 1.0;

    const auto current_time = this->now();
    const double elapsed = (current_time - phase_started_at_).seconds();

    if (phase_ == TrajectoryPhase::BACK_UP &&
        elapsed >= backup_duration)
    {
      phase_ = TrajectoryPhase::STOPPED;
      phase_started_at_ = current_time;
      RCLCPP_INFO(this->get_logger(), "Recul termine : tortue arretee.");
    }
    else if (phase_ == TrajectoryPhase::MOVE_RIGHT &&
             elapsed >= circle_radius / linear_speed)
    {
      phase_ = TrajectoryPhase::TURN_UP;
      phase_started_at_ = current_time;
    }
    else if (phase_ == TrajectoryPhase::TURN_UP &&
             elapsed >= (pi / 2.0) / turn_speed)
    {
      phase_ = TrajectoryPhase::CIRCLE;
      phase_started_at_ = current_time;
    }

    trajectory_interfaces::msg::TrajectoryCommand message;
    message.avoid_obstacle = false;

    switch (phase_)
    {
      case TrajectoryPhase::MOVE_RIGHT:
        message.linear_speed = linear_speed;
        message.angular_speed = 0.0;
        break;

      case TrajectoryPhase::TURN_UP:
        message.linear_speed = 0.0;
        message.angular_speed = turn_speed;
        break;

      case TrajectoryPhase::CIRCLE:
        message.linear_speed = linear_speed;
        message.angular_speed = linear_speed / circle_radius;
        break;

      case TrajectoryPhase::BACK_UP:
        message.linear_speed = -linear_speed;
        message.angular_speed = 0.0;
        message.avoid_obstacle = true;
        break;

      case TrajectoryPhase::STOPPED:
        message.linear_speed = 0.0;
        message.angular_speed = 0.0;
        message.avoid_obstacle = true;
        break;
    }

    trajectory_publisher_->publish(message);
  }

  rclcpp::Publisher<trajectory_interfaces::msg::TrajectoryCommand>::SharedPtr
    trajectory_publisher_;
  rclcpp::TimerBase::SharedPtr trajectory_timer_;
  rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr obstacle_subscription_;

  TrajectoryPhase phase_{TrajectoryPhase::MOVE_RIGHT};
  rclcpp::Time phase_started_at_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<CommanderNode>());
  rclcpp::shutdown();
  return 0;
}