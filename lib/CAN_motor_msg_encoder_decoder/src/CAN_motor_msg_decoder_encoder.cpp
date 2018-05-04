#include "CAN_motor_msg_decoder_encoder.h"

CAN_motor_msg_decoder_encoder::CAN_motor_msg_decoder_encoder()
{
}

decoded_motor_CAN_msg CAN_motor_msg_decoder_encoder::decodeMsg(const uint8_t * CANBufData)
{
  decoded_motor_CAN_msg tempMsg;
  if(0b10000000 & CANBufData[0])  {tempMsg.motorIs = tempMsg.metaData::sender;}
  else                            {tempMsg.motorIs = tempMsg.metaData::receiver;}

  if(0b01000000 & CANBufData[0])  {tempMsg.msgIs = tempMsg.metaData::request;}
  else                            {tempMsg.msgIs = tempMsg.metaData::command;}

  switch (0b00000011 & CANBufData[0])
  {
    case 0: tempMsg.commandIs = tempMsg.metaData::stop;  break;
    case 1: tempMsg.commandIs = tempMsg.metaData::drive; break;
    case 2: tempMsg.commandIs = tempMsg.metaData::driveToPosition; break;
    case 3: tempMsg.commandIs = tempMsg.metaData::driveDistance;   break;
  }

  tempMsg.position =  static_cast<int32_t>(CANBufData[3]) << 24 |
                      static_cast<int32_t>(CANBufData[4]) << 16 |
                      static_cast<int32_t>(CANBufData[5]) << 8  |
                      static_cast<int32_t>(CANBufData[6]);

  tempMsg.distance =  (static_cast<uint16_t>(CANBufData[1]) << 8) |
                      static_cast<uint16_t>(CANBufData[2]);

  if(((CANBufData[7]) & (1<<7)) == (1<<7))  {tempMsg.direction = true;}
  else                                      {tempMsg.direction = false;}

  tempMsg.tempo = CANBufData[7] & 0b01111111;
  if(tempMsg.tempo > 100) {tempMsg.tempo = 100;}

  return tempMsg;
}

void CAN_motor_msg_decoder_encoder::encodeMsg(decoded_motor_CAN_msg msgToEncodeFrom, uint8_t * CANBufToEncode)
{
  for(uint8_t i = 0; i < 8; ++i)
  {
    CANBufToEncode[i] = 0;
  }
  switch (msgToEncodeFrom.motorIs) {
    case msgToEncodeFrom.metaData::sender: CANBufToEncode[0] |= (1<<7);     break;
    case msgToEncodeFrom.metaData::receiver: CANBufToEncode[0] &= !(1<<7);  break;
    default: break;
  }
  switch (msgToEncodeFrom.msgIs) {
    case msgToEncodeFrom.metaData::request: CANBufToEncode[0] |= (1<<6); break;
    case msgToEncodeFrom.metaData::command: CANBufToEncode[0] &= !(1<<6); break;
    default: break;
  }
  switch (msgToEncodeFrom.commandIs) {
    case msgToEncodeFrom.metaData::stop: CANBufToEncode[0] &= !(0b00000011); break;
    case msgToEncodeFrom.metaData::drive: CANBufToEncode[0] |= 1; break;
    case msgToEncodeFrom.metaData::driveToPosition: CANBufToEncode[0] |= 2; break;
    case msgToEncodeFrom.metaData::driveDistance: CANBufToEncode[0] |= 3; break;
    default: break;
  }

  uint16_t distH = (msgToEncodeFrom.distance >> 7) & (0x00FF);
  uint16_t distL = msgToEncodeFrom.distance & (0x00FF);
  CANBufToEncode[1] = static_cast<uint8_t>(distH);
  CANBufToEncode[2] = static_cast<uint8_t>(distL);

  int32_t pos31_24  =  (msgToEncodeFrom.position >> 23)  & (0x000000FF);
  int32_t pos23_16  =  (msgToEncodeFrom.position >> 15)  & (0x000000FF);
  int32_t pos15_8   =  (msgToEncodeFrom.position >> 7)   & (0x000000FF);
  int32_t pos7_0    =  (msgToEncodeFrom.position) & (0x000000FF);
  CANBufToEncode[3] = static_cast<uint8_t>(pos31_24);
  CANBufToEncode[4] = static_cast<uint8_t>(pos23_16);
  CANBufToEncode[5] = static_cast<uint8_t>(pos15_8);
  CANBufToEncode[6] = static_cast<uint8_t>(pos7_0);

  uint8_t dir = (static_cast<uint8_t>(msgToEncodeFrom.direction) << 7) & (0b10000000);
  CANBufToEncode[7] = dir | msgToEncodeFrom.tempo;
}
