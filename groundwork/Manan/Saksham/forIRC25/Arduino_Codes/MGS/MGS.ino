#include <Wire.h>
#include "MutichannelGasSensor.h"
#include <ros.h>
#include <std_msgs/Float32MultiArray.h>

// ROS node handle
ros::NodeHandle nh;

// ROS publisher
std_msgs::Float32MultiArray gas_data_msg;
ros::Publisher gas_publisher("/mgs", &gas_data_msg);

// Buffer for gas data message
char gas_data_buffer[256];

void setup() {
  // Initialize serial communication for debugging
  

  // Initialize ROS node handle
  nh.initNode();
  nh.advertise(gas_publisher);

  // Initialize the I2C bus and the multichannel gas sensor
  Wire.begin();
  gas.begin(0x04);

  gas_data_msg.data_length = 8;
  gas_data_msg.data = (float*)malloc(gas_data_msg.data_length * sizeof(float));
   // Default I2C address is 0x04
    

  // Calibrate the sensor (optional; uncomment if calibration is needed)
  // gas.powerOn();
}

void loop() {
  // Read gas concentration data
  float vals[8];
  vals[0] = gas.measure_NH3();
  vals[1] = gas.measure_CO();
  vals[2] = gas.measure_NO2();
  vals[3] = gas.measure_C3H8();
  vals[4] = gas.measure_C4H10();
  vals[5] = gas.measure_CH4();
  vals[6] = gas.measure_H2();
  vals[7] = gas.measure_C2H5OH();
  gas_data_msg.data = vals;



  // Publish the gas data to the /mgs topic
  // gas_data_msg.data = gas_data_buffer;
  gas_publisher.publish(&gas_data_msg);

  // Spin ROS to process callbacks
  nh.spinOnce();


  // Wait before the next reading
  delay(1000);  // Adjust based on your sensor's response time
}
