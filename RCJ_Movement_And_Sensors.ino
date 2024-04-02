#include "Motor.h"
#include "utils.h"
#include "RobotMap.h"
#include "timeOfFlight.h"
using namespace utils;

Motor motorL(MP2);
Motor motorR(MP1, true, true);

void setup() {
  Serial.begin(9600);
  utils::setMotors(&motorR, &motorL);
  configIMU();
  oled.begin();
  oled.setFlipMode(0);
  oled.setFont(u8x8_font_chroma48medium8_r);
  oled.setCursor(0, 0);
  // left90();


  for (int i = TOF_START; i <= TOF_NUMBER; i++) {
    tcaselect(i);
    if (!tof.init()) {
      Serial.print("Bruh :( sensor ");
      Serial.print(i);
      Serial.println(" is broken");
    } else {
      Serial.print("Yay! sensor ");
      Serial.print(i);
      Serial.println(" is init");
    }
  }
  oled.println("Hello");
  oled.clear();
}
int count = 0;

void loop() {
  // left90();
  left90();
  stopMotors();
  delay(1000);
}

void left90() {
  Serial.println("beginning...");
  int startingX = getAngleX();
  Serial.print("startingX: ");
  Serial.println(startingX);
  delay(1000);
  oled.clear();
  int goal = startingX - 90;
  if (goal < -180) {
    goal = 360 + goal;
  }
  Serial.print("goal: ");
  Serial.println(goal);
  forward(100,-100);
  while (getAngleX() != goal) {
    int j = getAngleX();
    Serial.println(j);
    oled.print(j);
    oled.print("--->");
  }
  Serial.println("done...");
}

void right90() {
  Serial.println("beginning...");
  int startingX = getAngleX();
  Serial.print("startingX: ");
  Serial.println(startingX);
  delay(1000);
  oled.clear();
  int goal = startingX + 90;
  if (goal > 180) {
    goal = -1*(360 - goal);
  }
  Serial.print("goal: ");
  Serial.println(goal);
  while (getAngleX() != goal) {
    int j = getAngleX();
    // Serial.println(j);
    oled.print(j);
    oled.print("--->");
    forward(-200, 200);

  }
  Serial.println("done...");
}



