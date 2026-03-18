#!/usr/bin/env python3

import rospy
from std_msgs.msg import Float32MultiArray
import math 
# from sensor_msgs.msg import Joy


class send_angles:
    def __init__(self):
        rospy.init_node('Seek_angles')

        self.rate = rospy.Rate(10)

        rospy.Subscriber('ArmToMove', Float32MultiArray, self.callback)
        rospy.Subscriber('Arm_angles', Float32MultiArray, self.callbackAngles)
        self.currentAngles = Float32MultiArray()
        self.currentAngles.data = [0,0]
        self.angleValues = Float32MultiArray()
        self.angleValues.data = [0,0,0]

        self.motor_values = Float32MultiArray()
        self.motor_values.data = [0] *12
        self.pubMotors = rospy.Publisher('/motors', Float32MultiArray, queue_size=10)

        while not rospy.is_shutdown():
            if self.angleValues.data[2] == 1:
                
                self.move_Arm()
            else:
                self.motor_values.data[2] = 0
                self.motor_values.data[3] = 0
            
            self.pubMotors.publish(self.motor_values)
            self.rate.sleep()
        
    def callback(self, msg):
        self.angleValues.data = msg.data 
        


    def callbackAngles(self,msg):
        self.currentAngles.data = msg.data
    
    def move_Arm(self):

        diff1 = self.angleValues.data[0] - self.currentAngles.data[0]
        diff2 = self.angleValues.data[1] - self.currentAngles.data[1]   
        print(diff1, " ", diff2)
        if diff1 < -0.009:
            # print('Moving angle 1 UP')
            self.motor_values.data[0] = 1
            self.motor_values.data[2] = 255

        elif diff1 > 0.009:
            # print('Moving angle 1 DOWN')

            self.motor_values.data[0] = 0
            self.motor_values.data[2] = 255
        else:
            self.motor_values.data[2] = 0 

        if diff2 < -0.009:
            # print('Moving angle 2 UP')

            self.motor_values.data[1] = 1
            self.motor_values.data[3] = 120

        elif diff2 > 0.009:
            # print('Moving angle 2 down')

            self.motor_values.data[1] = 0
            self.motor_values.data[3] = 120
        else:
            self.motor_values.data[3] = 0 

if __name__ == '__main__':
    send_angles()