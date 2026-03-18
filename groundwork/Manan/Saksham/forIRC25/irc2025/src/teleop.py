#!/usr/bin/env python3

import rospy
from std_msgs.msg import Float32MultiArray
from sensor_msgs.msg import Joy
import math 
import time


class drive:
    def __init__(self):
        rospy.init_node('Drive')
        self.rate = rospy.Rate(10)
        self.rover_x = 0
        self.rover_z = 0 
        rospy.Subscriber("/joy0", Joy, self.callback)

        self.values = Float32MultiArray()
        self.actualValues = [0] *6
        self.values.data = self.actualValues
        self.pub = rospy.Publisher("/rover", Float32MultiArray,queue_size=10 )
        while not rospy.is_shutdown():
            self.actualValues = self.move()
            # print(self.actualValues)
            self.values.data = self.actualValues
            self.pub.publish(self.values)
            self.rate.sleep()

            self.rate.sleep()
    def teleop(self, linear, rotational, speed):
        # print(linear, " ", rotational, " ", speed)
        
        self.rover_x=(linear*((speed+1)/2)+rotational*((speed+1)/2))*100
        self.rover_z=(linear*((speed+1)/2)-rotational*((speed+1)/2))*100

        # Calculate magnitude of the vector
        magnitude = math.sqrt(self.rover_x**2 + self.rover_z**2)

        # Map to circular space if magnitude exceeds 100
        if magnitude > 100:
            scale_factor = 100 / magnitude
            self.rover_x *= scale_factor
            self.rover_z *= scale_factor


        # self.rover_x = (linear + rotational)*speed*100
        # self.rover_z = (linear - rotational) * speed * 100
        # print(self.rover_x, self.rover_z)

    
    def callback(self, msg):
        # print('eh')
        if msg.axes[1]>0.1 or msg.axes[1] < -0.1 or msg.axes[0] > 0.1 or msg.axes[0] < -0.1:
            self.teleop(msg.axes[1], msg.axes[0], msg.axes[2])
            # print('eh')
        
        else:
            self.rover_x = 0
            self.rover_z = 0
    
    def move(self):
        velx=self.rover_x*0.7
        velz=self.rover_z*0.7

        # if (velx > 100 or velz > 100):
        #    velx = 100
        #    velz = 100
        
        # if(velx > 100):
        #     velx = 100
        # if(velx < -100):
        #     velx = -100
        # if(velz > 100):
        #     velz = 100
        # if(velz < -100):
        #     velz = -100

        

        print(velx, " ", velz)

        if (velx >= 0 and velz>=0):
            left_wheel_front=(((velx+velz)/2+(velx-velz)/2)*100.0)/151.0 #//* (100/131) 
            right_wheel_front=(((velx+velz)/2-(velx-velz)/2)*100.0)/144.0# //* (100/142.1) 
            right_wheel_mid=(((velx+velz)/2+(velx-velz)/2)*100.0)/147.5 #//* (100/130)
            left_wheel_mid=(((velx+velz)/2-(velx-velz)/2)*100.0)/158.0 #//* (100/142)
            right_wheel_back=(((velx+velz)/2+(velx-velz)/2)*100.0)/76.0 #//* (100/113.1)
            left_wheel_back=(((velx+velz)/2-(velx-velz)/2)*100.0)/164.0 #//* (100/151)
        
        if(velx<=0 and velz<=0):
            right_wheel_front=((velx+velz)/2+(velx-velz)/2) * (100.0/148.0)
            left_wheel_front=((velx+velz)/2-(velx-velz)/2) * (100.0/147.3)
            right_wheel_mid=((velx+velz)/2+(velx-velz)/2) * (100.0/151.4)
            left_wheel_mid=((velx+velz)/2-(velx-velz)/2) * (100.0/152.35)
            right_wheel_back=((velx+velz)/2+(velx-velz)/2) * (100.0/86.0)
            left_wheel_back=((velx+velz)/2-(velx-velz)/2) * (100.0/149.2)
        
        if(velx>=0 and velz<=0):
            right_wheel_front=((velx+velz)/2+(velx-velz)/2) * (100.0/147.3)
            left_wheel_front=((velx+velz)/2-(velx-velz)/2) * (100.0/144.0)
            right_wheel_mid=((velx+velz)/2+(velx-velz)/2) * (60.0/147.5)
            left_wheel_mid=((velx+velz)/2-(velx-velz)/2) * (60.0/152.35)
            right_wheel_back=((velx+velz)/2+(velx-velz)/2)* (100.0/76.0)
            left_wheel_back=((velx+velz)/2-(velx-velz)/2) * (100.0/149.2)
         
        if(velx<=0 and velz>=0):
            right_wheel_front=((velx+velz)/2+(velx-velz)/2) * (100.0/151.0)
            left_wheel_front=((velx+velz)/2-(velx-velz)/2) * (100.0/148.0)
            right_wheel_mid=((velx+velz)/2+(velx-velz)/2) * (60.0/151.4)
            left_wheel_mid=((velx+velz)/2-(velx-velz)/2) * (60.0/158.0)
            right_wheel_back=((velx+velz)/2+(velx-velz)/2) * (100.0/86.0)
            left_wheel_back=((velx+velz)/2-(velx-velz)/2) * (100.0/164.0)
         
        if (velx==0 and velz==0):
            right_wheel_front=0
            left_wheel_front=0
            right_wheel_mid=0
            left_wheel_mid=0
            right_wheel_back=0
            left_wheel_back=0
        
        # if(right_wheel_front > 100):
        #     right_wheel_front = 100
        # if(right_wheel_front < -100):
        #     right_wheel_front = -100
        # if(left_wheel_front > 100):
        #     left_wheel_front = 100
        # if(left_wheel_front < -100):
        #     left_wheel_front = -100
        
        # if(right_wheel_mid > 100):
        #     right_wheel_mid = 100
        # if(right_wheel_mid < -100):
        #     right_wheel_mid = -100
        # if(left_wheel_mid > 100):
        #     left_wheel_mid = 100
        # if(left_wheel_mid < -100):
        #     left_wheel_mid = -100
        
        # if(right_wheel_back > 100):
        #     right_wheel_back = 100
        # if(right_wheel_back < -100):
        #     right_wheel_back = -100
        # if(left_wheel_back > 100):
        #     left_wheel_back = 100
        # if(left_wheel_back < -100):
        #     left_wheel_back = -100

        # List of all wheel speeds
        wheel_speeds = [
            right_wheel_front,
            left_wheel_front,
            right_wheel_mid,
            left_wheel_mid,
            right_wheel_back,
            left_wheel_back,
        ]

        # Find the maximum absolute speed
        max_speed = max(abs(speed) for speed in wheel_speeds)

        # If the maximum speed exceeds the allowed limit
        if max_speed > 70:
            scale_factor = 70 / max_speed  # Calculate scale factor
            wheel_speeds = [speed * scale_factor for speed in wheel_speeds]

            # Update individual wheel variables
            right_wheel_front, left_wheel_front, right_wheel_mid, left_wheel_mid, right_wheel_back, left_wheel_back = wheel_speeds


        return right_wheel_front, left_wheel_front, right_wheel_mid, left_wheel_mid, right_wheel_back, left_wheel_back
        

        
if __name__ == '__main__':
    drive()