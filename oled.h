  

#include <Arduino.h>
#include <U8x8lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif

// Please UNCOMMENT one of the contructor lines below
U8X8_SSD1306_128X64_NONAME_SW_I2C oled(28, 30);



void oledConfig()
{
  
  oled.begin();
  oled.setPowerSave(0);
  oled.setFont(u8x8_font_chroma48medium8_r);
  
  
}

// void loop(void)
// {
//   u8x8.drawString(0,1,"Hello World!");
//   u8x8.setInverseFont(1);
//   u8x8.drawString(0,0,"012345678901234567890123456789");
//   u8x8.setInverseFont(0);
//   //u8x8.drawString(0,8,"Line 8");
//   //u8x8.drawString(0,9,"Line 9");
//   u8x8.refreshDisplay();		// only required for SSD1606/7  
//   delay(2000);
// }
