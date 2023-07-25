#ifndef SpiderModel_h
#define SpiderModel_h

#include "LEDOutput.h"
#include "MotorOutput.h"
#include <math.h>


#define MODE_INITIAL 0
#define MODE_EDGE1 1
#define MODE_EDGE2 2
#define MODE_AUTO_PLAY 3

#define MIN_ALLOWED_DISTANCE 10
#define GEARS 10
// 48 ticks ~ 1m
#define SLOWING_DISTANCE 96.0
#define MAX_SPEED 500.0
#define MIN_MOTION_SPEED 20.0
#define MOTOR_POLARITY -1

class SpiderModel {

  MotorOutput* motor;
  LEDOutput* leds;

  unsigned char mode;
  int currentPosition;
  int targetPosition;
  int currentSpeed;
  int edge1;
  int edge2;
  int maxAutoPlaySpeed;
  bool autoPlayOn;
  int eSign;  // edge vector sign

public:
  SpiderModel(MotorOutput* motor, LEDOutput* leds)
    : motor(motor), leds(leds) {
  }

  void reset() {
    this->mode = MODE_INITIAL;
    this->maxAutoPlaySpeed = 0;
    this->currentSpeed = 1500;
    this->currentPosition = 0;
    this->motor->setSpeed(this->currentSpeed);
    this->leds->reset();
    this->autoPlayOn;
  }


  void tick(unsigned long clickDurationUS, int speedInput, int position, bool autoPlayOn) {
    this->setAutoPlayMode(autoPlayOn);
    if (clickDurationUS > 0) {
      this->changeMode(clickDurationUS, speedInput, position);
    }
    this->adjustSpeed(speedInput, position);
    this->leds->setBlinkPin(this->currentSpeed > 0 ? 1 : 0);
    this->currentPosition = position;
  }

private:

  void setAutoPlayMode(bool on) {
    if (this->mode < MODE_EDGE2) {
      return;
    }
    if (this->autoPlayOn == on) {
      // ignore if no change
      return;
    }

    if (on) {
      this->maxAutoPlaySpeed = max(abs(this->currentSpeed), 100);
      // keep direction or towards edge 2
      this->targetPosition = this->targetPosition == this->edge1 ? this->edge1 : this->edge2;
      this->leds->setAutoPilotMode(true);
      this->mode = MODE_AUTO_PLAY;
    } else {
      this->leds->setAutoPilotMode(false);
      this->targetPosition = this->currentPosition;  // stop!
      this->mode = MODE_EDGE2;
    }

    this->autoPlayOn = on;
  }


  void changeMode(unsigned long clickDurationUS, int speedInput, int position) {

    switch (this->mode) {
      case MODE_INITIAL:
        this->mode = MODE_EDGE1;
        this->edge1 = position;
        this->leds->setSinglePositionSet();
        break;
      case MODE_EDGE1:
        if (abs(this->edge1 - position) > MIN_ALLOWED_DISTANCE) {
          if (position > this->edge1) {
            this->edge2 = position;
            this->eSign = -1;
          } else {
            this->edge2 = this->edge1;
            this->edge1 = position;
            this->eSign = 1;
          }
          this->mode = MODE_EDGE2;

          this->leds->setBothPositionsSet();
        }
        break;
      case MODE_EDGE2:
      case MODE_AUTO_PLAY:
        break;
      default:
        this->leds->setFatalError();
        while (true)
          ;  // code error
    }
  }

  void adjustSpeed(int speedInput, int position) {
    if (this->mode == MODE_INITIAL || this->mode == MODE_EDGE1) {
      this->currentSpeed = speedInput - 1500;
    } else if (this->mode == MODE_EDGE2) {

      int speed = 10 * ((constrain(speedInput, 1000, 2000) - 1500) / 10);
      int sign = sgn(speed);// * this->eSign;
      this->targetPosition = sign < 0 ? this->edge1 : sign > 0 ? this->edge2 : this->currentPosition;
      int distance = this->currentPosition - this->targetPosition;
      int dir = sgn(distance);
      distance = abs(distance);

      double adjustedSpeed = abs(speed);
      // if (distance < SLOWING_DISTANCE) {
      adjustedSpeed = min(adjustedSpeed, this->maxAllowedSpeedAt(this->targetPosition));
      // }

      this->currentSpeed = dir * adjustedSpeed * MOTOR_POLARITY;

      // Serial.print(dir * distance);
      // Serial.print(" ");
      // Serial.print(this->currentSpeed);
      // Serial.print(" ");
      // Serial.print(this->currentPosition);
      // Serial.print(" ");
      // Serial.print(this->targetPosition);
      // // Serial.print(" ");
      // Serial.println();
      delay(10);
    } else if (this->mode == MODE_AUTO_PLAY) {
      int speed = constrain(this->maxAutoPlaySpeed  + 100 * ((speedInput - 1500) / 100), 0, 500);
      int distance = this->currentPosition - this->targetPosition;
      int dir = sgn(distance);
      distance = abs(distance);
      if (distance <= 50) {
        this->targetPosition = this->targetPosition == this->edge1 ? this->edge2 : this->edge1;
        distance = this->currentPosition - this->targetPosition;
      }
       double adjustedSpeed = abs(speed);
      // if (distance < SLOWING_DISTANCE) {
        adjustedSpeed = min(adjustedSpeed, this->maxAllowedSpeedAt(this->targetPosition));
      // }

      this->currentSpeed = dir * adjustedSpeed * MOTOR_POLARITY;

      // Serial.print(distance);
      // Serial.print(" ");
      // Serial.print(speed);
      // Serial.print(" ");
      // Serial.print(this->currentSpeed);
      // // Serial.print(" ");
      // // Serial.print(this->targetPosition);
      // // Serial.print(" ");
      // Serial.println();
      delay(10);
    }


    // final final final speed :)
    int fffSpeed = constrain(this->currentSpeed, -500, 500);
    if (fffSpeed > 10 && fffSpeed < 100) {
      fffSpeed = 100;
    }
    else if (fffSpeed < -10 && fffSpeed > -100) {
      fffSpeed = -100;
    }

    if  (abs(fffSpeed) < 10 && fffSpeed != 0) {
      fffSpeed = 10 * sgn(fffSpeed);
    }

    this->motor->setSpeed(fffSpeed + 1500);
  }

  double maxAllowedSpeedAt(double position) {
    return constrain(
          map(
            abs(position - this->currentPosition), 
            0.0, SLOWING_DISTANCE, 
            MIN_MOTION_SPEED, MAX_SPEED
            )
          , 0.0, 500.0
      );
  }
};

#endif SpiderModel_h