

#include <ros.h>
#include <std_msgs/Float32MultiArray.h>
#include <std_msgs/Int32MultiArray.h>
#include <math.h>

ros::NodeHandle nh;

int pwmac1 = 0;
int dirac1 = 4;
int pwmac2 = 32;
int dirac2 = 33;

int pwmb1 = 25;
int dirb1 = 26;
int pwmb2 = 14;
int dirb2 = 12;

int pwmbase = 15;
int dirbase = 5;
int pwmgripper =13;
int dirgripper = 27;

void callbackaxes(const std_msgs::Float32MultiArray &msg) {
  int speedac1, speedac2, speedb, speedbase;
  

  if (msg.data[0] < 0.0) {
    speedac1 = (int)((-msg.data[0]) * 255.0);
    analogWrite(pwmac1, speedac1);
    digitalWrite(dirac1, LOW);
  } else if (msg.data[0] > 0.0) {
    speedac1 = (int)(msg.data[0] * 255.0);
    analogWrite(pwmac1, speedac1);
    digitalWrite(dirac1, HIGH);
  } else {
    analogWrite(pwmac1, 0);
  }

  if (msg.data[1] < 0.0) {
    speedac2 = (int)((-msg.data[1]) * 255.0);
    analogWrite(pwmac2, speedac2);
    digitalWrite(dirac2, LOW);
  } else if (msg.data[1] > 0.0) {
    speedac2 = (int)(msg.data[1] * 255.0);
    analogWrite(pwmac2, speedac2);
    digitalWrite(dirac2, HIGH);
  } else {
    analogWrite(pwmac2, 0);
  }

  
  if (abs(msg.data[3]) >= abs(msg.data[4])) {
    if (msg.data[3] < 0.0) {
      speedb = (int)((-msg.data[3]) * 255.0);
      analogWrite(pwmb1, speedb);
      digitalWrite(dirb1, LOW);
      analogWrite(pwmb2, speedb);
      digitalWrite(dirb2, LOW);
    } else if (msg.data[3] > 0.0) {
      speedb = (int)((msg.data[3]) * 255.0);
      analogWrite(pwmb1, speedb);
      digitalWrite(dirb1, HIGH);
      analogWrite(pwmb2, speedb);
      digitalWrite(dirb2, HIGH);
    } else {
      analogWrite(pwmb1, 0);
      analogWrite(pwmb2, 0);
    }
  } else if (abs(msg.data[4]) >= abs(msg.data[3])) {
    if (msg.data[4] < 0.0) {
      speedb = (int)((-msg.data[4]) * 255.0);
      analogWrite(pwmb1, speedb);
      digitalWrite(dirb1, LOW);
      analogWrite(pwmb2, speedb);
      digitalWrite(dirb2, HIGH);
    } else if (msg.data[4] > 0.0) {
      speedb = (int)((msg.data[4]) * 255.0);
      analogWrite(pwmb1, speedb);
      digitalWrite(dirb1, HIGH);
      analogWrite(pwmb2, speedb);
      digitalWrite(dirb2, LOW);
    } else {
      analogWrite(pwmb1, 0);
      analogWrite(pwmb2, 0);
    }
  }

  
  if (msg.data[2] < 1.0) {
    float adjusted_base_value2 = msg.data[2] + 1.0;  
    speedbase = map(adjusted_base_value2, 2, 0, 0, 255);
    analogWrite(pwmbase, speedbase);
    digitalWrite(dirbase, LOW);
  }

  if (msg.data[5] < 1.0) {
    float adjusted_base_value5 = msg.data[5] + 1.0;
    speedbase = map(adjusted_base_value5, 2, 0, 0, 255);
    analogWrite(pwmbase, speedbase);
    digitalWrite(dirbase, HIGH);
  }
}

void callbackbutton(const std_msgs::Int32MultiArray &msg) {
  if (msg.data[0] == 1) {
    analogWrite(pwmgripper, 255);
    digitalWrite(dirgripper, LOW);
  } else if (msg.data[1] == 1) {
    analogWrite(pwmgripper, 255);
    digitalWrite(dirgripper, HIGH);
  }
}

ros::Subscriber<std_msgs::Float32MultiArray> sub1("/armkt4axes", &callbackaxes);
ros::Subscriber<std_msgs::Int32MultiArray> sub2("/armktbutton", &callbackbutton);

void setup() {

  pinMode(pwmac1, OUTPUT);
  pinMode(dirac1, OUTPUT);
  pinMode(pwmac2, OUTPUT);
  pinMode(dirac2, OUTPUT);
  
  pinMode(pwmb1, OUTPUT);
  pinMode(dirb1, OUTPUT);
  pinMode(pwmb2, OUTPUT);
  pinMode(dirb2, OUTPUT);
  
  pinMode(pwmbase, OUTPUT);
  pinMode(dirbase, OUTPUT);
  
  pinMode(pwmgripper, OUTPUT);
  pinMode(dirgripper, OUTPUT);
  
  nh.initNode();
  nh.subscribe(sub1);
  nh.subscribe(sub2);
}

void loop() {
  nh.spinOnce();
  delay(1);
}
