#include <Wire.h>

// I2C address for the AS5600 encoders (both encoders share the same address)
#define AS5600_ADDR 0x36

// Create Two I2C Buses
TwoWire I2C_1 = TwoWire(0);  // I2C_1 on Wire(0)
TwoWire I2C_2 = TwoWire(1);  // I2C_2 on Wire(1)

void setup() {
  Serial.begin(115200);

  // Initialize I2C_1 (Encoder 1) on GPIO 21 (SDA) and GPIO 22 (SCL)
  I2C_1.begin(21, 22);  // SDA, SCL

  // Initialize I2C_2 (Encoder 2) on GPIO 19 (SDA) and GPIO 18 (SCL)
  I2C_2.begin(19, 18);  // SDA, SCL

  Serial.println("I2C buses initialized.");
}

uint16_t readAS5600Angle(TwoWire &i2c) {
  i2c.beginTransmission(AS5600_ADDR);
  i2c.write(0x0C);  // Address for raw angle register
  if (i2c.endTransmission() != 0) {
    Serial.println("Error in transmission");
    return 0;  // Return 0 if transmission fails
  }

  i2c.requestFrom(AS5600_ADDR, 2);  // Request 2 bytes from AS5600

  if (i2c.available() == 2) {
    uint16_t highByte = i2c.read();
    uint16_t lowByte = i2c.read();
    return (highByte << 8) | lowByte;  // Combine bytes into one 12-bit value
  } else {
    Serial.println("Failed to receive data from encoder");
  }

  return 0;  // Return 0 if no data is available
}

void readEncoders() {
  // Small delay between reads to stabilize communication
  delay(100);

  // Read Encoder 1 (on I2C_1)
  uint16_t angle1 = readAS5600Angle(I2C_1);
  float degrees1 = (angle1 * 360.0) / 4096.0;
  Serial.print("Encoder 1 Angle (Raw): ");
  Serial.print(angle1);
  Serial.print(" | Degrees: ");
  Serial.println(degrees1);

  delay(100);  // Small delay between readings

  // Read Encoder 2 (on I2C_2)
  uint16_t angle2 = readAS5600Angle(I2C_2);
  float degrees2 = (angle2 * 360.0) / 4096.0;
  Serial.print("Encoder 2 Angle (Raw): ");
  Serial.print(angle2);
  Serial.print(" | Degrees: ");
  Serial.println(degrees2);
}

void loop() {
  readEncoders();  // Read both encoders
  delay(2000);
}

