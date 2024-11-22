#include <Wire.h>
#include "SparkFun_AS7265X.h"

AS7265X mySensor;

float reference[18];  // Store reference values for absorbance calculation

void setup() {
  Serial.begin(115200);
  Wire.begin();  // Initialize I2C communication

  if (mySensor.begin() == false) {
    Serial.println("AS7265X not detected. Check wiring.");
    while (1);
  }

  // Take reference reading for absorbance (blank sample)
  takeReference();
}

void loop() {
  // Get the spectral data
  mySensor.takeMeasurements();

  // Wavelengths in nm corresponding to the AS7265x channels
  float wavelengths[18] = {410, 435, 460, 485, 510, 535, 560, 585, 610, 645, 
                           680, 705, 730, 760, 810, 860, 900, 940};

  // Prepare absorbance values for sending to Python
  String data = "";  // Create a string to hold the values

  // Get calibrated values for all 18 channels
  float calibratedValues[18] = {
    mySensor.getCalibratedA(), mySensor.getCalibratedB(), mySensor.getCalibratedC(),
    mySensor.getCalibratedD(), mySensor.getCalibratedE(), mySensor.getCalibratedF(),
    mySensor.getCalibratedG(), mySensor.getCalibratedH(), mySensor.getCalibratedI(),
    mySensor.getCalibratedJ(), mySensor.getCalibratedK(), mySensor.getCalibratedL(),
    mySensor.getCalibratedR(), mySensor.getCalibratedS(), mySensor.getCalibratedT(),
    mySensor.getCalibratedU(), mySensor.getCalibratedV(), mySensor.getCalibratedW()
  };

  // Calculate absorbance and prepare the data to send via Serial
  for (int i = 0; i < 18; i++) {
    float absorbance = -log10(calibratedValues[i] / reference[i]);
    data += String(wavelengths[i]) + "," + String(absorbance);
    if (i < 17) {
      data += ";";  // Separate values with a semicolon, except for the last one
    }
  }

  // Send the absorbance data to the serial port
  Serial.println(data);

  delay(5000);  // Wait before taking the next reading
}

void takeReference() {
  // Take multiple readings for stability
  for (int i = 0; i < 10; i++) {
    mySensor.takeMeasurements();
  }

  // Read reference values for all 18 channels
  reference[0] = mySensor.getCalibratedA();
  reference[1] = mySensor.getCalibratedB();
  reference[2] = mySensor.getCalibratedC();
  reference[3] = mySensor.getCalibratedD();
  reference[4] = mySensor.getCalibratedE();
  reference[5] = mySensor.getCalibratedF();
  reference[6] = mySensor.getCalibratedG();
  reference[7] = mySensor.getCalibratedH();
  reference[8] = mySensor.getCalibratedI();
  reference[9] = mySensor.getCalibratedJ();
  reference[10] = mySensor.getCalibratedK();
  reference[11] = mySensor.getCalibratedL();
  reference[12] = mySensor.getCalibratedR();
  reference[13] = mySensor.getCalibratedS();
  reference[14] = mySensor.getCalibratedT();
  reference[15] = mySensor.getCalibratedU();
  reference[16] = mySensor.getCalibratedV();
  reference[17] = mySensor.getCalibratedW();

  Serial.println("Reference readings captured.");
}
