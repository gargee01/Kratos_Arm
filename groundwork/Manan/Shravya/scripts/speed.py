#!/usr/bin/env python3

import rospy
from std_msgs.msg import Float32
from sensor_msgs.msg import Joy


def joy_callback(msg:Joy):
    global pub  
    message=Float32()
    message.data= msg.axes[0]
    pub.publish(message)
    rospy.loginfo(message.data)
    
    
if __name__=='__main__':
    rospy.init_node("kt2_node")
    pub=rospy.Publisher('/kt2',Float32,queue_size=10) 
    sub=rospy.Subscriber('/joy',Joy, callback=joy_callback)   
    rate=rospy.Rate(10)
    while not rospy.is_shutdown():
        rate.sleep()




























