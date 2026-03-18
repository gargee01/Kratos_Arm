#!/usr/bin/env python3

import rospy
from std_msgs.msg import Float32MultiArray
from sensor_msgs.msg import Joy
import math 
import time

# from sensor_msgs.msg import Joy


class send_angles:
    def __init__(self):
        rospy.init_node('Seek_angles')

        self.rate = rospy.Rate(10)

        rospy.Subscriber('ArmToMove', Float32MultiArray, self.callback)
        rospy.Subscriber('Arm_angles', Float32MultiArray, self.callbackAngles)
        rospy.Subscriber('joy', Joy, self.callbackJoystick)
        self.currentAngles = Float32MultiArray()
        self.currentAngles.data = [0,0]
        self.angleValues = Float32MultiArray()
        self.angleValues.data = [0,0,0]

        self.motor_values = Float32MultiArray()
        self.motor_values.data = [0] *12
        self.pubMotors = rospy.Publisher('/motors', Float32MultiArray, queue_size=10)
        self.start = 1
        self.startTime = 0
        self.currentTime = 0
        self.move_on_x = 0 
        self.move_on_y = 0
        while not rospy.is_shutdown():
            if not (self.move_on_x == 0 and self.move_on_y == 0):
                # print("hello")
                self.in_use = 0
                self.currentTime = time.time()
                if self.angleValues.data[2] == 1:
                    if self.start == 1:
                        self.startTime = time.time()
                        self.start = 0
                    else:
                        self.start = 1
                    self.move_Arm()
                else:
                    self.motor_values.data[2] = 0
                    self.motor_values.data[3] = 0
            
            
                self.pubMotors.publish(self.motor_values)
                self.rate.sleep()
            
            
    
    def callbackJoystick(self, msg):
        self.move_on_x = msg.axes[7]
        self.move_on_y = msg.axes[6]
        
    def callback(self, msg):
        self.angleValues.data = msg.data 
        


    def callbackAngles(self,msg):
        self.currentAngles.data = msg.data
    
    def move_Arm(self):
        
        
        diff1 = self.angleValues.data[0] - self.currentAngles.data[0]
        diff2 = self.angleValues.data[1] - self.currentAngles.data[1]   
        print(diff1, diff2)
        timediff = self.currentTime - self.startTime
        # print(diff1, " ", diff2)
        if diff1 < -0.015:
            print('Moving angle 1 DOWN')
            self.motor_values.data[0] = 1
            self.motor_values.data[2] = 250 #* abs(diff1) *80 + 200 *abs(diff1) * timediff * 10000

        elif diff1 > 0.015:
            print('Moving angle 1 UP')

            self.motor_values.data[0] = 0
            self.motor_values.data[2] = 250 #* abs(diff1) *80 + 200*abs(diff1) * timediff * 10000
        else:
            # print('bruh')
            self.motor_values.data[2] = 0 

        if diff2 < -0.00015:
            print('Moving angle 2 UP')

            self.motor_values.data[1] = 1
            self.motor_values.data[3] = 120 # * abs(diff2) *10 + abs(diff2) * timediff * 100

        elif diff2 > 0.00015:
            print('Moving angle 2 DOWN')

           

            self.motor_values.data[1] = 0
            self.motor_values.data[3] = 120 # *abs(diff2) *10+ abs(diff2) * timediff * 100
        else:
            
            self.motor_values.data[3] = 0 

if __name__ == '__main__':
    send_angles()