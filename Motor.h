#ifndef _MOTORS_H_
#define _MOTORS_H_
#define MP1 0
#define MP2 1
#define MP3 2
#define MP4 3


struct motor_pins{
  uint8_t h1;
  uint8_t h2;
  uint8_t pwm;
  uint8_t cha;
};

const motor_pins pi_motor_pins[4] = {{35,34,12,18},{36,37, 8,19},{42,43, 9,3},{A5,A4,5,2}};

class Motor{
  public:
  Motor(int port, bool attachEnc = true, bool reverse = false){
    this->port = port;
    mult = 1;
    enc_mult[port] = 1;
    if(reverse)
    {
      mult *= -1;
      enc_mult[port] *= -1;
    }
  }



  void run(int speed){
    #ifndef MOTORSOFF
    speed *= mult;
    speed = min(255, speed);
    speed = max(-255, speed);
    if(speed >= 0){
      digitalWrite(pi_motor_pins[port].h2, LOW);
      digitalWrite(pi_motor_pins[port].h1, HIGH);
      analogWrite(pi_motor_pins[port].pwm, speed);
      dir[port] = true;
      
    }
    if(speed < 0){
      digitalWrite(pi_motor_pins[port].h1, LOW);
      digitalWrite(pi_motor_pins[port].h2, HIGH);
      analogWrite(pi_motor_pins[port].pwm, -speed);
      dir[port] = false;
    }
    #endif
  }

		   
  void stop(){
    run(0);
  }

 
private: 
  int port = 0;
  static inline int32_t ticks[4] = {0};
  static inline bool dir[4] = {true};
  static inline int enc_mult[4] = {1};
  int mult;
};
#endif


