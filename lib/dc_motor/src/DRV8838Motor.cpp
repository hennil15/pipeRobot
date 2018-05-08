#include "DRV8838Motor.h"

DRV8838Motor::DRV8838Motor(uint8_t pwmPin,uint8_t dirPin, uint8_t inA, uint8_t inB)
:encoder(inA,inB)
{
  pwmPin_ = pwmPin;
  dirPin_ = dirPin;
  pinMode(pwmPin, OUTPUT);
  pinMode(dirPin, OUTPUT);

  DRV8838Motor::setAcceleration(15); //sets default acceleration for DRV8838 Motor
  DRV8838Motor::setTempo(0); //sets initial tempo to be 0
  DRV8838Motor::setDirection(1); //sets initial direction to be 1.

  encoder.begin();
}


void DRV8838Motor::drive(bool dir, uint8_t tempo)
{
   bool lastDir = DRV8838Motor::getDirection();
  //
   if(tempo == 0)
   {
     DRV8838Motor::stopMotor();
   }
   else if(dir != lastDir)
   {
     DRV8838Motor::stopMotor();
     DRV8838Motor::setDirection(dir);
     DRV8838Motor::setTempo(tempo);
     DRV8838Motor::startMotor();
   }
   else if(dir == lastDir)
   {
     DRV8838Motor::changeTempo(tempo);
   }
}

void DRV8838Motor::drive(float distance, bool dir, uint8_t tempo) //distance in rev, dir 0 =CW - 1=CCW, tempo 0-100
{

  float startPosition = encoder.getPosition(); //gets position in revs
  float netDistance;
  if(dir) {netDistance = distance;} //sets distance to move positive if direction is positive
  else {netDistance = -distance;} //sets distance to move negative if direction is negative
  float desiredPosition = startPosition + netDistance;
  DRV8838Motor::driveTo(desiredPosition, tempo);

}


void DRV8838Motor::driveTo(float position, uint8_t tempo)
{
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
   float pwmTempo = getRealTempo()*2.55;
   uint8_t desiredAcceleration = DRV8838Motor::getAcceleration();
   while(pwmTempo > 0) //decellerates at a rate set by acceleration variable
   {
     pwmTempo--;
     analogWrite(pwmPin_,pwmTempo);
     delay(desiredAcceleration);
   }
   setRealTempo(0); //sets real tempo equal to 0 (stopped)
   analogWrite(pwmPin_,0); //for redundancy
}

void DRV8838Motor::startMotor()
{
  float pwmTempo = 0;
  float desiredTempo = DRV8838Motor::getTempo()*2.55;
  uint8_t desiredAcceleration = DRV8838Motor::getAcceleration();

  while(pwmTempo < desiredTempo) //accelerates to set tempo
  {
    pwmTempo++;
    analogWrite(pwmPin_,pwmTempo);
    delay(desiredAcceleration);
  }
  analogWrite(pwmPin_,desiredTempo); //for redundancy
  setRealTempo(desiredTempo);
}


void DRV8838Motor::regulatedStop(const float & desiredPosition)
{
  float maxTempo = DRV8838Motor::getTempo()*2.55;
  pid.begin(127,1,0); //initializes PID with inputs P,I,D
  pid.setSaturation(-maxTempo,maxTempo); //sets speed limits for the PID
  float momentaryPosition = encoder.getPosition(); //gets the current absolute position
  float tolerance = 0.001; //sets tolerance for error in revs
  float err = desiredPosition - momentaryPosition;

  float pwm;

  int i = 0;
  while(err > tolerance || err < -tolerance) //JAKOB! Argumentet må være steady state!
  {
    pwm = pid.output(err); //sets pwm = output from PID controller

    if(pwm>=0) {analogWrite(pwmPin_, pwm);}
    else
    {
      //if negative, the direction has to be changed before writing absolute value to pwmPin.
      digitalWrite(dirPin_,0);
      pwm = -pwm;
      analogWrite(pwmPin_, pwm);
    }

    err = desiredPosition - encoder.getPosition();
    ++i;

    //*****for tuning/debugging****:
    /*if(i>1000)
    {
      Serial.print(desiredPosition); Serial.print("\t");
      Serial.println(encoder.getPosition());
      i = 0;
    }*/
}
pid.loopDone(); //ends PID control
analogWrite(pwmPin_,0); //sets tempo to 0.
setRealTempo(0);
}

void DRV8838Motor::changeTempo(uint8_t tempo)
{
  DRV8838Motor::setTempo(tempo);
  float pwmTempo = tempo*2.55;
  analogWrite(pwmPin_,pwmTempo);
}
