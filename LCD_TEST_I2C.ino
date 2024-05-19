#include "LiquidCrystal_I2C.h"

#include  "Wire.h"

//initialize the liquid crystal library
//the first parameter is  the I2C address
//the second parameter is how many rows are on your screen
//the  third parameter is how many columns are on your screen
LiquidCrystal_I2C lcd(0x27,  16, 2);

void setup() {
  
  //initialize lcd screen
  Serial.begin(9600);
  lcd.init();
  // turn on the backlight
  lcd.backlight();
  
}


void loop() {
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil("\n");
    Serial.print("You sent me: ");
    Serial.print(data);
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print(data);
  }  
}
