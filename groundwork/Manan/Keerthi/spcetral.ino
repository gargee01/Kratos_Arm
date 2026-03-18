#include <Wire.h>
#include "SparkFun_AS7265X.h" // Include the SparkFun AS7265X library
#include <ros.h>
#include <std_msgs/Float64MultiArray.h>

AS7265X sensor; // Create the sensor object
ros::NodeHandle nh; // Create a ROS node handle

// Define the ROS message and publisher
std_msgs::Float64MultiArray spectral_msg;
ros::Publisher spectral_pub("spectral", &spectral_msg);

// Fixed-size array for storing spectral data
float spectral_data[18];

void setup()
{
  // Initialize I2C
  Wire.begin(21, 22); // ESP32 I2C pins (SDA on GPIO21, SCL on GPIO22)

  // Initialize ROS node
  nh.initNode();

  // Initialize the sensor
  if (sensor.begin() == false)
  {
    nh.logerror("Sensor not connected. Check wiring!");
    while (1)
    {
      nh.spinOnce(); // Process ROS callbacks
      delay(1000);
    }
  }

  nh.loginfo("Spectral sensor initialized. Publishing data to /spectral.");

  // Set up the spectral message
  spectral_msg.data_length = 18; // Fixed array size
  spectral_msg.data = spectral_data;

  // Advertise the topic
  nh.advertise(spectral_pub);
}

void loop()
{
  // Take spectral measurements
  sensor.takeMeasurements();

  // Populate the spectral data array
  spectral_data[0] = sensor.getCalibratedA();
  spectral_data[1] = sensor.getCalibratedB();
  spectral_data[2] = sensor.getCalibratedC();
  spectral_data[3] = sensor.getCalibratedD();
  spectral_data[4] = sensor.getCalibratedE();
  spectral_data[5] = sensor.getCalibratedF();
  spectral_data[6] = sensor.getCalibratedG();
  spectral_data[7] = sensor.getCalibratedH();
  spectral_data[8] = sensor.getCalibratedR();
  spectral_data[9] = sensor.getCalibratedI();
  spectral_data[10] = sensor.getCalibratedS();
  spectral_data[11] = sensor.getCalibratedJ();
  spectral_data[12] = sensor.getCalibratedT();
  spectral_data[13] = sensor.getCalibratedU();
  spectral_data[14] = sensor.getCalibratedV();
  spectral_data[15] = sensor.getCalibratedW();
  spectral_data[16] = sensor.getCalibratedK();
  spectral_data[17] = sensor.getCalibratedL();

  // Publish the spectral data
  spectral_pub.publish(&spectral_msg);

  // Spin ROS node
  nh.spinOnce();

  // Wait before the next measurement
  delay(1000); // 1-second interval
}
