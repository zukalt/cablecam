#include <Arduino.h>

#include "common.h"
#include "PWMInput.h"
#include "PWMButton.h"
#include "LEDOutput.h"
#include "MotorOutput.h"
#include "RotationTracker.h"

#include "SpiderModel.h"



RotationTracker hallSensors(PCINT0, PCINT1, PCINT2);
PWMInput speed(PCINT3);
PWMButton btn(PCINT4);

LEDOutput leds(2, 3, 4);
MotorOutput motor(6);

SpiderModel model(&motor, &leds);


void setup() {
  Serial.begin(9600);
  Serial.println("setup");

  cli();
  // enable interrupts on PINB
  PCICR |= (1 << PCIE0);

  // Hall sensors
  PCMSK0 |= (1 << PCINT0);  // pin  8
  PCMSK0 |= (1 << PCINT1);  // pin  9
  PCMSK0 |= (1 << PCINT2);  // pin 10

  // Speed
  PCMSK0 |= (1 << PCINT3);  // pin 11
  // Button
  PCMSK0 |= (1 << PCINT4);  // pin 12

  sei();

  // input devices
  hallSensors.attach();
  speed.attach();
  btn.attach();
  // output devices
  leds.attach();
  motor.attach();

  hallSensors.reset();
  model.reset();

  Serial.println("loop start");
}

void loop() {
  const unsigned long time = micros();

  btn.tick(time);
  leds.tick(time);

  if (IS_RESET_PRESS(btn.currentPressDuration())) {
    return arduinoHardReset(); // hard reset from
  }

  const unsigned long clicked = btn.consumeClickEvent();
  const int newSpeed = speed.currentSpeed();

  if (hallSensors.sensorError()) {
    leds.setError();
  }

  model.tick(clicked, newSpeed, hallSensors.position());


  // Serial.print(clicked);
  // Serial.print(" ");
  // Serial.print(s);
  // Serial.print(" ");
  Serial.print(hallSensors.position());
  // Serial.print(" ");
  // Serial.print(hallSensors.entry);
  // Serial.print(" ");
  // Serial.print(hallSensors.last);
  Serial.print(" ");
  Serial.print(motor.speed());
  Serial.println();
  delay(50);
}


ISR(PCINT0_vect) {
  const unsigned long time = micros();
  speed.onISREvent(time);
  btn.onISREvent(time);
  hallSensors.onISREvent(time);
}
