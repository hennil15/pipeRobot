#include <Arduino.h>
#include <A4990Motor.h>
#include <FlexCan.h>
#include <CAN_message_handler.h>


const uint8_t motorPitchPin1 = 6;
const uint8_t motorPitchPin2 = 5;
const uint8_t encoderPitchPinA = 2;
const uint8_t encoderPitchPinB = 1;
const uint8_t motorYawPin1 = 21;
const uint8_t motorYawPin2 = 9;
const uint8_t encoderYawPinA = 23;
const uint8_t encoderYawPinB = 22;
A4990Motor motorPitch(motorPitchPin1, motorPitchPin2, encoderPitchPinA, encoderPitchPinB);
A4990Motor motorYaw(motorYawPin1, motorYawPin2, encoderYawPinA, encoderYawPinB);
const bool posPitch = true; //what way to rotate the motor to get positive pitch
const bool negPitch = !posPitch;
const bool posYaw = true;//what way to rotate the motor to get positive yaw
const bool negYaw = !posYaw;

static CAN_message_t msg;
CAN_message_handler msgHandler;

bool msgForMe()//Checks if an incoming CAN-message is relevant
{
  while(Can0.available())
  {
    Can0.read(msg);
    return msgHandler.msgIsRelevant(msg.id);
  }
  return false;
}

/*The bending module only has implementations for two different messages; drive and stop
the two next functions checks if the message is one or the other*/
bool msgIsDrive()
{
  return  (msgHandler.motorMsg.motorIs == msgHandler.motorMsg.metaData::receiver) &&
          (msgHandler.motorMsg.msgIs == msgHandler.motorMsg.metaData::command) &&
          (msgHandler.motorMsg.commandIs == msgHandler.motorMsg.metaData::drive);
}

bool msgIsStop()
{
  return  (msgHandler.motorMsg.motorIs == msgHandler.motorMsg.metaData::receiver) &&
          (msgHandler.motorMsg.msgIs == msgHandler.motorMsg.metaData::command) &&
          (msgHandler.motorMsg.commandIs == msgHandler.motorMsg.metaData::stop);
}

void setup()
{
  //Serial.begin(115200);

  //setup for CAN-bus
  Can0.begin();
  msg.ext = 0;
  msg.len = 8;
  msg.id = 0x000;
  for(int i = 0; i < 8; ++i)
  {
    msg.buf[i] = 0;
  }

  msgHandler.addIdToFilter(msgHandler.CANIds::motor_01);  //this motor is for pitch
  msgHandler.addIdToFilter(msgHandler.CANIds::motor_02);  //this motor is for yaw
}

void loop()
{
  if(msgForMe())
  {
    msgHandler.extractCANData(msgHandler.typeOfMsg(msg.id), msg.buf);
    switch (msg.id)
    {
      case msgHandler.CANIds::motor_01:
        //drive or stop pitch
        if(msgIsDrive())
        {
          bool dir;
          if(msgHandler.motorMsg.direction) {dir = posPitch;}
          else  {dir = negPitch;}
          motorPitch.drive(dir, msgHandler.motorMsg.tempo);
        }
        else if(msgIsStop())
        {
          motorPitch.stopMotor();
        }
        else
        {
          //Serial.println("motorMsg to pitchMotor was neither to drive or to stop");
        }
        break;
      case msgHandler.CANIds::motor_02:
        //drive or stop yaw
        if(msgIsDrive())
        {
          bool dir;
          if(msgHandler.motorMsg.direction) {dir = posYaw;}
          else  {dir = negYaw;}
          motorYaw.drive(dir, msgHandler.motorMsg.tempo);
        }
        else if(msgIsStop())
        {
          motorYaw.stopMotor();
        }
        else
        {
          //Serial.println("motorMsg to yawMotor was neither to drive or to stop");
        }
        break;
      default:
        //msg not relevant after all
        break;
    }
  }
}
