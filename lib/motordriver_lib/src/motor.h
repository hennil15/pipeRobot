#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "arduino.h" //includes arduiono library
#include <stdint.h> //includes stdint library

class Motor
{
public:

//virtual Motor() {};
virtual ~ Motor() {}; //constructor

virtual void setTempo(uint8_t tempo) //set the pre defined standard tempo (0-100%)
{
  tempo_ = tempo;
}

virtual uint8_t getTempo() //get the pre defined standard tempo (0-100%)
{
  return tempo_;
}

virtual void setRealTempo(uint8_t realTempo) //Sets the real tempo (0-100%)
{
  realTempo_ = realTempo;
}

virtual uint8_t getRealTempo() //return the real tempo (0-100%)
{
  return realTempo_;
}

virtual void setDirection(bool dir) //sets the direction
{
  dir_ = dir;
}

virtual bool getDirection() //returns the direction
{
  return dir_;
}

virtual void setAcceleration(uint8_t acceleration)
//sets the time in ms between each step of the deceleration (255 steps)
{
  acceleration_ = acceleration;
}

virtual uint8_t getAcceleration()
//returns the time is ms between each step of the deceleration (255 steps)
{
  return acceleration_;
}

virtual void stopMotor()=0; //stops the motor at a set acceleration

virtual void startMotor()=0; //starts the motor with a set acceleration

virtual void changeTempo(uint8_t tempo)=0;

virtual void drive(bool dir, uint8_t tempo); // drive indefinately
virtual void drive(float distance, bool dir, uint8_t tempo); //drives the robot a certain distance
virtual void driveTo(float position, uint8_t tempo); //drive to absolute position


private:
  bool dir_;
  uint8_t tempo_;
  uint8_t realTempo_;
  uint8_t acceleration_;

};

#endif
