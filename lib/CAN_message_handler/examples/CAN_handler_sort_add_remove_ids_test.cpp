#include <Arduino.h>
#include <FlexCan.h>
#include <CAN_message_handler.h>

CAN_message_handler msgHandler;

void setup() {
  msgHandler.addIdToFilter(msgHandler.CANIds::motor_00);
  msgHandler.addIdToFilter(msgHandler.CANIds::motor_01);
  Serial.begin(115200);
  delay(3000);
}

uint16_t id = 0x0009;
uint8_t buf[] = {0b10000001, 0, 0, 0, 0, 0, 0, 0b10101011};

void loop() {
  Serial.println(id);
  if(msgHandler.msgIsRelevant(id))
  {
    Serial.print("Msg is relevant. Type = ");
    Serial.println(msgHandler.typeOfMsg(id));
    switch (msgHandler.typeOfMsg(id)) {
      case msgHandler.CANMsgType::motor:
        msgHandler.extractCANData(msgHandler.CANMsgType::motor, buf);
        Serial.print("Motor is "); Serial.print(msgHandler.motorMsg.motorIs); Serial.print("\t");
        Serial.print("Msg is "); Serial.print(msgHandler.motorMsg.msgIs); Serial.print("\t");
        Serial.print("Command is "); Serial.print(msgHandler.motorMsg.commandIs); Serial.print("\t");
        Serial.print("Distance = "); Serial.print(msgHandler.motorMsg.distance); Serial.print("\t");
        Serial.print("Position = "); Serial.print(msgHandler.motorMsg.position); Serial.print("\t");
        Serial.print("Direction = "); Serial.print(msgHandler.motorMsg.direction); Serial.print("\t");
        Serial.print("Tempo = "); Serial.println(msgHandler.motorMsg.tempo);
        break;
      default:
        Serial.println("Msg is not for motor");
        break;
    }
    Serial.println();
  }
  else
  {
    Serial.println("Msg is not relevant\n");
  }
  id++;

  if(id > msgHandler.CANIds::motor_03)
  {
    msgHandler.addIdToFilter(msgHandler.CANIds::motor_05);
  }
  if(id > msgHandler.CANIds::motor_07)
  {
    msgHandler.removeIdToFilter(msgHandler.CANIds::motor_00);
    id = msgHandler.CANIds::global_event_10;
  }

  delay(1000);
}
