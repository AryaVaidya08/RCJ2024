#include "Motor.h"
#include "utils.h"
#include "bno.h"
#include <VL53L0X.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>



using namespace utils;
using namespace std;

#define DROP_ANGLE 180
#define INTAKE_ANGLE 0
// #define TEST

int count = 0;
int offset = 0;

int currentX = 0;
int currentY = 0;


void setup() {
  Serial.begin(115200);
  initServos();

  utils::setMotors(&motorR, &motorL);
  bnoSetup();
  // if (tcs.begin()) {
  //   Serial.println("Found sensor");
  // } else {
  //   Serial.println("No TCS34725 found ... check your connections");
  // }
  for (int i = TOF_START; i <= 8; i++) {
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
  leftServo.attach(7);
  leftServo.writeMicroseconds(5000);
  // rightServo.attach(8);
  // leftServo.write(DROP_ANGLE);
  // rightServo.write(DROP_ANGLE);
}

void dropRescueKitLeft() {
  leftServo.write(INTAKE_ANGLE);
  delay(500);
  leftServo.write(DROP_ANGLE);
}

void dropRescueKitRight() {
  rightServo.write(INTAKE_ANGLE);
  delay(500);
  rightServo.write(DROP_ANGLE);
}

uint16_t silverVal = 0;
uint16_t whiteVal = 0;
uint16_t blueVal = 0;
uint16_t blackVal = 0;

uint16_t silver_white_thresh = (silverVal + whiteVal) / 2;
uint16_t white_blue_thresh = (whiteVal + blueVal) / 2;
uint16_t blue_black_thresh = (blueVal + blackVal) / 2;
uint16_t r, g, b, c;

void loop() {

  #ifndef TEST

    bno.getEvent(&bnoInfo, Adafruit_BNO055::VECTOR_EULER);
    offset = bnoInfo.orientation.x;
    if(offset > 315 || offset < 45) {
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
    } else if (tofCheck(1, 60, 2)) {
      Serial.println("right");
      goodSpinRight(85);
    } else if (tofCheck(5, 60, 2)) {
      Serial.println("Left");
      goodSpinLeft(85);

      if (tofCheckMin(3, 100, 2)) {
        backCenter();
      }
      offset = bnoInfo.orientation.x;
    } else {
      Serial.println("Left");
      goodSpinLeft(180);
      offset = bnoInfo.orientation.x;

      if (tofCheckMin(3, 100, 2)) {
        backCenter();
      }
      // String path;
      // Tile* result = bfs(currentTile, path);
      // priv ntGraph();
      //  Serial.print("Path: ");
      // Serial.println(path);
    }

  #endif
  #ifdef TEST
  // goodSpinRight(85);
  // delay(1000);
  // bno.getEvent(&bnoInfo, Adafruit_BNO055::VECTOR_EULER);
  // offset = bnoInfo.orientation.x;
  // if(offset > 315 || offset < 45) {
  //   offset = 0;
  // } else if (offset >= 45 && offset < 135) {
  //   offset = 90;
  // } else if (offset >= 135 && offset < 225) {
  //   offset = 180;
  // } else {
  //   offset = 270;
  // }
  // straightDrive(30, 150, 2, 60);
  // delay(1000);
  goodSpinLeft(85);
  delay(1000);
  bno.getEvent(&bnoInfo, Adafruit_BNO055::VECTOR_EULER);
  offset = bnoInfo.orientation.x;
  if(offset > 315 || offset < 45) {
    offset = 0;
  } else if (offset >= 45 && offset < 135) {
    offset = 90;
  } else if (offset >= 135 && offset < 225) {
    offset = 180;
  } else {
    offset = 270;
  }
  straightDrive(30, 150, 2, 60);

  #endif
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

  return ((double) total/ (double) sampleNumber) > (double) maxDist;
}

boolean tofCheckMin(int id, int maxDist, int sampleNumber) {
  int total = 0;
  tcaselect(id);
  for (int i = 0; i < sampleNumber; i++) {
    total += tof.readRangeSingleMillimeters();
  }

  return ((double) total/ (double) sampleNumber) < (double) maxDist;
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

  // bno.begin(Adafruit_BNO055::);
  int angle;
  utils::resetTicks();
  float p, d, i = 0;
  float p_turn, d_turn, last_difference = 0;
  float PID;
  float last_dist = abs(motorL.getEncoders() / abs(encoders));

  //conversion from cm to encoders
  tcaselect(0);

  while (abs(motorL.getEncoders()) < abs(encoders) && abs(motorR.getEncoders()) < abs(encoders) && tof.readRangeSingleMillimeters() > 50) {

 if(abs(offset-bnoInfo.orientation.x) > 180) {
    p_turn = offset - bnoInfo.orientation.x + 360;
  } else {
    p_turn = offset - bnoInfo.orientation.x;
  }

    /*
    
    // tcs.getRawData(&r, &g, &b, &c);

    // if (c < blue_black_thresh) {
    //   stopMotors();
    //   delay(20);
    //   right(180, 150, true);
    //   break;
    // }

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

    forward(120 + p_turn*KP_STRAIGHTEN, 120 - p_turn*KP_STRAIGHTEN);
  }
  utils::stopMotors();
}



