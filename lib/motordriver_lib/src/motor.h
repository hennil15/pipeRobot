#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "arduino.h"
#include <stdint.h>

class Motor
{
public:

//virtual Motor() {};
virtual ~ Motor() {};

virtual void setTempo(uint8_t tempo)
{
  tempo_ = tempo;
}
virtual uint8_t getTempo()
{
  return tempo_;
}

virtual void setDirection(bool dir)
{
  dir_ = dir;
}

virtual bool getDirection()
{
  return dir_;
}

virtual void setAcceleration(uint8_t acceleration)
{
  acceleration_ = acceleration;
}

virtual uint8_t getAcceleration()
{
  return acceleration_;
}

virtual void stopMotor()=0;
virtual void startMotor()=0;

//virtual void changeDirection(bool dir)=0;
virtual void changeTempo(uint8_t tempo)=0;


virtual void drive(bool dir, uint8_t tempo);
virtual void drive(float distance, bool dir, uint8_t tempo);
virtual void driveTo(float position, uint8_t tempo);


private:
  bool dir_;
  uint8_t tempo_;
  uint8_t acceleration_;

};

#endif
