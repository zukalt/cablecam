#ifndef PWMButton_h
#define PWMButton_h

#include "PWMInput.h"


class PWMButton : public PWMInput {

  unsigned long buttonPressStart;
  unsigned long buttonPressedMicros = 0;

public:
  PWMButton(int pin)
    : PWMInput(pin) {}


  unsigned long consumeClickEvent() {
    if (this->buttonPressStart == 0) {
      unsigned long time = this->buttonPressedMicros;
      this->buttonPressedMicros = 0;
      return time;
    }
    return 0;
  }

  inline unsigned long currentPressDuration() {
    return this->buttonPressedMicros;
  }


  void tick(unsigned long time) {
    if (this->speed > 1800) {
      if (!this->buttonPressStart) {
        this->buttonPressStart = time;
      }
      this->buttonPressedMicros = time - this->buttonPressStart;
    } else if (this->buttonPressStart) {
      this->buttonPressedMicros = time - this->buttonPressStart;
      this->buttonPressStart = 0;
    }
  }
};

#endif PWMButton_h