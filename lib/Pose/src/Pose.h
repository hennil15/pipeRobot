#ifndef _POSE_H_
#define  _POSE_H_

//holy shit this code is useless at the moment..

#include <stdint.h>
#include <cmath>

class Pose
{
  private:
    float _transformationToModule[3][3];
    float _transformationToGravity[3][3];
    float _moduleVector[3];
    void convertToUnitVector(float * rawVector);
  public:
    Pose();
    void setImuPose(const float * transformationMatrix);
    void imuIntput(const float * imuVector);
    float getXdir();
    float getYdir();
    float getZdir();

};



#endif  //_POSE_H_
