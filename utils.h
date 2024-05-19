#ifndef _UTILS_H_
#define _UTILS_H_

#include "Motor.h"
#include <Servo.h>

//#define MUXADDR 0x70

namespace utils {

Motor* motor;
Motor* motor2;
Servo leftServo;
Servo rightServo;

void setMotors(Motor* _motor1, Motor* _motor2) {
  motor = _motor1;
  motor2 = _motor2;
}

void forward(int rightSpeed, int leftSpeed) {
  if(rightSpeed > 255)
    rightSpeed = 255;
  if(leftSpeed > 255)
    leftSpeed = 255;
  if(rightSpeed < -255)
    rightSpeed = -255;
  if(leftSpeed < -255)
    leftSpeed < -255;
  motor->run(rightSpeed);
  motor2->run(leftSpeed);
}

void forward(int speed) {
  forward(speed, speed);
}

void resetTicks() {
  motor->resetTicks();
  motor2->resetTicks();
}

void stopMotors() {
  motor->run(0);
  motor2->run(0);
}

void forwardTicks(int speed, int ticks, bool reset = true) {
  if (reset)
    resetTicks();
  while (abs(motor->getEncoders()) <= abs(ticks)) {
    forward(speed);
  }
  stopMotors();
}

void resetServo() {
  leftServo.write(90);
  delay(200);
  rightServo.write(90);
  delay(30);
}

};
#endif