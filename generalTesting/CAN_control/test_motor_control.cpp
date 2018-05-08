#include <Arduino.h>
#include <FlexCan.h>
#include <CAN_message_handler.h>
#include <A4990Motor.h>

static CAN_message_t msg;
CAN_message_handler msgHandler;
A4990Motor motor(21, 20, 1, 0, 1);

void setup() {
  msgHandler.addIdToFilter(msgHandler.CANIds::motor_00);
  Serial.begin(115200);
  msg.ext = 0;
  msg.len = 8;
  delay(5000);
}

bool relevantCAN()
{
  while(Can0.available())
  {
    Can0.read(msg);
    if(msgHandler.msgIsRelevant(msg.id))
    {
      return true;
    }
    else
    {
      return false;
    }
  }
  return false;
}

void loop() {
  if(relevantCAN())
  {
    Serial.print("Msg is relevant. Type = ");
    Serial.println(msgHandler.typeOfMsg(msg.id));
    switch (msgHandler.typeOfMsg(msg.id)) {
      case msgHandler.CANMsgType::motor:
        msgHandler.extractCANData(msgHandler.CANMsgType::motor, msg.buf);
        Serial.print("Motor is "); Serial.print(msgHandler.motorMsg.motorIs); Serial.print("\t");
        Serial.print("Msg is "); Serial.print(msgHandler.motorMsg.msgIs); Serial.print("\t");
        Serial.print("Command is "); Serial.print(msgHandler.motorMsg.commandIs); Serial.print("\t");
        Serial.print("Distance = "); Serial.print(msgHandler.motorMsg.distance); Serial.print("\t");
        Serial.print("Position = "); Serial.print(msgHandler.motorMsg.position); Serial.print("\t");
        Serial.print("Direction = "); Serial.print(msgHandler.motorMsg.direction); Serial.print("\t");
        Serial.print("Tempo = "); Serial.println(msgHandler.motorMsg.tempo);
        switch (msgHandler.motorMsg.motorIs) {
          case msgHandler.motorMsg.metaData::sender:
            Serial.println("Do nothing");
            break;
          case msgHandler.motorMsg.metaData::receiver:
            switch (msgHandler.motorMsg.msgIs) {
              case msgHandler.motorMsg.metaData::request:
                Serial.println("Do nothing");
                break;
              case msgHandler.motorMsg.metaData::command:
                switch (msgHandler.motorMsg.commandIs)
                {
                  case msgHandler.motorMsg.metaData::drive:
                    motor.drive(msgHandler.motorMsg.direction, msgHandler.motorMsg.tempo);
                    break;
                  default: break;
                }
                break;
              default: break;
            }
          default: break;
        }
        break;
      default:
        Serial.println("Msg is not for motor");
        break;
    }
    Serial.println();
  }
}
