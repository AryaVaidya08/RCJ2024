#include "Motor.h"
#include "utils.h"
#include "oled.h"
#include "bno.h"
#include <VL53L0X.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>



using namespace utils;
using namespace std;



int count = 0;
int offset = 0;

int currentX = 0;
int currentY = 0;


void setup() {
  Serial.begin(115200);
  // lcd.init();
  // lcd.backlight();
  // lcd.init();
  // lcd.backlight();
  // leftServo.attach(7);
  // rightServo.attach(8);
  // Serial.println("spinning");
  // leftServo.write(20);
  // delay(500);
  // Serial.println("spun");

  utils::setMotors(&motorR, &motorL);
  bnoSetup();
  // if (tcs.begin()) {
  //   Serial.println("Found sensor");
  // } else {
  //   Serial.println("No TCS34725 found ... check your connections");
  // }
  // tofCheck();
  // calibrateTOF();
  // for (int i = TOF_START; i <= 8; i++) {
  //   if (true) {
  //     tcaselect(i);
  //     if (!tof.init()) {
  //       Serial.print("Bruh :( sensor ");
  //       Serial.print(i);
  //       Serial.println(" is broken");
  //     } else {
  //       Serial.print("Yay! sensor ");
  //       Serial.print(i);
  //       Serial.println(" is init");
  //     }
  //     delay(5);
  //     //tof.setTimeout(500);
  //     //tof.startContinuous();
  //   };
//}
  //Tile start;
  //*currentTile = start;


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

// void dropRescueKitLeft() {
//   leftServo.write(0);
//   delay(500);
//   leftServo.write(90);
// }

// void dropRescueKitRight() {
//   rightServo.write(0);
//   delay(500);
//   rightServo.write(90);
// }

// uint16_t silverVal = 0;
// uint16_t whiteVal = 0;
// uint16_t blueVal = 0;
// uint16_t blackVal = 0;

// uint16_t silver_white_thresh = (silverVal + whiteVal) / 2;
// uint16_t white_blue_thresh = (whiteVal + blueVal) / 2;
// uint16_t blue_black_thresh = (blueVal + blackVal) / 2;
// uint16_t r, g, b, c;


void loop() {
  right(90, 150, true);
  delay(1000);
  // if (tofCheck(0, 200)) {
  //   Serial.println("Drive");
  //   straightDrive(30, 150, 2, 60);
  //   stopMotors();
  //   delay(500);
  // } else if (tofCheck(1, 200)) {
  //   Serial.println("right");
  //   while (turnCenterCheck()) {
  //     motorL.run(150);
  //     motorR.run(150);
  //   }
  //   stopMotors();
  //   right(90, 150, true);
  //   offset = orientationData.orientation.x;
  // } else if (tofCheck(5, 200)) {
  //   while (turnCenterCheck()) {
  //     motorL.run(150);
  //     motorR.run(150);
  //   }
  //   stopMotors();
  //   Serial.println("Left");
  //   left(90, 150, true);
  //   offset = orientationData.orientation.x;
  // } else {
  //   // String path;
  //   // Tile* result = bfs(currentTile, path);
  //   // printGraph();
  //   // Serial.print("Path: ");
  //   // Serial.println(path);
  // }
}

// bool turnCenterCheck() {
//   tcaselect(0);
//   return tof.readRangeSingleMillimeters() > 80;
// }

// void getTOFValues(int i) {
//   tcaselect(i);
//   return tof.readRangeSingleMillimeters();
// }

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

// bool wallDetected() {
//   tcaselect(0);
//   if (tof.readRangeSingleMillimeters() < 20) {
//     return true;
//   } else {
//     return false;
//   }
// }

// void printArray(int array1[]) {
//   Serial.print("[");
//   for (int i = 0; i < 7; i++) {
//     Serial.print(array1[i]);
//     if (i != 6) {
//       Serial.print(", ");
//     }
//   }
//   Serial.println("]");
// }

// void tofCheck() {
//   for (int i = TOF_START; i <= TOF_NUMBER; i++) {
//     tcaselect(i);
//     if (!tof.init()) {
//       Serial.print("Bruh :( sensor ");
//       Serial.print(i);
//       Serial.println(" is broken");
//     } else {
//       Serial.print("Yay! sensor ");
//       Serial.print(i);
//       Serial.println(" is init");
//     }
//     delay(5);
//     //tof.setTimeout(500);
//     //tof.startContinuous();
//   };
// }
// void straightDrive(int cm, int speed, int tolerance, int milDist) {
//   double tireCircum = 25.4;  //10 inches in cm
//   double encPerRev = 368;    //8 ticks per rev on a 1:46 gear ratio
//   double multiplier = encPerRev / tireCircum;
//   int encoders = cm * multiplier;

//   // bno.begin(Adafruit_BNO055::);
//   int angle;
//   utils::resetTicks();
//   float p, d, i = 0;
//   float p_turn, d_turn, last_difference = 0;
//   float PID;
//   float last_dist = abs(motorL.getTicks() / abs(encoders));

//   //conversion from cm to encoders

//   while (abs(motorL.getTicks()) < abs(encoders) && abs(motorR.getTicks()) < abs(encoders)) {

//     // tcs.getRawData(&r, &g, &b, &c);

//     // if (c < blue_black_thresh) {
//     //   stopMotors();
//     //   delay(20);
//     //   right(180, 150, true);
//     //   break;
//     // }

//     bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
//     int minspeed = 50;
//     p = speed * (float)(abs(encoders) - abs(motorL.getTicks())) / abs(encoders);
//     i = i + p;
//     d = p - last_dist;
//     PID = p * KP_FORWARD + i * KI_FORWARD + d * KD_FORWARD;


//     if (orientationData.orientation.x > 180 + offset) {
//       p_turn = orientationData.orientation.x - 360 - offset;
//     } else {
//       p_turn = orientationData.orientation.x - offset;
//     }
//     // Serial.print("Right: ");
//     // Serial.println(PID - p_turn + 10);
//     // Serial.print("Left: ");
//     // Serial.println(PID + p_turn);
//     utils::forward(PID - p_turn + 10, PID + p_turn);
//     angle = orientationData.orientation.x;
//     // Serial.println(orientationData.orientation.x);
//   }
//   utils::stopMotors();
// }




// void tofSensorCheck() {
//   for (int i = TOF_START; i <= TOF_NUMBER; i++) {
//     tcaselect(i);
//     if (!tof.init()) {
//       Serial.print("Bruh :( sensor ");
//       Serial.print(i);
//       Serial.println(" is broken");
//     } else {
//       Serial.print("Yay! sensor ");
//       Serial.print(i);
//       Serial.println(" is init");
//     }
//   }
// }
