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
        self.toSendAngles = Float32MultiArray()
        self.toSendAngles.data = [0,0,0]
        
        self.pubToMoveAngles = rospy.Publisher('ArmToMove', Float32MultiArray, queue_size=10)

        self.angles = [0, 0]
        self.previousAngles = [0,0]

        self.calibrate = 0
        self.move_on_x = 0
        self.move_on_y = 0
        self.lengths = [4.04, 5.76]

        while not rospy.is_shutdown():
            self.forward_kinematics()
            self.previousAngles = self.angles
            if self.move_on_x != 0 :
                # print(self.angles, self.toSendAngles.data[0])
                if  abs( self.angles[0] - self.toSendAngles.data[0]) <= 0.07 and abs(self.angles[1] - self.toSendAngles.data[1]) >= -0.07:
                    
                    self.toSendAngles.data[0], self.toSendAngles.data[1] = self.inverse_kinematics_y()
                    self.toSendAngles.data[2] = 1
            else:
                self.toSendAngles.data[0] = self.angles[0]
                self.toSendAngles.data[1] = self.angles[1]
                self.toSendAngles.data[2] = 0
            
            if self.move_on_y !=0:
                if  abs( self.angles[0] - self.toSendAngles.data[0]) <= 0.0000000004 and abs(self.angles[1] - self.toSendAngles.data[1]) >= -0.0000000004:
                    
                    self.toSendAngles.data[0], self.toSendAngles.data[1] = self.inverse_kinematics_x()
                    self.toSendAngles.data[2] = 1
            else:
                self.toSendAngles.data[0] = self.angles[0]
                self.toSendAngles.data[1] = self.angles[1]
                self.toSendAngles.data[2] = 0

                
            self.pubToMoveAngles.publish(self.toSendAngles)
            self.rate.sleep()

        

    def callbackJoy(self,msg):
        # if Y is pressed on controller
        
        self.calibrate = msg.buttons[2]
        self.move_on_x = msg.axes[7]
        self.move_on_y = msg.axes[6]
        self.done = 0
        if self.move_on_x or self.move_on_y and self.done == 0:
            self.static_x, self.static_y = self.forward_kinematics()
            self.done = 1
        else:
            self.done = 0

    def callback(self, msg):
        self.angles = msg.data 
    
    def forward_kinematics(self):
        self.x = self.lengths[0] * math.cos(self.angles[0]) + self.lengths[1] * math.cos(self.angles[0] +  self.angles[1] - math.pi)
        self.y = self.lengths[0] * math.sin(self.angles[0]) + self.lengths[1] * math.sin( self.angles[0] + self.angles[1] - math.pi)
        return self.x, self.y  
    
    def inverse_kinematics_y(self):
        self.forward_kinematics()
        l1, l2 = self.lengths

        destination_y = self.y + self.move_on_x *0.3

        # print(self.x, self.y, destination_y, self.static_x)
        try: 
            angle1 = (math.atan(destination_y/self.static_x) + math.atan((l2 * math.sin(math.pi - self.angles[1]))/(l1 + l2* (math.cos(math.pi - self.angles[1])))))
            angle2 =  math.pi -math.acos((self.static_x* self.static_x + destination_y * destination_y - l1 **2 - l2**2 )/(2 * l2 * l1))
            print(angle1, angle2)
            return angle1, angle2
        except:
            self.toSendAngles.data[2] == 0
            return self.angles, self.angles
        
    
    def inverse_kinematics_x(self):
        self.forward_kinematics()
        l1, l2 = self.lengths

        destination_x = self.x + self.move_on_y *0.3

        # print(self.x, self.y, destination_y, self.static_x)
        try:
            angle1 = (math.atan(self.static_y/destination_x) + math.atan((l2 * math.sin(math.pi - self.angles[1]))/(l1 + l2* (math.cos(math.pi - self.angles[1])))))
            angle2 =  math.pi -math.acos((destination_x* destination_x + self.static_y * self.static_y - l1 **2 - l2**2 )/(2 * l2 * l1))
            print(angle1, angle2)
            return angle1, angle2
        except:
            self.toSendAngles.data[2] == 0
            return self.angles, self.angles
    
def calibrate(self):
   
    rospy.loginfo("Starting calibration process...")
    
    # Set motor values for calibration (fully retract)
    self.motorValues = Float32MultiArray()
    self.motorValues.data = [0, 0, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0]  # Default values

    # Publish angles and motor values until calibration completes
    while not rospy.is_shutdown():
        # Adjust target angles for calibration
        self.toSendAngles.data = [self.angles[0] + 100, self.angles[1] - 100, 1]

        # Publish target angles
        self.pubToMoveAngles.publish(self.toSendAngles)

        # Check if the arm has stopped moving (indicating calibration complete)
        if abs(self.previousAngles[0] - self.angles[0]) < 0.01 and abs(self.previousAngles[1] - self.angles[1]) < 0.01:
            rospy.loginfo("Calibration complete. Arm is in zero position.")
            self.motorValues.data[2] = 0  # Stop the motors
            self.motorValues.data[3] = 0
            self.pubToMoveAngles.publish(self.toSendAngles)  # Publish final angles
            self.pubMotors.publish(self.motorValues)  # Stop motors
            break
        else:
            # Update motor values and continue moving
            self.pubMotors.publish(self.motorValues)

        self.previousAngles = self.angles.copy()
        self.rate.sleep()

    


        
if __name__ == "__main__":
    arm_move()

        
