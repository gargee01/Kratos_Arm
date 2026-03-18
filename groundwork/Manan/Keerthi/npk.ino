#include <SoftwareSerial.h>
#include <Wire.h>
#include <Arduino.h>
#include <ros.h>
#include <std_msgs/Float64MultiArray.h>
ros::NodeHandle  nh;

std_msgs::Float64MultiArray npk;

// RE and DE Pins set the RS485 module
// to Receiver or Transmitter mode
#define RE 32
#define DE 33

ros::Publisher sens("/npk", &npk);

// Modbus RTU requests for reading NPK values
const byte nitro[] = {0x01,0x03, 0x00, 0x1e, 0x00, 0x01, 0xe4, 0x0c};
const byte phos[] = {0x01,0x03, 0x00, 0x1f, 0x00, 0x01, 0xb5, 0xcc};
const byte pota[] = {0x01,0x03, 0x00, 0x20, 0x00, 0x01, 0x85, 0xc0};
const byte temp[] = {0x01,0x03, 0x00, 0x13, 0x00, 0x01, 0x75, 0xcf};//
const byte mois[]  = {0x01,0x03,0x00,0x12,0x00,0x01,0x24,0x0F};
const byte econ[] = {0x01,0x03, 0x00, 0x15, 0x00, 0x01, 0x95, 0xce};
const byte ph[] = {0x01,0x03, 0x00, 0x06, 0x00, 0x01, 0x64, 0x0b};

// A variable used to store NPK values
byte values[11];

// Sets up a new SoftwareSerial object
// Digital pins 10 and 11 should be used with a Mega or Mega 2560
SoftwareSerial mod(16, 17);
//SoftwareSerial mod(10, 11);
 
void setup() {
  // Set the baud rate for the Serial port
 // Serial.begin(115200);

  // Set the baud rate for the SerialSoftware object
  mod.begin(9600);
  nh.initNode();
  nh.advertise(sens);
  npk.layout.dim=NULL;
  npk.layout.data_offset=0;

  // Define pin modes for RE and DE
  pinMode(RE, OUTPUT);
  pinMode(DE, OUTPUT);
  
  delay(500);
} 

void loop() {
  byte val1,val2,val3,val4,val5,val6,val7;
  val1 = moisture()/1.8;
  delay(10);
  val2 = temperature()/10.0;
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

  float array[]={val1,val2,val3,val4,val5,val6,val7};
  npk.data=array;
  npk.data_length=7;
  sens.publish(&npk);
  nh.spinOnce();
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