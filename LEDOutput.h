#ifndef LEDOutput_h
#define LEDOutput_h

#include "common.h"


class LEDOutput {
  const byte errorPin, pin1, pin2;
  bool blinkMode = false;
  unsigned long lastChange = 0;

public:
  LEDOutput(byte errPin, byte led1Pin, byte led2Pin)
    : errorPin(errPin), pin1(led1Pin), pin2(led2Pin) {}

  void attach() {
    pinMode(this->errorPin, OUTPUT);
    pinMode(this->pin1, OUTPUT);
    pinMode(this->pin2, OUTPUT);

    this->setFatalError();
    delay(1000);
  }

  void setError() {
    digitalWrite(this->errorPin, HIGH);
  }

  void reset() {
    digitalWrite(this->errorPin, LOW);
    digitalWrite(this->pin1, LOW);
    digitalWrite(this->pin2, LOW);
    this->blinkMode = false;    
  }

  void setFatalError() {
    digitalWrite(this->errorPin, HIGH);
    digitalWrite(this->pin1, HIGH);
    digitalWrite(this->pin2, HIGH);
  }

  void setSinglePositionSet() {
    digitalWrite(this->pin1, HIGH);
    digitalWrite(this->pin2, LOW);
    this->blinkMode = false;
  }

  void setBothPositionsSet() {
    digitalWrite(this->pin1, HIGH);
    digitalWrite(this->pin2, HIGH);
    this->blinkMode = false;
  }

  void setAutoPilotMode(bool on) {
    if (on) {
      digitalWrite(this->pin1, HIGH);
      digitalWrite(this->pin2, LOW);
    }
    else {
      this->setBothPositionsSet();
    }
    this->blinkMode = on;
  }

  void tick(unsigned long time) {
    if (!this->blinkMode){
      return;
    }
    if (time - this->lastChange > 500000) {
      digitalWrite(this->pin1, !digitalRead(this->pin1));
      digitalWrite(this->pin2, !digitalRead(this->pin2));
      this->lastChange = time;
    }
  }
};

#endif LEDOutput_h