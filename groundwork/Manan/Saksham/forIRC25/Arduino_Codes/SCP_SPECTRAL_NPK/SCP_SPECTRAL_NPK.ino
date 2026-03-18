#include <Wire.h>
#include <SoftwareSerial.h>

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
#define RE 32
#define DE 33

std_msgs::Float64MultiArray npk;
ros::Publisher sensor1("/npk", &npk);

// Modbus RTU requests for reading NPK values
const byte nitro[] = {0x01,0x03, 0x00, 0x1e, 0x00, 0x01, 0xe4, 0x0c};
const byte phos[] = {0x01,0x03, 0x00, 0x1f, 0x00, 0x01, 0xb5, 0xcc};
const byte pota[] = {0x01,0x03, 0x00, 0x20, 0x00, 0x01, 0x85, 0xc0};
const byte temp[] = {0x01,0x03, 0x00, 0x13, 0x00, 0x01, 0x75, 0xcf};//
const byte mois[]  = {0x01,0x03,0x00,0x12,0x00,0x01,0x24,0x0F};
const byte econ[] = {0x01,0x03, 0x00, 0x15, 0x00, 0x01, 0x95, 0xce};
const byte ph[] = {0x01,0x03, 0x00, 0x06, 0x00, 0x01, 0x64, 0x0b};

Adafruit_SGP30 sgp;
TwoWire I2C_1(0);
TwoWire I2C_2(1);

// A variable used to store NPK values
byte values[11];

// Sets up a new SoftwareSerial object
// Digital pins 10 and 11 should be used with a Mega or Mega 2560
SoftwareSerial mod(16, 17);
//SoftwareSerial mod(10, 11);

void setup() {
  // Start the serial communication
  //Serial.begin(9600);
  //Serial.println("SGP30 CO2 and TVOC Sensor Test");
  I2C_1.begin(25,26);
  I2C_2.begin(21,22);

  mod.begin(9600);
  nh.advertise(sensor1);
  npk.layout.dim=NULL;
  npk.layout.data_offset=0;

  // Define pin modes for RE and DE
  pinMode(RE, OUTPUT);
  pinMode(DE, OUTPUT);
  
  delay(500);

  // Initialize the I2C communication and check for the sensor
  sgp.begin(&I2C_1);
   
  nh.initNode();
      
     // Keep checking for connection
       
      // Short delay to prevent busy-waiting
  
  sensor.begin(I2C_2);

  // Set up the spectral message
  spectral_msg.data_length = 18; // Fixed array size
  spectral_msg.data = spectral_data;

  // Advertise the topic
  nh.advertise(spectral_pub);

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
  byte val11,val22,val3,val4,val5,val6,val7;
  val11 = moisture()/1.8;
  delay(10);
  val22 = temperature()/10.0;
  delay(10);
  val3 = econduc();
  delay(10);
  val4 = phydrogen()/25;
  delay(10);
  val5 = nitrogen();
  delay(10);
  val6 = phosphorous();
  delay(10);
  val7 = potassium();
  delay(10);

  float array2[]={val11,val22,val3,val4,val5,val6,val7};
  npk.data=array2;
  npk.data_length=7;
  sensor1.publish(&npk);
  nh.spinOnce();
  delay(1000);
  
}

byte moisture() {
  // clear the receive buffer
  //mod.flushInput();

  // switch RS-485 to transmit mode
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(1);

  // write out the message
  for (uint8_t i = 0; i < sizeof(mois); i++) mod.write(mois[i]);

  // wait for the transmission to complete
  mod.flush();

  // switching RS485 to receive mode
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);

  // delay to allow response bytes to be received!
  delay(200);

  // read in the received bytes
  for (byte i = 0; i < 7; i++) {
    values[i] = mod.read();
    // Serial.print(values[i], HEX);
    // Serial.print(' ');
  }
  return values[4];
}

byte temperature() {
  // clear the receive buffer
  //mod.flushInput();

  // switch RS-485 to transmit mode
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(1);

  // write out the message
  for (uint8_t i = 0; i < sizeof(temp); i++) mod.write(temp[i]);

  // wait for the transmission to complete
  mod.flush();

  // switching RS485 to receive mode
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);

  // delay to allow response bytes to be received!
  delay(200);

  // read in the received bytes
  for (byte i = 0; i < 7; i++) {
    values[i] = mod.read();
    // Serial.print(values[i], HEX);
    // Serial.print(' ');
  }
  return values[3]<<8|values[4];
}

byte econduc() {
  // clear the receive buffer
  //mod.flushInput();

  // switch RS-485 to transmit mode
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(1);

  // write out the message
  for (uint8_t i = 0; i < sizeof(econ); i++) mod.write(econ[i]);

  // wait for the transmission to complete
  mod.flush();

  // switching RS485 to receive mode
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);

  // delay to allow response bytes to be received!
  delay(200);

  // read in the received bytes
  for (byte i = 0; i < 7; i++) {
    values[i] = mod.read();
    // Serial.print(values[i], HEX);
    // Serial.print(' ');
  }
  return values[4];
}

byte phydrogen() {
  // clear the receive buffer
  //mod.flushInput();
  // switch RS-485 to transmit mode
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(1);

  // write out the message
  for (uint8_t i = 0; i < sizeof(ph); i++) mod.write(ph[i]);

  // wait for the transmission to complete
  mod.flush();

  // switching RS485 to receive mode
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);

  // delay to allow response bytes to be received!
  delay(200);

  // read in the received bytes
  for (byte i = 0; i < 7; i++) {
    values[i] = mod.read();
    // Serial.print(values[i], HEX);
    // Serial.print(' ');
  }
  return values[4];
}

byte nitrogen() {
  // clear the receive buffer
  //mod.flushInput();

  // switch RS-485 to transmit mode
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(1);

  // write out the message
  for (uint8_t i = 0; i < sizeof(nitro); i++) mod.write(nitro[i]);

  // wait for the transmission to complete
  mod.flush();

  // switching RS485 to receive mode
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);

  // delay to allow response bytes to be received!
  delay(200);

  // read in the received bytes
  for (byte i = 0; i < 7; i++) {
    values[i] = mod.read();
    // Serial.print(values[i], HEX);
    // Serial.print(' ');
  }
  return values[4];
}

byte phosphorous() {
  //mod.flushInput();
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(1);
  for (uint8_t i = 0; i < sizeof(phos); i++) mod.write(phos[i]);
  mod.flush();
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);
  // delay to allow response bytes to be received!
  delay(200);
  for (byte i = 0; i < 7; i++) {
    values[i] = mod.read();
    // Serial.print(values[i], HEX);
    // Serial.print(' ');
  }
  return values[4];
}

byte potassium() {
  //mod.flushInput();
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(1);
  for (uint8_t i = 0; i < sizeof(pota); i++) mod.write(pota[i]);
  mod.flush();
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);
  // delay to allow response bytes to be received!
  delay(200);
  for (byte i = 0; i < 7; i++) {
    values[i] = mod.read();
    // Serial.print(values[i], HEX);
    // Serial.print(' ');
  }
  return values[4];
}

