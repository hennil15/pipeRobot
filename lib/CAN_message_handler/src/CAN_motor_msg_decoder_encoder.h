#ifndef _CAN_MOTOR_MSG_DECODER_ENCODER_H_
#define _CAN_MOTOR_MSG_DECODER_ENCODER_H_

#include <stdint.h>

struct decoded_motor_CAN_msg
{
  enum metaData
  {
    sender,
    receiver,
    request,
    command,
    stop,
    drive,
    driveToPosition,
    driveDistance
  };
  metaData motorIs; // motorIs = either sender or recipient
  metaData msgIs;   // msgIs = either request or command
  metaData commandIs; //cammandIs = either stop, drive, driveToPosition, driveDistance
  int32_t position;
  uint16_t distance;
  bool direction;
  uint8_t tempo;
};

class CAN_motor_msg_decoder_encoder
{
public:
  CAN_motor_msg_decoder_encoder();
  // decoded_motor_CAN_msg motorMsg;
  // decoded_motor_CAN_msg prepMotorMsg;
  decoded_motor_CAN_msg decodeMsg(const uint8_t * CANBufData);
  void encodeMsg(decoded_motor_CAN_msg msgToEncodeFrom, uint8_t * CANBufToEncode);
};

#endif  //_CAN_MOTOR_MSG_DECODER_ENCODER_H_
