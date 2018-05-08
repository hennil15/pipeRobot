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
  uint16_t numberOfReadings = 1000;
  uint16_t xReadings[numberOfReadings];

for(uint16_t i =0;i<numberOfReadings;i++)
{
  xReadings[i] = analogRead(inputPin_);
  //Serial.print("Value placed in array: ");
  //Serial.println(xReadings[i]);
}
//Serial.print("value of array pos 0");
//Serial.println(xReadings[0]);

for(uint16_t i =0; i<numberOfReadings; i++)
{
  /*Serial.print("sum of reading incoming: ");
  Serial.println(sumOfReadings);
  Serial.print("x reading incoming: ");
  Serial.println(xReadings[i]);*/

  sumOfReadings = sumOfReadings + xReadings[i];

  //Serial.print("sum of reading inside: ");
  //Serial.println(sumOfReadings);
}
//Serial.print("sum of readings outside: ");
//Serial.println(sumOfReadings);

float averageValue = static_cast<float>(sumOfReadings)/static_cast<float>(numberOfReadings);
//Serial.print("averageValue =");
//Serial.println(averageValue);

return averageValue;

}

float currentSensor::readValueAmpere()
{
  float rawValue = readRawValue();
  float voltage = (rawValue*3.3)/1024;
  float current = voltage/(shuntResistor_*gain_);

  //Serial.print("return value current =");
  //Serial.println(current);

  return current;
}
