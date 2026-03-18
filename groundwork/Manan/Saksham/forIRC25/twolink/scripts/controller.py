#!/usr/bin/env python3

import rospy
from sensor_msgs.msg import JointState
from std_msgs.msg import Float32MultiArray
from math import pi

class display:
    def __init__(self):
        rospy.init_node('DISPLAY')
        self.rate = rospy.Rate(10)
        self.pub = rospy.Publisher('/joint_states', JointState, queue_size= 5)
        rospy.Subscriber('Arm_angles', Float32MultiArray, callback=self.callback)
        self.joint_state = JointState()
        self.joint_state.header.stamp = rospy.Time.now()
        self.joint_state.name = ['baseHinge', 'interArm']
        self.joint_state.position = [0, 0]
        self.pub.publish(self.joint_state)

        while not rospy.is_shutdown():
            self.rate.sleep()
        


    def callback(self,msg):
        T = rospy.get_param('~period')
        #R = rospy.get_param('~controller_pub_rate')

        
        self.joint_state.header.stamp = rospy.Time.now()
        self.joint_state.name = ['baseHinge', 'interArm']
        self.joint_state.position = [msg.data[0], msg.data[1] - pi]
        self.pub.publish(self.joint_state)
        
if __name__ == '__main__':
    display()
