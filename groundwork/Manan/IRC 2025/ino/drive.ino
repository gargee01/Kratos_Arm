#include <ArduinoHardware.h>
#include <geometry_msgs/Point.h>

#include <Cytron_SmartDriveDuo.h>
#include <ros.h>

ros::NodeHandle nh;

#define IN1 19
#define IN2 5
#define IN3 18
#define BAUDRATE 115200
Cytron_SmartDriveDuo motor_back(SERIAL_SIMPLIFIED, IN1, BAUDRATE);
Cytron_SmartDriveDuo motor_mid(SERIAL_SIMPLIFIED, IN2, BAUDRATE);
Cytron_SmartDriveDuo motor_front(SERIAL_SIMPLIFIED, IN3, BAUDRATE);

geometry_msgs::Point vels;

ros::Publisher pub1("feedback", &vels);
float right_wheel=0.0;
float left_wheel=0.0;
float right_wheel_mid=0.0; 
float left_wheel_mid=0.0;
float right_wheel_front=0.0; 
float left_wheel_front=0.0;
float right_wheel_back=0.0; 
float left_wheel_back=0.0;
float velx,velz=0.0;
 


void callback(const geometry_msgs::Point& msg)
{ 
  velx=msg.x*0.67;
  velz=msg.z*0.67;
  right_wheel_front=(velx+velz)/2+(velx-velz)/2*1.48;
  left_wheel_front=(velx+velz)/2-(velx-velz)/2*1.48;
  right_wheel_mid=(velx+velz)/2+(velx-velz)/2;
  left_wheel_mid=(velx+velz)/2-(velx-velz)/2;
  right_wheel_back=(velx+velz)/2+(velx-velz)/2*1.3;
  left_wheel_back=(velx+velz)/2-(velx-velz)/2*1.3 ;



}
ros::Subscriber<geometry_msgs::Point> sub("/rover",&callback);

void setup()
{ 
  nh.initNode();
  nh.subscribe(sub);
  nh.advertise(pub1);
}

void loop()
{
  motor_back.control(left_wheel_back,right_wheel_back);
  motor_mid.control(left_wheel_mid,right_wheel_mid);
  motor_front.control(left_wheel_front,right_wheel_front);
  pub1.publish(&vels);
  nh.spinOnce();
}
