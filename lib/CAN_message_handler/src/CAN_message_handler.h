#ifndef CAN_MESSAGE_HANDLER_H_
#define CAN_MESSAGE_HANDLER_H_

#include <stdint.h>
#include <CAN_motor_msg_decoder_encoder.h>

class CAN_message_handler
{
public:
  decoded_motor_CAN_msg motorMsg;

  enum CANIds : uint16_t
  {
    dummy = 0xFFFF,   // invalid CAN-id (because higher than 2048). Used in _idsToFilter[]
    global_event_00	=	0x0000	,
    global_event_01	=	0x0001	,
    global_event_02	=	0x0002	,
    global_event_03	=	0x0003	,
    global_event_04	=	0x0004	,
    global_event_05	=	0x0005	,
    global_event_06	=	0x0006	,
    global_event_07	=	0x0007	,
    global_event_08	=	0x0008	,
    global_event_09	=	0x0009	,
    global_event_10	=	0x000A	,
    global_event_11	=	0x000B	,
    global_event_12	=	0x000C	,
    global_event_13	=	0x000D	,
    global_event_14	=	0x000E	,
    global_event_15	=	0x000F	,
    motor_00	=	0x0010	,
    motor_01	=	0x0011	,
    motor_02	=	0x0012	,
    motor_03	=	0x0013	,
    motor_04	=	0x0014	,
    motor_05	=	0x0015	,
    motor_06	=	0x0016	,
    motor_07	=	0x0017	,
    motor_08	=	0x0018	,
    motor_09	=	0x0019	,
    motor_10	=	0x001A	,
    motor_11	=	0x001B	,
    motor_12	=	0x001C	,
    motor_13	=	0x001D	,
    motor_14	=	0x001E	,
    motor_15	=	0x001F	,
    imu_00	=	0x0020	,
    imu_01	=	0x0021	,
    imu_02	=	0x0022	,
    imu_03	=	0x0023	,
    imu_04	=	0x0024	,
    imu_05	=	0x0025	,
    imu_06	=	0x0026	,
    imu_07	=	0x0027	,
    imu_08	=	0x0028	,
    imu_09	=	0x0029	,
    imu_10	=	0x002A	,
    imu_11	=	0x002B	,
    imu_12	=	0x002C	,
    imu_13	=	0x002D	,
    imu_14	=	0x002E	,
    imu_15	=	0x002F
  };
  enum CANMsgType : uint8_t
  {
    global_event,
    motor,
    imu,
    current_sensor,
    noType
  };

  CAN_message_handler();
  void addIdToFilter(CANIds id);
  void removeIdToFilter(CANIds id);
  bool msgIsRelevant(uint16_t msgId); // checks msg id to see if it is relevant for the microcontroller


  CANMsgType typeOfMsg(uint16_t msgId);
  void extractCANData(CANMsgType msgType, const uint8_t * CANBuf);
  void encodeCANData(CANMsgType msgType, uint8_t * CANBuf);


private:
  CAN_motor_msg_decoder_encoder motorMsgHandler;
  CANIds _idsToFilter[100];
  uint16_t _widthOfArray;
  // const CANIds _idsToAlwayFilter[] =
  // {
  //   global_event_00,
  //   global_event_01,
  //   global_event_02,
  //   global_event_03,
  //   global_event_04,
  //   global_event_05,
  //   global_event_06,
  //   global_event_07,
  //   global_event_08,
  //   global_event_09,
  //   global_event_10,
  //   global_event_11,
  //   global_event_12,
  //   global_event_13,
  //   global_event_14,
  //   global_event_15
  // };
};

#endif //CAN_MESSAGE_HANDLER_H_
