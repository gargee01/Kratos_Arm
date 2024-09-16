#!/usr/bin/env python3

import rospy
from std_msgs.msg import Float32MultiArray
import tf
import math

class imu:
    def __init__(self):
        rospy.init_node('blah')
        self.rate = rospy.Rate(10)
        self.br = tf.TransformBroadcaster()
        self.sub = rospy.Subscriber('/angles', Float32MultiArray, self.callb)
        rospy.spin()
    
    def callb(self, msg):
        x = msg.data[0]/math.pi
        y  = msg.data[1]/math.pi
        z = msg.data[2]/math.pi
        (x,y,z,w) = tf.transformations.quaternion_from_euler(x, y, z)
        self.br.sendTransform((0, 0, 0),  # Translation (optional)
                 (x, y, z, w),
                 rospy.Time.now(),
                 "imu_frame",
                 "world")
# Inside your callback:
if __name__ == '__main__':
    imu()
