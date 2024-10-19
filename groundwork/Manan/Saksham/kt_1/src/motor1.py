#!/usr/bin/env python3


import rospy
from std_msgs.msg import Int32



if __name__ == '__main__':
    
    rospy.init_node('Publishing_details_motor1')

    rate = rospy.Rate(10)
    pub = rospy.Publisher('/motor1', Int32, queue_size=10 )
    inp = 0
    while not rospy.is_shutdown():
        msg = Int32()
        inp = int(input('Enter state '))
        msg.data = inp
        pub.publish(msg)

        rate.sleep()
