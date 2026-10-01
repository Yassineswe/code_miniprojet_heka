"""
executor_node.py

Node "tortue" du projet : ecoute les commandes de haut niveau publiees par
le node "cerveau" (C++, brain_commander) sur /trajectory_cmd, et les
transforme en Twist envoye a turtlesim sur /turtle1/cmd_vel.
"""

import rclpy
from rclpy.node import Node
from geometry_msgs.msg import Twist
from trajectory_interfaces.msg import TrajectoryCommand


class ExecutorNode(Node):

    def __init__(self):
        super().__init__('executor_node')

        self.publisher_ = self.create_publisher(
            Twist,
            '/turtle1/cmd_vel',
            10
        )

        self.subscription = self.create_subscription(
            TrajectoryCommand,
            '/trajectory_cmd',
            self.command_callback,
            10
        )

        self.obstacle_alert_logged = False

        self.get_logger().info('executor_node demarre, en attente de /trajectory_cmd...')

    def command_callback(self, msg):
        twist = Twist()
        twist.linear.x = msg.linear_speed
        twist.angular.z = msg.angular_speed

        self.publisher_.publish(twist)

        if msg.avoid_obstacle and not self.obstacle_alert_logged:
            self.get_logger().info('Obstacle signale.')
            self.obstacle_alert_logged = True
        elif not msg.avoid_obstacle:
            self.obstacle_alert_logged = False


def main(args=None):
    rclpy.init(args=args)
    node = ExecutorNode()

    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == '__main__':
    main()