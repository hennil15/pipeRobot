#include "MagneticEncoder_2.h"
#include <Arduino.h>

MagneticEncoder_2* MagneticEncoder_2::refs[];
uint8_t MagneticEncoder_2::_encoderCount;
uint8_t MagneticEncoder_2::_inputs[max_encoder_capacity][2];

MagneticEncoder_2::MagneticEncoder_2(uint8_t inputApin, uint8_t inputBpin)
:_inputApin(inputApin), _inputBpin(inputBpin)
{
  _positionCount = 0;
  _direction = 0;
  _stateA = 0;
  _stateB = 0;
  _lastA = 0;
  _lastB = 0;
  _id = _encoderCount;
  if(_encoderCount < max_encoder_capacity)
  {
    refs[_encoderCount] = this;
    _inputs[_encoderCount][0] = _inputApin;
    _inputs[_encoderCount][1] = _inputBpin;
  }
  ++_encoderCount;
}

MagneticEncoder_2::~MagneticEncoder_2()
{
  --_encoderCount;
}

inline uint8_t MagneticEncoder_2::maxCount()
{
  return _encoderCount < max_encoder_capacity ? _encoderCount : max_encoder_capacity;
}

void MagneticEncoder_2::begin() //heter egentlig instanceISR()
{
  for(uint8_t i = 0; i < maxCount(); ++i)
  {
    pinMode(_inputs[i][0], INPUT_PULLUP);
    pinMode(_inputs[i][1], INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(_inputs[i][0]), isr, CHANGE);
  }
  // attachInterrupt(digitalPinToInterrupt(_inputs[0][0]), isr0, CHANGE);
  // attachInterrupt(digitalPinToInterrupt(_inputs[1][0]), isr1, CHANGE);
}


bool MagneticEncoder_2::getDirection()  {return _direction;}
float MagneticEncoder_2::getPosition()  {return static_cast<float>(_positionCount)/static_cast<float>(_countPrRev);}
int32_t MagneticEncoder_2::getPositionCount() {return _positionCount;}

void MagneticEncoder_2::isrA()
{
  _stateA = digitalRead(_inputApin);
  _stateB = digitalRead(_inputBpin);
  if(_stateA != _lastA)
  {
    if(_lastA != _stateB) {++_positionCount; _direction = true;}
    else {--_positionCount; _direction = false;}
  }
  _lastA = _stateA;
  _lastB = _stateB;
}

void MagneticEncoder_2::isr()
{
  for(uint8_t i = 0; i < maxCount(); ++i)
  {
    refs[i]->isrA();
  }
}

// void MagneticEncoder_2::isr0()
// {refs[0]->isrA();}
// void MagneticEncoder_2::isr1()
// {refs[1]->isrA();}

void MagneticEncoder_2::selfDestruct()
{
  _positionCount = 0;
}
