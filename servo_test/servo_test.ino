#include <ESP32Servo.h>
#include <ESP32Tone.h>
#include <ESP32PWM.h>

Servo servo_1; // servo controller (multiple can exist)

int servo_pin = 5; // PWM pin for servo control
int pos = 0;    // servo starting position
int pot;

void setup() {
    Serial.begin(19200);
  servo_1.attach(servo_pin); // start servo control
  pinMode(18, INPUT_PULLUP);
}

void loop() {
//  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
//    Serial.println(pos);
//    // in steps of 1 degree
//    servo_1.write(pos);              // tell servo to go to position in variable 'pos'
//    delay(15); // delay to allow the servo to reach the desired position
//    if (pos==90){
//      delay(5000); //wait 5 seconds once positioned at 90 degrees
//    }
//  }
pot = analogRead(A0);
Serial.print("pot:");
Serial.print(pot);
Serial.print(",servo:");
// pos = random(0, 90);
pos = pot * 180 / 4095;
    Serial.println(pos);
  servo_1.write(pos); 
  delay(10); 

//pot = analogRead(A0);
//Serial.print("pot:");
//Serial.print(pot);
//Serial.print(",servo:");
//// pos = random(90, 180);
//pos = pot * 180 / 4095;
//    Serial.println(pos);
//  servo_1.write(pos); 
//  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
//    servo_1.write(pos);              // tell servo to go to position in variable 'pos'
//    delay(15);
//    if (pos==90){
//      delay(5000); // wait 5 seconds once positioned at 90 degrees
//    }
//  }
//  delay(1000); // wait 5 seconds after arriving back at 0 degrees
}
