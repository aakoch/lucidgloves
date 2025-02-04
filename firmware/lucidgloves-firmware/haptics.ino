#if USING_FORCE_FEEDBACK


void setupServoHaptics(){
  pinkyServo.attach(PIN_PINKY_MOTOR);
  ringServo.attach(PIN_RING_MOTOR);
  middleServo.attach(PIN_MIDDLE_MOTOR);
  indexServo.attach(PIN_INDEX_MOTOR);
  thumbServo.attach(PIN_THUMB_MOTOR);
}

//static scaling, maps to entire range of servo
void scaleLimits(int* hapticLimits, float* scaledLimits){
  for (int i = 0; i < 5; i++){
    scaledLimits[i] = 180.0f - hapticLimits[i] / 1000.0f * 180.0f;
  }
}

//dynamic scaling, maps to the limits calibrated from your finger
void dynScaleLimits(int* hapticLimits, float* scaledLimits){
  //will be refactored to take min and max as an argument

  /* this implementation of dynamic scaling relies on the assumption 
   * that the servo reaches 2/3 of the potentiometer's range, 
   * and that 0 degrees is geared to the start of the potentiometer.
   * Different hardware types may need to handle dynamic scaling differently.
   */
  for (int i = 0; i < sizeof(hapticLimits); i++){
    scaledLimits[i] = hapticLimits[i] / 1000.0f * 180.0f;
  }
}

void writeServoHaptics(int* hapticLimits){
  float scaledLimits[5];
  Serial.println("hapticLimits: "+ (String)hapticLimits[0] + " " + (String)hapticLimits[1] + " " + (String)hapticLimits[2] + " " + (String)hapticLimits[3] + " " + (String)hapticLimits[4] + " ");
  dynScaleLimits(hapticLimits, scaledLimits);
  pinkyServo.write(scaledLimits[4]);
  ringServo.write(scaledLimits[3]);
  middleServo.write(scaledLimits[2]);
  indexServo.write(scaledLimits[1]);
  thumbServo.write(scaledLimits[0]);
  Serial.println("scaledLimits: "+ (String)scaledLimits[0] + " " + (String)scaledLimits[1] + " " + (String)scaledLimits[2] + " " + (String)scaledLimits[3] + " " + (String)scaledLimits[4] + " ");
}

#endif
