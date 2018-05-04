#include "Pose.h"

Pose::Pose()
{

}

void Pose::convertToUnitVector(float * rawVector)
{
  float vectorLength = sqrt(pow(rawVector[0], 2) + pow(rawVector[1], 2) + pow(rawVector[2], 2));
  for(uint8_t i = 0; i < 3; ++i)
  {
    rawVector[i] *= 1/vectorLength;
  }
}

void Pose::imuInput(const float * imuVector)
{
  float tempVector = imuVector;
  convertToUnitVector(tempVector);
  _moduleVector[0] = 0;
  _moduleVector[1] = 0;
  _moduleVector[2] = 0;
  for(uint8_t i = 0; i < 3; ++i)
  {
    _moduleVector[0] += _transformationToModule[0][i] * tempVector[i];
    _moduleVector[1] += _transformationToModule[1][i] * tempVector[i];
    _moduleVector[2] += _transformationToModule[2][i] * tempVector[i];
  }
}
