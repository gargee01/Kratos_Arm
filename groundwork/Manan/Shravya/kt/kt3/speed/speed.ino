#include <ros.h>
#include <std_msgs/Float32.h>  // Use Float32 since your callback expects it

ros::NodeHandle nh;
int pwm = 22;
int dir = 23;
int speed = 0;

void callback(const std_msgs::Float32 &msg) {
  int val = 0;  // Declare val outside of if-else to use it later

  if (msg.data < 0) {
    val = msg.data * 1000000 * (-1);
  } else if (msg.data > 0) {
    val = msg.data * 1000000;
  }

  speed = map(val, 0, 1000000, 0, 255);  // Map val to speed

  if (msg.data < 0.0) {
    analogWrite(pwm, speed);
    digitalWrite(dir, LOW);
  } else if (msg.data > 0.0) {
    analogWrite(pwm, speed);
    digitalWrite(dir, HIGH);
  } else {
    analogWrite(pwm, 0);  // Stop motor if msg.data is 0
  }
}

// Subscriber expecting Float32 messages
ros::Subscriber<std_msgs::Float32> sub("/kt2", &callback);

void setup() {
  pinMode(pwm, OUTPUT);  // Corrected variable names
  pinMode(dir, OUTPUT);
  nh.initNode();
  nh.subscribe(sub);
}

void loop() {
  nh.spinOnce();
  delay(1);
}

























// #include <ros.h>
// #include <std_msgs/Float32.h>

// ros::NodeHandle nh;
// int pwm = 22;
// int dir = 23;

// void callback(const std_msgs::Float32 &msg) {
//   int speed;

//   if (msg.data < 0.0) {
//     speed = (int)((-msg.data) * 255.0);
//     digitalWrite(dir, LOW);
//   } else if (msg.data > 0.0) {
//     speed = (int)(msg.data * 255.0);
//     digitalWrite(dir, HIGH);
//   } else {
//     speed = 0;
//   }

//   analogWrite(pwm, speed);
// }

// ros::Subscriber<std_msgs::Float32> sub("/kt2", &callback);

// void setup() {
//   pinMode(pwm, OUTPUT);
//   pinMode(dir, OUTPUT);
//   nh.initNode();
//   nh.subscribe(sub);
// }

// void loop() {
//   nh.spinOnce();
//   delay(1);
// }
