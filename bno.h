#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include "RobotMap.h"
#include "utils.h"

using namespace utils;



/* This driver reads raw data from the BNO055

   Connections
   ===========
   Connect SCL to analog 5
   Connect SDA to analog 4
   Connect VDD to 3.3V DC
   Connect GROUND to common ground

   History
   =======
   2015/MAR/03  - First release (KTOWN)
*/

/* Set the delay between fresh samples */
#define BNO055_SAMPLERATE_DELAY_MS (100)

// Check I2C device address and correct line below (by default address is 0x29 or 0x28)
//                                   id, address
Adafruit_BNO055 bno = Adafruit_BNO055(-1, 0x28, &Wire);

/**************************************************************************/
/*
    Arduino setup function (automatically called at startup)
*/
/**************************************************************************/




void bnoSetup() {

  while (!Serial) delay(10);  // wait for serial port to open!

  Serial.println("Orientation Sensor Raw Data Test");
  Serial.println("");

  /* Initialise the sensor */
  if (!bno.begin()) {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
  }

  delay(1000);

  /* Display the current temperature */
  int8_t temp = bno.getTemp();
  Serial.print("Current Temperature: ");
  Serial.print(temp);
  Serial.println(" C");
  Serial.println("");
  bno.setExtCrystalUse(true);

  Serial.println("Calibration status values: 0=uncalibrated, 3=fully calibrated");
}




// void spinLeft(double relative_angle) {

//   if (abs(relative_angle) < 1) {
//     return;
//   }

//   double p, i = 0, d;
//   double PID;
//   bno.getEvent(&bnoInfo, Adafruit_BNO055::VECTOR_EULER);



//   const double initial_angle = bnoInfo.orientation.x;
//   double orientation = bnoInfo.orientation.x;
//   double angle = orientation - relative_angle;
//   if (angle < -30) {
//     angle = 270;
//   } else if (angle < 0) {
//     angle = 0;
//   }

//   if (angle > 80 && angle < 100) {
//     angle = 90;
//   } else if (angle > 170 && angle < 190) {
//     angle = 180;
//   } else if (angle > 260 && angle < 280) {
//     angle = 270;
//   } else {
//     angle = 0;
//   }


//   double tstart = millis();


//   Serial.print("angle: ");
//   Serial.println(angle);
//   Serial.print("current orientation: ");
//   Serial.println(orientation);
//   double last_error = angle - bnoInfo.orientation.x;


//   while (abs(orientation - angle) > 2 && millis() - tstart < 5000) {

//     if (!(0 <= orientation && orientation < 20)) {
//       p = (orientation - angle);
//     } else {
//       p = (orientation + 360 - angle);
//     }
//     last_error = p;
//     //  i = i + last_error;
//     PID = KP_TURN * p;

//     Serial.print("orientation: ");
//     Serial.println(orientation);
//     Serial.print("pid: ");
//     Serial.println(PID);
//     forward(-135, 135);

//     bno.getEvent(&bnoInfo, Adafruit_BNO055::VECTOR_EULER);
//     orientation = bnoInfo.orientation.x;
//   }
//   Serial.println("out");
//   utils::stopMotors();
// }



// void left(int relative_angle) {
//   utils::stopMotors();
//   delay(100);
//   bno.getEvent(&bnoInfo, Adafruit_BNO055::VECTOR_EULER);

//   spinLeft(relative_angle);

//   Serial.println("done turning");

//   utils::stopMotors();

//   //stopMotors();
//   delay(1000);
// }

void goodSpinLeft(double relative_angle) {
  
   // while(!bno.begin()) {
  //   Serial.println("BNO BEGIN");
  //   delay(10);
  // }
  bno.getEvent(&bnoInfo, Adafruit_BNO055::VECTOR_EULER);
  double initial = bnoInfo.orientation.x;
  double orientation = initial;

  int angle = (int) (orientation - relative_angle);

  if(angle < 0) {
    angle = 360 - abs(angle);
  }

  while (abs(orientation - angle) > 2) {

    delay(10);
    bno.getEvent(&bnoInfo, Adafruit_BNO055::VECTOR_EULER);
    delay(10);
    orientation = bnoInfo.orientation.x;

    if (abs(angle - orientation) > 180) {
      orientation -= 360;
    }

    Serial.print("Orientation: ");
    Serial.print(bnoInfo.orientation.x);
    Serial.print(" Goal: ");
    Serial.println(angle);

    forward(-115, 115);
  }

  utils::stopMotors();
  delay(1000);

}

void goodSpinRight(double relative_angle) {
  
  // while(!bno.begin()) {
  //   Serial.println("BNO BEGIN");
  //   delay(10);
  // }
  bno.getEvent(&bnoInfo, Adafruit_BNO055::VECTOR_EULER);
  double initial = bnoInfo.orientation.x;
  double orientation = initial;

  int angle = (int) (orientation + relative_angle) % 360;

  while (abs(orientation - angle) > 2) {

    delay(10);
    bno.getEvent(&bnoInfo, Adafruit_BNO055::VECTOR_EULER);
    delay(10);
    orientation = bnoInfo.orientation.x;

    if (abs(angle - orientation) > 180) {
      orientation -= 360;
    }

    Serial.print("Orientation: ");
    Serial.print(bnoInfo.orientation.x);
    Serial.print(" Goal: ");
    Serial.println(angle);

    forward(115, -115);
  }

  utils::stopMotors();
  delay(1000);
  bno.getEvent(&bnoInfo, Adafruit_BNO055::VECTOR_EULER);
  delay(1000);
}



void spinRight(double relative_angle) {

  if (abs(relative_angle) < 1) {
    return;
  }

  double p, i = 0, d;
  double PID;


  const double initial_angle = bnoInfo.orientation.x;
  double orientation = bnoInfo.orientation.x;
  double angle = orientation + relative_angle;
  if (angle > 400) {
    angle = 90;
  } else if (angle >= 350) {
    angle = 0;
  }

  double tstart = millis();


  Serial.print("angle: ");
  Serial.println(angle);
  Serial.print("current orientation: ");
  Serial.println(orientation);
  double last_error = angle - bnoInfo.orientation.x;


  while (abs(orientation - angle) > 2) {

    p = (angle - orientation);
    last_error = p;
    i = i + last_error;
    PID = KP_TURN * p + KI_TURN * i;

    Serial.print("pid: ");
    Serial.println(PID);
    if (millis() - tstart < 3000 && PID > 0) {
      forward((PID + 75), (-1 * PID - 75));
      Serial.println("turning");
    } else {
      forward(210, -210);
    }

    bno.getEvent(&bnoInfo, Adafruit_BNO055::VECTOR_EULER);
    orientation = bnoInfo.orientation.x;
  }
  Serial.println("out");
  utils::stopMotors();
}


// void spinRight(double relative_angle) {

//   if (abs(relative_angle) < 1) {
//     return;
//   }

//   double p, i = 0, d;
//   double PID;
//   bno.getEvent(&bnoInfo, Adafruit_BNO055::VECTOR_EULER);



//   const double initial_angle = bnoInfo.orientation.x;
//   double orientation = bnoInfo.orientation.x;
//   double angle = orientation + relative_angle;
//   if (angle > 400) {
//     angle = 90;
//   } else if (angle >= 350) {
//     angle = 0;
//   }

//   double tstart = millis();


//   Serial.print("angle: ");
//   Serial.println(angle);
//   Serial.print("current orientation: ");
//   Serial.println(orientation);
//   double last_error = angle - bnoInfo.orientation.x;


//   while (abs(orientation - angle) > 2) {

//     p = (angle - orientation);
//     last_error = p;
//     i = i + last_error;
//     PID = KP_TURN * p + KI_TURN * i;

//     Serial.print("pid: ");
//     Serial.println(PID);
//     if (millis() - tstart < 3000 && PID > 0) {
//       forward((PID + 75), (-1 * PID - 75));
//       Serial.println("turning");
//     } else {
//       forward(210, -210);
//     }

//     bno.getEvent(&bnoInfo, Adafruit_BNO055::VECTOR_EULER);
//     orientation = bnoInfo.orientation.x;
//   }
//   Serial.println("out");
//   utils::stopMotors();
// }



void right(int relative_angle) {
  utils::stopMotors();
  delay(100);
  bno.getEvent(&bnoInfo, Adafruit_BNO055::VECTOR_EULER);

  Serial.println("starting turning");

  spinRight(relative_angle);

  Serial.println("done turning");

  utils::stopMotors();

  //stopMotors();
  delay(1000);
}