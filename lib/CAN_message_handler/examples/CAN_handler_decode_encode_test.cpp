#include <Arduino.h>
#include <FlexCan.h>
#include <CAN_message_handler.h>

CAN_message_handler msgHandler;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
}

uint8_t buf[] = {0b10000001, 2, 3, 0, 0, 0, 40, 0b10101011};
void loop() {
  msgHandler.extractCANData(msgHandler.CANMsgType::motor, buf);
  Serial.print("Motor is "); Serial.print(msgHandler.motorMsg.motorIs); Serial.print("\t");
  Serial.print("Msg is "); Serial.print(msgHandler.motorMsg.msgIs); Serial.print("\t");
  Serial.print("Command is "); Serial.print(msgHandler.motorMsg.commandIs); Serial.print("\t");
  Serial.print("Distance = "); Serial.print(msgHandler.motorMsg.distance); Serial.print("\t");
  Serial.print("Position = "); Serial.print(msgHandler.motorMsg.position); Serial.print("\t");
  Serial.print("Direction = "); Serial.print(msgHandler.motorMsg.direction); Serial.print("\t");
  Serial.print("Tempo = "); Serial.println(msgHandler.motorMsg.tempo);

  delay(1000);

  if(msgHandler.motorMsg.motorIs == msgHandler.motorMsg.metaData::receiver)  {msgHandler.motorMsg.motorIs = msgHandler.motorMsg.metaData::sender;}
  else  {msgHandler.motorMsg.motorIs = msgHandler.motorMsg.metaData::receiver;}
  if(msgHandler.motorMsg.msgIs == msgHandler.motorMsg.metaData::command) {msgHandler.motorMsg.msgIs = msgHandler.motorMsg.metaData::request;}
  else  {msgHandler.motorMsg.msgIs = msgHandler.motorMsg.metaData::command;}
  msgHandler.motorMsg.commandIs = msgHandler.motorMsg.metaData::command;
  msgHandler.motorMsg.distance++;
  msgHandler.motorMsg.position++;
  msgHandler.motorMsg.direction = !msgHandler.motorMsg.direction;
  msgHandler.motorMsg.tempo++;
  if(msgHandler.motorMsg.tempo == 100) {msgHandler.motorMsg.tempo = 0;}

  msgHandler.encodeCANData(msgHandler.CANMsgType::motor, buf);
  Serial.print("Buf = ");
  for(uint8_t i = 0; i < 8; ++i)
  {
    Serial.print(buf[i], BIN);
    Serial.print("\t");
  }
  Serial.println();

  delay(1000);
}
