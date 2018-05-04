#include "A4990Motor.h"

A4990Motor::A4990Motor(uint8_t inHigh, uint8_t inLow, uint8_t acceleration, uint8_t inA, uint8_t inB)
:encoder(inA, inB)
{
  pinMode(inHigh, OUTPUT);
  pinMode(inLow, OUTPUT);

  inHigh_ = inHigh;
  inLow_ = inLow;

  A4990Motor::setAcceleration(acceleration);
  A4990Motor::setTempo(0);
  A4990Motor::setDirection(1);

  analogWrite(inHigh,0);
  analogWrite(inLow,256);

  encoder.begin();


}

void A4990Motor::drive(bool dir, uint8_t tempo)
{
  // bool lastDir = A4990Motor::getDirection();
  //
  // if(tempo == 0)
  // {
  //   A4990Motor::stopMotor();
  // }
  // else if(dir != lastDir)
  // {
  //   A4990Motor::stopMotor();
  //   A4990Motor::setDirection(dir);
  //   A4990Motor::setTempo(tempo);
  //   A4990Motor::startMotor();
  // }
  // else if(dir == lastDir)
  // {
  //   A4990Motor::changeTempo(tempo);
  // }
  uint8_t pwm = map(tempo, 0, 100, 0, 255);
  if(dir)
  {
    analogWrite(inHigh_, pwm);
    analogWrite(inLow_, 255);
  }
  else
  {
    analogWrite(inHigh_, (0));
    analogWrite(inLow_, (255-pwm));
  }
}

void A4990Motor::drive(float distance, bool dir, uint8_t tempo) //distance in rev, dir 0 =CW - 1=CCW, tempo 0-100
{
  // float stopDistance = 0.2;
  // A4990Motor::setTempo(tempo);
  // A4990Motor::setDirection(dir);

  float startPosition = encoder.getPosition();
  float netDistance;
  if(dir) {netDistance = distance;}
  else {netDistance = -distance;}
  float desiredPosition = startPosition + netDistance;
  A4990Motor::driveTo(desiredPosition, tempo);

  // if (dir == 0)
  // {
  //   distance = -distance+stopDistance;
  //   float endPosition = startPosition + distance;
  //
  //   A4990Motor::startMotor();
  //
  //   while(encoder.getPosition() > endPosition)
  //   {
  //     //wait
  //   }
  //   A4990Motor::stopMotor();
  //
  // }
  // else if (dir ==1)
  // {
  //   distance = distance-stopDistance;
  //   float endPosition = startPosition + distance;
  //
  //   A4990Motor::startMotor();
  //   Serial.println("Motor started");
  //   while(encoder.getPosition() < endPosition)
  //   {
  //     Serial.println(encoder.getPosition());
  //   }
  //   A4990Motor::stopMotor();
  // }
}


void A4990Motor::driveTo(float position, uint8_t tempo)
{
  // float stopDistance = 0.5;
  A4990Motor::setTempo(tempo);

  if (encoder.getPosition() == position)
  {
    //do nothing
  }
  else
  {
    int i = 0;
    if (encoder.getPosition() < position)
    {
      A4990Motor::setDirection(1);
      A4990Motor::startMotor();
      while(encoder.getPosition() < position - _stopDistance)
      {
        //wait
        ++i;
        if(i > 1000) {i = 0; Serial.println(encoder.getPosition());}
      }
      A4990Motor::regulatedStop(position);
    }
    else if(encoder.getPosition() > position)
    {
      A4990Motor::setDirection(0);
      A4990Motor::startMotor();
      while(encoder.getPosition() > position + _stopDistance)
      {
        //wait
        ++i;
        if(i > 1000) {i = 0; Serial.println(encoder.getPosition());}
      }
      A4990Motor::regulatedStop(position);
    }
  }

}



void A4990Motor::startMotor()
{

  float tempo = A4990Motor::getTempo() * 2.55;
  bool dir = A4990Motor::getDirection();

  if (dir == 1)
  {
  float pwmTempo = 0;
  analogWrite(inLow_,255);
  analogWrite(inHigh_,0);
  while(pwmTempo < tempo)
  {
    pwmTempo++;
    analogWrite(inHigh_,pwmTempo);
    delay(A4990Motor::getAcceleration());
  }
  }
  else if (dir == 0)
  {
  analogWrite(inHigh_,0);
  analogWrite(inLow_,255);
  float pwmTempo = 255;
  while(pwmTempo > 255-tempo)
  {
    pwmTempo--;
    analogWrite(inLow_,pwmTempo);
    delay(A4990Motor::getAcceleration());
  }
  }
}

void A4990Motor::stopMotor()
{
  // // float startPosition = encoder.getPosition();
  // bool dir = A4990Motor::getDirection();
  // float pwmTempo;
  // if(dir)
  // {
  //   pwmTempo = A4990Motor::getTempo()*2.55;
  //   while(pwmTempo > 0)
  //   {
  //     pwmTempo--;
  //     analogWrite(inHigh_, pwmTempo);
  //     delay(A4990Motor::getAcceleration());
  //   }
  // }
  // else
  // {
  //   pwmTempo = 255-A4990Motor::getTempo()*2.55;
  //   while(pwmTempo < 255)
  //   {
  //     pwmTempo++;
  //     analogWrite(inLow_, pwmTempo);
  //     delay(A4990Motor::getAcceleration());
  //   }
  // }
  analogWrite(inHigh_, 0);
  analogWrite(inLow_, 255);
}

void A4990Motor::regulatedStop(const float & desiredPosition)
{
  float maxTempo = A4990Motor::getTempo()*2.55;
  pid.begin(127, 1, 0);
  pid.setSaturation(-maxTempo, maxTempo);
  float momentaryPosition = encoder.getPosition();
  float tolerance = 0.001;
  float err = desiredPosition - momentaryPosition;
  // float errSum = err;
  float pwm;
  // float kp = 255/_stopDistance;
  // float ki = 0.8;
  int i = 0;
  while(err > tolerance || err < -tolerance)
  {
    pwm = pid.output(err);
    // pwm = err*kp + errSum*ki;
    // if(pwm > maxTempo) {pwm = maxTempo;}
    // else if(pwm < -maxTempo)  {pwm = -maxTempo;}
    if(pwm >= 0)  {analogWrite(inHigh_, pwm);}
    else  {analogWrite(inLow_, 255 + pwm);}
    err = desiredPosition - encoder.getPosition();
    // errSum += err;
    ++i;
    if(i > 1000)
    {
      Serial.print(desiredPosition); Serial.print("\t");
      Serial.println(encoder.getPosition());
      i = 0;
    }
  }
  pid.loopDone();
  analogWrite(inHigh_, 0);
  analogWrite(inLow_, 255);
}

void A4990Motor::changeTempo(uint8_t tempo)
{
  A4990Motor::setTempo(tempo);
  bool dir = A4990Motor::getDirection();
  int pwmTempo = tempo*2.55;
  if(dir == 1)
  {
    analogWrite(inHigh_,255);
    analogWrite(inLow_,pwmTempo);
  }
  else if(dir == 0)
  {
    analogWrite(inHigh_,0);
    analogWrite(inLow_, 255-pwmTempo);
  }

}
/*
void A4990Motor::changeDirection(bool dir)
{
  uint8_t tempo = A4990Motor::getTempo();
  if( dir =! A4990Motor::getDirection())
  {
    A4990Motor::stopMotor();
    A4990Motor::setDirection(dir);
    A4990Motor::setTempo(tempo);
    A4990Motor::startMotor();
  }
  else if (dir == A4990Motor::getDirection())
  {
    //do nothing
  }
}
*/
