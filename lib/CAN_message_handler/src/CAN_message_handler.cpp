#include "CAN_message_handler.h"

CAN_message_handler::CAN_message_handler()
{
  CANIds alwaysFilter[] = {
    global_event_00,
    global_event_01,
    global_event_02,
    global_event_03,
    global_event_04,
    global_event_05,
    global_event_06,
    global_event_07,
    global_event_08,
    global_event_09,
    global_event_10,
    global_event_11,
    global_event_12,
    global_event_13,
    global_event_14,
    global_event_15
  };
  for(uint16_t i = 0; i < 16; ++i)
  {
    _idsToFilter[i] = alwaysFilter[i];
  }
  _widthOfArray = 16; //keeps trach of how many IDs are stored in the _idsToFilter[] array
}

void CAN_message_handler::addIdToFilter(CANIds id)
{
  bool addId = true;
  for(uint8_t i = 0; i < _widthOfArray; ++i)  //checks if id is already being filtered
  {
    if(id == _idsToFilter[i]) {
      addId = false;
    }
  }
  if(addId){
    _idsToFilter[_widthOfArray] = id;   // because arrays start at 0, position _widthOfArray is a free space
    _widthOfArray++;
  }
}
void CAN_message_handler::removeIdToFilter(CANIds id)
{
  uint16_t idPosition;
  for(uint16_t i = 16; i < _widthOfArray; ++i)
  {
    if(_idsToFilter[i] == id)
    {
      _idsToFilter[i] = CANIds::dummy;  // the dummy Id can never occur on the can bus. Assigning it to the array is redundant however
      idPosition = i;
    }
  }
  if(idPosition < --_widthOfArray)  // if the removed Id was not at the end of the array, the elements that comes after it are moved on space to the left
  {
    for (uint16_t i = idPosition; i < _widthOfArray; ++i)
    {
      _idsToFilter[i] = _idsToFilter[(i + 1)];
    }
  }
}

//checks if the incoming msg id corresponds to any of the relevant ones
bool CAN_message_handler::msgIsRelevant(uint16_t msgId)
{
  for(uint16_t i = 0; i < _widthOfArray; ++i)
  {
    if(_idsToFilter[i] == msgId)
    {return true;}
  }
  return false;
}

//identifies and returnes the message type based on its Id
CAN_message_handler::CANMsgType CAN_message_handler::typeOfMsg(uint16_t msgId)
{
  if(msgId >= CANIds::global_event_00 && msgId <= CANIds::global_event_15){
    return CANMsgType::global_event;
  }
  else if(msgId >= CANIds::motor_00 && msgId <= CANIds::motor_15){
    return CANMsgType::motor;
  }
  else if(msgId >= CANIds::imu_00 && msgId <= CANIds::imu_15){
    return CANMsgType::imu;
  }
  return CANMsgType::noType;
}

//extracts the CAN data by calling the corresponding msg type decoder and assigning it in the appropriate struct
void CAN_message_handler::extractCANData(CANMsgType msgType, const uint8_t * CANBuf)
{
  switch (msgType) {
    case motor:
      motorMsg = motorMsgHandler.decodeMsg(CANBuf);
      break;
    // case imu:
    //   imuMsg = imuMsgHandler.decodeMsg(CANBuf);
    //   break;
    // case global_event:
    default: break;
  }
}

//encode the content of a msg-type-struct into an array that can be sent on the CAN-bus
void CAN_message_handler::encodeCANData(CANMsgType msgType, uint8_t * CANBuf)
{
  switch (msgType) {
    case motor:
      motorMsgHandler.encodeMsg(motorMsg, CANBuf);
      break;
    default: break;
  }
}
