#!/usr/bin/env python3

import rospy

from std_msgs.msg import Int32

import serial

def callback(msg):
    global state
    state = msg.data

if __name__ == '__main__':
    arduino = serial.Serial(port = '/dev/ttyUSB0', baudrate=115200, timeout=0.1)

    rospy.init_node('asdfghj')
    rate = rospy.Rate(10)
    rospy.Subscriber('motor1', Int32, callback=callback)
    arduino.write()
    
