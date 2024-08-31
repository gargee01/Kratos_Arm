#!/usr/bin/env python3

import rospy
from sensor_msgs.msg import Joy
from kt_1.msg import state

class drive:
    def __init__(self):
        rospy.init_node('MOVING_ARM')
        self.stateMotors = state()
        self.rate = rospy.Rate(10)
        self.pub = rospy.Publisher('/motors', state, queue_size= 5)
        
        rospy.Subscriber('/joy', Joy, callback= self.callback)
        
        while not rospy.is_shutdown():
            self.rate.sleep()

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
            self.stateMotors.dir_actuator1 = 1
        else:
            self.stateMotors.dir_actuator1 = 0

        if lin_act2 > 0:
    
            self.stateMotors.dir_actuator2 = 1
        else:
            self.stateMotors.dir_actuator2 = 0

        
        if abs(pitch) > abs(yaw):
            if pitch > 0:
                self.stateMotors.dir_bevel1 = 1 
                self.stateMotors.dir_bevel2 = 0
            else:
                self.stateMotors.dir_bevel1 = 0
                self.stateMotors.dir_bevel2 = 1

            pwm_bevel = pitch

        else:
            if yaw > 0:
                self.stateMotors.dir_bevel1 = 1
                self.stateMotors.dir_bevel2 = 1
            else:
                self.stateMotors.dir_bevel1 = 0
                self.stateMotors.dir_bevel2 = 0
            
            pwm_bevel = yaw
        
        if arm_1 > arm_2:
            self.stateMotors.dir_base = 1
            pwm_arm = arm_1/2
            
        else:
            self.stateMotors.dir_base = 0
            pwm_arm = arm_2/2
        
        if (button_1 == 1) and (button_2 == 0):
            self.stateMotors.gripper = 1
            self.stateMotors.grip_opener = 175
        
        elif (button_1 == 0) and (button_2 == 1):
            self.stateMotors.gripper = 0
            self.stateMotors.grip_opener = 175
        
        else:
            self.stateMotors.grip_opener = 0
        
        # print(pwm_arm)
        self.stateMotors.pwm_actuator1 = abs(lin_act1) * 255
        self.stateMotors.pwm_actuator2 = abs(lin_act2) * 255

        self.stateMotors.pwm_bevel1 = (abs(pwm_bevel) * 255) / 3
        self.stateMotors.pwm_bevel2 = abs(pwm_bevel) * 255

        self.stateMotors.pwm_base = pwm_arm * 255

        self.pub.publish(self.stateMotors)

                    
            
                

        

if __name__ == '__main__':
    drived = drive()