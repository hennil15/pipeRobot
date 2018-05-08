#ifndef _REMOTE_CONTROLLER_CAN_MESSAGES_H_
#define _REMOTE_CONTROLLER_CAN_MESSAGES_H_

#include <stdint.h>
#include <CAN_message_handler.h>

class Remote_controller_CAN_messages
{
public:
  Remote_controller_CAN_messages();
  CAN_message_handler CANHandler;
  void stopDrivingFront(uint16_t * id, uint8_t * buf);
  void stopDrivingRear(uint16_t * id, uint8_t * buf);
  void stopPitch(uint16_t * id, uint8_t * buf);
  void stopYaw(uint16_t * id, uint8_t * buf);
  void driveFront(bool dir, uint8_t tempo, uint16_t * id, uint8_t * buf);
  void driveRear(bool dir, uint8_t tempo, uint16_t * id, uint8_t * buf);
  void drivePitch(bool dir, uint8_t tempo, uint16_t * id, uint8_t * buf);
  void driveYaw(bool dir, uint8_t tempo, uint16_t * id, uint8_t * buf);
private:
  void stop(uint8_t * buf);
  void drive(bool dir, uint8_t tempo, uint8_t * buf);
};

#endif  //_REMOTE_CONTROLLER_CAN_MESSAGES_H_
