#!/usr/bin/env python3

import rospy
from sensor_msgs.msg import Joy
from kt_1.msg import state
import math

class drive:
    def __init__(self):
        rospy.init_node('Differential_drive')
        self.stateMotors = state()
        self.stateMotors.dir1 = 1
        self.stateMotors.dir2 = 1
        self.stateMotors.pwm1 = 0
        self.stateMotors.pwm2 = 0
        self.rate = rospy.Rate(10)
        self.pub = rospy.Publisher('/motors', state, queue_size= 5)
        
        rospy.Subscriber('/joy', Joy, callback= self.callback)
        while not rospy.is_shutdown():
            self.rate.sleep()

    def callback(self, msg):
        motor1 = msg.axes[0]
        motor2 = msg.axes[1]
        if motor1 > 0:
            self.stateMotors.dir1 = 1
        else:
            self.stateMotors.dir1 = 0
        
        if motor2 > 0:
            self.stateMotors.dir2 = 1
        else:
            self.stateMotors.dir2 = 0
        
        self.stateMotors.pwm1 = abs(motor1) * 255
        self.stateMotors.pwm2 = abs(motor2) * 255
        
        self.pub.publish(self.stateMotors)

                    
            
                

        

if __name__ == '__main__':
    drived = drive()