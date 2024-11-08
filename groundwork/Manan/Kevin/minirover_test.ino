
#include <BluetoothSerial.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run make menuconfig to enable it.
#endif

BluetoothSerial SerialBT;

const int PWM1=5;
const int PWM2= 21;
const int DIR1 =18;
const int DIR2 =19;
#define BAUDRATE 115200


// Movement and speed variables
int speed = 30;
int dir1=0, dir2=0;

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
  SerialBT.begin("Kratos_ESP32");  // Bluetooth device name
  Serial.println("Bluetooth device started, now you can pair it with your smartphone!");
  pinMode(PWM1,OUTPUT);
  pinMode(PWM2,OUTPUT);
  pinMode(DIR1,OUTPUT);
  pinMode(DIR2,OUTPUT);
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
      dir1=0;
      dir2=0;
      Serial.println("F");
      break;
    case BACKWARD:
      dir1=1;
      dir2=1;
      Serial.println("B");
      break;
    case LEFT:
      dir1=1;
      dir2=0;
      Serial.println("L");
      break;
    case RIGHT:
      dir1=0;
      dir2=1;
      Serial.println("R");
      break;
    case CIRCLE:
      // Perform action for circle
      break;
    case CROSS:
      speed -= 10;
      break;
    case TRIANGLE:
      speed += 10;
      break;
    case SQUARE:
      // Perform action for retrieving and sending status information
      break;
    case START:
      // Perform action for starting a process or operation
      break;
    case PAUSE:
      analogWrite(PWM1,0);
      analogWrite(PWM2,0);
      return;
      break;
    default:
      Serial.println("Invalid command received!");
      break;
  }
  digitalWrite(DIR1,dir1);
  digitalWrite(DIR2,dir2);
  analogWrite(PWM1,speed);
  analogWrite(PWM2,speed);

}