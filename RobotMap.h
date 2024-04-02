#define MUXADDR 0x70
#define OLED_CLK 28
#define OLED_DATA 30
#include <U8x8lib.h>
#include <VL53L0X.h>


U8X8_SSD1306_128X64_NONAME_SW_I2C oled(OLED_CLK, OLED_DATA);

#ifdef DEBUG_DISPLAY
#define oled_println(...) oled.println(__VA_ARGS__)
#define oled_print(...) oled.print(__VA_ARGS__)
#define oled_clear() \
  oled.clear(); \
  oled.clearDisplay(); \
  oled.setCursor(0, 0)
#else
#define oled_println(...)
#define oled_print(...)
#define oled_clear()
#endif

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

const int SPEED = 150;
const int ALIGN_SPEED = 85;
