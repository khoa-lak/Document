#include "SIMA7xxx.h"
CLASS_SIMA7xxx    mSIM;
#define Led PC13
void (*runningFunc)();
bool conditionRunMain = false;
unsigned long was_time = millis();
char g_cHost[] = "mqtt.iotoom.com";
uint16_t g_ui16Port = 1883;
char g_cDeviceID[100] = "simTest";
char p_TopicTest[] = "test/testSIM";
uint32_t g_ui32delay = millis();
uint64_t i = 0;
void setup() {
  Serial.begin(9600);
  pinMode(Led, OUTPUT);
  digitalWrite(Led,  HIGH);
  mSIM.Func_ConfigSIM();
}

void loop() {
  if (mSIM.Func_ConnectSim())
    mSIM.Func_FollowTopic();
  BlinkLed();
}
void BlinkLed() {
  uint8_t stateL13 = digitalRead(Led);
  if (millis() - was_time > 200) {
    digitalWrite(Led, !stateL13);
    was_time = millis();
  }
}
