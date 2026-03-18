#include <Wire.h>
#include "Adafruit_SGP30.h"
#include <Arduino.h>
#include <ros.h>
#include <std_msgs/Float64MultiArray.h>
ros::NodeHandle nh;

std_msgs::Float64MultiArray sgptype;

ros::Publisher sens("/sgp", &sgptype);
// Create an SGP30 object
Adafruit_SGP30 sgp;

void setup() {
  // Start the serial communication
  //Serial.begin(9600);
  //Serial.println("SGP30 CO2 and TVOC Sensor Test");

  // Initialize the I2C communication and check for the sensor
  if (!sgp.begin()) {
    //Serial.println("Sensor not found :(");
    while (1);
  }
  nh.initNode();
      while (!nh.connected()) {
    nh.spinOnce();  // Keep checking for connection
    delay(100);     // Short delay to prevent busy-waiting
}
  nh.advertise(sens);
  sgptype.layout.dim=NULL;
  sgptype.layout.data_offset=0;
  // Print the unique ID of the sensor
  //Serial.print("Found SGP30 with serial #");
  //Serial.print(sgp.serialnumber[0], HEX);
  //Serial.print(sgp.serialnumber[1], HEX);
  //Serial.println(sgp.serialnumber[2], HEX);

  // Initialize baseline values (optional)
  // These can be retrieved and set after running the sensor for about 12 hours.
  // sgp.setIAQBaseline(0x8973, 0x8AAE);
}

void loop() {
  // Perform a measurement
  if (!sgp.IAQmeasure()) {
    //Serial.println("Measurement failed");
    return;
  }
  float val1,val2;
  // Print the eCO2 (equivalent CO2) and TVOC (total volatile organic compounds)
  //Serial.print("eCO2: ");
  val1=sgp.eCO2;
  //Serial.print(" ppm, TVOC: ");
  val2=sgp.TVOC;
  float array[]={val1,val2};
  sgptype.data=array;
  sgptype.data_length=2;
  sens.publish(&sgptype);
  nh.spinOnce();
  //Serial.println(" ppb");

  // Wait for a second before taking the next reading
  delay(1000);

  // To get more accurate long-term results, baseline values should be saved and restored periodically.
  // Here's how you can print the baseline values every 10 seconds (after at least 12 hours of continuous running):
  static uint32_t last_baseline_time = 0;
  if (millis() - last_baseline_time > 10000) {
    uint16_t TVOC_base, eCO2_base;
    if (!sgp.getIAQBaseline(&eCO2_base, &TVOC_base)) {
      //Serial.println("Failed to get baseline readings");
      return;
    }
    /*Serial.print("Baseline eCO2: 0x");
    Serial.print(eCO2_base, HEX);
    Serial.print(", TVOC: 0x");
    Serial.println(TVOC_base, HEX);*/
    last_baseline_time = millis();
  }
}