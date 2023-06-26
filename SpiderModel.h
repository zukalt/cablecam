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
  }

  void changeMode(unsigned long clickDurationUS, int speedInput, int position) {

    // long press exit auto pilot mode, then reset
    if (IS_LONG_PRESS(clickDurationUS) && this->mode != MODE_AUTO_PLAY) {
      Serial.println("resetting...");
      this->reset();
      return;
    }

    switch (this->mode) {
      case MODE_INITIAL:
        this->mode = MODE_EDGE1;
        this->edge1 = position;
        this->leds->setSinglePositionSet();
        break;
      case MODE_EDGE1:
        if (abs(this->edge1 - position) > MIN_ALLOWED_DISTANCE) {
          this->edge2 = position;
          this->mode = MODE_EDGE2;
          this->leds->setBothPositionsSet();
        }
        break;
      case MODE_EDGE2:
        this->maxAutoPlaySpeed = max(abs(speedInput - 1500), 100);
        this->mode = MODE_AUTO_PLAY;
        this->targetPosition = sgn(speedInput) == 1 ? min(this->edge1, this->edge2) : max(this->edge1, this->edge2);  // TODO: detect direction and set the correct edge
        this->leds->setAutoPilotMode(true);
        break;
      case MODE_AUTO_PLAY:
        if (IS_LONG_PRESS(clickDurationUS)) {
          this->mode = MODE_EDGE2;
          this->leds->setAutoPilotMode(false);
          this->targetPosition = this->currentPosition;  // stop!
        }
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
      int gear = ((speed * GEARS) / 500);
      int sign = sgn(this->edge2 - this->edge1);
      int len = abs(this->edge1 - this->edge2);
      int upper = max(this->edge1, this->edge2);
      int lower = min(this->edge1, this->edge2);
      int newPosition = this->currentPosition + sign * (gear * len / 5);
      this->targetPosition = constrain(newPosition, lower, upper);
      int distance = this->currentPosition - this->targetPosition;
      int dir = sgn(distance);
      distance = abs(distance);
      double coef = distance < 90 
      ? ( distance < 20 ? 100.0/speed : sin((3.1416*distance)/100.0) )
      : 1.0;
      this->currentSpeed = dir * abs(speed) * abs(coef);

      Serial.print(distance);
      Serial.print(" ");
      Serial.print(this->currentSpeed);
      // Serial.print(" ");
      // Serial.print(this->currentPosition);
      // Serial.print(" ");
      // Serial.print(this->targetPosition);
      // Serial.print(" ");
      Serial.println();
      delay(50);
    } else if (this->mode == MODE_AUTO_PLAY) {
      int speed = this->maxAutoPlaySpeed;
      int distance = this->currentPosition - this->targetPosition;
      if (distance == 0) {
        this->targetPosition = this->currentPosition == this->edge1 ? this->edge2 : this->edge1;
      }
      int dir = sgn(distance);
      distance = abs(distance);
      double coef = distance < 90 
      ? ( distance < 20 ? 100.0/speed : sin((3.1416*distance)/100.0) )
      : 1.0;
      this->currentSpeed = dir * abs(speed) * abs(coef);

      Serial.print(distance);
      Serial.print(" ");
      Serial.print(this->currentSpeed);
      // Serial.print(" ");
      // Serial.print(this->currentPosition);
      // Serial.print(" ");
      // Serial.print(this->targetPosition);
      // Serial.print(" ");
      Serial.println();
      delay(50);
    }
    // set final position
    // on autopilot change to opposition when reached
    // calibrate speed based on current, max and min (
    // do not accelerate high,
    // do not suspend,
    // maintain max if possible,
    // calculate max as min set + extra applied
    //)

    this->motor->setSpeed(constrain(this->currentSpeed, -500, 500) + 1500);
  }

  // void setCurrentSpeed(int newSpeed) {

  //   this->currentSpeed = 10*((newSpeed - 1500)/10);

  //   if (this->leftSet && this->rightSet) {

  //     int edgeDistance = min(abs(this->letPosition - this->currentPosition), abs(this->rightPosition - this->currentPosition));
  //     Serial.print(this->letPosition);
  //     Serial.print(" ");
  //     Serial.print(this->rightPosition);
  //     Serial.print(" ");
  //     Serial.print(this->currentPosition);
  //     Serial.print(" ");
  //     Serial.println(edgeDistance); delay(50);

  //     int speed = 0;
  //     // auto pilot mode
  //     if (this->minSpeedSet) {
  //       // change direction if reach to 0 point
  //       if (edgeDistance < 1 && this->changeDirrectionOnLimit) {
  //         this->changeDirrectionOnLimit = false;
  //         this->minSpeedSet *= -1;
  //         Serial.println("Changing direction");
  //       }
  //       else if (edgeDistance < 2) {
  //         this->changeDirrectionOnLimit = true;
  //       }
  //       // speed = sgn(this->minSpeedSet) * max(abs(this->currentSpeed), abs(this->minSpeedSet));
  //       speed = constrain( this->minSpeedSet + (this->currentSpeed/5), -500, 500);
  //     }
  //     else {
  //       speed = this->currentSpeed;
  //     }

  //     this->applySpeed(speed, edgeDistance);
  //   }
  //   else {
  //     this->motor->setSpeed(newSpeed);
  //   }
  // }


  void tick(unsigned long clickDurationUS, int speedInput, int position) {

    if (clickDurationUS > 0) {
      this->changeMode(clickDurationUS, speedInput, position);
    }
    this->adjustSpeed(speedInput, position);
    this->currentPosition = position;
  }

  // private:
  //   void applySpeed(int speed, int edgeDistance) {
  //       if (edgeDistance < 500) {
  //         // Serial.println("Restricting speed");
  //         // Serial.print("Edge = ");
  //         // Serial.print(edgeDistance);
  //         // Serial.print(" speed = ");
  //         // Serial.print(speed);
  //         if (sgn(this->letPosition - this->currentPosition) != sgn(speed)) {
  //           speed = sgn(speed) * min(sqrt(edgeDistance)+50, abs(speed));
  //         }
  //         else {
  //           speed = 0;
  //         }
  //         // Serial.print(" new speed = ");
  //         // Serial.println(speed);
  //       }
  //       this->motor->setSpeed(speed + 1500);
  //   }
};

#endif SpiderModel_h