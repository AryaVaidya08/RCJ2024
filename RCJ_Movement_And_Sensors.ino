#include "Motor.h"
#include "utils.h"
#include "timeOfFlight.h"
#include "oled.h"
#include <VL53L0X.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <LittleVector.h>

using namespace utils;
using namespace std;


class Point2d {
public:
  int x;
  int y;
  int hash() {
    return (x << 25 + y << 24) % (int)(1e9 + 7);
  }
};

// LittleVector<LittleVector<Point2d>> adj(10);


// Vector<Vector<Point2d>> adj(1007);



class Tile {
public:
  Tile* N = NULL;
  Tile* S = NULL;
  Tile* W = NULL;
  Tile* E = NULL;
  bool arrived = false;
  Point2d point;
};

class Queue {
public:
  struct Node {
    Tile* data;
    String path;
    Node* next;
  };

  Node* front;
  Node* rear;

public:
  Queue()
    : front(nullptr), rear(nullptr) {}

  bool isEmpty() {
    return front == nullptr;
  }

  void enqueue(Tile* tile, String path) {
    Node* newNode = new Node;
    newNode->data = tile;
    newNode->path = path;
    newNode->next = nullptr;
    if (isEmpty()) {
      front = rear = newNode;
    } else {
      rear->next = newNode;
      rear = newNode;
    }
  }

  Node* dequeue() {
    if (isEmpty()) {
      return nullptr;
    } else {
      Node* temp = front;
      front = front->next;
      if (front == nullptr) {
        rear = nullptr;
      }
      return temp;
    }
  }
};

// Breadth-first search function
Tile* bfs(Tile* currentTile, String& path) {
  if (!currentTile) return nullptr;

  Queue q;
  q.enqueue(currentTile, "");

  while (!q.isEmpty()) {
    Queue::Node* node = q.dequeue();
    Tile* tile = node->data;
    String currentPath = node->path;

    if (!tile->arrived) {
      path = currentPath;
      return tile;
    }

    if (tile->N != NULL) q.enqueue(tile->N, currentPath + "N");
    if (tile->S != NULL) q.enqueue(tile->S, currentPath + "S");
    if (tile->W != NULL) q.enqueue(tile->W, currentPath + "W");
    if (tile->E != NULL) q.enqueue(tile->E, currentPath + "E");
  }

  return nullptr;
}

int tofCheck(int id, int maxDist) {
  tcaselect(id);
  return tof.readRangeSingleMillimeters() > maxDist;
}

int setAngles[5] = { 0, 270, 180, 90, 0 };
int tofSensorValues[4] = { 0, 0, 0, 0 };
int count = 0;
int offset = 0;

Tile* currentTile;
int currentX = 0;
int currentY = 0;





void setup() {
  Serial.begin(115200);
  myservo.attach(A6);
  myservo2.attach(A7);

  resetServo();
  utils::setMotors(&motorR, &motorL);
  bnoSetup();
  // tofCheck();
  // calibrateTOF();
  for (int i = TOF_START; i <= TOF_NUMBER; i++) {
    if ((i == 0 || i == 1 || i == 3 || i == 5)) {
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
      delay(5);
      //tof.setTimeout(500);
      //tof.startContinuous();
    };
  }
  // Tile start;
  // *currentTile = start;


  // if (false) {
  // if (tofCheck(3, 200)) {  //front
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
  // if (tofCheck(0, 200)) {  //behind
  //   Tile newTile;
  //   currentTile->S = &newTile;
  // }
  // }
  // currentTile->arrived = true;
  // currentTile->point.x = currentX;
  // currentTile->point.y = currentY;
  Serial.println("complete setup");
}


void loop() {
  if (tofCheck(3, 200)) {
    Serial.println("Drive");
    straightDrive(30, 150, 2, 60);
    // if (orientationData.orientation.x > 350 || orientationData.orientation.x < 10) {
    //   Tile x = *currentTile->N;
    //   currentTile = &x;
    //   currentTile->arrived = true;

    //   currentTile->point.y++;
    //   if (tofCheck(3, 200)) {  //front
    //     Tile newTile;
    //     adj[currentTile->point.hash()].push_back(newTile.point);
    //     adj[newTile.point.hash()].push_back(currentTile->point);

    //     currentTile->N = &newTile;
    //   }
    //   if (tofCheck(5, 200)) {  //left
    //     Tile newTile;
    //     adj[currentTile->point.hash()].push_back(newTile.point);
    //     adj[newTile.point.hash()].push_back(currentTile->point);

    //     currentTile->W = &newTile;
    //   }
    //   if (tofCheck(1, 200)) {  //right
    //     Tile newTile;
    //     adj[currentTile->point.hash()].push_back(newTile.point);
    //     adj[newTile.point.hash()].push_back(currentTile->point);

    //     currentTile->E = &newTile;
    //   }
    //   if (tofCheck(0, 200)) {  //behind
    //     Tile newTile;
    //     adj[currentTile->point.hash()].push_back(newTile.point);
    //     adj[newTile.point.hash()].push_back(currentTile->point);
    //     currentTile->S = &newTile;
    //   }
    // }
    // else if (orientationData.orientation.x > 80 && orientationData.orientation.x < 100) {
    //   Tile x = *currentTile->E;
    //   currentTile = &x;
    //   currentTile->arrived = true;

    //   currentTile->point.x++;
    //   if (tofCheck(3, 200)) {  //front
    //     Tile newTile;
    //     adj[currentTile->point.hash()].push_back(newTile.point);
    //     adj[newTile.point.hash()].push_back(currentTile->point);
    //     currentTile->E = &newTile;
    //   }
    //   if (tofCheck(5, 200)) {  //left
    //     Tile newTile;
    //     adj[currentTile->point.hash()].push_back(newTile.point);
    //     adj[newTile.point.hash()].push_back(currentTile->point);
    //     currentTile->N = &newTile;
    //   }
    //   if (tofCheck(1, 200)) {  //right
    //     Tile newTile;
    //     adj[currentTile->point.hash()].push_back(newTile.point);
    //     adj[newTile.point.hash()].push_back(currentTile->point);
    //     currentTile->S = &newTile;
    //   }
    //   if (tofCheck(0, 200)) {  //behind
    //     Tile newTile;
    //     adj[currentTile->point.hash()].push_back(newTile.point);
    //     adj[newTile.point.hash()].push_back(currentTile->point);
    //     currentTile->W = &newTile;
    //   }
    // }

    // if (orientationData.orientation.x > 170 && orientationData.orientation.x < 190) {
    //   Tile x = *currentTile->S;
    //   currentTile = &x;
    //   currentTile->arrived = true;

    //   currentTile->point.y--;
    //   if (tofCheck(3, 200)) {  //front
    //     Tile newTile;
    //     adj[currentTile->point.hash()].push_back(newTile.point);
    //     adj[newTile.point.hash()].push_back(currentTile->point);
    //     currentTile->S = &newTile;
    //   }
    //   if (tofCheck(5, 200)) {  //left
    //     Tile newTile;
    //     adj[currentTile->point.hash()].push_back(newTile.point);
    //     adj[newTile.point.hash()].push_back(currentTile->point);
    //     currentTile->E = &newTile;
    //   }
    //   if (tofCheck(1, 200)) {  //right
    //     Tile newTile;
    //     adj[currentTile->point.hash()].push_back(newTile.point);
    //     adj[newTile.point.hash()].push_back(currentTile->point);
    //     currentTile->W = &newTile;
    //   }
    //   if (tofCheck(0, 200)) {  //behind
    //     Tile newTile;
    //     adj[currentTile->point.hash()].push_back(newTile.point);
    //     adj[newTile.point.hash()].push_back(currentTile->point);
    //     currentTile->N = &newTile;
    //   }
    // }

    // if (orientationData.orientation.x > 260 && orientationData.orientation.x < 280) {
    //   Tile x = *currentTile->W;
    //   currentTile = &x;
    //   currentTile->arrived = true;

    //   currentTile->point.x--;
    //   if (tofCheck(3, 200)) {  //front
    //     Tile newTile;
    //     adj[currentTile->point.hash()].push_back(newTile.point);
    //     adj[newTile.point.hash()].push_back(currentTile->point);
    //     currentTile->W = &newTile;
    //   }
    //   if (tofCheck(5, 200)) {  //left
    //     Tile newTile;
    //     adj[currentTile->point.hash()].push_back(newTile.point);
    //     adj[newTile.point.hash()].push_back(currentTile->point);

    //     currentTile->S = &newTile;
    //   }
    //   if (tofCheck(1, 200)) {  //right
    //     Tile newTile;
    //     adj[currentTile->point.hash()].push_back(newTile.point);
    //     adj[newTile.point.hash()].push_back(currentTile->point);

    //     currentTile->N = &newTile;
    //   }
    //   if (tofCheck(0, 200)) {  //behind
    //     Tile newTile;
    //     adj[currentTile->point.hash()].push_back(newTile.point);
    //     adj[newTile.point.hash()].push_back(currentTile->point);
    //     currentTile->E = &newTile;
    //   }
    // }

  } else if (tofCheck(1, 200)) {
    while (turnCenterCheck()) {
      motorL.run(150);
      motorR.run(150);
    }
    stopMotors();
    Serial.println("right");
    right(90, 150, true);
    offset = orientationData.orientation.x;
  } else if (tofCheck(5, 200)) {
    while (turnCenterCheck()) {
      motorL.run(150);
      motorR.run(150);
    }
    stopMotors();
    Serial.println("Left");
    left(90, 150, true);
    offset = orientationData.orientation.x;
  } else {
    // String path;
    // Tile* result = bfs(currentTile, path);
    // printGraph();
    // Serial.print("Path: ");
    // Serial.println(path);
  }
}

bool turnCenterCheck() {
  tcaselect(3);
  return tof.readRangeSingleMillimeters() > 80;
}

void getTOFValues(int i) {
  tcaselect(i);
  return tof.readRangeSingleMillimeters();
}

// void printGraph() {
//   for (int i = 0; i < adj.size(); i++) {
//     if (adj[i].size() != 0) {
//       Serial.println(i);
//       for (int j = 0; j < adj[i].size(); j++) {
//         Serial.print(adj[i][j].x);
//         Serial.print(" ");
//         Serial.println(adj[i][j].y);
//       }
//     }
//   }
//   Serial.println("");
// }

bool wallDetected() {
  tcaselect(3);
  if (tof.readRangeSingleMillimeters() < 20) {
    return true;
  } else {
    return false;
  }
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

void tofCheck() {
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
    delay(5);
    //tof.setTimeout(500);
    //tof.startContinuous();
  };
}
void straightDrive(int cm, int speed, int tolerance, int milDist) {
  double tireCircum = 25.4;  //10 inches in cm
  double encPerRev = 368;    //8 ticks per rev on a 1:46 gear ratio
  double multiplier = encPerRev / tireCircum;
  int encoders = cm * multiplier;

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


    if (orientationData.orientation.x > 180 + offset) {
      p_turn = orientationData.orientation.x - 360 - offset;
    } else {
      p_turn = orientationData.orientation.x - offset;
    }
    utils::forward(PID - p_turn + 10, PID + p_turn);
    angle = orientationData.orientation.x;
    // Serial.println(orientationData.orientation.x);
  }
  utils::stopMotors();
}




void tofSensorCheck() {
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
}