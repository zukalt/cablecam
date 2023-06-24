// // #include "common.h"
// #include "PWMSpeedInput.h"
// #include "LEDOutput.h"
// #include "PWMButton.h"
// #include <Servo.h>

// #define MOTOR_PIN 6

// PWMSpeedInput speedInput(PCINT3);
// PWMButton btn(PCINT4);
// LEDOutput output(2,3,4);
// Servo ESC;

// void setup() {
//   Serial.begin(9600);
//   Serial.println("start");

//   PCICR |= (1 << PCIE0);
  
//   // Hall sensors
//   // PCMSK0 |= (1 << PCINT0);
//   // PCMSK0 |= (1 << PCINT1);
//   // PCMSK0 |= (1 << PCINT2);

//   // Speed
//   PCMSK0 |= (1 << PCINT3);
//   PCMSK0 |= (1 << PCINT4);
//   // PCMSK0 |= (1 << PCINT5);

//   output.attach();
//   btn.attach();
//   ESC.attach(MOTOR_PIN,1000,2000); 

//   output.reset();
//   ESC.writeMicroseconds(1500);
// }

// void loop() {
//   const unsigned long time = micros();
//   output.tick(time);
//   btn.tick(time);

//   const int speed = speedInput.currentSpeed();

//   Serial.print(speed);
//   Serial.print(' ');
//   Serial.print(btn.currentSpeed());
//   Serial.print(' ');
//   Serial.print(btn.pressTime());

//   if (btn.isLongPressed()) {
//     btn.consumeEvent();
//     output.reset();
//   }
//   else if (btn.isPressed()) {
//     btn.consumeEvent();
//     output.setSinglePositionSet();
//   }
//   else if (speed < 1100) {
//     output.setError();
//   }
//   else if (speed < 1400) {
//     output.setBothPositionsSet();
//   }

//   ESC.writeMicroseconds(speed);

//   // const int pwmSpeed = map(speed, 990, 2010, 25, 225);
//   // analogWrite(MOTOR_PIN, pwmSpeed);

//   // Serial.print(" ");
//   // Serial.print(pwmSpeed);
//   Serial.println();
//   delay(50);
// }

// ISR(PCINT0_vect){
//   const unsigned long time = micros();
//   speedInput.onISREvent(time);
//   btn.onISREvent(time);
// }

