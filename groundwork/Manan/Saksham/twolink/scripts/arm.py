#!/usr/bin/env python3

import rospy
from std_msgs.msg import Float32MultiArray
import math 
from sensor_msgs.msg import Joy
from kt_1.msg import state

class arm_move:
    def __init__(self):
        # basic setups
        rospy.init_node('Arm_Moves')
        self.rate = rospy.Rate(10)

        # the subscriptions 
        rospy.Subscriber('angles', Float32MultiArray, callback= self.callback)
        rospy.Subscriber('joy', Joy, callback=self.callbackJoy)
        

        # the initial values
        self.stateMotors = state()
        self.anglePub = rospy.Publisher('/angles', Float32MultiArray, queue_size=10)
        self.pubMotors = rospy.Publisher('/motors', state, queue_size=10)
        self.angles = [0, 0]
        self.previousAngles = [0,0]
        self.new_angle = Float32MultiArray()

        self.calibrate = 0
        self.move_on_x = 0
        self.move_on_y = 0
        self.lengths = [3.01, 4.01]


        # bruh
        while not rospy.is_shutdown():
            self.forward_kinematics()
            self.previousAngles = self.angles
            self.rate.sleep()
    
    def callbackJoy(self,msg):
        # if Y is pressed on controller

        self.calibrate = msg.buttons[3]
        self.move_on_x = msg.axes[0]

        self.move_on_y = msg.axes[1]

        if self.move_on_x >= self.move_on_y:
            self.inverse_kinematics_x()
            self.anglePub.publish(self.new_angle)

        


    def callback(self, msg):
        self.angles = msg.data 
    
    def forward_kinematics(self):
        self.x = self.lengths[0] * math.cos(self.angles[0]) + self.lengths[1] * math.cos(self.angles[0] + self.angles[1])
        self.y = self.lengths[0] * math.sin(self.angles[0]) - self.lengths[1] * math.sin(- self.angles[0] - self.angles[1])
        print(self.x, self.y)       
    
    def calibration(self):
        #for linearact1
        dir1 = [1,0]
        if self.calibrate and (self.angles - self.previousAngles) > 0:
            pass
    
    def inverse_kinematics_x(self):
        destination_y  = self.y + 0.2
        angle1 = -(math.atan(destination_y/self.x) - math.atan((4.01 * math.sin(self.angles[1]))/(3.01 + 4.01 * (math.cos(self.angles[1])))))
        #print((self.x * self.x + destination_y * destination_y - 3.01 * 3.01 - 4.01 * 4.01 )/(2 * 4.01 * 3.01))
        angle2 =  - math.acos((self.x * self.x + destination_y * destination_y - 3.01 * 3.01 - 4.01 * 4.01 )/(2 * 4.01 * 3.01))
        self.new_angle.data = [angle1, angle2]        



    

if __name__ == "__main__":
    arm_move()
