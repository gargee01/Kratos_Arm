#!/usr/bin/env python3

import rospy
from std_msgs.msg import Float32MultiArray

class WheelControl:
    def __init__(self):
        rospy.init_node('WheelControl')
        
        # Publisher for the rover topic
        self.pub = rospy.Publisher("/rover", Float32MultiArray, queue_size=10)
        self.rate = rospy.Rate(10)

        # Initialize all wheel velocities to 0
        self.wheel_velocities = [0] * 6
        self.values = Float32MultiArray()
        self.values.data = self.wheel_velocities
        
        # Parameters for wheel number and velocity
        self.wheel_number = rospy.get_param('~wheel_number', 1)  # Default to wheel 1
        self.velocity = rospy.get_param('~velocity', 0)  # Default to velocity 0

        # Set the velocity for the specified wheel number
        self.set_wheel_velocity(self.wheel_number, self.velocity)

    def set_wheel_velocity(self, wheel_number, velocity):
        # Validate wheel number and velocity
        if 1 <= wheel_number <= 6 and -100 <= velocity <= 100:
            # Set all wheel velocities to 0
            self.wheel_velocities = [0] * 6
            # Set the specified wheel's velocity
            self.wheel_velocities[wheel_number - 1] = velocity

            # Update the message and publish it
            self.values.data = self.wheel_velocities
            self.pub.publish(self.values)
        else:
            rospy.logwarn("Invalid wheel number or velocity value.")

    def spin(self):
        rospy.spin()

if __name__ == '__main__':
    # Create the wheel control object and spin
    wheel_control = WheelControl()
    wheel_control.spin()
