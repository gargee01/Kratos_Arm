#!/usr/bin/env python3

import rospy
from std_msgs.msg import Float32MultiArray
from sensor_msgs.msg import Joy
from math import pi

class angle_sender:
    def __init__(self):
        rospy.init_node('calibration_angles')
        self.rate = rospy.Rate(0.5)
        self.pub = rospy.Publisher('calibrated_angles', Float32MultiArray, queue_size= 5)
        rospy.Subscriber('angles', Float32MultiArray, self.callbackAngles)
        rospy.Subscriber("joy", Joy, self.callback)
        self.angles = [0,0]

        self.msg = Float32MultiArray()
        self.msg.data = [ 0 , 0]
        
       
        while not rospy.is_shutdown():
            self.pub.publish(self.msg)
            self.rate.sleep()
    

    def callback(self, msg):
        if msg.buttons[5] == 1:
            angle_change = list(self.angles)
            angle_change[1] -= 7
            # print(angle_change)
            
            self.msg.data = angle_change
            # self.msg.data[1] -= 7
    
    def callbackAngles(self, msg):
        self.angles = msg.data
    


    
        

if __name__ == '__main__':
    angle_sender()
