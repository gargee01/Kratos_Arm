#!/usr/bin/env python3

import rospy
from std_msgs.msg import Float32MultiArray
import math 
from sensor_msgs.msg import Joy


# [0] dir_actuator1 - ASSUMPTION: {L1, 1: EXTEND, 0: RETRACT }
# [1]dir_actuator2  - ASSUMPTION: {L2, 1: EXTEND, 0: RETRACT }
# [2] pwm_actuator1 
# [3] pwm_actuator2
# [4] dir_bevel1
# [5] dir_bevel2
# [6] pwm_bevel1
# [7] pwm_bevel2
# [8] gripper 
# [9] grip_opener 
# [10] dir_base
# [11] pwm_base


class arm_move:
    def __init__(self):
        # basic setups
        rospy.init_node('Arm_Moves')
        self.rate = rospy.Rate(10)

        # the subscriptions 
        rospy.Subscriber('Arm_angles', Float32MultiArray, callback= self.callback)
        rospy.Subscriber('joy', Joy, callback=self.callbackJoy)
        

        # the initial values
        self.motor_values = Float32MultiArray()
        self.motor_values.data = [0] *12
        # self.anglePub = rospy.Publisher('/angles', Float32MultiArray, queue_size=10)
        self.pubMotors = rospy.Publisher('/motors', Float32MultiArray, queue_size=10)
        self.angles = [0, 0]
        self.previousAngles = [0,0]
        self.new_angle = Float32MultiArray()

        self.calibrate = 0
        self.move_on_x = 0
        self.move_on_y = 0
        self.lengths = [4.04, 5.76]


        # bruh
        while not rospy.is_shutdown():
            self.forward_kinematics()
            self.previousAngles = self.angles
            if self.move_on_x:
                self.inverse_kinematics_x(self.static_y)
                # self.anglePub.publish(self.new_angle)
                
            elif self.move_on_y:
                self.inverse_kinematics_y(self.static_x)    
                # self.anglePub.publish(self.new_angle)
            
            else:
                self.motor_values.data[2] = 0
                self.motor_values.data[3] = 0
            
            self.pubMotors.publish(self.motor_values)

                        
            self.rate.sleep()
    
    def callbackJoy(self,msg):
        # if Y is pressed on controller
        self.static_x, self.static_y = self.forward_kinematics()
        self.calibrate = msg.buttons[3]
        self.move_on_x = msg.axes[6]

        self.move_on_y = msg.axes[7]

        


    def callback(self, msg):
        self.angles = msg.data 
    
    def forward_kinematics(self):

        self.x = self.lengths[0] * math.cos(self.angles[0]) + self.lengths[1] * math.cos(self.angles[0] + self.angles[1])
        self.y = self.lengths[0] * math.sin(self.angles[0]) + self.lengths[1] * math.sin( self.angles[0] + self.angles[1])
        return self.x, self.y      
    
    def calibration(self):
        #for linearact1
        dir1 = [1,0]
        if self.calibrate and (self.angles - self.previousAngles) > 0:
            pass
    
    def inverse_kinematics_y(self,static_x):
        self.forward_kinematics()
        l1, l2 = self.lengths
        destination_y  = self.y + self.move_on_y *0.1
        angle1 = (math.atan(destination_y/static_x) - math.atan((l2 * math.sin(self.angles[1]))/(l1 + l2* (math.cos(self.angles[1])))))
        angle2 =  -math.acos((static_x* static_x + destination_y * destination_y - l1 **2 - l2**2 )/(2 * l2 * l1))
        # print(angle1, angle2)
        self.new_angle.data = [angle1, angle2]
        diff1  = angle1 - self.angles[0]
        diff2 =  angle2 - self.angles[1]
        ratio = diff1/diff2


        if (self.move_on_y > 0 and diff1 > 0 and diff2 > 0):
            self.motor_values.data[0] = 0
            self.motor_values.data[1] = 0
            if abs(ratio) > 1:
                self.motor_values.data[2] = 200
                self.motor_values.data[3] = 200/ratio
            else:
                self.motor_values.data[3] = 200
                self.motor_values.data[2] = 200/ratio
        
        elif (self.move_on_y < 0 and diff1 < 0 and diff2 < 0):
            self.motor_values.data[0] = 1
            self.motor_values.data[1] = 1
            if abs(ratio) > 1:
                self.motor_values.data[2] = 200
                self.motor_values.data[3] = 200/ratio
            else:
                self.motor_values.data[3] = 200
                self.motor_values.data[2] = 200/ratio   
        else:
            self.motor_values.data[2] = 0
            self.motor_values.data[3] = 0




    def inverse_kinematics_x(self,static_y):
        self.forward_kinematics()
        l1, l2 = self.lengths
        destination_x  = self.x - self.move_on_x *0.1
        angle1 = (math.atan(static_y/destination_x) - math.atan((l2 * math.sin(self.angles[1]))/(l1 + l2* (math.cos(self.angles[1])))))
        angle2 =  -math.acos((destination_x * destination_x + static_y * static_y - l1 **2 - l2**2 )/(2 * l2 * l1))
        # print(angle1, angle2)
        self.new_angle.data = [angle1, angle2]      
        diff1  = angle1 - self.angles[0]
        diff2 =  angle2 - self.angles[1]
        ratio = diff1/diff2


        if (self.move_on_x > 0 and diff1 > 0 and diff2 < 0):
            self.motor_values.data[0] = 0
            self.motor_values.data[1] = 1
            if abs(ratio) > 1:
                self.motor_values.data[2] = 200
                self.motor_values.data[3] = 200/ratio
            else:
                self.motor_values.data[3] = 200
                self.motor_values.data[2] = 200/ratio
        
        elif (self.move_on_x < 0 and diff1 < 0 and diff2 > 0):
            self.motor_values.data[0] = 1
            self.motor_values.data[1] = 0
            if abs(ratio) > 1:
                self.motor_values.data[2] = 200
                self.motor_values.data[3] = 200/ratio
            else:
                self.motor_values.data[3] = 200
                self.motor_values.data[2] = 200/ratio   

        else:
            self.motor_values.data[2] = 0
            self.motor_values.data[3] = 0

 



    

if __name__ == "__main__":
    arm_move()
