#include <ros.h>
#include <std_msgs/Float32MultiArray.h>
#include <Wire.h>

#define PI 3.14159
#define AS5600_ADDR 0x36  // Default I2C address for AS5600

// Create Two I2C Buses
TwoWire I2C_1 = TwoWire(0);  // I2C_1 on Wire(0)
TwoWire I2C_2 = TwoWire(1);  // I2C_2 on Wire(1)

ros::NodeHandle nh;
std_msgs::Float32MultiArray angles;
ros::Publisher angle_sender("angles", &angles);

// Function to read the raw angle from AS5600
uint16_t readAS5600Angle(TwoWire &i2c) {
  i2c.beginTransmission(AS5600_ADDR);
  i2c.write(0x0C);  // Address for raw angle register
  if (i2c.endTransmission() != 0) {
    return 0;  // Return 0 if transmission fails
  }

  i2c.requestFrom(AS5600_ADDR, 2);  // Request 2 bytes from AS5600

  if (i2c.available() == 2) {
    uint16_t highByte = i2c.read();
    uint16_t lowByte = i2c.read();
    return (highByte << 8) | lowByte;  // Combine bytes into one 12-bit value
  } 

  return 0;  // Return 0 if no data is available
}

void setup() {
  nh.initNode();
  I2C_1.begin(21, 22);  // SDA, SCL
  I2C_2.begin(19, 18);  // SDA, SCL

  angles.data_length = 2;
  angles.data = (float *)malloc((sizeof(float)) * angles.data_length);
  nh.advertise(angle_sender);
}

void loop() {
  // Read raw angles from both sensors
  uint16_t rawangle1 = readAS5600Angle(I2C_1);
  uint16_t rawangle2 = readAS5600Angle(I2C_2);

  // Convert raw values to degrees
  float angle1 = (rawangle1 * 360.0) / 4096.0;
  float angle2 = (rawangle2 * 360.0) / 4096.0;
  
  // change the 187.47 and 49.04 to calibrated angles

  angle2 = angle2 - 187.47 + 61.739;
  angle1 = angle1 - 49.04 + 68.162;

  

  // Compute final angle2 value based on a formula
  angle2 = PI * (angle2) / 180 ;
  angle1 = PI * (angle1)/180 - PI;

  angles.data[0] = angle2;
  angles.data[1] = angle1;
  // Populate and publish ROS message
  

  angle_sender.publish(&angles);
  nh.spinOnce();  // Process ROS communication
  delay(200);     // Delay for readability
}
