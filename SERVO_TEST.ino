#include "Servo.h"

Servo leftServo;
Servo rightServo;

void setup() {
leftServo.attach(30);
rightServo.attach(22);




for(int j = 0; j < 6; j++) {
//   leftServo.write(180);
//   delay(500);
//  for(int i = 0; i < 80; i+=1) {
//    leftServo.write(i);
//     delay(50);
//  }


  rightServo.write(0);
delay(500);
 for(int i = 180; i > 100; i-=1) {
   rightServo.write(i);
    delay(40);
 }
 rightServo.write(180);
 delay(2000);

}
//leftServo.write(180);
rightServo.write(0);

}



void loop() {
  // put your main code here, to run repeatedly:

}
