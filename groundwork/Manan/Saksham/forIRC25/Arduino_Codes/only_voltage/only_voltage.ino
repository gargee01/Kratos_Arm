#include <Wire.h>
#include <Adafruit_INA219.h>

// Create an instance of the sensor
Adafruit_INA219 ina219;

void setup() {
  // Start the serial monitor at 115200 baud
  Serial.begin(115200);
  
  // Initialize the INA219 sensor
  if (!ina219.begin()) {
    Serial.println("Failed to find INA219 sensor. Check wiring!");
    while (1);
  }

  // Print a startup message
  Serial.println("INA219 Current and Voltage Measurement");
}

void loop() {
  // Get the current and voltage measurements
  float voltage = ina219.getBusVoltage_V(); // Voltage in volts
  float current = ina219.getCurrent_mA(); // Current in milliamps
  
  // Print the results to the Serial Monitor
  Serial.print("Bus Voltage: ");
  Serial.print(voltage);
  Serial.println(" V");
  
  Serial.print("Current: ");
  Serial.print(current);
  Serial.println(" mA");
  
  // Add a small delay before the next reading
  delay(1000);
}