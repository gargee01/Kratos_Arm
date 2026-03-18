#include <BluetoothSerial.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run make menuconfig to enable it.
#endif

BluetoothSerial SerialBT;

const int PWM1 = 5;
const int PWM2 = 14;
const int DIR1 = 18;
const int DIR2 = 12;
#define BAUDRATE 115200

// Movement and speed variables
int speed = 30;
int dir1 = 0, dir2 = 0;
bool paused = false;  // Flag to track paused state

// Command definitions
#define FORWARD 'F'
#define BACKWARD 'B'
#define LEFT 'L'
#define RIGHT 'R'
#define CIRCLE 'C'
#define CROSS 'X'
#define TRIANGLE 'T'
#define SQUARE 'S'
#define START 'A'
#define PAUSE 'P'

void setup() {
  Serial.begin(115200);  // Start serial communication for debugging
  SerialBT.begin("mini_rover");  // Bluetooth device name
  Serial.println("Bluetooth device started, now you can pair it with your smartphone!");
  pinMode(PWM1, OUTPUT);
  pinMode(PWM2, OUTPUT);
  pinMode(DIR1, OUTPUT);
  pinMode(DIR2, OUTPUT);
}

void loop() {
  if (SerialBT.available()) {
    char command = SerialBT.read();
    executeCommand(command);
  }
  delay(100);
}

void executeCommand(char command) {
  if (speed > 100) speed = 100;
  if (speed < 1) speed = 0;

  switch (command) {
    case FORWARD:
      if (!paused) {  // Only move if not paused
        dir1 = 0;
        dir2 = 0;
        Serial.println("F");
      }
      break;
    case BACKWARD:
      if (!paused) {
        dir1 = 1;
        dir2 = 1;
        Serial.println("B");
      }
      break;
    case LEFT:
      if (!paused) {
        dir1 = 0;
        dir2 = 1;
        Serial.println("L");
      }
      break;
    case RIGHT:
      if (!paused) {
        dir1 = 1;
        dir2 = 0;
        Serial.println("R");
      }
      break;
    case CIRCLE:
      // Perform action for circle
      break;
    case CROSS:
      speed -= 10;
      if (speed < 1) speed = 1;  // Prevent speed from going negative
      break;
    case TRIANGLE:
      speed += 10;
      if (speed > 100) speed = 100;  // Cap the speed at 100
      break;
    case SQUARE:
      // Perform action for retrieving and sending status information
      break;
    case START:
      // Perform action for starting a process or operation
      break;
    case PAUSE:
      paused = !paused;  // Toggle the paused state
      if (paused) {
        analogWrite(PWM1, 0);  // Stop motors
        analogWrite(PWM2, 0);
        Serial.println("Paused");
      } else {
        // If unpaused, resume with the last direction and speed
        analogWrite(PWM1, speed);
        analogWrite(PWM2, speed);
        digitalWrite(DIR1, dir1);
        digitalWrite(DIR2, dir2);
        Serial.println("Resumed");
      }
      break;
    default:
      Serial.println("Invalid command received!");
      break;
  }

  if (!paused) {  // Only set PWM if not paused
    digitalWrite(DIR1, dir1);
    digitalWrite(DIR2, dir2);
    analogWrite(PWM1, speed);
    analogWrite(PWM2, speed);
  }
}
