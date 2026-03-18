/*
 * Complete Project Details http://randomnerdtutorials.com
*/

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Arduino.h>
#include <ros.h>
#include <std_msgs/Float64MultiArray.h>

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)

ros::NodeHandle nh;

std_msgs::Float64MultiArray bmemsg;

ros::Publisher sens("/bme_msg", &bmemsg);

Adafruit_BME280 bme; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

unsigned long delayTime;

void setup() {
 // Serial.begin(9600);
 // Serial.println(F("BME280 test"));

  bool status;
  
  // default settings
  // (you can also pass in a Wire library object like &Wire2)
  status = bme.begin();  
 // if (!status) {
  //  Serial.println("Could not find a valid BME280 sensor, check wiring!");
 //   while (1);
 // }
  
 // Serial.println("-- Default Test --");
  delayTime = 1000;
  nh.initNode();
  nh.advertise(sens);
  bmemsg.layout.dim=NULL;
  bmemsg.layout.data_offset=0;

 // Serial.println();
}


void loop() { 
  printValues();
  delay(delayTime);
}


void printValues() {
  //Serial.print("Temperature = ");
  float val1=bme.readTemperature();
  //Serial.println(" *C");
  
  // Convert temperature to Fahrenheit
  /*Serial.print("Temperature = ");
  Serial.print(1.8 * bme.readTemperature() + 32);
  Serial.println(" *F");*/
  
  //Serial.print("Pressure = ");
  float val2=(bme.readPressure() / 100.0F);
  //Serial.println(" hPa");
  
  //Serial.print("Approx. Altitude = ");
  float val3=bme.readAltitude(SEALEVELPRESSURE_HPA);
 // Serial.println(" m");
  
  //Serial.print("Humidity = ");
  float val4=bme.readHumidity();
 // Serial.println(" %");
  
  //Serial.println();
  float array[]={val1,val2,val3,val4};
  bmemsg.data=array;
  bmemsg.data_length=4;
  sens.publish(&bmemsg);
  nh.spinOnce(); 
  delay(100);

}