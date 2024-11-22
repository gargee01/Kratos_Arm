#!/usr/bin/env python3

import rospy
from std_msgs.msg import Float32
from sensor_msgs.msg import Joy


def joy_callback(msg:Joy):
    global pub  
    motor1=Float32()
    motor1.data= msg.axes[0]
    motor2=Float32()
    motor2.data=msg.axes[1]
    pub1.publish(motor1)
    rospy.loginfo(motor1.data)
    pub2.publish(motor2)
    rospy.loginfo(motor2.data)
    
    
if __name__=='__main__':
    rospy.init_node("kt3_node")
    pub1=rospy.Publisher('/kt3m1',Float32,queue_size=10)
    pub2=rospy.Publisher('/kt3m2',Float32,queue_size=10) 
    sub=rospy.Subscriber('/joy',Joy, callback=joy_callback)   
    rate=rospy.Rate(10)
    while not rospy.is_shutdown():
        rate.sleep()