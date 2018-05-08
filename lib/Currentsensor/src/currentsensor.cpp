#include "currentsensor.h"

currentSensor::currentSensor(int inputPin,float shuntResistor,float gain)
{
  inputPin_ = inputPin;
  shuntResistor_=shuntResistor; //resistance in ohm of shunt resistor
  gain_=gain; //calculated gain
  pinMode(inputPin,INPUT_PULLDOWN);
}

float currentSensor::readRawValue()
{
  uint32_t sumOfReadings = 0;
  uint16_t numberOfReadings = 500;
  uint16_t xReadings[numberOfReadings];

for(uint16_t i =0;i<numberOfReadings;i++)
{
  xReadings[i] = analogRead(inputPin_);
}

for(uint16_t i =0; i<numberOfReadings; i++)
{
  sumOfReadings = sumOfReadings + xReadings[i];
}

float averageValue = static_cast<float>(sumOfReadings)/static_cast<float>(numberOfReadings);

return averageValue;

}

float currentSensor::readValueAmpere()
{
  float rawValue = readRawValue();
  float voltage = (rawValue*3.3)/1024;
  float current = voltage/(shuntResistor_*gain_);

  return current;
}
