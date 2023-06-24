#ifndef LEDOutput_h
#define LEDOutput_h

#include "common.h"


class LEDOutput {
  const byte errorPin, pin1, pin2;

public:
  LEDOutput(byte errPin, byte led1Pin, byte led2Pin)
    : errorPin(errPin), pin1(led1Pin), pin2(led2Pin) {}

  void attach() {
    pinMode(this->errorPin, OUTPUT);
    pinMode(this->pin1, OUTPUT);
    pinMode(this->pin2, OUTPUT);

    digitalWrite(this->errorPin, HIGH);
    digitalWrite(this->pin1, HIGH);
    digitalWrite(this->pin2, HIGH);

    delay(1000);


  }

  void setError() {
    digitalWrite(this->errorPin, HIGH);
  }

  void reset() {
    digitalWrite(this->errorPin, LOW);
    digitalWrite(this->pin1, LOW);
    digitalWrite(this->pin2, LOW);
  }

  void setSinglePositionSet() {
    digitalWrite(this->pin1, HIGH);
    digitalWrite(this->pin2, LOW);
  }
  void setBothPositionsSet() {
    digitalWrite(this->pin1, HIGH);
    digitalWrite(this->pin2, HIGH);
  }

  void tick(unsigned long time) {
  }
};

#endif LEDOutput_h