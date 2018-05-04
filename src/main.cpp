#include <Arduino.h>
#include <FlexCAN.h>
#include <A4990Motor.h>
#include <Remote_controller_CAN_messages.h>

static CAN_message_t msg;
Remote_controller_CAN_messages prepMsg;

const uint8_t bendTempo = 100;
const uint8_t driveTempo = 100;
uint16_t tempId;

//pins for buttons
const uint8_t bUp = 23;//21;   // negative pitch
const uint8_t bDown = 22;//20; // positive pitch
const uint8_t bRight = 21;//18;// negatice yaw
const uint8_t bLeft = 20;//19;  // positive yaw
const uint8_t bForward =19;//23;// drive forward
const uint8_t bBackward = 18;//22;//drive backwards

volatile unsigned long last_micros1;
volatile unsigned long last_micros2;
volatile unsigned long last_micros3;
volatile unsigned long last_micros4;
volatile unsigned long last_micros5;
volatile unsigned long last_micros6;
volatile bool sUp = LOW;
volatile bool sDown = LOW;
volatile bool sRight = LOW;
volatile bool sLeft = LOW;
volatile bool sForward = LOW;
volatile bool sBackward = LOW;
const long debouncing_time = 200; //time in milliseconds

unsigned long lastPoll = 0;
const unsigned long pollInterval = 200; //time in milliseconds

void isrUp()
{
  if((long)(micros() - last_micros1) >= debouncing_time * 1000) {
  sUp = 1;
  last_micros1 = micros();
  }
}

void isrDown()
{
  if((long)(micros() - last_micros2) >= debouncing_time * 1000) {
  sDown = 1;
  last_micros2 = micros();
  }
}

void isrRight()
{
  if((long)(micros() - last_micros3) >= debouncing_time * 1000) {
  sRight = 1;
  last_micros3 = micros();
  }
}

void isrLeft()
{
  if((long)(micros() - last_micros4) >= debouncing_time * 1000) {
  sLeft = 1;
  last_micros4 = micros();
  }
}

void isrForward()
{
  if((long)(micros() - last_micros5) >= debouncing_time * 1000) {
  sForward = 1;
  last_micros5 = micros();
  }
}

void isrBackward()
{
  if((long)(micros() - last_micros6) >= debouncing_time * 1000) {
  sBackward = 1;
  last_micros6 = micros();
  }
}

void doubleCheckButtonState()
{
  if((millis() - lastPoll) > pollInterval)
  {
    lastPoll = millis();
    if(digitalRead(bUp) && digitalRead(bDown))
    {
      Serial.println("Stopping pitch");
      prepMsg.stopPitch(&tempId, msg.buf);
      msg.id = static_cast<uint32_t>(tempId);
      Can0.write(msg);
    }
    if(digitalRead(bRight) && digitalRead(bLeft))
    {
      Serial.println("Stopping yaw");
      prepMsg.stopYaw(&tempId, msg.buf);
      msg.id = static_cast<uint32_t>(tempId);
      Can0.write(msg);
    }
    if(digitalRead(bForward) && digitalRead(bBackward))
    {
      Serial.println("Stopping");
      prepMsg.stopDrivingFront(&tempId, msg.buf);
      msg.id = static_cast<uint32_t>(tempId);
      Can0.write(msg);
      prepMsg.stopDrivingRear(&tempId, msg.buf);
      msg.id = static_cast<uint32_t>(tempId);
      Can0.write(msg);
    }
  }
}

void setup()
{
  Serial.begin(115200);
  Can0.begin();
  msg.ext = 0;
  msg.len = 8;
  msg.id = 0x000;
  for(int i = 0; i < 8; ++i)
  {
    msg.buf[i] = 0;
  }

  pinMode(bUp, INPUT_PULLUP);
  pinMode(bDown, INPUT_PULLUP);
  pinMode(bRight, INPUT_PULLUP);
  pinMode(bLeft, INPUT_PULLUP);
  pinMode(bForward, INPUT_PULLUP);
  pinMode(bBackward, INPUT_PULLUP);

  digitalWrite(bUp,HIGH);
  digitalWrite(bDown,HIGH);
  digitalWrite(bRight,HIGH);
  digitalWrite(bLeft,HIGH);
  digitalWrite(bForward,HIGH);
  digitalWrite(bBackward,HIGH);

  attachInterrupt(digitalPinToInterrupt(bUp), isrUp , CHANGE);
  attachInterrupt(digitalPinToInterrupt(bDown),isrDown,CHANGE);
  attachInterrupt(digitalPinToInterrupt(bRight),isrRight,CHANGE);
  attachInterrupt(digitalPinToInterrupt(bLeft),isrLeft,CHANGE);
  attachInterrupt(digitalPinToInterrupt(bForward),isrForward,CHANGE);
  attachInterrupt(digitalPinToInterrupt(bBackward),isrBackward,CHANGE);
}

void loop()
{
  if (sUp == true)  // aka button event has occured
  {
    if(digitalRead(bUp))  // button is not pressed
    {
      Serial.println("send CAN msg stop pitch");
      prepMsg.stopPitch(&tempId, msg.buf);
      msg.id = static_cast<uint32_t>(tempId);
      Can0.write(msg);
    }
    else  // button is pressed
    {
      Serial.println("send CAN msg drive motor pich negative dir");
      prepMsg.drivePitch(0, bendTempo, &tempId, msg.buf);
      msg.id = static_cast<uint32_t>(tempId);
      Can0.write(msg);
    }
    sUp = false;  //clear flag
  }

  if (sDown == true)  // aka button event has occured
  {
    if(digitalRead(bDown))  // button is not pressed
    {
      Serial.println("send CAN msg stop pitch");
      prepMsg.stopPitch(&tempId, msg.buf);
      msg.id = static_cast<uint32_t>(tempId);
      Can0.write(msg);
    }
    else
    {
      Serial.println("send CAN msg drive motor pich positive dir");
      prepMsg.drivePitch(1, bendTempo, &tempId, msg.buf);
      msg.id = static_cast<uint32_t>(tempId);
      Can0.write(msg);
    }
    sDown = 0;
  }

  if (sRight == true) // aka button event has occured
  {
    if(digitalRead(bRight)) // button is not pressed
    {
      Serial.println("send CAN msg stop yaw");
      prepMsg.stopYaw(&tempId, msg.buf);
      msg.id = static_cast<uint32_t>(tempId);
      Can0.write(msg);
    }
    else
    {
      Serial.println("send CAN msg drive motor yaw negative dir");
      prepMsg.driveYaw(0, bendTempo, &tempId, msg.buf);
      msg.id = static_cast<uint32_t>(tempId);
      Can0.write(msg);
    }
    sRight = 0;
  }

  if (sLeft == true)
  {
    if(digitalRead(bLeft))  // button is not pressed
    {
      Serial.println("send CAN msg stop yaw");
      prepMsg.stopYaw(&tempId, msg.buf);
      msg.id = static_cast<uint32_t>(tempId);
      Can0.write(msg);
    }
    else
    {
      Serial.println("send CAN msg drive motor yaw positive dir");
      prepMsg.driveYaw(1, bendTempo, &tempId, msg.buf);
      msg.id = static_cast<uint32_t>(tempId);
      Can0.write(msg);
    }
    sLeft = 0;
  }

  if (sForward == true)
  {
    if(digitalRead(bForward))   // button is not pressed
    {
      Serial.println("send CAN msg stop fwd");
      prepMsg.stopDrivingFront(&tempId, msg.buf);
      msg.id = static_cast<uint32_t>(tempId);
      Can0.write(msg);
      prepMsg.stopDrivingRear(&tempId, msg.buf);
      msg.id = static_cast<uint32_t>(tempId);
      Can0.write(msg);
    }
    else
    {
      Serial.println("send CAN msg drive forward");
      prepMsg.driveFront(1, driveTempo, &tempId, msg.buf);
      msg.id = static_cast<uint32_t>(tempId);
      Can0.write(msg);
      prepMsg.driveRear(1, driveTempo, &tempId, msg.buf);
      msg.id = static_cast<uint32_t>(tempId);
      Can0.write(msg);
    }
    sForward = 0;
  }

  if (sBackward == true)
  {
    if(digitalRead(bBackward))   // button is not pressed
    {
      Serial.println("send CAN msg stop bwd");
      prepMsg.stopDrivingFront(&tempId, msg.buf);
      msg.id = static_cast<uint32_t>(tempId);
      Can0.write(msg);
      prepMsg.stopDrivingRear(&tempId, msg.buf);
      msg.id = static_cast<uint32_t>(tempId);
      Can0.write(msg);
    }
    else
    {
      Serial.println("send CAN msg drive backward");
      prepMsg.driveFront(0, driveTempo, &tempId, msg.buf);
      msg.id = static_cast<uint32_t>(tempId);
      Can0.write(msg);
      prepMsg.driveRear(0, driveTempo, &tempId, msg.buf);
      msg.id = static_cast<uint32_t>(tempId);
      Can0.write(msg);
    }
    sBackward = 0;
  }
  doubleCheckButtonState();
}
