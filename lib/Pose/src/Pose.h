#ifndef _POSE_H_
#define  _POSE_H_

//holy shit this code is useless at the moment..

#include <stdint.h>
//#include <cmath>

class Pose
{
  private:
    int16_t _ref1[3];
    int16_t _ref2[3];
    float _diff[3];
    float _sensitivity;

  public:
    Pose();
    void input1(int16_t x, int16_t y, int16_t z);
    void input2(int16_t x, int16_t y, int16_t z);
    float diffX();
    float diffY();
    float diffZ();

    void setSensitivity(uint8_t fs);
};



#endif  //_POSE_H_
