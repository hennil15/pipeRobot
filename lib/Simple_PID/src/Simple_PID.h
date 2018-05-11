#ifndef _SIMPLE_PID_H_
#define _SIMPLE_PID_H_

#include <stdint.h>

class Simple_PID
{
private:
  float _kp, _ki, _kd;
  float _err, _sumErr, _diffErr, _lastDtErr;
  float _output;
  float _satH, _satL;    //saturation. Sets max/min output
  uint16_t _dt_us, _last_dt;
  float _dt_sec;

public:
  Simple_PID(float kp, float ki, float kd);
  Simple_PID();
  float output(const float & err);
  void loopDone();
  void set_kp(float kp);
  void set_ki(float ki);
  void set_kd(float kd);
  void set_dt(uint16_t us);
  void begin(float kp, float ki, float kd);
  void setSaturation(float satL, float satH);
};

#endif //_SIMPLE_PID_H_
