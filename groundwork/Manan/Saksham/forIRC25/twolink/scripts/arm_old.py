#!/usr/bin/env python3

import rospy
from sensor_msgs.msg import Joy
from std_msgs.msg import Float32MultiArray

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

class drive:
    def __init__(self):
        rospy.init_node('MOVING_ARM')
        self.stateMotors = Float32MultiArray()
        self.stateMotors.data = [0] * 12
        self.rate = rospy.Rate(10)
        self.pub = rospy.Publisher('/motors', Float32MultiArray, queue_size= 5)
        self.axes_x = 0
        self.axes_y = 0
        rospy.Subscriber('/joy', Joy, callback= self.callback)
        
        while not rospy.is_shutdown():
            
            if  self.axes_x ==0 and self.axes_y ==0:
                # print(self.axes_x, self.axes_y)
                self.pub.publish(self.stateMotors)
                self.rate.sleep()
            # self.rate.sleep()

    def callback(self, msg):
    
        # this is the first joy-stick 
        lin_act1 = msg.axes[0] #linear actuator1
        lin_act2 = msg.axes[1] #linear actuator2

        #this is the second
        pitch = msg.axes[3] #bevel
        yaw = msg.axes[4] #bevel
        pwm_bevel = 0 # pwm to be used for bevel


        #this is the peeche wale buttons
        arm_1 = (msg.axes[2] - 1) * -1
        arm_2 = (msg.axes[5] - 1) * -1
        pwm_arm = 0

        #this is the button
        button_1 = msg.buttons[0]
        button_2 = msg.buttons[1]

        if lin_act1 > 0:
            self.stateMotors.data[0] = 1
        else:
            self.stateMotors.data[0] = 0

        if lin_act2 > 0:
    
            self.stateMotors.data[1] = 0
        else:
            self.stateMotors.data[1] = 1

        
        if abs(pitch) > abs(yaw):
            if pitch > 0:
                self.stateMotors.data[4] = 1 
                self.stateMotors.data[5] = 0
            else:
                self.stateMotors.data[4] = 0
                self.stateMotors.data[5] = 1

            pwm_bevel = pitch

        else:
            if yaw > 0:
                self.stateMotors.data[4] = 1
                self.stateMotors.data[5] = 1
            else:
                self.stateMotors.data[4] = 0
                self.stateMotors.data[5] = 0
            
            pwm_bevel = yaw
        
        if arm_1 > arm_2:
            self.stateMotors.data[10] = 1
            pwm_arm = arm_1/2
            
        else:
            self.stateMotors.data[10] = 0
            pwm_arm = arm_2/2
        
        if (button_1 == 1) and (button_2 == 0):
            self.stateMotors.data[8] = 1
            self.stateMotors.data[9] = 175
        
        elif (button_1 == 0) and (button_2 == 1):
            self.stateMotors.data[8] = 0
            self.stateMotors.data[9] = 175
        
        else:
            self.stateMotors.data[9] = 0
        
        # print(pwm_arm)
        
        self.stateMotors.data[2] = abs(lin_act1) * 255
        self.stateMotors.data[3] = abs(lin_act2) * 255

        self.stateMotors.data[6] = (abs(pwm_bevel) * 255) / 5
        self.stateMotors.data[7] = abs(pwm_bevel) * 255 /5

        self.stateMotors.data[11] = pwm_arm * 255

        self.axes_x = msg.axes[6]
        self.axes_y = msg.axes[7]
        


                    
            
                

        

if __name__ == '__main__':
    drived = drive()