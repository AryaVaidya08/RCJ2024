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
  Motor(int port, bool reverse = false){
    this->port = port;
    attachEncoder();
    mult = 1;
    enc_mult[port] = 1;
    if(reverse)
    {
      mult *= -1;
      enc_mult[port] *= -1;
    }
    boost = 0;
      
//   //The PWM frequency is 976 Hz
// #if defined(__AVR_ATmega32U4__) //MeBaseBoard use ATmega32U4 as MCU
//   TCCR1A =  _BV(WGM10);
//   TCCR1B = _BV(CS11) | _BV(CS10) | _BV(WGM12);

//   TCCR3A = _BV(WGM30);
//   TCCR3B = _BV(CS31) | _BV(CS30) | _BV(WGM32);

//   TCCR4B = _BV(CS42) | _BV(CS41) | _BV(CS40);
//   TCCR4D = 0;

// #elif defined(__AVR_ATmega328__) // else ATmega328

//   TCCR1A = _BV(WGM10);
//   TCCR1B = _BV(CS11) | _BV(CS10) | _BV(WGM12);

//   TCCR2A = _BV(WGM21) | _BV(WGM20);
//   TCCR2B = _BV(CS22);

// #elif defined(__AVR_ATmega2560__) //else ATmega2560
//   TCCR1A = _BV(WGM10);
//   TCCR1B = _BV(CS11) | _BV(CS10) | _BV(WGM12);

//   TCCR2A = _BV(WGM21) | _BV(WGM20);
//   TCCR2B = _BV(CS22);
// #endif
  }



  void _run(int speed){
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
		   
void run(int speed){
	int _speed = 0;
	if(speed > 0)
	_speed = boost + speed;
	if(speed < 0)
	_speed = -boost + speed;
	_run(_speed);
}

void addBoost(int speed){
	boost = speed;
}
		   
  void stop(){
    run(0);
  }
#define ATTACH_INT2(x) attachInterrupt(digitalPinToInterrupt(pi_motor_pins[x].cha),interupt##x,RISING)
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
  int32_t& getTicks(){
    return ticks[port];
  }

  void resetTicks(){
  	getTicks() = 0;
  }

  void setTicks(int32_t val) {
    getTicks() = val;
  }
  
private: 
#define CREATE_INTERUPT2(x) static void interupt##x(){ \
  if(dir[x]) ticks[x] += 1 * enc_mult[x]; \
    else ticks[x] -= 1 * enc_mult[x]; \
  }
#define CREATE_INTERUPT(x) CREATE_INTERUPT2(x)
  
  CREATE_INTERUPT(MP1);
  CREATE_INTERUPT(MP2);
  CREATE_INTERUPT(MP3);
  CREATE_INTERUPT(MP4);

#undef CREATE_INTERUPT
#undef CREATE_INTERUPT2

  int port = 0;
  uint8_t boost;
  static inline int32_t ticks[4] = {0};
  static inline bool dir[4] = {true};
  static inline int enc_mult[4] = {1};
  int mult;
};
#endif