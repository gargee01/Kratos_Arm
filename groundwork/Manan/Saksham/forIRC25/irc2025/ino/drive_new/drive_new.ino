#include <ArduinoHardware.h>
#include <std_msgs/Float32MultiArray.h>

#include <Cytron_SmartDriveDuo.h>
#include <ros.h>

ros::NodeHandle nh;

#define IN1 27
#define IN2 14
#define IN3 12
#define BAUDRATE 115200

#define fan1 18
#define fan2 23
Cytron_SmartDriveDuo motor_back(SERIAL_SIMPLIFIED, IN1, BAUDRATE);
Cytron_SmartDriveDuo motor_mid(SERIAL_SIMPLIFIED, IN2, BAUDRATE);
Cytron_SmartDriveDuo motor_front(SERIAL_SIMPLIFIED, IN3, BAUDRATE);



// ros::Publisher pub1("feedback", &vels);
float right_wheel=0.0;  
float left_wheel=0.0;
float right_wheel_mid=0.0; 
float left_wheel_mid=0.0;
float right_wheel_front=0.0; 
float left_wheel_front=0.0;
float right_wheel_back=0.0; 
float left_wheel_back=0.0;
float velx,velz=0.0;
 
void callback(const std_msgs::Float32MultiArray& msg)
{ 
  right_wheel_front = msg.data[0];
  left_wheel_front = msg.data[1];
  right_wheel_mid = msg.data[2];
  left_wheel_mid = msg.data[3];
  right_wheel_back = msg.data[4];
  left_wheel_back = msg.data[5];
}

ros::Subscriber<std_msgs::Float32MultiArray> sub("/rover",&callback);

void setup()
{ 
  pinMode(fan1, OUTPUT);
  pinMode(fan2, OUTPUT);
  nh.initNode();

  nh.subscribe(sub);
  // nh.advertise(pub1);
}

void loop()
{
  digitalWrite(fan1,HIGH);
  digitalWrite(fan2, HIGH);
  motor_back.control(left_wheel_back,right_wheel_back);
  motor_mid.control(left_wheel_mid,right_wheel_mid);
  motor_front.control(left_wheel_front,right_wheel_front);
//   pub1.publish(&vels);
  nh.spinOnce();
}
