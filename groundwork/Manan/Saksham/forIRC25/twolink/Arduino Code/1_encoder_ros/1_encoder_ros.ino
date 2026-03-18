#include <Wire.h>
#include <ros.h>
//#include <std_msgs/String.h>
#include <std_msgs/Float32MultiArray.h>

#define PI 3.14159
#define AS5600_I2C_ADDR 0x36  // Default I2C address for AS5600
#define RAW_ANGLE_REGISTER 0x0C  // Register to read the raw angle value

ros::NodeHandle nh;
std_msgs::Float32MultiArray angles;
ros::Publisher angle_sender("angles", &angles);

void setup() {
  nh.initNode();
  //Serial.begin(9600);
  Wire.begin();  // Initialize I2C communication
  angles.data_length = 2;
  angles.data = (float *)malloc((sizeof(float))*angles.data_length);
  nh.advertise(angle_sender);
}

void loop() {
  
  
  uint16_t rawAngle = readRawAngle();
  float angleDegrees = rawAngle * 0.0879;  // Convert raw value to degrees (0-360)
  
  //Serial.print("Angle: ");
  //Serial.print(angleDegrees);
  //Serial.println("°");
  if (angleDegrees >= 170){
    angleDegrees = angleDegrees - 353.18;
  }
  else {
    angleDegrees = (360 - 353.18) + angleDegrees;
  }
  
  float angle1 = (PI *61.739)/180;
  float angle2 =  PI *(68.162 + angleDegrees)/180 - PI;
  angles.data[0] = angle1;
  angles.data[1] = angle2;
  delay(100);
  angle_sender.publish(&angles);
  nh.spinOnce(); // Delay for readability
}

// Function to read the raw angle from AS5600
uint16_t readRawAngle() {
  Wire.beginTransmission(AS5600_I2C_ADDR);
  Wire.write(RAW_ANGLE_REGISTER);  // Point to the angle register
  Wire.endTransmission();

  Wire.requestFrom(AS5600_I2C_ADDR, 2);  // Request 2 bytes for angle data
  uint16_t rawAngle = 0;
  
  if (Wire.available() == 2) {
    rawAngle = Wire.read();            // MSB
    rawAngle = (rawAngle << 8) | Wire.read();  // LSB
  }
  


  return rawAngle;
}