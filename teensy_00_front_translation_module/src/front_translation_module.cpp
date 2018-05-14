#include <Arduino.h>
#include <DRV8838Motor.h>
#include <FlexCan.h>
#include <CAN_message_handler.h>


const uint8_t motorPwmPin = 5;
const uint8_t motorDirPin = 0;
const uint8_t encoderPinA = 22;
const uint8_t encoderPinB = 21;
DRV8838Motor motor(motorPwmPin, motorDirPin, encoderPinA, encoderPinB);
const bool fwd = true;  //the direction the motor turns in order to drive forward
const bool bwd = !fwd;

static CAN_message_t msg;
CAN_message_handler msgHandler;

bool msgForMe() //Checks if an incoming CAN-message is relevant
{
  while(Can0.available())
  {
    Can0.read(msg);
    return msgHandler.msgIsRelevant(msg.id);
  }
  return false;
}

/*The translation modules only has implementations for two different messages; drive and stop
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

  msgHandler.addIdToFilter(msgHandler.CANIds::motor_00); //adds motor_00 as a relavant CAN-id
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
          //Serial.println("motorMsg was neither to drive or to stop");
        }
        break;
      default:
        //msg not relevant after all
        break;
    }
  }
}
