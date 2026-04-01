/*
 * PROJECT: MAIN IOT APP (MQTT + FOTA)
 * Yêu cầu: Linker Script chạy từ 0x08008000
 */
#include "FuncMain.h"
FuncMain funcMain;

void setup() {
  Serial.begin(115200);
  funcMain.setupMain();
}

void loop() {
  funcMain.runMain();
}