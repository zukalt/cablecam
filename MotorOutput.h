#ifndef MotorOutput_h
#define MotorOutput_h

#include <Servo.h>


class MotorOutput {

  const byte pin;
  const Servo ESC;

public:
  MotorOutput(byte pin)
    : pin(pin) {}

  void attach() {
    ESC.attach(this->pin, 1000, 2000);
  }

  void setSpeed(int speed) {
    ESC.writeMicroseconds(speed);
  }
  int speed() {
    return ESC.readMicroseconds();
  }
};

#endif MotorOutput_h