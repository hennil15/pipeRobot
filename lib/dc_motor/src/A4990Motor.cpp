#include "A4990Motor.h"

A4990Motor::A4990Motor(uint8_t inHigh, uint8_t inLow, uint8_t inA, uint8_t inB)
:encoder(inA, inB)
{
  pinMode(inHigh, OUTPUT);
  pinMode(inLow, OUTPUT);

  inHigh_ = inHigh;
  inLow_ = inLow;

  A4990Motor::setAcceleration(15); //sets std acceleration of A4990Motor
  A4990Motor::setTempo(0); //sets initial tempo = 0
  A4990Motor::setDirection(1); //sets initial direction positive

  analogWrite(inHigh,0); //in combination with next line => stops motor
  analogWrite(inLow,255);

  encoder.begin();

}

void A4990Motor::drive(bool dir, uint8_t tempo)
{
   bool lastDir = A4990Motor::getDirection();
  //
   if(tempo == 0) //if desired tempo is 0, stop motor
   {
     A4990Motor::stopMotor();
   }
   else if(dir != lastDir) //if direction is changed, stop and turn
   {
     A4990Motor::stopMotor();
     A4990Motor::setDirection(dir);
     A4990Motor::setTempo(tempo);
     A4990Motor::startMotor();
   }
   else if(dir == lastDir)
   {
     A4990Motor::changeTempo(tempo); //if direction is the same, change tempo
   }
 }

void A4990Motor::drive(float distance, bool dir, uint8_t tempo) //distance in rev, dir 0 =CW - 1=CCW, tempo 0-100
{

  float startPosition = encoder.getPosition(); //gets absolute position in revs
  float netDistance;
  if(dir) {netDistance = distance;}
  else {netDistance = -distance;} //sets netdistance negative if direction is negative
  float desiredPosition = startPosition + netDistance;
  A4990Motor::driveTo(desiredPosition, tempo);
}


void A4990Motor::driveTo(float position, uint8_t tempo)
{
  A4990Motor::setTempo(tempo);

  if (encoder.getPosition() == position) //if position is the same, do nothing
  {
    //do nothing
  }
  else
  {
    int i = 0;
    if (encoder.getPosition() < position) // if position is greater than the current, go in positive direction
    {
      A4990Motor::setDirection(1);
      A4990Motor::startMotor();
      while(encoder.getPosition() < position - _stopDistance)
      {
        //wait
        ++i;

        //if(i > 1000) {i = 0; Serial.println(encoder.getPosition());}
      }
      A4990Motor::regulatedStop(position); //regulated stop starts when position-stopDistance has been reached
    }
    else if(encoder.getPosition() > position) //if position is less than current position, go in negative direction.
    {
      A4990Motor::setDirection(0);
      A4990Motor::startMotor();
      while(encoder.getPosition() > position + _stopDistance)
      {
        //wait
        ++i;
        //if(i > 1000) {i = 0; Serial.println(encoder.getPosition());}
      }
      A4990Motor::regulatedStop(position); //regulated stop starts when position-stopDistance has been reached.
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
  setRealTempo(tempo);
}

void A4990Motor::stopMotor()
{

   bool dir = A4990Motor::getDirection();
   float pwmTempo;
   uint8_t desiredAcceleration = A4990Motor::getAcceleration();
   if(dir) //if positive direction
   {
     pwmTempo = A4990Motor::getRealTempo()*2.55;
     while(pwmTempo > 0) //decelerate
     {
       pwmTempo--;
       analogWrite(inHigh_, pwmTempo);
       delay(desiredAcceleration);
     }
   }
   else //if negative direction
   {
     pwmTempo = 255-A4990Motor::getRealTempo()*2.55;
     while(pwmTempo < 255) //decelerate
     {
       pwmTempo++;
       analogWrite(inLow_, pwmTempo);
       delay(desiredAcceleration);
     }
   }
   analogWrite(inLow_,0);
   analogWrite(inHigh_,255);
   setRealTempo(0);

}

void A4990Motor::regulatedStop(const float & desiredPosition)
{
  float maxTempo = A4990Motor::getTempo()*2.55;
  pid.begin(127, 1, 0);
  pid.setSaturation(-maxTempo, maxTempo);
  float momentaryPosition = encoder.getPosition();
  float tolerance = 0.001;
  float err = desiredPosition - momentaryPosition;
  float pwm;
  int i = 0;
  while(err > tolerance || err < -tolerance) //Sudo::JAKOB fix!
  {
    pwm = pid.output(err);
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
  setRealTempo(0);
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
