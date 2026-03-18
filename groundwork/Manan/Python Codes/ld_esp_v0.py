#!/usr/bin/env python3

import rospy
from std_msgs.msg import Int16MultiArray
from sensor_msgs.msg import Joy

class JoystickControl:
    def __init__(self):
        self.var_lift = 0  # Variable to control direction of scissor lift
        self.var_auger = 0  # Variable to control direction of auger
        self.var_magnet = 0  # Variable to control direction of magnet
        self.var_servo1 = 9000  # Servo angle as an integer (scaled by 100)
        self.var_base = 0  # Variable to control base
        self.vel_base = 0  # Velocity of base
        self.var_gripper = 0  # Variable to control gripper        
        self.msg = Int16MultiArray()

        self.sub = rospy.Subscriber("/joy", Joy, self.callback)
        self.pub = rospy.Publisher("/control_ld", Int16MultiArray, queue_size=1)

    def callback(self, data):
        xAxis1 = data.axes[0]
        yAxis1 = data.axes[1]
        xAxis2 = data.axes[3]
        yAxis2 = data.axes[4]

        r2 = data.axes[5]
        l2 = data.axes[2]

        triangle = data.buttons[2]
        cross = data.buttons[0]

        self.var_lift = 0
        self.var_auger = 0
        self.var_magnet = 0
        self.var_base = 0
        self.vel_base = 0
        self.var_gripper = 0

        if r2 < 1:
            self.var_base = 1
            self.vel_base = int(abs(r2 - 1) * 255 / 2)
        elif l2 < 1:
            self.var_base = 2
            self.vel_base = int(abs(l2 - 1) * 255 / 2)

        if yAxis1 > 0 and abs(yAxis1) > abs(xAxis1):
            self.var_lift = 1
        elif yAxis1 < 0 and abs(yAxis1) > abs(xAxis1):
            self.var_lift = -1
        elif xAxis1 > 0 and abs(xAxis1) > abs(yAxis1):
            self.var_auger = 1
        elif xAxis1 < 0 and abs(xAxis1) > abs(yAxis1):
            self.var_auger = -1

        if yAxis2 > 0 and abs(yAxis2) > abs(xAxis2):
            self.var_magnet = 1
        elif yAxis2 < 0 and abs(yAxis2) > abs(xAxis2):
            self.var_magnet = -1

        if xAxis2 > 0 and abs(xAxis2) > abs(yAxis2):
            self.var_servo1 = max(self.var_servo1 - 5, 0)  # Scaled by 100
        elif xAxis2 < 0 and abs(xAxis2) > abs(yAxis2):
            self.var_servo1 = min(self.var_servo1 + 5, 18000)  # Scaled by 100

        if triangle == 1:
            self.var_gripper = 1
        elif cross == 1:
            self.var_gripper = 2

        self.msg.data = [
            self.var_lift,
            self.var_auger,
            self.var_magnet,
            self.var_servo1 // 100,  # Scale back to integer representation
            self.var_base,
            self.vel_base,
            self.var_gripper,
        ]
        self.pub.publish(self.msg)

if __name__ == '__main__':
    rospy.init_node("joy_control", anonymous=True)
    joystick_control = JoystickControl()
    rospy.spin()