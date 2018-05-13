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
//this function gave the error 'invalid types 'const float[uint8_t {aka unsigned char}]' for array subscript'
// void Pose::setTransformationMatrix(const float * rotationMatrix)
// {
//   for (uint8_t i = 0; i < 3; ++i)
//   {
//     for (uint8_t u = 0; u < 3; ++u)
//     {
//       _transformationToModule[i][u] = rotationMatrix[i][u];
//     }
//   }
// }

void Pose::setTransformationMatrix( float a, float b, float c,
                                    float d, float e, float f,
                                    float g, float h, float i)
{
  _transformationToModule[0][0] = a;
  _transformationToModule[0][1] = b;
  _transformationToModule[0][2] = c;
  _transformationToModule[1][0] = d;
  _transformationToModule[1][1] = e;
  _transformationToModule[1][2] = f;
  _transformationToModule[2][0] = g;
  _transformationToModule[2][1] = h;
  _transformationToModule[2][2] = i;
}


void Pose::imuInput(const float * imuVector)
{
  float tempVector[3] = {imuVector[0], imuVector[1], imuVector[2]};
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

float Pose::getXdir(){return _moduleVector[0];}
float Pose::getYdir(){return _moduleVector[1];}
float Pose::getZdir(){return _moduleVector[2];}
