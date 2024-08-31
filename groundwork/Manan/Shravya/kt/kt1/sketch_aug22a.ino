#include <ros.h>
#include <std_msgs/Int32.h>

ros::NodeHandle nh;

int pwm2 = 22;
int dir2 = 23;

void callback(const std_msgs::Int32 &msg) {
  int val = msg.data;

  if (val == 1) {
    analogWrite(pwm2, 255);
    digitalWrite(dir2, LOW);
  }
  else if (val == 2) {
    analogWrite(pwm2, 255);
    digitalWrite(dir2, HIGH);
  }
  else if (val == 3) {
    analogWrite(pwm2, 0);
  }
}


ros::Subscriber<std_msgs::Int32> sub("/kt1", &callback);

void setup() {
  
  pinMode(pwm2, OUTPUT);
  pinMode(dir2, OUTPUT);
  nh.initNode();
  nh.subscribe(sub);
}


void loop()
{
  nh.spinOnce();
  delay(1);
}