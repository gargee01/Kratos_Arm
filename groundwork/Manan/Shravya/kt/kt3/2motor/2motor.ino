#include <ros.h>
#include <std_msgs/Float32.h>  // Use Float32 since your callback expects it

ros::NodeHandle nh;
int pwm1 = 22;
int dir1 = 23;
int pwm2 = 19;
int dir2 = 18;
int speed = 0;

void callback1(const std_msgs::Float32 &msg) {
  int val1 = 0;  

  if (msg.data < 0) {
    val1 = msg.data * 1000000 * (-1);
  } else if (msg.data > 0) {
    val1 = msg.data * 1000000;
  }

  speed = map(val1, 0, 1000000, 0, 255);  //
  if (msg.data < 0.0) {
    analogWrite(pwm1, speed);
    digitalWrite(dir1, LOW);
  } else if (msg.data > 0.0) {
    analogWrite(pwm1, speed);
    digitalWrite(dir1, HIGH);
  } else {
    analogWrite(pwm1, 0);  
  }
}

void callback2(const std_msgs::Float32 &msg) {
  int val2 = 0;  

  if (msg.data < 0) {
    val2 = msg.data * 1000000 * (-1);
  } else if (msg.data > 0) {
    val2 = msg.data * 1000000;
  }

  speed = map(val2, 0, 1000000, 0, 255);  

  if (msg.data < 0.0) {
    analogWrite(pwm2, speed);
    digitalWrite(dir2, LOW);
  } else if (msg.data > 0.0) {
    analogWrite(pwm2, speed);
    digitalWrite(dir2, HIGH);
  } else {
    analogWrite(pwm2, 0);  
  }
}

ros::Subscriber<std_msgs::Float32> sub1("/kt3m1", &callback1);
ros::Subscriber<std_msgs::Float32> sub2("/kt3m2", &callback2);

void setup() {
  pinMode(pwm1, OUTPUT);
  pinMode(dir1, OUTPUT);
  pinMode(pwm2, OUTPUT);
  pinMode(dir2, OUTPUT);
  nh.initNode();
  nh.subscribe(sub1);
  nh.subscribe(sub2);

void loop() {
  nh.spinOnce();
  delay(1);
}










// #include <ros.h>
// #include <std_msgs/Float32.h>

// ros::NodeHandle nh;
// int pwm1 = 22;
// int dir1 = 23;
// int pwm2 = 19;
// int dir2 = 18;

// void callback1(const std_msgs::Float32 &msg) {
//   int speed;

//   if (msg.data < 0.0) {
//     speed = (int)((-msg.data) * 255.0);
//     analogWrite(pwm1, speed);
//     digitalWrite(dir1, LOW);
//   } else if (msg.data > 0.0) {
//     speed = (int)(msg.data * 255.0);
//     analogWrite(pwm1, speed);
//     digitalWrite(dir1, HIGH);
//   } else {
//     analogWrite(pwm1, 0);
//   }
// }

// void callback2(const std_msgs::Float32 &msg) {
//   int speed;

//   if (msg.data < 0.0) {
//     speed = (int)((-msg.data) * 255.0);
//     analogWrite(pwm2, speed);
//     digitalWrite(dir2, LOW);
//   } else if (msg.data > 0.0) {
//     speed = (int)(msg.data * 255.0);
//     analogWrite(pwm2, speed);
//     digitalWrite(dir2, HIGH);
//   } else {
//     analogWrite(pwm2, 0);
//   }
// }

// ros::Subscriber<std_msgs::Float32> sub1("/kt3m1", &callback1);
// ros::Subscriber<std_msgs::Float32> sub2("/kt3m2", &callback2);

// void setup() {
//   pinMode(pwm1, OUTPUT);
//   pinMode(dir1, OUTPUT);
//   pinMode(pwm2, OUTPUT);
//   pinMode(dir2, OUTPUT);
//   nh.initNode();
//   nh.subscribe(sub1);
//   nh.subscribe(sub2);
// }

// void loop() {
//   nh.spinOnce();
//   delay(1);
// }

