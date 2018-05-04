#include <Arduino.h>
#include <DRV8838Motor.h>
#include <FlexCan.h>
#include <CAN_message_handler.h>


const uint8_t motorPwmPin = 5;
const uint8_t motorDirPin = 0;
const uint8_t encoderPinA = 22;
const uint8_t encoderPinB = 21;
DRV8838Motor motor(motorPwmPin, motorDirPin, 1, encoderPinA, encoderPinB);
const bool fwd = true;
const bool bwd = !fwd;

static CAN_message_t msg;
CAN_message_handler msgHandler;

bool msgForMe()
{
  while(Can0.available())
  {
    Can0.read(msg);
    return msgHandler.msgIsRelevant(msg.id);
  }
  return false;
}

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
  Serial.begin(115200);
  Can0.begin();
  msg.ext = 0;
  msg.len = 8;
  msg.id = 0x000;
  for(int i = 0; i < 8; ++i)
  {
    msg.buf[i] = 0;
  }

  msgHandler.addIdToFilter(msgHandler.CANIds::motor_00);
}

void loop()
{
  if(msgForMe())
  {
    msgHandler.extractCANData(msgHandler.typeOfMsg(msg.id), msg.buf);
    switch (msg.id)
    {
      case msgHandler.CANIds::motor_00:
        //drive or stop
        if(msgIsDrive())
        {
          bool dir;
          if(msgHandler.motorMsg.direction) {dir = fwd;}
          else  {dir = bwd;}
          motor.drive(dir, msgHandler.motorMsg.tempo);
        }
        else if(msgIsStop())
        {
          motor.stopMotor();
        }
        else
        {
          Serial.println("motorMsg was neither to drive or to stop");
        }
        break;
      default:
        //msg not relevant after all
        break;
    }
  }
}
