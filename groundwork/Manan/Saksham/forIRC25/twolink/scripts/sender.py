#!/usr/bin/env python3

#---------------------------------------------#
# This only exists for debugging purposes
#---------------------------------------------#

import rospy
from std_msgs.msg import Float32MultiArray
from math import pi
class angle_sender:
    def __init__(self):
        rospy.init_node('angle_sender')
        self.rate = rospy.Rate(10)
        self.pub = rospy.Publisher('ArmToMove', Float32MultiArray, queue_size= 5)

        self.msg = Float32MultiArray()
        
        #self.msg.data = [pi - 61.739 * pi/180, 68.162 * pi/180]
        self.pub.publish(self.msg)
        self.rate.sleep()

        while not rospy.is_shutdown():
            angle1 = float(input(""))
            angle2 = float(input(""))
            option3 = float(input(""))
            self.msg.data = [angle1 * pi/180 ,  angle2 *pi/180 , option3] 
            self.pub.publish(self.msg)
            self.rate.sleep()

    
        

if __name__ == '__main__':
    angle_sender()
