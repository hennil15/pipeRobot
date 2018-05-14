#ifndef _POSE_H_
#define  _POSE_H_

//holy shit this code is useless at the moment..

#include <stdint.h>
#include <cmath>

class Pose
{
  private:
    float _transformationToModule[3][3];
    float _moduleVector[3];
    void convertToUnitVector(float * rawVector);
  public:
    Pose();
    // void setTransformationMatrix(const float * rotationMatrix);
    // The original setTransformationMatrix() didn't work, and there wasn't time to fix it, so it was hard-coded instead
    void setTransformationMatrix( float a, float b, float c,
                                  float d, float e, float f,
                                  float g, float h, float i);
    void imuInput(const float * imuVector);
    float getXdir();
    float getYdir();
    float getZdir();
};



#endif  //_POSE_H_
