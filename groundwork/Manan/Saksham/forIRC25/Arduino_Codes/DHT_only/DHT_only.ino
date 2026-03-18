/*
* before uploading install "DHT sensor library" and "Adafruit Unified Sensor" libraries published by Adafruit
*/
#include "DHT.h" 
#include <ros.h>
#include <std_msgs/Float32.h>

ros::NodeHandle nh;

#define DHTPIN 13 //change later
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

std_msgs::Float32 temp_c;
ros::Publisher tempdat("vitals", &temp_c);

void setup()
{
  dht.begin();
  nh.initNode();
  nh.advertise(tempdat);
}

void loop()
{
  //getTemp();
  temp_c.data = dht.readTemperature(); //celsius data
  tempdat.publish(&temp_c);
  nh.spinOnce();
  delay(1000);
}

/*
void getTemp()
{
  temp_c.data = dht.readTemperature(); //celsius data
  tempdat.publish(&temp_c);
}
*/