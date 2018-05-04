#include "DRV8838Motor.h"

DRV8838Motor::DRV8838Motor(uint8_t pwmPin,uint8_t dirPin, uint8_t acceleration, uint8_t inA, uint8_t inB)
:encoder(inA,inB)
{
  pwmPin_ = pwmPin;
  dirPin_ = dirPin;
  pinMode(pwmPin, OUTPUT);
  pinMode(dirPin, OUTPUT);

  DRV8838Motor::setAcceleration(acceleration);
  DRV8838Motor::setTempo(0);
  DRV8838Motor::setDirection(1);

  encoder.begin();
}


void DRV8838Motor::drive(bool dir, uint8_t tempo)
{
  // bool lastDir = DRV8838Motor::getDirection();
  //
  // if(tempo == 0)
  // {
  //   DRV8838Motor::stopMotor();
  // }
  // else if(dir != lastDir)
  // {
  //   DRV8838Motor::stopMotor();
  //   DRV8838Motor::setDirection(dir);
  //   DRV8838Motor::setTempo(tempo);
  //   DRV8838Motor::startMotor();
  // }
  // else if(dir == lastDir)
  // {
  //   DRV8838Motor::changeTempo(tempo);
  // }
  digitalWrite(dirPin_, dir);
  analogWrite(pwmPin_, map(tempo, 0, 100, 0, 255));
}

void DRV8838Motor::drive(float distance, bool dir, uint8_t tempo) //distance in rev, dir 0 =CW - 1=CCW, tempo 0-100
{
  //float stopDistance = 0.5;
  //DRV8838Motor::setTempo(tempo);
  //DRV8838Motor::setDirection(dir);

  float startPosition = encoder.getPosition();
  float netDistance;
  if(dir) {netDistance = distance;}
  else {netDistance = -distance;}
  float desiredPosition = startPosition + netDistance;
  DRV8838Motor::driveTo(desiredPosition, tempo);

  /*if (dir == 0)
  {
    distance = -distance+stopDistance;
    float endPosition = startPosition + distance;

    DRV8838Motor::startMotor();

    while(encoder.getPosition() > endPosition)
    {
      //wait
    }
    DRV8838Motor::stopMotor();

  }
  else if (dir ==1)
  {
    distance = distance-stopDistance;
    float endPosition = startPosition + distance;

    DRV8838Motor::startMotor();

    while(encoder.getPosition() < endPosition)
    {
      //wait
    }
    DRV8838Motor::stopMotor();
  }
*/
}


void DRV8838Motor::driveTo(float position, uint8_t tempo)
{
  //float stopDistance = 0.5;
  DRV8838Motor::setTempo(tempo);

  if (encoder.getPosition() == position)
  {
    //do nothing
  }
  else
  {
    int i = 0;
    if (encoder.getPosition() < position)
    {
      DRV8838Motor::setDirection(1);
      DRV8838Motor::startMotor();
      while(encoder.getPosition() < position-_stopDistance)
      {
        //wait
        ++i;
        if(i>1000) {i = 0; Serial.println(encoder.getPosition());}
        }
        DRV8838Motor::regulatedStop(position);
      }
    else if(encoder.getPosition() > position)
    {
      DRV8838Motor::setDirection(0);
      DRV8838Motor::startMotor();
      while(encoder.getPosition() > position + _stopDistance)
      {
        //wait
        ++i;
        if(i>1000) {i = 0; Serial.println(encoder.getPosition());}
      }
      DRV8838Motor::regulatedStop(position);
    }
  }

}

void DRV8838Motor::stopMotor()
{
  // float pwmTempo = getTempo()*2.55;
  // while(pwmTempo > 0)
  // {
  //   pwmTempo--;
  //   analogWrite(pwmPin_,pwmTempo);
  //   delay(DRV8838Motor::getAcceleration());
  // }
  analogWrite(pwmPin_,0);
}

void DRV8838Motor::startMotor()
{
  float pwmTempo = 0;
  while(pwmTempo < DRV8838Motor::getTempo()*2.55)
  {
    pwmTempo++;
    analogWrite(pwmPin_,pwmTempo);
    delay(DRV8838Motor::getAcceleration());
  }
  analogWrite(pwmPin_,DRV8838Motor::getTempo()*2.55);
}
/*
void DRV8838Motor::changeDirection(bool dir)
{
  float pwmTempo = getTempo()*2.55;
  while(pwmTempo > 0)
  {
    pwmTempo--;
    analogWrite(pwmPin_,pwmTempo);
    delay(DRV8838Motor::getAcceleration());
  }

    digitalWrite(dirPin_, dir);

    while(pwmTempo < DRV8838Motor::getTempo()*2.55)
  {
    pwmTempo++;
    analogWrite(pwmPin_,pwmTempo);
    delay(DRV8838Motor::getAcceleration());
  }
    setDirection(dir);
}
*/

void DRV8838Motor::regulatedStop(const float & desiredPosition)
{
  float maxTempo = DRV8838Motor::getTempo()*2.55;
  pid.begin(127,1,0);
  pid.setSaturation(-maxTempo,maxTempo);
  float momentaryPosition = encoder.getPosition();
  float tolerance = 0.001;
  float err = desiredPosition - momentaryPosition;

  float pwm;

  int i = 0;
  while(err > tolerance || err < -tolerance)
  {
    pwm = pid.output(err);

    if(pwm>=0) {analogWrite(pwmPin_, pwm);}
    else
    {
      digitalWrite(dirPin_,0);
      pwm = -pwm;
      analogWrite(pwmPin_, pwm);
    }

    err = desiredPosition - encoder.getPosition();
    ++i;
    if(i>1000)
    {
      Serial.print(desiredPosition); Serial.print("\t");
      Serial.println(encoder.getPosition());
      i = 0;
  }
}
pid.loopDone();
analogWrite(pwmPin_,0);
}

void DRV8838Motor::changeTempo(uint8_t tempo)
{
  DRV8838Motor::setTempo(tempo);
  float pwmTempo = tempo*2.55;
  analogWrite(pwmPin_,pwmTempo);
}
