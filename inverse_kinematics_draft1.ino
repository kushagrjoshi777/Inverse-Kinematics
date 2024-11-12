#include "USBHost_t36.h"
// try this code after removing all usb lines
USBHost myusb;
USBHub hub1(myusb);
USBHIDParser hid1(myusb);
JoystickController joystick1(myusb);
BluetoothController bluet(myusb, true, "0000");

USBDriver *drivers[] = {&hub1, &joystick1, &hid1};
#define CNT_DEVICES (sizeof(drivers)/sizeof(drivers[0]))

int m1dir = 0;
float m1speed = 1;
int m2dir = 2;
float m2speed = 3;
int m3dir = 4;
float m3speed = 5;

int leftX = 0;
int leftY = 0;
int rightX = 0;
int rightY = 0;
void setup() {
  Serial.begin(2000000);
  while (!Serial);
  Serial.println("\nPS4 Controller Joystick Test");
  myusb.begin();

  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  pinMode(m1dir, OUTPUT);
  pinMode(m1speed, OUTPUT);
  pinMode(m2dir, OUTPUT);
  pinMode(m2speed, OUTPUT);
  pinMode(m3dir, OUTPUT);
  pinMode(m3speed, OUTPUT);
}

void loop() {
  myusb.Task();

  if (joystick1.available()) {

    int lX = joystick1.getAxis(0);
    int lY = joystick1.getAxis(1);
    int rX = joystick1.getAxis(2);
    int rY = joystick1.getAxis(5); //change to 3 if not working

    leftX = map(lX, 0, 255, -127, 127);
    leftY = map(lY, 0, 255, 127, -127);
    rightX = map(rX, 0, 255, -127, 127);
    rightY = map(rY, 0, 255, 127, -127);

    inversekino(leftX, leftY, rightX);
    joystick1.joystickDataClear();
  }
}

void inversekino(int leftX, int leftY, int rightX){
  m1speed = 0.333*(-2*leftX+rightX);
  m2speed = 0.333*(leftX-leftY+rightX);
  m3speed = 0.333*(leftX+leftY+rightX);

  directionControl(m1speed,m1dir);
  directionControl(m2speed,m2dir);
  directionControl(m3speed,m3dir);

  Serial.print("Speed motor 1: ");
  Serial.print(abs(m1speed));
  Serial.print(" motor 2: ");
  Serial.print(abs(m2speed));
  Serial.print(" motor 3: ");
  Serial.println(abs(m3speed));
  
  delay(50);
}

void directionControl(int n, int m){
  if(n>=0){
    digitalWrite(m, HIGH);
  }
  else{
    digitalWrite(m, LOW);
  }
}