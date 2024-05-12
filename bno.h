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











void raw_right(double relative_angle, int speed, bool alignment) {

  if (abs(relative_angle) < 1) {
    return;
  }

  double p, i = 0, d;
  double PID;
  bool cross_over = false;
  bool backwards = false;
  bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);



  const double initial_angle = orientationData.orientation.x;
  double orientation = orientationData.orientation.x;
  double angle = orientation + relative_angle;
  if(angle > 400) {
    angle = 90;
  } else if (angle >= 360) {
    angle = 0;
  }
  

  double tstart = millis();


  Serial.print("angle: ");
  Serial.println(angle);
  Serial.print("current orientation: ");
  Serial.println(orientation);
  double last_error = angle - orientationData.orientation.x;


  while (abs(orientation - angle) > 2) {

    p = (angle - orientation);
    last_error = p;
    i = i + last_error;
    PID = KP_TURN * p + KI_TURN * i;

    Serial.print("pid: ");
    Serial.println(PID);
     if (millis() - tstart < 3000 && PID > 0) {
    forward((PID + 75), (-1*PID - 75));
       Serial.println("turning");
     } else {
       forward(115, -115);
     }

    bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
    orientation = orientationData.orientation.x;

  }
  Serial.println("out");
  utils::stopMotors();
}



void right(int relative_angle, int speed, bool turn_status = true) {
  utils::stopMotors();
  delay(100);
  bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);

  raw_right(relative_angle, speed, true);

  Serial.println("done turning");

  utils::stopMotors();

  //stopMotors();
  delay(300);
}