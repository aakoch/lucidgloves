#include "ESP32Servo.h"

//joystick configuration
#define JOYSTICK_BLANK false //make true if not using the joystick
#define JOY_FLIP_X false
#define JOY_FLIP_Y false
#define JOYSTICK_DEADZONE 10 //deadzone in the joystick to prevent drift (in percent)
  
// Servo servo;
// int pinValue;
// int digitalPinValue;
// int i = 180;
// boolean calibrating = true;
// int notFound = 0;

// void setupServoCalibration() {
//   pinMode(4, INPUT_PULLUP);
//   Serial.begin(115200);
//   while(!Serial) {
//     ;
//   }
//   servo.attach(17);
//   delay(500); // give me time to bring up serial monitor
//   servo.write(i);
//   Serial.println("Servo calibration");
// }

// void loopServoCalibration() {

//   while (notFound < 10 && calibrating && i > 0) {
//     pinValue = analogRead(4);
//     Serial.print("pin:");
//     Serial.println(pinValue);  // get value of Touch 0 pin = GPIO 4

//     if (pinValue == 0) {
//       notFound++;
//     }
//     else {
//       servo.write(i--);
//       notFound = 0;
//     }

//     delay(2);   
//   }

//   if (i < 10) {
//     Serial.print(i);
//     Serial.println(" Could not calibrate\n");
//   }
//   else {
//     Serial.print("Stop found at ");
//     Serial.print(i);
//     Serial.println(" degrees\n");
//   }

//   notFound = 0;
//   delay(10000);
//   i = 180;
//   servo.write(i);
//   delay(500);    

//   // while (Serial.available() > 0) {

//     // look for the next valid integer in the incoming serial stream:
//     // int value = Serial.parseInt();
//     // Serial.print("Read:"); 
//     // Serial.println(value); 
//     // servo.write(value);
//     // delay(10000);

//   // if (pinValue < 10) {
//   //   delay(10);
//   //   pinValue = analogRead(4);f
//   //   Serial.print(",pin2:");
//   //   Serial.print(pinValue);  // get value of Touch 0 pin = GPIO 4
//   //   if (pinValue < 10) {
//   //     // stop
//   //     Serial.println(",stop");  // get value of Touch 0 pin = GPIO 4
//   //   }
//   // }
//   // else {
//   //   // keep moving
//   //   Serial.print(",i:");
//   //   Serial.println(i);
//   //   servo.write(i--);
//   //   if (i <= -180) {
//   //     i = 180;
//   //   }
//   // }
  
//   delay(10);
// }
