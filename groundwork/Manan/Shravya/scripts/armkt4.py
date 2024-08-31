#!/usr/bin/env python3

import rospy
from std_msgs.msg import Float32MultiArray, Int32MultiArray
from sensor_msgs.msg import Joy

def joy_callback(msg: Joy):
    global pub1, pub2  

    
    joystickbuttons = Int32MultiArray()
    joystickbuttons.data = [0] * 2
    joystickbuttons.data[0] = msg.buttons[1]
    joystickbuttons.data[1] = msg.buttons[2]   
    joystickaxes = Float32MultiArray()
    joystickaxes.data = [0.0] * len(msg.axes) 
    
    for i in range(min(5, len(msg.axes))):  
        joystickaxes.data[i] = msg.axes[i]
    
    
    pub1.publish(joystickaxes)
    pub2.publish(joystickbuttons)

if __name__ == '__main__':
    rospy.init_node("kt4_node")
    
    pub1 = rospy.Publisher('/armkt4axes', Float32MultiArray, queue_size=10)
    pub2 = rospy.Publisher('/armkt4button', Int32MultiArray, queue_size=10)
    
    sub = rospy.Subscriber('/joy', Joy, joy_callback)   
    rate = rospy.Rate(10)
    
    while not rospy.is_shutdown():
        rate.sleep()

