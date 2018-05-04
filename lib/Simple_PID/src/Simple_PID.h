#ifndef _SIMPLE_PID_H_
#define _SIMPLE_PID_H_

class Simple_PID
{
private:
  float _kp, _ki, _kd;
  float _err, _sumErr, _diffErr;
  float _satH, _satL;    //saturation. Sets max/min output
  unsigned int _reset;

public:
  Simple_PID(float kp, float ki, float kd);
  Simple_PID();
  float output(const float & err);
  void loopDone();
  void set_kp(float kp);
  void set_ki(float ki);
  void set_kd(float kd);
  void begin(float kp, float ki, float kd);
  void setSaturation(float satL, float satH);
};

#endif //_SIMPLE_PID_H_
