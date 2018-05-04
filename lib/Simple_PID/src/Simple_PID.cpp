#include "Simple_PID.h"


Simple_PID::Simple_PID(float kp, float ki, float kd)
:_kp(kp), _ki(ki), _kd(kd), _err(0), _sumErr(0), _diffErr(0)
{}

Simple_PID::Simple_PID(){}

float Simple_PID::output(const float & err)
{
  _diffErr = err - _err; // TODO: divide by dt somehow
  _err = err;
  _sumErr += _err;
  //if(_reset >= 32000) {_reset = 0; _sumErr = 0;}  //  resets _sumErr if the output has been saturateted for to long

  float output = _kp*_err + _ki*_sumErr + _kd*_diffErr;
  if(output > _satH)  {++_reset; return _satH;}
  else if(output < _satL) {++_reset; return _satL;}
  return output;
}

void Simple_PID::loopDone(){_err = 0; _sumErr = 0; _diffErr = 0;}
void Simple_PID::set_kp(float kp){_kp = kp;}
void Simple_PID::set_ki(float ki){_ki = ki;}
void Simple_PID::set_kd(float kd){_kd = kd;}
void Simple_PID::begin(float kp, float ki, float kd)
{
  set_kp(kp);
  set_ki(ki);
  set_kd(kd);
}
void Simple_PID::setSaturation(float satL, float satH)
{
  _satL = satL;
  _satH = satH;
}
