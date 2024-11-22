#!/usr/bin/env python3

import rospy
from std_msgs.msg import Int32
from sensor_msgs.msg import Joy


def joy_callback(msg:Joy):
    global pub
    if msg.buttons[1]==1:
        message.data=1
    elif msg.buttons[3]==1:
        message.data=2
    else:
        message.data=3
    pub.publish(message)
    rospy.loginfo(message.data)
    
    
if __name__=='__main__':
    rospy.init_node("kt1_node")
    pub=rospy.Publisher('/kt1',Int32,queue_size=10) 
    sub=rospy.Subscriber('/joy',Joy, callback=joy_callback)
    message=Int32()
    rate=rospy.Rate(10)
    while not rospy.is_shutdown():
        rate.sleep()








































































