#include <ArduinoHardware.h>
#include <geometry_msgs/Point.h>
#include "DHT.h" 
#include <std_msgs/Float32MultiArray.h>
#include <std_msgs/Int8.h>
#include <Cytron_SmartDriveDuo.h>
#include <ros.h>
#include <cmath>
ros::NodeHandle nh;

#define IN1 27
#define IN2 12
#define IN3 14
#define BAUDRATE 115200
Cytron_SmartDriveDuo motor_back(SERIAL_SIMPLIFIED, IN1, BAUDRATE);
Cytron_SmartDriveDuo motor_mid(SERIAL_SIMPLIFIED, IN2, BAUDRATE);
Cytron_SmartDriveDuo motor_front(SERIAL_SIMPLIFIED, IN3, BAUDRATE);

#define DHTPIN1 16 //change later
#define DHTTYPE1 DHT22
DHT dht1(DHTPIN1, DHTTYPE1);

#define DHTPIN2 17 //change later
#define DHTTYPE2 DHT22
DHT dht2(DHTPIN2, DHTTYPE2);

#define fans 2

geometry_msgs::Point vels;
std_msgs::Float32MultiArray temp_c;
ros::Publisher tempdat("vitals", &temp_c);

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
 


void callback0(const std_msgs::Int8& msg)
{ 
  if(temp_c.data[2] >=35)
  {
    digitalWrite(fans,HIGH);
  }
  else
  {
    if(msg.data==0)
    digitalWrite(fans,LOW);
    else
    digitalWrite(fans,HIGH);
  }
}

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
ros::Subscriber<std_msgs::Int8> sub0("/fan_control",&callback0);

void setup()
{ 
  dht1.begin();
  dht2.begin();
  nh.initNode();
  nh.subscribe(sub);
  nh.subscribe(sub0);
  nh.advertise(pub1);
  nh.advertise(tempdat);
  temp_c.data_length = 3;
  pinMode(fans,OUTPUT);
  digitalWrite(fans,LOW);
}

void loop()
{
  float data[3] = {0, 0, 0 };
  motor_back.control(left_wheel_back,right_wheel_back);
  motor_mid.control(left_wheel_mid,right_wheel_mid);
  motor_front.control(left_wheel_front,right_wheel_front);
  data[0] = dht1.readTemperature(); //celsius data
  data[1] = dht2.readTemperature(); //celsius data
  if (isnan(data[0]) && isnan(data[1])){
    data[2] = 0;
    
  }
  else if (isnan(data[0]) && !isnan(data[1])){
    data[2] = data[1]; 
  }
  else if (!isnan(data[0]) && isnan(data[1])){
    data[2] = data[0];
  }
  else {
    data[2] = (data[0] + data[1])/2;
  }
  temp_c.data = data;
  tempdat.publish(&temp_c);
  pub1.publish(&vels);
  nh.spinOnce();
}