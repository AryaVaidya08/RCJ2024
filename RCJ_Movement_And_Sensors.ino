#include "Motor.h"
#include "utils.h"
#include "timeOfFlight.h"
#include "oled.h"
#include <VL53L0X.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

using namespace utils;


int setLeftAngles[5] = { 10, 280, 190, 100, 10 };

void setup() {
  Serial.begin(9600);
  utils::setMotors(&motorR, &motorL);
  tofSensorCheck();
  bnoSetup();
}


int count = 0;

void loop() {
  // straightDrive(500, 150, 3);
  // tofSensorCheck();
  // Serial.flush();
  // oled.println("restart");
  // delay(500);
}

void straightDrive(int cm, int speed, int tolerance) {

  int coeff = 32;  //??? no idea how many encoders per rotatoin
  int encoders = (cm / (5.7 * 3.14159265)) * coeff;

  // bno.begin(Adafruit_BNO055::);
  int angle;
  utils::resetTicks();
  float p, d, i = 0;
  float p_turn, d_turn, last_difference = 0;
  float PID;
  float last_dist = abs(motorL.getTicks() / abs(encoders));

  //conversion from cm to encoders



  while (abs(motorL.getTicks()) < abs(encoders) && abs(motorR.getTicks()) < abs(encoders)) {
    bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);

    int minspeed = 50;
    p = speed * (float)(abs(encoders) - abs(motorL.getTicks())) / abs(encoders);
    i = i + p;
    d = p - last_dist;
    PID = p * KP_FORWARD + i * KI_FORWARD + d * KD_FORWARD;

    if (orientationData.orientation.x > 180) {
      p_turn = orientationData.orientation.x - 360;
    } else {
      p_turn = orientationData.orientation.x;
    }
    utils::forward(PID + p_turn, PID - p_turn);
    angle = orientationData.orientation.x;
    Serial.println(orientationData.orientation.x);
  }
  utils::stopMotors();
}



void left90() {
  int startingX = setLeftAngles[count];
  delay(200);
  int goal = setLeftAngles[count + 1];

  count++;
  while (!(getAngX() - goal < 3 && getAngX() - goal > -3)) {
    forward(100, -100);
    Serial.print("Current Angle: ");
    Serial.println(getAngX());
    Serial.print("Goal: ");
    Serial.println(goal);
    char buf[10];
    sprintf(buf, "%3d", getAngX());
  }
  stopMotors();
  Serial.println("done...");
  if (count == 4) {
    count = 0;
  }
}

void right90() {
  Serial.println("beginning...");
  int startingX = getAngX();
  Serial.print("startingX: ");
  Serial.println(startingX);
  delay(1000);
  oled.clear();
  int goal = startingX + 90;
  if (goal > 180) {
    goal = -1 * (360 - goal);
  }
  Serial.print("goal: ");
  Serial.println(goal);
  while (getAngX() != goal) {
    int j = getAngX();
    // Serial.println(j);
    oled.print(j);
    oled.print("--->");
    forward(-200, 200);
  }
  Serial.println("done...");
}

void tofSensorCheck() {
  
  // for (int i = TOF_START; i <= TOF_NUMBER; i++) {
  //   tcaselect(i);
    // if (!tof.init()) {
      tof.init();
      Serial.print("Bruh :( sensor ");
      Serial.print(tof.readRangeSingleMillimeters());
      Serial.println(" is broken");
    // } else {
    //   Serial.print("Yay! sensor ");
    //   Serial.print(i);
    //   Serial.println(" is init");
    // }
 // }
}

