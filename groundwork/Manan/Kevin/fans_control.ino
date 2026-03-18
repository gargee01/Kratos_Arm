#include <ros.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Int32MultiArray.h>
const int fan_pin = 26;  //change
const int fan_interrupt = 16; //change, add pull up 10k Ohm resistor between this pin and 5v
int temp = 30;
volatile int count =0;
int mode = 0;
int con = 50;
int rpm;
ros::NodeHandle nh;

void messageCb( const std_msgs::Float32& vitals){
  temp = (int)vitals.data;
}
void callb( const std_msgs::Int32MultiArray& msg){
  mode = msg.data[0];
  con =  msg.data[1];
}

ros::Subscriber<std_msgs::Float32> sub1("vitals", &messageCb ); //change when finalised

ros::Subscriber<std_msgs::Int32MultiArray> sub2("fan_control", &callb ); //change when method decided

void setup() {
  nh.initNode();
  pinMode(fan_pin, OUTPUT);
  analogWrite(fan_pin,0);
  attachInterrupt(digitalPinToInterrupt(fan_interrupt), counter, RISING); //comment out if we dont want RPM feedback
}

void loop()
{
  if(mode==0)
    auto_control();
  else
    manual_control();  
  
  nh.spinOnce();
  delay(1000);
}

void auto_control()
{
  if(temp<30)
  {
    analogWrite(fan_pin,0);
    delay(5000);
  }
  else if(temp>70)
  {
    analogWrite(fan_pin,255);
    delay(5000);
  }
  else
  {
    analogWrite(fan_pin,map(temp,30,70,0,255));
    delay(5000);
  }
  //rpm=count*30;
}

void manual_control()
{
    analogWrite(fan_pin,map(con,0,100,0,255));
    delay(5000);
    //rpm=count*30;
}

void counter(){
  count++;
}
