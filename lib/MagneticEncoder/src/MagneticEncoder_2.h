#ifndef _MAGNETIC_ENCODER_2_H_
#define _MAGNETIC_ENCODER_2_H_


#include <stdint.h>

class MagneticEncoder_2
{
private:
  const uint16_t _countPrRev = 1787;
  static const uint8_t max_encoder_capacity = 2;
  uint8_t _inputApin, _inputBpin;

  volatile int32_t _positionCount;
  volatile bool _direction;
  volatile bool _stateA;
  volatile bool _stateB;
  volatile bool _lastA;
  volatile bool _lastB;

  static MagneticEncoder_2* refs[max_encoder_capacity];
  static uint8_t _encoderCount;
  static uint8_t _inputs[max_encoder_capacity][2];  //stores the input pins of each instance
  uint8_t _id;

public:
  MagneticEncoder_2(uint8_t inputApin, uint8_t inputBpin);
  ~MagneticEncoder_2();
  static inline uint8_t maxCount();
  static void begin();
  static void isr();
  bool getDirection();
  float getPosition();
  int32_t getPositionCount();
  void isrA();  // isr for pcint
  void selfDestruct();

};



#endif  //_MAGNETIC_ENCODER_H_
