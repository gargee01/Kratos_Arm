import rospy
from std_msgs.msg import Float32MultiArray
import math 
from sensor_msgs.msg import Joy


class calibrate_Start:
    def __init__(self):

        rospy.Subscriber('angles', Float32MultiArray,callback=self.callback)
        self.pubMotors = rospy.Publisher('/motors', Float32MultiArray, queue_size=10)
        # the initial values
        self.motor_values = Float32MultiArray()
        self.motor_values.data = [0] *12
        self.angles = [0,0]
        self.previousAngles = [0,0]
        self.done = 0 
        
        
            
            

    def callback(self,msg):
        self.previousAngles = self.angles

        self.angles = msg.data
    
    

    
    