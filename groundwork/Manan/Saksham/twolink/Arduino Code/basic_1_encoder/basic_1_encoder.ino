#include <Wire.h>

#define AS5600_I2C_ADDR 0x36     // Default I2C address for AS5600
#define RAW_ANGLE_REGISTER 0x0C  // Register to read the raw angle value

void setup() {
  Serial.begin(9600);
  Wire.begin();  // Initialize I2C communication
}

void loop() {
  uint16_t rawAngle = readRawAngle();
  float angleDegrees = rawAngle * 0.0879;  // Convert raw value to degrees (0-360)

  Serial.print("Angle: ");
  Serial.print(angleDegrees);
  Serial.println("°");

  delay(100);  // Delay for readability
}

// Function to read the raw angle from AS5600
uint16_t readRawAngle() {
  Wire.beginTransmission(AS5600_I2C_ADDR);
  Wire.write(RAW_ANGLE_REGISTER);  // Point to the angle register
  Wire.endTransmission();

  Wire.requestFrom(AS5600_I2C_ADDR, 2);  // Request 2 bytes for angle data
  uint16_t rawAngle = 0;

  if (Wire.available() == 2) {
    rawAngle = Wire.read();                    // MSB
    rawAngle = (rawAngle << 8) | Wire.read();  // LSB
  }

  return rawAngle;
}