#include <ros.h>
#include <std_msgs/Float32MultiArray.h>
#include <std_msgs/Int64.h>
#include <ESP32Servo.h>
#include "ArduinoHardware.h"
ros::NodeHandle nh;
int dir1=4;
int pwm1=0; 

int dir2=33;
int pwm2=32;

int dir3=26; 
int pwm3=25;
int dir4=12; //scissor lift
int pwm4=14;
int dir5=27; // augre
int pwm5=13;
int dir6=5; //magnetic mixer
int pwm6=15;
int en=18;
int dirst=19;
int step=23;
float b[13];
Servo myservo;
int servo=11;
int initial_position=0;

void Callback( const std_msgs::Float32MultiArray& val){
  
'['
  memcpy(b, val.data, sizeof(b));
}

 ros::Subscriber<std_msgs::Float32MultiArray> Sub("publish", &Callback);



void setup() {
  pinMode(dir1,OUTPUT);
  pinMode(dir2,OUTPUT);
  pinMode(dir3,OUTPUT);
  pinMode(dir4,OUTPUT);
  pinMode(dir5,OUTPUT);
  pinMode(dir6,OUTPUT);
  pinMode(pwm1,OUTPUT);
  pinMode(pwm2,OUTPUT);
  pinMode(pwm3,OUTPUT);
  pinMode(pwm4,OUTPUT);
  pinMode(pwm5,OUTPUT);
  pinMode(pwm6,OUTPUT);
  pinMode(en,OUTPUT);
  pinMode(dirst,OUTPUT);
  pinMode(step,OUTPUT);
  digitalWrite(en,HIGH);
  myservo.attach(servo);
  myservo.write(0);
  nh.getHardware()->setBaud(115200);
  nh.subscribe(Sub);
  nh.initNode();

}

void loop() {
  analogWrite(pwm1,0);
  analogWrite(pwm2,0);
  analogWrite(pwm3,0);
  analogWrite(pwm4,0);
  analogWrite(pwm5,0);
  analogWrite(pwm6,0);



  if(b[0]==1)
  { //pumps are unidirectional and on digital high
    digitalWrite(dir1, HIGH); 
 
    digitalWrite(pwm1,HIGH);  
  
  }
  else if(b[0]==2)
  {
    digitalWrite(dir2, HIGH); 
 
    digitalWrite(pwm2,HIGH);  
  
  }
  else if(b[0]==3)
  {
    digitalWrite(dir3, HIGH); 
 
    digitalWrite(pwm3,HIGH);  
  
  }
  
  // the 4 buttons on the right. If rightmost is pressed then b[2]=1 (scissor lift) if uppermost is pressed then b[2] is 2 (augre) and if leftmost is pressed then b[2] is 3 (magnet)
  // have to hold down the mentioned button and then move the left joystick up or down to change direction (button must be pressed while doing this)
  else if(b[2]==1 && b[3]>0.1)  // scissor lift, magnetic spinner and augre are bi directional and analog
  {
    digitalWrite(dir4, HIGH); 

    analogWrite(pwm4,b[3]*255);  

 
  }
  else if(b[2]==1 && b[3]<-0.1)
  {
    digitalWrite(dir4, LOW); 

    analogWrite(pwm4,(-1*b[3])*255);  

 
  }
  else if(b[2]==2 && b[3]>0.1)
  {
    digitalWrite(dir5, HIGH); 

    analogWrite(pwm5,b[3]*255);  

  }
  else if(b[2]==2 && b[3]<-0.1)
  {
    digitalWrite(dir5, LOW); 

    analogWrite(pwm5,(-1*b[3])*255);  

  }
  else if(b[2]==3 && b[3]>0.1)
  {
    digitalWrite(dir6, HIGH); 

    analogWrite(pwm6,b[3]*255);  

  
  }
  else if(b[2]==3 && b[3]<-0.1)
  {
    digitalWrite(dir6, LOW); 

    analogWrite(pwm6,(-1*b[3])*255);  


 
  }
  //b[4] is 1 when left trigger is pressed: enabled. b[4]=0 when right trigger is pressed. b[5] is 1 when the lowermost of the left 4 buttons is pressed. is -1 when lowermost of right 4 buttons is pressed.
  if(b[4]==1){
    digitalWrite(en,LOW); // stepper is enabled 
    if(b[5]==1)
   {
    digitalWrite(dirst,HIGH);
    digitalWrite(step, HIGH);  
    delayMicroseconds(2000);
    digitalWrite(step, LOW) ;
    delayMicroseconds(2000); // prev 5 lines make  one step in one direction
   }
    else if(b[5]==-1)
    {
      digitalWrite(dirst,LOW);
      digitalWrite(step ,HIGH);
      delayMicroseconds(2000);
      digitalWrite(step, LOW);
      delayMicroseconds(2000); // prev 5 lines make  one step in other direction

    }
  }

  


   
  myservo.write(b[6]);
  delay(100);
  

  

  nh.spinOnce();
  delay(100);
}