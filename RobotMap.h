#define MUXADDR 0x70
#define OLED_CLK 28
#define OLED_DATA 30
#include <U8x8lib.h>
#include <VL53L0X.h>


#define TOF_NUMBER 6
#define TOF_START 0

#define MUXADDR 0x70

VL53L0X tof;
inline void tcaselect(uint8_t i) {
  if (i > 7)
    return;

  Wire.beginTransmission(MUXADDR);
  Wire.write(1 << i);
  Wire.endTransmission();
}

const float CM_TO_ENCODERS = 360.f/(7.7f*PI);
const float ENCODERS_TO_CM = 1.f/CM_TO_ENCODERS;


const int minspeed = 200;
const double KP_TURN = 2;
const double KI_TURN = 0.00003;
const double KD_TURN = 0.243;
const int DRIVE_BOOST = 40;
const int TURN_BOOST = 140;
const int ALIGN_TURN_BOOST = 70;

Motor motorL(MP3, true);
Motor motorR(MP4);


const double DRIVE_STRAIGHT_KP = 3.0;
const double KP_FORWARD = 2;
const double KI_FORWARD = 0.003;
const double KD_FORWARD = 0.01;
const double SAMPLERATE_DELAY_MS = 10.0;
const double TIMES_PER_SECOND = 1000.0 / SAMPLERATE_DELAY_MS;
volatile int32_t global_angle = 0;
volatile bool restart = false;

const int MAX_SPEED = 150;
const int ALIGN_SPEED = 85;


#define BNO_X orientationData.orientation.x
#define BNO_Y orientationData.orientation.y
#define BNO_Z orientationData.orientation.z
#define FRONT_LEFT A14
#define FRONT_RIGHT A13
#define BACK_RIGHT A10
#define BACK_LEFT A8
#define UPDATE_BNO() bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER)
#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))
#define DIGITAL_READ(x) digitalRead(x) && digitalRead(x)

sensors_event_t accelerometerData, gyroData, orientationData, linearAccelData;
