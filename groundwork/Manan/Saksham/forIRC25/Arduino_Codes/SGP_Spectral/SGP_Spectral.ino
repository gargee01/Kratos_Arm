#include <Wire.h>
#include "Adafruit_SGP30.h"
#include "SparkFun_AS7265X.h" // Include the SparkFun AS7265X library
#include <Arduino.h>
#include <ros.h>
#include <std_msgs/Float64MultiArray.h>

ros::NodeHandle nh;
AS7265X sensor; // Create the sensor object

// Define the ROS message and publisher
std_msgs::Float64MultiArray spectral_msg;
ros::Publisher spectral_pub("spectral", &spectral_msg);

// Fixed-size array for storing spectral data
float spectral_data[18];

std_msgs::Float64MultiArray sgptype;

ros::Publisher sens("/sgp", &sgptype);
// Create an SGP30 object
Adafruit_SGP30 sgp;
TwoWire I2C_1(0);
TwoWire I2C_2(1);
void setup() {
  // Start the serial communication
  //Serial.begin(9600);
  //Serial.println("SGP30 CO2 and TVOC Sensor Test");
  I2C_1.begin(25,26);
  I2C_2.begin(21,22);

  // Initialize the I2C communication and check for the sensor
  sgp.begin(&I2C_1);
   
  nh.initNode();
      while (!nh.connected()) {
    nh.spinOnce();  // Keep checking for connection
    delay(100);   
      // Short delay to prevent busy-waiting
  
  sensor.begin(I2C_2);

  // Set up the spectral message
  spectral_msg.data_length = 18; // Fixed array size
  spectral_msg.data = spectral_data;

  // Advertise the topic
  nh.advertise(spectral_pub);
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
  sgp.begin(&I2C_1);
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
  // Take spectral measurements
  sensor.begin(I2C_2);
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


  // Wait before the next measurement
  nh.spinOnce();
  delay(1000);
  //Serial.println(" ppb");

  // Wait for a second before taking the next reading

  // To get more accurate long-term results, baseline values should be saved and restored periodically.
  // Here's how you can print the baseline values every 10 seconds (after at least 12 hours of continuous running):

  
}