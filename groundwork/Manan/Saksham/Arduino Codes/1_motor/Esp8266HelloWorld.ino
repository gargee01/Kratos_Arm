

#include <ros.h>
#include <std_msgs/Float32.h>
#include <cmath>

ros::NodeHandle  nh;
int pwm = 23;
int dir = 22;


float state = 0;

void servo_cb( const std_msgs::Float32& cmd_msg){
  state = cmd_msg.data;
}


ros::Subscriber<std_msgs::Float32> sub("/motor1", servo_cb);

void setup(){
  pinMode(pwm, OUTPUT);
  pinMode(dir, OUTPUT);


  nh.initNode();
  //nh.getHardware()->setBaud(115200);

  nh.subscribe(sub);
  //attach it to pin 9
}

void loop(){
  nh.spinOnce();
  if (state > 0){
    digitalWrite(dir, HIGH);
    

  }
  else{
    digitalWrite(dir, LOW);

  }
  analogWrite(pwm, 255 * abs(state));

  delay(1);
}