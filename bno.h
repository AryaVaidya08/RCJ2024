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

int getAngX() {

  imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);

  return (euler.x());

  delay(BNO055_SAMPLERATE_DELAY_MS);
}


void raw_left(double relative_angle, int speed, bool alignment) {

  if (abs(relative_angle) < 1) {
    return;
  }

#ifndef MOTORSOFF


  double p, i = 0, d;
  double PID;
  bool cross_over = false;
  bool backwards = false;
  bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);

  if (orientationData.orientation.x - relative_angle < 0) {
    cross_over = true;
  }


  const double initial_angle = orientationData.orientation.x;
  double orientation = cross_over ? orientationData.orientation.x + relative_angle : orientationData.orientation.x;
  double angle = orientation - relative_angle;
  double last_error = abs((orientationData.orientation.x - angle) / angle);

  double tstart = millis();

  if(angle > 350 && angle < 10)
    angle = 0;
  if(angle > 260 && angle < 280)
    angle = 270;
  if(angle > 170 && angle < 190)
    angle = 180;
  if(angle > 80 && angle < 100) 
    angle = 90;

#ifndef NO_PID
  while (abs(orientation - angle) > 0.5) {
#else
  while (abs(orientation - angle) > 0.5) {
#endif
    p = ((orientation - angle) / relative_angle);
    PID = KP_TURN * p;
    last_error = p;

#ifndef NO_PID
    if (millis() - tstart > 5000 && !backwards) {
      utils::stopMotors();
      delay(200);
      utils::resetTicks();

      while (abs(motorL.getTicks()) < 7 * CM_TO_ENCODERS) {
        utils::forward(-255, 0);
      }

      utils::stopMotors();
      delay(200);
      backwards = true;
      continue;
    }


    if (millis() - tstart < 3000) {
      utils::forward((PID * -speed - 40), (PID * speed + 40));
    } else {
      utils::forward(-115, 115);
    }
#else
    if (millis() - tstart < 3000) {
      forward(-210, 210);
    } else {
      forward(-115, 115);
    }
#endif

    if (PID <= 0.01)
      break;

    bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
    orientation = cross_over ? orientationData.orientation.x + relative_angle : orientationData.orientation.x;

    if (orientationData.orientation.x > initial_angle) {
      orientation -= 360;
    }

    // if (orientationData.orientation.x < 1.0) {
    //   orientation += 360;
    // }
  }
  utils::stopMotors();
#endif
}

void left(int relative_angle, int speed, bool turn_status = true) {
  utils::stopMotors();
  delay(100);
  bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);

  raw_left(relative_angle, speed, true);



  utils::stopMotors();

  // stopMotors();
  //delay(300);
}







void raw_right(double relative_angle, int speed, bool alignment) {

  if (abs(relative_angle) < 1) {
    return;
  }

#ifndef MOTORSOFF


  double p, i = 0, d;
  double PID;
  bool cross_over = false;
  bool backwards = false;
  bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);

  if (relative_angle + orientationData.orientation.x > 360) {
    cross_over = true;
  }


  const double initial_angle = orientationData.orientation.x;
  double orientation = cross_over ? 0 : orientationData.orientation.x;
  double angle = relative_angle + orientation;
  double last_error = abs((orientationData.orientation.x - angle) / angle);

  double tstart = millis();

    if(angle > 350 && angle < 10)
    angle = 0;
  if(angle > 260 && angle < 280)
    angle = 270;
  if(angle > 170 && angle < 190)
    angle = 180;
  if(angle > 80 && angle < 100) 
    angle = 90;

#ifndef NO_PID
  while (abs(orientation - angle) > 0.5) {
#else
  // while (orientation < angle) {
  while(abs(orientation - angle) > 0.5) {
#endif

    p = (angle - orientation) / relative_angle;
    PID = KP_TURN * p;
    last_error = p;

#ifndef NO_PID
    if (millis() - tstart > 5000 && !backwards) {
      utils::stopMotors();
      delay(200);
      utils::resetTicks();

      while (abs(motorL.getTicks()) < 7 * CM_TO_ENCODERS) {
        // utils::forward(0, -255);
      }

      utils::stopMotors();
      delay(200);
      backwards = true;
      continue;
    }


    if (millis() - tstart < 3000) {
      utils::forward((PID * speed + 40), (PID * -speed - 40));
    } else {
      utils::forward(115, -115);
    }
#else
    if (millis() - tstart < 3000) {
      forward(210, -210);
     
    } else {
      forward(115, -115);

    }
#endif

    if (PID <= 0.01)
      break;

    bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
    orientation = cross_over ? orientationData.orientation.x + relative_angle : orientationData.orientation.x;
    // Serial.print("orientatoin: ");
    // Serial.println(orientationData.orientation.x);

    if (orientationData.orientation.x < initial_angle) {
      orientation -= 360;
    }

    // if (orientationData.orientation.x < 1.0) {
    //   orientation += 360;
    // }
  }
  utils::stopMotors();
#endif
}


void right(int relative_angle, int speed, bool turn_status = true) {
  utils::stopMotors();
  delay(100);
  bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);

  raw_right(relative_angle, speed, true);



  utils::stopMotors();

  //stopMotors();
  delay(300);
}
