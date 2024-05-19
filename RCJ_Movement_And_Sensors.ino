#include "Motor.h"
#include "utils.h"
#include "bno.h"
#include <VL53L0X.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_TCS34725.h>
#include <utility/imumaths.h>
#include "AS726X.h"




using namespace utils;
using namespace std;

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);
AS726X sensor;

uint16_t r, g, b, c;


void updateTCS() {
  r = 0;
  b = 0;
  g = 0;
  c = 0;
  tcaselect(7);
  tcs.getRawData(&r, &g, &b, &c);
  r = map(r, 0, 65535, 0, 255);
  g = map(g, 0, 65535, 0, 255);
  b = map(b, 0, 65535, 0, 255);
  c = map(c, 0, 65535, 0, 255);
  char buff[100];
  sprintf(buff, "r: %d, g: %d, b: %d, c:%d", r, g, b, c);
  Serial.println(buff);
}

#define DROP_ANGLE 180
#define INTAKE_ANGLE 0
// #define TEST

int count = 0;
int offset = 0;

int currentX = 0;
int currentY = 0;


void setup() {
  pinMode(29, OUTPUT);
  Serial.begin(9600);
  initServos();
  noTone(26);


  utils::setMotors(&motorR, &motorL);
  bnoSetup();
  // tcaselect(7);
  if (sensor.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No AS726X found ... check your connections");
  }
  for (int i = TOF_START; i < 7; i++) {
    if (true) {
      tcaselect(i);
      if (!tof.init()) {
        Serial.print(":( sensor ");
        Serial.print(i);
        Serial.println(" is dead");
      } else {
        Serial.print(":) sensor ");
        Serial.print(i);
        Serial.println("is init");
      }
      delay(5);
    };
  }


  // updateTCS();
  // if ((double)b / (double)r > 2) {
  //   Serial.println("Blue");
  // } else if (c < 20) {
  //   Serial.println("Black");
  // } else {
  //   Serial.println("white/silver");
  // }
  // //Tile start;
  // //*currentTile = start;


  // if (false) {
  // if (tofCheck(0, 200)) {  //front
  //   Tile newTile;
  //   currentTile->N = &newTile;
  // }
  // if (tofCheck(5, 200)) {  //left
  //   Tile newTile;
  //   currentTile->W = &newTile;
  // }
  // if (tofCheck(1, 200)) {  //right
  //   Tile newTile;
  //   currentTile->E = &newTile;
  // }
  // if (tofCheck(3, 200)) {  //behind
  //   Tile newTile;
  //   currentTile->S = &newTile;
  // }
  // }
  // currentTile->arrived = true;
  // currentTile->point.x = currentX;
  // currentTile->point.y = currentY;
  Serial.println("complete setup");
}


void initServos() {
  leftServo.attach(30);
  rightServo.attach(22);
  leftServo.write(180);
  rightServo.write(0);
}

void dropRescueKitLeft() {
  leftServo.write(180);
  delay(500);
  for (int i = 0; i < 120; i += 1) {
    leftServo.write(i);
    delay(60);
  }
  leftServo.write(180);
}

void dropRescueKitRight() {
  rightServo.write(0);
  delay(500);
  for (int i = 180; i > 100; i -= 1) {
    rightServo.write(i);
    delay(40);
  }
  rightServo.write(0);
}

uint16_t silverVal = 0;
uint16_t whiteVal = 0;
uint16_t blueVal = 0;
uint16_t blackVal = 0;

uint16_t silver_white_thresh = (silverVal + whiteVal) / 2;
uint16_t white_blue_thresh = (whiteVal + blueVal) / 2;
uint16_t blue_black_thresh = (blueVal + blackVal) / 2;


void loop() {
  bno.getEvent(&bnoInfo, Adafruit_BNO055::VECTOR_EULER);
  offset = bnoInfo.orientation.x;
  if (offset > 315 || offset < 45) {
    offset = 0;
  } else if (offset >= 45 && offset < 135) {
    offset = 90;
  } else if (offset >= 135 && offset < 225) {
    offset = 180;
  } else {
    offset = 270;
  }

  Serial.print("TofCheck: ");
  Serial.print(tofCheck(0, 200, 2));
  if (tofCheck(0, 200, 2)) {

    Serial.println("Drive");
    straightDrive(30, 150, 2, 60);
    stopMotors();
    delay(1000);

    if (tofCheckMin(0, 200, 2)) {
      center();
    }

    utils::stopMotors();
    delay(1000);
  } else if (tofCheck(1, 200, 2)) {
    Serial.println("right");
    goodSpinRight(85);
  } else if (tofCheck(5, 200, 2)) {
    Serial.println("Left");
    goodSpinLeft(85);
    offset = bnoInfo.orientation.x;
    if (offset > 315 || offset < 45) {
      offset = 0;
    } else if (offset >= 45 && offset < 135) {
      offset = 90;
    } else if (offset >= 135 && offset < 225) {
      offset = 180;
    } else {
      offset = 270;
    }
    if (tofCheckMin(3, 200, 2)) {
      backCenter();
    }
  } else {
    Serial.println("Left");
    goodSpinLeft(180);
    offset = bnoInfo.orientation.x;
    offset = bnoInfo.orientation.x;
    if (offset > 315 || offset < 45) {
      offset = 0;
    } else if (offset >= 45 && offset < 135) {
      offset = 90;
    } else if (offset >= 135 && offset < 225) {
      offset = 180;
    } else {
      offset = 270;
    }
    if (tofCheckMin(3, 200, 2)) {
      backCenter();
    }
    // String path;
    // Tile* result = bfs(currentTile, path);
    // priv ntGraph();
    //  Serial.print("Path: ");
    // Serial.println(path);
  }


  sensor.takeMeasurementsWithBulb();
  Serial.print("ratio: ");
  double blue = sensor.getCalibratedBlue();
  double red = sensor.getCalibratedRed();
  Serial.println(blue / red);
  if ((double)sensor.getCalibratedBlue() / (double)sensor.getCalibratedRed() > 5) {
    stopMotors();
    digitalWrite(29, HIGH);
    delay(6000);
  }

  // dropRescueKitRight();
}

bool turnCenterCheck() {
  tcaselect(0);
  return tof.readRangeSingleMillimeters() > 80;
}


void center() {
  tcaselect(0);
  while (tof.readRangeSingleMillimeters() > 70) {
    forward(120, 120);
  }

  utils::stopMotors();
  delay(500);

  while (tof.readRangeSingleMillimeters() < 50) {
    forward(-120, -120);
  }

  utils::stopMotors();
  delay(500);
}

void backCenter() {
  tcaselect(3);

  while (tof.readRangeSingleMillimeters() > 70) {
    forward(-80, -80);
  }

  utils::stopMotors();
  delay(500);

  while (tof.readRangeSingleMillimeters() < 50) {
    forward(80, 80);
  }

  utils::stopMotors();
  delay(500);
}

void getTOFValues(int i) {
  tcaselect(i);
  return tof.readRangeSingleMillimeters();
}

// // void printGraph() {
// //   for (int i = 0; i < adj.size(); i++) {
// //     if (adj[i].size() != 0) {
// //       Serial.println(i);
// //       for (int j = 0; j < adj[i].size(); j++) {
// //         Serial.print(adj[i][j].x);
// //         Serial.print(" ");
// //         Serial.println(adj[i][j].y);
// //       }
// //     }
// //   }
// //   Serial.println("");
// // }

bool wallDetected() {
  tcaselect(0);
  if (tof.readRangeSingleMillimeters() < 20) {
    return true;
  } else {
    return false;
  }
}

boolean tofCheck(int id, int maxDist) {
  tcaselect(id);
  return tof.readRangeSingleMillimeters() > maxDist;
}

boolean tofCheck(int id, int maxDist, int sampleNumber) {
  int total = 0;
  tcaselect(id);
  for (int i = 0; i < sampleNumber; i++) {
    total += tof.readRangeSingleMillimeters();
  }

  return ((double)total / (double)sampleNumber) > (double)maxDist;
}

boolean tofCheckMin(int id, int maxDist, int sampleNumber) {
  int total = 0;
  tcaselect(id);
  for (int i = 0; i < sampleNumber; i++) {
    total += tof.readRangeSingleMillimeters();
  }

  return ((double)total / (double)sampleNumber) < (double)maxDist;
}

void printArray(int array1[]) {
  Serial.print("[");
  for (int i = 0; i < 7; i++) {
    Serial.print(array1[i]);
    if (i != 6) {
      Serial.print(", ");
    }
  }
  Serial.println("]");
}

void straightDrive(int cm, int speed, int tolerance, int milDist) {
  int encoders = cm * (368 / 25.4);
  bool seenBlack = false;
  // bno.begin(Adafruit_BNO055::);
  int angle;
  utils::resetTicks();
  float p, d, i = 0;
  float p_turn, d_turn, last_difference = 0;
  float PID;
  float last_dist = abs(motorL.getEncoders() / abs(encoders));

  //conversion from cm to encoders
  // tcaselect(0);

  while (abs(motorL.getEncoders()) < abs(encoders) && abs(motorR.getEncoders()) < abs(encoders) && tof.readRangeSingleMillimeters() > 50 && seenBlack == false) {

    bno.getEvent(&bnoInfo, Adafruit_BNO055::VECTOR_EULER);
    if (abs(offset - bnoInfo.orientation.x) > 180) {
      p_turn = offset - bnoInfo.orientation.x + 360;
    } else {
      p_turn = offset - bnoInfo.orientation.x;
    }



    sensor.takeMeasurementsWithBulb();
    Serial.print("B: ");
    Serial.print(sensor.getCalibratedBlue());
    Serial.print("R: ");
    Serial.println(sensor.getCalibratedRed());


    if (sensor.getCalibratedBlue() < 1000) {
      Serial.println("Black detected");
      stopMotors();
      delay(1000);
      seenBlack = true;
    }
    if (!seenBlack) {
      bno.getEvent(&bnoInfo, Adafruit_BNO055::VECTOR_EULER);
      Serial.println(bnoInfo.orientation.z);
      if (bnoInfo.orientation.z > 3) {
        while (bnoInfo.orientation.z > 3) {
          bno.getEvent(&bnoInfo, Adafruit_BNO055::VECTOR_EULER);
          forward(255, 255);
        }
      } else if (bnoInfo.orientation.z < -3) {
        while (bnoInfo.orientation.z < -3) {
          bno.getEvent(&bnoInfo, Adafruit_BNO055::VECTOR_EULER);
          forward(40, 40);
        }
      } else {
        forward(50 + p_turn * KP_STRAIGHTEN, 50 - p_turn * KP_STRAIGHTEN);
      }
    }
    /*
    bno.getEvent(&bnoInfo, Adafruit_BNO055::VECTOR_EULER);

    p = speed * (float)(abs(encoders) - abs(motorL.getEncoders())) / abs(encoders);
    i = i + p;
    d = p - last_dist;
    PID = p * KP_FORWARD + i * KI_FORWARD + d * KD_FORWARD;




 
  p_turn*=-1;
    // Serial.print("Right: ");
    // Serial.println(PID - p_turn + 10);
    // Serial.print("Left: ");
    // Serial.println(PID + p_turn);
    // Serial.println(PID);
    Serial.println(p_turn);
    // if(abs(bnoInfo.orientation.x - offset) < 2 || abs(bnoInfo.orientation.x - offset) > 300)
      utils::forward(PID - (p_turn * KP_STRAIGHTEN), PID + (KP_STRAIGHTEN * p_turn));
    // else {
    //   utils::forward(-p_turn*KP_STRAIGHTEN, p_turn*KP_STRAIGHTEN);
    // }
    angle = bnoInfo.orientation.x;
    // Serial.println(bnoInfo.orientation.x);


    */
  }
  if (seenBlack) {
    stopMotors();
    Serial.println(motorL.getEncoders());
    while (abs(motorL.getEncoders()) > 5) {
      Serial.println(motorL.getEncoders());
      forward(-100);
    }
    stopMotors();
    goodSpinLeft(180);
    offset = bnoInfo.orientation.x;
    offset = bnoInfo.orientation.x;
    if (offset > 315 || offset < 45) {
      offset = 0;
    } else if (offset >= 45 && offset < 135) {
      offset = 90;
    } else if (offset >= 135 && offset < 225) {
      offset = 180;
    } else {
      offset = 270;
    }
  }
  // utils::stopMotors();
}
