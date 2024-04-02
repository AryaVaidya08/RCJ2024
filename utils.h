#ifndef _UTILS_H_
#define _UTILS_H_

#include "Motor.h"
#include "MPU5060.h"
#include <Servo.h>

namespace utils {

Motor* motor;
Motor* motor2;

void setMotors(Motor* _motor1, Motor* _motor2) {
  motor = _motor1;
  motor2 = _motor2;
}

void forward(int rightSpeed, int leftSpeed) {
  motor->run(rightSpeed);
  motor2->run(leftSpeed);
}

void forward(int speed) {
  forward(speed, speed);
}


void stopMotors() {
  motor->stop();
  motor2->stop();
}

void turnLeft90() {
  int currentAngle = getAngleX();
  int goal = currentAngle - 90;
  while (getAngleX() > goal) {
    motor->run(100);
    motor2->run(-100);
  }
  stopMotors();
}


void turnRight90() {
  int goal = getAngleX() + 90;
  while (getAngleX() < goal) {
    Serial.println(getAngleX());
    Serial.println(goal);
    // motor->run(-100);
    motor2->run(100);
  }
  stopMotors();
}
}
#endif