#include "Remote_controller_CAN_messages.h"
#include <CAN_message_handler.h>

Remote_controller_CAN_messages::Remote_controller_CAN_messages()
{}
void Remote_controller_CAN_messages::stopDrivingFront(uint16_t * id, uint8_t * buf)
{
  stop(buf);
  *id = CANHandler.CANIds::motor_00;
}
void Remote_controller_CAN_messages::stopDrivingRear(uint16_t * id, uint8_t * buf)
{
  stop(buf);
  *id = CANHandler.CANIds::motor_03;
}
void Remote_controller_CAN_messages::stopPitch(uint16_t * id, uint8_t * buf)
{
  stop(buf);
  *id = CANHandler.CANIds::motor_01;
}
void Remote_controller_CAN_messages::stopYaw(uint16_t * id, uint8_t * buf)
{
  stop(buf);
  *id = CANHandler.CANIds::motor_02;
}
void Remote_controller_CAN_messages::driveFront(bool dir, uint8_t tempo, uint16_t * id, uint8_t * buf)
{
  drive(dir, tempo, buf);
  *id = CANHandler.CANIds::motor_00;
}
void Remote_controller_CAN_messages::driveRear(bool dir, uint8_t tempo, uint16_t * id, uint8_t * buf)
{
  drive(dir, tempo, buf);
  *id = CANHandler.CANIds::motor_03;
}
void Remote_controller_CAN_messages::drivePitch(bool dir, uint8_t tempo, uint16_t * id, uint8_t * buf)
{
  drive(dir, tempo, buf);
  *id = CANHandler.CANIds::motor_01;
}
void Remote_controller_CAN_messages::driveYaw(bool dir, uint8_t tempo, uint16_t * id, uint8_t * buf)
{
  drive(dir, tempo, buf);
  *id = CANHandler.CANIds::motor_02;
}

void Remote_controller_CAN_messages::stop(uint8_t * buf)
{
  CANHandler.motorMsg.motorIs = CANHandler.motorMsg.metaData::receiver;
  CANHandler.motorMsg.msgIs = CANHandler.motorMsg.metaData::command;
  CANHandler.motorMsg.commandIs = CANHandler.motorMsg.metaData::stop;
  CANHandler.encodeCANData(CANHandler.CANMsgType::motor, buf);
}
void Remote_controller_CAN_messages::drive(bool dir, uint8_t tempo, uint8_t * buf)
{
  CANHandler.motorMsg.motorIs = CANHandler.motorMsg.metaData::receiver;
  CANHandler.motorMsg.msgIs = CANHandler.motorMsg.metaData::command;
  CANHandler.motorMsg.commandIs = CANHandler.motorMsg.metaData::drive;
  CANHandler.motorMsg.direction = dir;
  CANHandler.motorMsg.tempo = tempo;
  CANHandler.encodeCANData(CANHandler.CANMsgType::motor, buf);
}
