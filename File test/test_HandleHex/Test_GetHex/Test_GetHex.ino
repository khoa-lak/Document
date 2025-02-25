//https://s3.ap-southeast-1.amazonaws.com/iotoom.com/hardware/test_OTA.txt
#define LED_13 PC13
#include "MainProcess.h"
MainProcess_ mainProcess;
String input_string = "";
uint32_t delayLed = millis();
void setup() {
  Serial.begin(9600);
  mainProcess.funcSetup();
  pinMode(LED_13, OUTPUT);
}

void loop() {
  //  if (mainProcess.funcMain()) {
  //    if (input_string.indexOf("pub_mes") >= 0) {
  //      input_string = "";
  //    }
  //    else if (input_string.indexOf("get_file") >= 0) {
  //      input_string = "";
  //    }
  //  }
  if (input_string.indexOf("file_hex") >= 0) {
    mainProcess.funcWriteFlash();
    input_string = "";
  }
  blinkLed();
  return;
}
void serialEvent() {
  input_string = "";
  while (Serial.available()) {
    char in_char = (char)Serial.read();
    if (in_char != '\n' && in_char != ' ' && in_char != '\r') {
      input_string += in_char;
    }
  }
  Serial.println(input_string);
}
void blinkLed() {
  uint8_t en = digitalRead(LED_13);
  if (millis() - delayLed > 200) {
    // IWatchdog.reload();
    digitalWrite(LED_13, !en);
    delayLed = millis();
  }
}
