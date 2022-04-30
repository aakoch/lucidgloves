#include "ESP32Servo.h"
#include "TickTwo.h"

Servo pinkyServo;
Servo ringServo;
Servo middleServo;
Servo indexServo;
Servo thumbServo;

void readButton();
void loop1();
void setupServoHaptics();

TickTwo readButtonTimer(readButton, 100);
TickTwo loop1Timer(loop1, 300);

#define ALWAYS_CALIBRATING CALIBRATION_LOOPS == -1
#define SERVO_CALIBRATION_STATE 0
#define CALIBRATION_STATE 1
#define RUNNING_STATE 2
#define NOOP_STATE 3

ICommunication* comm;
int loops = 0;

void setup() {
  #if COMMUNICATION == COMM_SERIAL
    comm = new SerialCommunication();
  #elif COMMUNICATION == COMM_BTSERIAL
    comm = new BTSerialCommunication();
  #endif  
  comm->start();

  setupInputs();

  // pinMode(4, INPUT_PULLUP);

  #if USING_FORCE_FEEDBACK
    setupServoHaptics();
  #endif
  readButtonTimer.start();
  loop1Timer.start();
}

unsigned long time1;
unsigned long time2;
unsigned int i;
bool direction;

bool joyButton;

int lastVal0 = random(0, 1024);
int lastVal1 = random(0, 1024);
int lastVal2 = random(0, 1024);
int lastVal3 = random(0, 1024);
int lastVal4 = random(0, 1024);

int randomValue(int inVal) {
  if (inVal > 1019) 
    return inVal - random(20);
  if (inVal < 5) 
    return inVal + random(20);
  return inVal + random(20) - 5;
}

// bool randomData(char* input) {
//   // lastVals = randomVals(&lastVals);
//   lastVal0 = randomValue(lastVal0);
//   lastVal1 = randomValue(lastVal1);
//   lastVal2 = randomValue(lastVal2);
//   lastVal3 = randomValue(lastVal3);
//   lastVal4 = randomValue(lastVal4);

//   // String message = "A" + (String) random(5095) + "B" + (String) random(5095) + "C" + (String) random(5095) + "D" + (String) random(5095) + "E" + (String) random(5095) + "F" + (String) random(5095) + "G" + (String) random(5095);
//   String message = "A" + (String) lastVal0+ "B" + (String) lastVal1 + "C" + (String) lastVal2 + "D" + (String) lastVal3 + "E" + (String) lastVal4;
//   // String message = "A" + (String) (sin(i / 314) * 1028) + "B" + (String) random(2048) + "C" + (String) random(2048) + "D" + (String) random(2048) + "E" + (String) random(2048) + "F" + (String) random(2048) + "G" + (String) random(2048);

//   strcpy(input, message.c_str());

//   if (i >= 1028) {
//     direction = true;
//   }
//   else if (i <= 0) {
//     direction = false;
//   }

//   if (direction) {
//     i -= 2;
//   }
//   else {
//     i += 2;
//   }

//   // 0 in hapticLimits -> 180 degrees in scaledLimits
//   // 1028 in hapticLimits -> 0 degrees in scaledLimits

//   return input != NULL && strlen(input) > 0;
// }


void loopRun() {
  if (comm->isOpen()){
    #if USING_CALIB_PIN
    bool calibButton = getButton(PIN_CALIB) != INVERT_CALIB;
    if (calibButton)
      loops = 0;
    #else
    bool calibButton = false;
    #endif
    
    bool calibrate = false;
    if (loops < CALIBRATION_LOOPS || ALWAYS_CALIBRATING){
      calibrate = true;
      loops++;
    }
    
    int* fingerPos = getFingerPositions(calibrate, calibButton);
    bool joyButton = getButton(PIN_JOY_BTN) != INVERT_JOY;

    #if TRIGGER_GESTURE
    bool triggerButton = triggerGesture(fingerPos);
    #else
    bool triggerButton = getButton(PIN_TRIG_BTN) != INVERT_TRIGGER;
    #endif

    bool aButton = getButton(PIN_A_BTN) != INVERT_A;
    bool bButton = getButton(PIN_B_BTN) != INVERT_B;

    #if GRAB_GESTURE
    bool grabButton = grabGesture(fingerPos);
    #else
    bool grabButton = getButton(PIN_GRAB_BTN) != INVERT_GRAB;
    #endif

    #if PINCH_GESTURE
    bool pinchButton = pinchGesture(fingerPos);
    #else
    bool pinchButton = getButton(PIN_PNCH_BTN) != INVERT_PINCH;
    #endif

    bool menuButton = getButton(PIN_MENU_BTN) != INVERT_MENU;
    
    comm->output(encode(fingerPos, getJoyX(), getJoyY(), joyButton, triggerButton, aButton, bButton, grabButton, pinchButton, calibButton, menuButton));

    #if USING_FORCE_FEEDBACK
      char received[100];
      if (comm->readData(received)){
        int hapticLimits[5];
        decodeData(received, hapticLimits);
        writeServoHaptics(hapticLimits);
      }
    #endif
    delay(LOOP_TIME);
  }
}

int angle = 0;
int state = 0;


void loop() {
  // Serial.print("loop1Timer.state()=");
  // Serial.println(loop1Timer.state());
  readButtonTimer.update();
  loop1Timer.update();
  if (!joyButton && state == RUNNING_STATE) {
    Serial.println("calling loopRun");
    loopRun();
  }
  else {
    // Serial.print("not calling loopRun - joyButton=");
    // Serial.println(joyButton);
    if (joyButton && loop1Timer.state() == PAUSED) {
      loop1Timer.resume();
    }
  }
}

void readButton() {
  joyButton = getButton(PIN_JOY_BTN) != INVERT_JOY;
  if (joyButton) {
    readButtonTimer.pause();
  }
}

void loop1() {

  if (joyButton) {
    readButtonTimer.resume();
    state++;
    if (state > NOOP_STATE) {
      state = SERVO_CALIBRATION_STATE;
    }
  }

  if (state == SERVO_CALIBRATION_STATE) {
    int joyY = getJoyY();
    Serial.println(joyY);
    Serial.println(joyButton);

    if (joyY < 1000) {
      angle = 0;
    }
    else if (joyY > 3500) {
      angle = 180;
    }

    pinkyServo.write(angle);
    ringServo.write(angle);
    middleServo.write(angle);
    indexServo.write(angle);
    thumbServo.write(angle);
  }
//   else if (state == CALIBRATION_STATE) {
//     while (notFound < 10 && calibrating && i > 0) {
//       pinValue = analogRead(4);
//       Serial.print("pin:");
//       Serial.println(pinValue);  // get value of Touch 0 pin = GPIO 4

//       if (pinValue == 0) {
//         notFound++;
//       }
//       else {
//         servo.write(i--);
//         notFound = 0;
//       }

//       delay(2);   
//     }

//     if (i < 10) {
//       Serial.print(i);
//       Serial.println(" Could not calibrate\n");
//     }
//     else {
//       Serial.print("Stop found at ");
//       Serial.print(i);
//       Serial.println(" degrees\n");
//     }

//     notFound = 0;
//     delay(10000);
//     i = 180;
//     servo.write(i);
//     delay(500);    

//     while (Serial.available() > 0) {

//       look for the next valid integer in the incoming serial stream:
//       int value = Serial.parseInt();
//       Serial.print("Read:"); 
//       Serial.println(value); 
//       servo.write(value);
//       delay(10000);

//     if (pinValue < 10) {
//       delay(10);
//       pinValue = analogRead(4);
//       Serial.print(",pin2:");
//       Serial.print(pinValue);  // get value of Touch 0 pin = GPIO 4
//       if (pinValue < 10) {
//         // stop
//         Serial.println(",stop");  // get value of Touch 0 pin = GPIO 4
//       }
//     }
//     else {
//       // keep moving
//       Serial.print(",i:");
//       Serial.println(i);
//       servo.write(i--);
//       if (i <= -180) {
//         i = 180;
//       }
//     }
//   }
  else if (state == RUNNING_STATE) {
    loop1Timer.pause();
  }

}
