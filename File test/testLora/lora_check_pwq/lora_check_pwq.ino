#include "lora_master.h"
LoraMaster lora;
ParamPond paramPond[NUM_POND] = {

  { true, false, "id-617", "pond-module-229a7bf97a9a456684df5143e1437617", nullptr, nullptr, 0, 0, 0, 120, 0},
  { true, false, "id-a43", "pond-module-0b1b512d71534bff9f4b6ec2f7642a43", nullptr, nullptr, 0, 0, 0, 400, 0},
  { true, false, "id-00e", "pond-module-cfa1226375b7449f80fa1193252ee00e", nullptr, nullptr, 0, 0, 0, 200, 0},
  { true, false, "id-711", "pond-module-a9b5914ba3b64fc096bd5dd2cdde9711", nullptr, nullptr, 0, 0, 0, 250, 0},
  { false, false, nullptr, nullptr, nullptr, nullptr, 0, 0, 0, 40, 0},
  { false, false, nullptr, nullptr, nullptr, nullptr, 0, 0, 0, 40, 0},
  { false, false, nullptr, nullptr, nullptr, nullptr, 0, 0, 0, 0, 0},
  { false, false, nullptr, nullptr, nullptr, nullptr, 0, 0, 0, 0, 0}
};
String inputString = "";
uint32_t delay_change_module;
uint32_t delay_led;
uint8_t pond;
void setup() {
  Serial.begin(9600);
  lora.setUp();
  pond = 0;
}

void loop() {
  if (inputString.indexOf("clear") >= 0) {
    lora.setUp();
    pond = 0;
    //    for (uint8_t i = 0 ; i < NUM_POND ; i ++) {
    //      paramPond[i].enable = true;
    //    }
    Serial.println("===============");
    inputString = "";
  }
  else if (inputString.indexOf("value") >= 0) {
    if (millis() - delay_change_module > 20000) {
      for (uint8_t i = 0 ; i < 5 ; i++) {
        lora.modulePondReady();
        delay(500);
      }
      delay(2000);
      for (uint8_t i = 0 ; i < NUM_POND; i++) {
        if (!paramPond[i].enable || paramPond[i].pondName == nullptr)
          continue;
        char* msg = lora.runValueSensor(paramPond[i].moduleId);
        if (msg != nullptr) {
          Serial.print("có msg: ");
          Serial.println(msg);
          delete[] msg;
          delay(200);
        }
        else {
          continue;
        }
      }
      //      if (!paramPond[pond].enable || paramPond[pond].pondName == nullptr) {
      //        pond ++;
      //        if (pond >= NUM_POND)
      //          pond = 0;
      //        return;
      //      }
      //      Serial.print("gọi module: ");
      //      Serial.println(paramPond[pond].moduleId);
      //      for (uint8_t i = 0 ; i < 5 ; i++) {
      //        lora.modulePondReady();
      //        delay(1000);
      //      }
      //      delay(20000);
      //      char* msg = lora.runValueSensor(paramPond[pond].moduleId);
      //      if (msg != nullptr) {
      //        Serial.print("có msg: ");
      //        Serial.println(msg);
      //        delete[] msg;
      //        delay_change_module = millis();
      //      }
      //      else {
      //        paramPond[pond].enable = false;
      //        lora.loraErr = 0;
      //      }
      //      pond ++;
      //      if (pond >= NUM_POND)
      //        pond = 0;
      Serial.println("===============");
      //inputString = "";
      delay_change_module = millis();
    }
  }
  else if (inputString.indexOf("pump") >= 0) {
    if (millis() - delay_change_module > 10000) {
      if (!paramPond[pond].enable || paramPond[pond].pondName == nullptr) {
        pond ++;
        if (pond >= NUM_POND)
          pond = 0;
        return;
      }
      if (lora.runProgram(true, paramPond[pond].moduleId, paramPond[pond].timePump)) {
        delay(paramPond[pond].timePump * 1000 );
        lora.runProgram(false,  paramPond[pond].moduleId, 0);
        Serial.println("===============");
        //inputString = "";
        delay_change_module = millis();

      }
      pond ++;
      if (pond >= NUM_POND)
        pond = 0;

    }
  }
  blinkLed();
}
void blinkLed() {
  uint8_t en = digitalRead(led);
  if (millis() - delay_led > 200) {
    digitalWrite(led, !en);
    delay_led = millis();
  }
}
void serialEvent() {
  inputString = "";
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar != '\n' && inChar != ' ' && inChar != '\r') {
      inputString += inChar;
    }
  }
  Serial.println(inputString);
}
