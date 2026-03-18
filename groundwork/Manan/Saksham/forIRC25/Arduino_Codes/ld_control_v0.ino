#include <ros.h>
#include <std_msgs/Float32MultiArray.h>

ros::NodeHandle nh;
int dir1=4;
int pwm1=0;
int dir2=33;
int pwm2=32;
int dir3=26;
int pwm3=25;
int dir4=12;
int pwm4=14;
int dir5=27;
int pwm5=13;
int dir6=5;
int pwm6=15;
int en=18;
int dirst=19;
int step=23;
float b[13];
int limit_switch = 16;


int force_stop = 0;

void Callback( const std_msgs::Float32MultiArray& val){
  

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
  pinMode(limit_switch, INPUT_PULLUP);
  nh.subscribe(Sub);
  nh.getHardware()->setBaud(57600);
  nh.initNode();

}

void loop() {
    static int lastState = HIGH;
    static unsigned long lastDebounceTime = 0;
    const unsigned long debounceDelay = 50;

    int currentState = digitalRead(limit_switch);
    if (currentState != lastState) {
        lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > debounceDelay) {
        if (currentState == LOW) {
            force_stop = 1;
        } else {
            force_stop = 0;
        }
    }

    lastState = currentState;

  analogWrite(pwm1,0);
  analogWrite(pwm2,0);
  analogWrite(pwm3,0);
  analogWrite(pwm4,0);
  analogWrite(pwm5,0);
  analogWrite(pwm6,0);


  if(b[0]==1)
  {
    digitalWrite(dir1, LOW); 
 
    analogWrite(pwm1,255);  
  
  }
  if(b[0]==2)
  {
    digitalWrite(dir2, HIGH); 
 
    analogWrite(pwm2,255);  
  
  }
  if(b[0]==3)
  {
    digitalWrite(dir3, LOW); 
 
    analogWrite(pwm3,255);  
  
  }
  if(b[2]==1 && b[3]>0.1 )
  {
    digitalWrite(dir4, HIGH); 

    analogWrite(pwm4,b[3]*255);  
 
  }
  if(b[2]==1 && b[3]<-0.1)
  {
    digitalWrite(dir4, LOW); 

    analogWrite(pwm4,(-1*b[3])*255);  
 
  }
  if(b[2]==2 && b[3]>0.1  && force_stop == 0)
  {
    digitalWrite(dir5, HIGH); 

    analogWrite(pwm5,b[3]*255);  
 
  }
  if(b[2]==2 && b[3]<-0.1)
  {
    digitalWrite(dir5, LOW); 

    analogWrite(pwm5,(-1*b[3])*255);  
 
  }
  if(b[2]==3 && b[3]>0.1)
  {
    digitalWrite(dir6, HIGH); 

    analogWrite(pwm6,b[3]*255);  
 
  }
  if(b[2]==3 && b[3]<-0.1)
  {
    digitalWrite(dir6, LOW); 

    analogWrite(pwm6,(-1*b[3])*255);  
 
  }
  if(b[4]==1){
    digitalWrite(en,LOW);
    if(b[5]==1)
   {
     digitalWrite(dirst,LOW); //Changes the direction of rotation
  for(int x = 0; x < 5; x++) {
    digitalWrite(step,HIGH);
    delayMicroseconds(20);
    digitalWrite(step,LOW);
    delayMicroseconds(20);
  
   }
   }
   
    if(b[5]==-1)
    {
      digitalWrite(dirst,HIGH); // Enables the motor to move in a particular direction
     for(int x = 0; x < 5; x++) {
    digitalWrite(step,HIGH); 
    delayMicroseconds(20); 
    digitalWrite(step,LOW); 
    delayMicroseconds(20);
  
    }


    }

  }
   
  nh.spinOnce();
  delay(100);
}

  