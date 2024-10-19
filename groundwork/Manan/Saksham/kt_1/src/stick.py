#!/usr/bin/env python3

import rospy
from std_msgs.msg import Float32
from sensor_msgs.msg import Joy


class joystick:
    def __init__(self):
        rospy.init_node('joystick')
        self.state = Float32()
        self.state.data = 0.0
        
        self.pub = rospy.Publisher('/motor1', Float32, queue_size= 5)
        self.rate = rospy.Rate(10)
        rospy.Subscriber('/joy', Joy, callback=self.callback)
        
        self.pub.publish(self.state)
        while not rospy.is_shutdown():
            self.rate.sleep()

    def callback(self, msg):
        self.pub.publish(self.state)
        self.state = msg.axes[0]
        
        
    
if __name__ == '__main__':
    joyst = joystick()

