#include <ros.h>
#include <std_msgs/Float32MultiArray.h>
#include <cmath>


ros::NodeHandle  nh;

// defining ARM + BASE(MOTOR DRIVER 1)
int pwm_arm = 0;
int dir_arm = 4;

int pwm_base = 32;
int dir_base = 33;

// defing linear actuators (MOTOR DRIVER 2)
int pwm_lin1 = 25;
int dir_lin1 = 26;

int pwm_lin2 = 14;
int dir_lin2 = 12;

//defing bevel (MOTOR DRIVER 3)
int pwm_mot1 = 13;
int dir_mot1 = 27;

int pwm_mot2 = 15;
int dir_mot2 = 5;


// -------------------------------Values ------------------------//
int pwm_armVal = 0;
int dir_armVal = 0;

int pwm_baseVal = 0;
int dir_baseVal = 0;

//---------------------------

int pwm_lin1Val = 0;
int dir_lin1Val = 0;

int pwm_lin2Val = 0;
int dir_lin2Val = 0;

//---------------------------

int pwm_mot1Val = 0;
int dir_mot1Val = 0;

int pwm_mot2Val = 0;
int dir_mot2Val = 0;

//---------------------------




//callback function
void val_assign( const std_msgs::Float32MultiArray& cmd_msg){
  dir_armVal = cmd_msg.data[8];
  pwm_armVal = cmd_msg.data[9];

  dir_lin1Val = cmd_msg.data[0];
  dir_lin2Val = cmd_msg.data[1];

  pwm_lin1Val = cmd_msg.data[2];
  pwm_lin2Val = cmd_msg.data[3];

  pwm_baseVal = cmd_msg.data[11];
  dir_baseVal = cmd_msg.data[10];

  pwm_mot1Val = cmd_msg.data[6];
  pwm_mot2Val = cmd_msg.data[7];

  dir_mot1Val = cmd_msg.data[4];
  dir_mot2Val = cmd_msg.data[5];
 
}

ros::Subscriber<std_msgs::Float32MultiArray> sub("/motors", val_assign);

void setup() {
  pinMode(pwm_arm, OUTPUT);
  pinMode(dir_arm, OUTPUT);

  pinMode(pwm_base, OUTPUT);
  pinMode(dir_base, OUTPUT);

  pinMode(pwm_lin1, OUTPUT);
  pinMode(dir_lin1, OUTPUT);

  pinMode(pwm_lin2, OUTPUT);
  pinMode(dir_lin2, OUTPUT);

  pinMode(pwm_mot1, OUTPUT);
  pinMode(dir_mot1, OUTPUT);

  pinMode(pwm_mot2, OUTPUT);
  pinMode(dir_mot2, OUTPUT);

  // blah
  nh.initNode();
  //nh.getHardware()->setBaud(115200);

  nh.subscribe(sub);

}

void loop() {
  // put your main code here, to run repeatedly:
  nh.spinOnce();

  //for linear actuators
  digitalWrite(dir_lin1, dir_lin1Val);
  digitalWrite(dir_lin2, dir_lin2Val);

  analogWrite(pwm_lin1, pwm_lin1Val);
  analogWrite(pwm_lin2, pwm_lin2Val);
  
  //for bevel
  digitalWrite(dir_mot1, dir_mot1Val);
  digitalWrite(dir_mot2, dir_mot2Val);

  analogWrite(pwm_mot1, pwm_mot1Val);
  analogWrite(pwm_mot2, pwm_mot2Val);

  //for baase
  digitalWrite(dir_base, dir_baseVal);
  analogWrite(pwm_base, pwm_baseVal);

  //for gripper
  analogWrite(pwm_arm, pwm_armVal);
  digitalWrite(dir_arm, dir_armVal);
}
