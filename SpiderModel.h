#ifndef SpiderModel_h
#define SpiderModel_h

#include "LEDOutput.h"
#include "MotorOutput.h"

#define LONG_PRESS 3000

class SpiderModel {

  MotorOutput* motor;
  LEDOutput* leds;

  int requestedSpeed;
  int currentSpeed;
  int currentPosition;
  
  bool leftSet;
  int letPosition;
  
  bool rightSet;
  int rightPosition;

  int minSpeedSet;
  byte dirrectionSet;

public:
  SpiderModel(MotorOutput* motor, LEDOutput* leds)
    : motor(motor), leds(leds) {
  }

  void reset() {
    this->requestedSpeed = 1500;
    this->currentSpeed = 1500;
    this->currentPosition = 0;
    this->motor->setSpeed(this->currentSpeed);
    this->leds->reset();
    this->leftSet = false;
    this->rightSet = false;
    this->minSpeedSet = 0;
    this->dirrectionSet = 0;
  }

  void changeMode() {
    if (!this->leftSet) {
      this->leftSet = true;
      this->letPosition = this->currentPosition;
      this->leds->setSinglePositionSet();
    }
    else if (!this->rightSet) {
        if (abs(this->letPosition - this->currentPosition) > 0) {
          this->rightPosition = this->currentPosition;
          this->rightSet = true;
          this->leds->setBothPositionsSet();
        }
    }
    else if (!this->minSpeedSet && abs(this->currentSpeed) > 10) {
      this->minSpeedSet = this->currentSpeed;
      this->leds->setAutoPilotMode(true);
    }
  }

  void setCurrentSpeed(int newSpeed) {
    this->currentSpeed = newSpeed - 1500;

    this->motor->setSpeed(newSpeed);
  }
  

  void tick(unsigned long clicked, int newSpeed, int position) {
    this->currentPosition = position;
    this->setCurrentSpeed(newSpeed);

    if (IS_CLICK(clicked)) {
      this->changeMode();
    }
    else if (IS_LONG_PRESS(clicked)) {
      if (this->minSpeedSet) {
        this->minSpeedSet = 0;
        this->leds->setAutoPilotMode(false);
      }
      else {
        this->reset();
      }
    }


  }
};

#endif SpiderModel_h