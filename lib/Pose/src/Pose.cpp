#include "Pose.h"

Pose::Pose()
:_sensitivity(16384)
{}

void Pose::input1(int16_t x, int16_t y, int16_t z)
{
  _ref1[0] = x;
  _ref1[1] = y;
  _ref1[2] = z;
}

void Pose::input2(int16_t x, int16_t y, int16_t z)
{
  _ref2[0] = x;
  _ref2[1] = y;
  _ref2[2] = z;
}

float Pose::diffX()
{
  _diff[0] = (static_cast<float>(_ref1[0]) - static_cast<float>(_ref2[0]));
  return _diff[0];
}
float Pose::diffY()
{
  _diff[1] = (static_cast<float>(_ref1[1]) - static_cast<float>(_ref2[1]));
  return _diff[1];
}
float Pose::diffZ()
{
  _diff[2] = (static_cast<float>(_ref1[2]) - static_cast<float>(_ref2[2]));
  return _diff[2];
}

void Pose::setSensitivity(uint8_t fs)
{
  switch (fs)
  {
    case 0:
      _sensitivity = 16384;
      break;
    case 1:
      _sensitivity = 8192;
      break;
    case 2:
      _sensitivity = 4096;
      break;
    case 3:
      _sensitivity = 2048;
      break;
    default:
      _sensitivity = 16384;
  }
}
