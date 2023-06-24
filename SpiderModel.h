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
        if (abs(this->letPosition - this->currentPosition) > 12) {
          this->rightPosition = this->currentPosition;
          this->rightSet = true;
          this->leds->setBothPositionsSet();
        }
    }
    else if (abs(this->currentSpeed) > 0) {
      this->minSpeedSet = abs(this->currentSpeed);
    }
  }


  

  void tick(unsigned long clicked, unsigned long newSpeed, int position) {
    this->currentPosition = position;
    if (IS_CLICK(clicked)) {
      this->changeMode();
    }
    else if (IS_LONG_PRESS(clicked)) {
      this->reset();
    }

    this->motor->setSpeed(newSpeed);

  }
};

#endif SpiderModel_h