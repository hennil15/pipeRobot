#ifndef _A4990MOTOR_H_
#define _A4990MOTOR_H_

#include "MagneticEncoder_2.h"
#include "motor.h"
#include <Simple_PID.h>

//Pololu A4990 double motor driver
class A4990Motor : public Motor
{
public:
  MagneticEncoder_2 encoder;
  Simple_PID pid;

A4990Motor(uint8_t inHigh, uint8_t inLow, uint8_t acceleration, uint8_t inA, uint8_t inB);

void drive(bool dir, uint8_t tempo);
void drive(float distance, bool dir, uint8_t tempo);
void driveTo(float position, uint8_t tempo);

void startMotor();
void stopMotor();

void changeTempo(uint8_t tempo);
//void changeDirection(bool dir);

private:
  void regulatedStop(const float & desiredPosition);
  const float _stopDistance = 0.2;
  uint8_t inHigh_;
  uint8_t inLow_;
  //uint8_t EF1_;
  //uint8_t EF2_;
};

#endif
