#define MUXADDR 0x70
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


const double KP_TURN = 10;
const double KI_TURN = 0.00;
const double KD_TURN = 0.243;

Motor motorL(MP3, true);
Motor motorR(MP4);


const double KP_FORWARD = 1.4;
const double KI_FORWARD = 0.003;
const double KD_FORWARD = 0.01;

const double KP_STRAIGHTEN = 3;




sensors_event_t accelerometerData, gyroData, bnoInfo, linearAccelData;
