#!/usr/bin/env python3
import rospy
from std_msgs.msg import Int16
from sensor_msgs.msg import Joy
a=[]
b=[]
c=[0,0,0,0,0]
global n
global prev1
prev1=0
global prev2
prev2=0
global prev3
prev3=0
global prev4
prev4=0
global prev5
prev5=0
global prev6
prev6=0
global prev7
prev7=0
global prev8
prev8=0
global prev9
prev9=0
global prev10
prev10=0

def callback(data: Joy):
    global b
    global angle 
    global angle2
    global angle3
    global angle4
    global angle5
    b=data.axes
    global prev1
    global prev2
    global prev3
    global prev4
    global prev5
    global prev6
    global prev7
    global prev8
    global prev9
    global prev10
    global a
    a=data.buttons
    global c
  
    
    if b[7]>=0.5 and angle>0 and prev1==0:
        angle=angle-5
        prev1=1

    
    if b[7]<=-0.5 and angle<180 and prev2==0:
        angle=angle+5
        prev2=1

    if b[9]==-1 and angle2>0 and prev3==0:
        angle2=angle2-5
        prev3=1
    
    if b[9]==1 and angle2<180 and prev4==0:
        angle2=angle2+5
        prev4=1

    if b[8]==1 and angle3>0 and prev5==0:
        angle3=angle3-5
        prev5=1
    
    if b[8]==-1 and angle3<180 and prev6==0:
        angle3=angle3+5
        prev6=1

    if a[1]==1 and angle4>0 and prev7==0:
        angle4=angle4-5
        prev7=1
    
    if a[3]==1 and angle4<180 and prev8==0:
        angle4=angle4+5
        prev8=1

    if a[9]==1 and angle5>0 and prev9==0:
        angle5=angle5-5
        prev9=1
    
    if a[8]==1 and angle5<180 and prev10==0:
        angle5=angle5+5
        prev10=1
               

    
    if b[7]<0.5:
        prev1=0
    if b[7]>-0.50:
        prev2=0
    if b[9]==0:
        prev3=0
        prev4=0
    if b[8]==0:
        prev5=0
        prev6=0
    if a[1]==0:
        prev7=0
    if a[3]==0:
        prev8=0
    if a[9]==0:
        prev9=0   
    if a[8]==0:
        prev10=0


    c[0]=angle
    c[1]=angle2
    c[2]=angle3
    c[3]=angle4
    c[4]=angle5



def talker():
    rospy.init_node('sum',anonymous=True),
    pub1=rospy.Publisher("servoc1",Int16,queue_size=1)
    pub2=rospy.Publisher("servoc2",Int16,queue_size=1)
    pub3=rospy.Publisher("servoc3",Int16,queue_size=1)
    pub4=rospy.Publisher("servoc4",Int16,queue_size=1)
    pub5=rospy.Publisher("servoc5",Int16,queue_size=1)
    rate = rospy.Rate(10)

    while not rospy.is_shutdown():

        
        rospy.Subscriber('/joy', Joy, callback)
        
        pub1.publish(c[0])
        pub2.publish(c[1])
        pub3.publish(c[2])
        pub4.publish(angle4)
        pub5.publish(angle5)
        rate.sleep()
    rospy.spin()

if __name__ == '__main__':
    angle =90
    angle2=90
    angle3=90
    angle4=90
    angle5=90
    try:
        talker()
    except rospy.ROSInterruptException:
        pass