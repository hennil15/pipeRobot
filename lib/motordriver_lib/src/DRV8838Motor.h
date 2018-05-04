#ifndef _DRV8838MOTOR_H_
#define _DRV8838MOTOR_H_

#include "motor.h"
#include "MagneticEncoder_2.h"
#include <Simple_PID.h>

//Pololu DRV8838 single motor driver
class DRV8838Motor : public Motor
{
  public:
    MagneticEncoder_2 encoder;
    Simple_PID pid;

  DRV8838Motor(uint8_t pwnPin, uint8_t dirPin, uint8_t acceleration, uint8_t inA, uint8_t inB);

  void drive(bool dir, uint8_t tempo);
  void drive(float distance, bool dir, uint8_t tempo);
  void driveTo(float position, uint8_t tempo);

  void stopMotor();
  void startMotor();

//  void changeDirection(bool dir);
  void changeTempo(uint8_t tempo);

private:
  void regulatedStop(const float & desiredPosition);
  const float _stopDistance = 0.2;
  uint8_t pwmPin_;
  uint8_t dirPin_;

};

#endif
