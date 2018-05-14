#include "Simple_PID.h"
#include <Arduino.h>


Simple_PID::Simple_PID(float kp, float ki, float kd)
:_kp(kp), _ki(ki), _kd(kd),
_err(0), _sumErr(0), _diffErr(0), _lastDtErr(0),
_dt_us(500), _steadyState(false), _ssCounter(0), _settlingMargin(10)
{}

Simple_PID::Simple_PID()  //default constructor
:_kp(1), _ki(0), _kd(0),
_err(0), _sumErr(0), _diffErr(0), _lastDtErr(0),
_dt_us(500), _steadyState(false), _ssCounter(0), _settlingMargin(10)
{}

float Simple_PID::output(const float & err)
{
  //warning! micros() overflows after 71.6 minutes (it's an uint32_t type). Code will have runtime errors after that
  if((micros()-_last_dt) > _dt_us)
  {
    _last_dt = micros();

    _err = err;
    _diffErr = (_err - _lastDtErr)/_dt_us;
    _lastDtErr = _err;
    _sumErr += _dt_us*_err;

    _output = _kp*_err + _ki*_sumErr + _kd*_diffErr;

    if(_output > _satH)  {_output = _satH;}
    else if(_output < _satL) {_output = _satL;}

    if(_err > -_errorBand && _err < _errorBand)
    {
      ++_ssCounter;
      if(_ssCounter > _settlingMargin){_steadyState = true;}
    }
    else{_ssCounter = 0;}
  }
  return _output;
}

void Simple_PID::loopDone()
{
  _err = 0;
  _sumErr = 0;
  _diffErr = 0;
  _steadyState = false;
  _ssCounter = 0;
}
void Simple_PID::set_kp(float kp){_kp = kp;}
void Simple_PID::set_ki(float ki){_ki = ki;}
void Simple_PID::set_kd(float kd){_kd = kd;}
void Simple_PID::set_dt(uint16_t us){_dt_us = us;}
void Simple_PID::begin(float kp, float ki, float kd)
{
  _kp = kp;
  _ki = ki;
  _kd = kd;
}
void Simple_PID::setSaturation(float satL, float satH)
{
  _satL = satL;
  _satH = satH;
}

void Simple_PID::setErrorBand(float errB){_errorBand = errB;}
void Simple_PID::setSettlingMargin(uint16_t settM){_settlingMargin = settM;}
bool Simple_PID::steadyState(){return _steadyState;}
