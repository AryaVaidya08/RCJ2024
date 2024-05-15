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

const motor_pins motorPins[4] = {{35,34,12,18},{36,37, 8,19},{42,43, 9,3},{A5,A4,5,2}};

class Motor{
  public:
  Motor(int port, bool reverse = false){
    this->port = port;
    attachEncoder();
    mult = 1;
    encs_multiplier[port] = 1;
    if(reverse)
    {
      mult *= -1;
      encs_multiplier[port] *= -1;
    }

  }



  void run(int speed){
    speed *= mult;
    speed = min(255, speed);
    speed = max(-255, speed);
    if(speed >= 0){
      digitalWrite(motorPins[port].h2, LOW);
      digitalWrite(motorPins[port].h1, HIGH);
      analogWrite(motorPins[port].pwm, speed);
      position[port] = true;
    }
    if(speed < 0){
      digitalWrite(motorPins[port].h1, LOW);
      digitalWrite(motorPins[port].h2, HIGH);
      analogWrite(motorPins[port].pwm, -speed);
      position[port] = false;
    }
  }



#define ATTACH_INT2(x) attachInterrupt(digitalPinToInterrupt(motorPins[x].cha),interupt##x,RISING)
#define ATTACH_INT(x) ATTACH_INT2(x)
  void attachEncoder(){
  //will make this better later I think
    if(port == MP1)
      ATTACH_INT(MP1);
    if(port == MP2)
      ATTACH_INT(MP2);
    if(port == MP3)
      ATTACH_INT(MP3);
    if(port == MP4)
      ATTACH_INT(MP4);
  }
#undef ATTACH_INT
#undef ATTACH_INT2
  int32_t& getEncoders(){
    return encs[port];
  }

  void resetTicks(){
  	getEncoders() = 0;
  }

  void setTicks(int32_t val) {
    getEncoders() = val;
  }
  
private: 
#define CREATE_INTERUPT2(x) static void interupt##x(){ \
  if(position[x]) encs[x] += 1 * encs_multiplier[x]; \
    else encs[x] -= 1 * encs_multiplier[x]; \
  }
#define CREATE_INTERUPT(x) CREATE_INTERUPT2(x)
  
  CREATE_INTERUPT(MP1);
  CREATE_INTERUPT(MP2);
  CREATE_INTERUPT(MP3);
  CREATE_INTERUPT(MP4);

#undef CREATE_INTERUPT
#undef CREATE_INTERUPT2

  int port = 0;
  static inline int32_t encs[4] = {0};
  static inline bool position[4] = {true};
  static inline int encs_multiplier[4] = {1};
  int mult;
};
#endif