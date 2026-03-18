#include <ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Int16MultiArray.h>
#include <ESP32Servo.h>

Servo servo1;
//Servo servo2;
//Servo servo3;

#define baserotationpwm 15
#define basedirection 5
#define magnet_pwm 14
#define magnet_dir 12
#define bevel_left_pwm  25
#define bevel_left_drn  26
#define lift_dir 33
#define lift_pwm 32
#define auger_dir 4
#define auger_pwm 0
#define gripperpwm 13
#define gripperdir 27

#define servo1pin 17
//#define servo2pin 8
//#define servo3pin 7

/*const int dirPin = 4; // can be 4 and 3 as well
const int stepPin = 11; // can be 11 and 2 as well
int stepp=0;
const int stepsPerRevolution = 50;*/

ros::NodeHandle n;
std_msgs::Int16MultiArray msg;
ros::Publisher chatter("chatter", &msg);

int vel1;
int vel2;
int var;
int var_lift;
int var_auger;
int var_base;
int var_magnet;
int vel_base;
int vel_bev;
int var_gripper;
int servo_angle; // Initial angle of the servo (neutral position)

void messageCallback(const std_msgs::Int16MultiArray& receivedMsg) {
  var_lift = receivedMsg.data[0];
  if (abs(receivedMsg.data[0]) <= 0.2) {
    digitalWrite(lift_pwm, LOW);
  }
  else if (receivedMsg.data[0] > 0.2) {
    digitalWrite(lift_dir, LOW);
    analogWrite(lift_pwm, 150);
  } 
  else if (receivedMsg.data[0] < -0.2){
    digitalWrite(lift_dir, HIGH);
    analogWrite(lift_pwm, 150);   
  }

var_auger = receivedMsg.data[1];
  if (abs(receivedMsg.data[1]) <= 0.2) {
    digitalWrite(auger_pwm, LOW);
  }
  else if (receivedMsg.data[1] > 0.2) {
    digitalWrite(auger_dir, LOW);
    analogWrite(auger_pwm, 150);
  } 
  else if (receivedMsg.data[1] < -0.2){
    digitalWrite(auger_dir, HIGH);
    analogWrite(auger_pwm, 150);   
  }

var_magnet = receivedMsg.data[2];
  if (abs(receivedMsg.data[2]) <= 0.2) {
    digitalWrite(magnet_pwm, LOW);
  }
  else if (receivedMsg.data[2] > 0.2) {
    digitalWrite(magnet_dir, LOW);
    analogWrite(magnet_pwm, 150);
  } 
  else if (receivedMsg.data[2] < -0.2){
    digitalWrite(magnet_dir, HIGH);
    analogWrite(magnet_pwm, 150);   
  }

  // Control Servo using index 3 (servo angle)
  servo_angle = receivedMsg.data[3]; // Limit servo angle to 0-180 degrees
  servo1.write(servo_angle); // Set the servo angle

  chatter.publish(&msg);

  /*if(var_st == 1){
    digitalWrite(dirPin, HIGH);
    
    for(int x=0; x<5; x++){
      
      digitalWrite(stepPin, HIGH);//gripper opens or closes
      delayMicroseconds(500);
      digitalWrite(stepPin, LOW);
      delayMicroseconds(500);
    }
  }
  else if(var_st == 2){
    digitalWrite(dirPin, LOW);
    
    for(int x=0; x<5; x++){
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(500);
      digitalWrite(stepPin, LOW);
      delayMicroseconds(500);
    }
  }*/
}

//void control_things(){}

ros::Subscriber<std_msgs::Int16MultiArray> sub1("/control_ld", messageCallback);

void setup() {
  pinMode(lift_dir, OUTPUT);
  pinMode(lift_pwm, OUTPUT);
  pinMode(auger_dir, OUTPUT);
  pinMode(auger_pwm, OUTPUT);
  pinMode(magnet_dir,OUTPUT);
  pinMode(magnet_pwm,OUTPUT);
  pinMode(baserotationpwm,OUTPUT);
  pinMode(basedirection,OUTPUT);
  pinMode(bevel_left_pwm,OUTPUT);
  pinMode(bevel_left_drn,OUTPUT);
  pinMode(gripperdir,OUTPUT);
  pinMode(gripperpwm,OUTPUT);

  pinMode(lift_pwm, LOW);
  pinMode(auger_pwm, LOW);
  pinMode(magnet_pwm, LOW);
  pinMode(baserotationpwm, LOW);
  pinMode(bevel_left_pwm, LOW);
  pinMode(gripperpwm, LOW);

  servo1.attach(servo1pin);
  //servo3.attach(servo3pin);
  //servo2.attach(servo2pin);

  n.initNode();
  n.subscribe(sub1);
  n.advertise(chatter);
}

void loop() {
  //control_things();
  n.spinOnce();
}
