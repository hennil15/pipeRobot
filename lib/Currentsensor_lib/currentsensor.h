#ifndef _CURRENTSENSOR_H_
#define _CURRENTSENSOR_H_

#include "arduino.h"
#include <stdint.h>

class currentSensor
{
public:
  currentSensor(int inputPin, float shuntResistor, float gain);
  float readRawValue();
  float readValueAmpere();

private:
  uint8_t inputPin_;
  float shuntResistor_;
  float gain_;

};
#endif
