#include <ros.h>
#include <kt_1/state.h>
#include <cmath>


ros::NodeHandle  nh;

// defining pins
int pwm1 = 23;
int dir1 = 22;

int pwm2 = 19;
int dir2 = 18;


//defining values to pins
int pwm1_val = 0;
int dir1_val = 0 ;

int pwm2_val = 0;
int dir2_val = 0;


//callback function
void val_assign( const kt_1::state& cmd_msg){
  pwm1_val = cmd_msg.pwm1;
  dir1_val = cmd_msg.dir1;

  pwm2_val = cmd_msg.pwm2;
  dir2_val = cmd_msg.dir2;
  
}

ros::Subscriber<kt_1::state> sub("/motors", val_assign);

void setup() {
  pinMode(pwm1, OUTPUT);
  pinMode(dir1, OUTPUT);

  pinMode(pwm2, OUTPUT);
  pinMode(dir2, OUTPUT);


  nh.initNode();
  //nh.getHardware()->setBaud(115200);

  nh.subscribe(sub);

}

void loop() {
  // put your main code here, to run repeatedly:
  nh.spinOnce();
  digitalWrite(dir1, dir1_val);
  digitalWrite(dir2, dir2_val);

  analogWrite(pwm1, pwm1_val);
  analogWrite(pwm2, pwm2_val);
}
