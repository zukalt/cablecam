#ifndef PWMInput_h
#define PWMInput_h

#include "common.h"


class PWMInput {

  const byte pin;
  volatile unsigned long pinHighStart;

protected:
  volatile int speed = 1500;

public:
  PWMInput(byte pin)
    : pin(pin) {}

  void attach() {
    // nothing
  }

  void onISREvent(unsigned long time) {
    if (bitcheck(PINB, this->pin)) {
      if (!this->pinHighStart) {
        this->pinHighStart = time;
      }
    } else if (this->pinHighStart) {
      this->speed = time - this->pinHighStart;
      this->pinHighStart = 0;
    }
  }

  inline int currentSpeed() const {
    return this->speed;
  }
};

#endif PWMInput_h