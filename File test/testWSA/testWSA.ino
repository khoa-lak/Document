#include "ConfigPin.h"
ConfigPin c_ConfigPin;
uint16_t g_Pulse = 150;
uint16_t g_Pulse1v1 = 20 * 150;
uint16_t g_Pulse1v = g_Pulse1v1;
unsigned long g_delayStep1 = micros();
unsigned long g_delayStep2 = micros();
unsigned long g_delayStep3 = micros();
int dem = 0 ;
unsigned long time1 = millis();
String inputString = "";         // a String to hold incoming data
bool stringComplete = false;

void (*runningFunc)();
void (*runningFunc1)();
void setup() {
  Serial.begin(9600);
  c_ConfigPin.Config_InOut();


}
void MR_RunningStep0(uint8_t Step, uint8_t Dir, uint8_t HorL, unsigned long * timeDelay ) {
  uint8_t Ena;
  uint16_t Delay;
  if (Step == pulse_Step1) {
    Ena = en_Step1;
    Delay = 3000;
  } else {
    Delay = 1000;
    Ena = en_Step23;
  }
  digitalWrite(Ena, 1);
  digitalWrite(Dir, HorL);
  uint8_t stateStep = digitalRead(Step);
  if (micros() - *timeDelay > Delay) {
    digitalWrite(Step , !stateStep);
    *timeDelay = micros();
  }
}

void MR_RunningStep1(uint8_t Step, uint8_t Dir, uint8_t HorL, unsigned long * timeDelay, uint16_t *pulse) {
  uint8_t Ena;
  uint16_t Delay;
  if (Step == pulse_Step1) {
    Ena = en_Step1;
    Delay = 3000;
  } else {
    Delay = 1000;
    Ena = en_Step23;
  }
  digitalWrite(Ena, 1);
  digitalWrite(Dir, HorL);
  uint8_t stateStep = digitalRead(Step);

  if (micros() - *timeDelay > Delay) {
    //    Serial.println("pulse: ");
    //    Serial.println(*pulse);
    digitalWrite(Step , !stateStep);
    *pulse -= 1;
    *timeDelay = micros();
  }
}


void BlinkLed() {
  uint8_t state11 = digitalRead(PC13);
  uint8_t state12 = digitalRead(PB11);
  //  uint8_t state13 = digitalRead(PA4);
  //  uint8_t state14 = digitalRead(PA0);
  if (millis() - time1 > 1000) {
    digitalWrite(PC13, !state11);
    digitalWrite(PB11, !state12);
    //    digitalWrite(PA4, !state13);
    //    digitalWrite(PA0, !state14);
    time1 = millis();
  }
}

void Run() {
  runningFunc();
}

void loop() {
  BlinkLed();
  if (inputString == "H") {
    TestVong();
  } else if (inputString == "X") {
    TestXuong();
  } else if (inputString == "L") {
    TestLen();
  } else if (inputString == "B") {
    TestBomThuoc(5000);
  } else if (inputString == "T") {
    TestXaThuoc();
  } else if (isDigit(inputString.charAt(0))) {
    int k = inputString.toInt();
    TestVong2(k);
  }

  if (inputString != "") {
    Serial.println(inputString);
    inputString = "";
  }
}

void TestVong() {
  while (limit_Up2 || limit_Up3 || limit_Home1) {
    if (limit_Up2) {
      MR_RunningStep0(pulse_Step2, dir_Step23, 0, &g_delayStep2);
    }
    if (limit_Up3) {
      MR_RunningStep0(pulse_Step3, dir_Step23, 0, &g_delayStep3);
    }

    if (!limit_Up2 && !limit_Up3) {
      if (limit_Home1) {
        g_Pulse = 40;
  while (g_Pulse > 0) {
    MR_RunningStep1(pulse_Step2, dir_Step23, 0, &g_delayStep2, &g_Pulse);
  }
  g_Pulse = 40;
  while (g_Pulse > 0) {
    MR_RunningStep1(pulse_Step3, dir_Step23, 0, &g_delayStep3, &g_Pulse);
  }
        MR_RunningStep0(pulse_Step1, dir_Step1, 1, &g_delayStep1);
      }
    }
  }
  
  g_Pulse = 40;
  while (g_Pulse > 0) {
    MR_RunningStep1(pulse_Step1, dir_Step1, 1, &g_delayStep1, &g_Pulse);
  }


  delay(2000);

}
void TestVong2(int soVong) {
  //g_Pulse = 20*300;
  for (int i = 0 ; i < soVong; i++) {
    g_Pulse =  320;
    while (g_Pulse > 0) {
      if (!limit_Up2 ) {
        MR_RunningStep1(pulse_Step1, dir_Step1, 0, &g_delayStep1, &g_Pulse);
      }
    }
  }
  delay(2000);
}
void TestXuong() {
  while (limit_Down2 || limit_Down3) {
    if (limit_Down2) {
      MR_RunningStep0(pulse_Step2, dir_Step23, 1, &g_delayStep2);
    }
    if (limit_Down3) {
      MR_RunningStep0(pulse_Step3, dir_Step23, 1, &g_delayStep3);
    }
  }
}
void TestLen() {
  while (limit_Up2 ) {
    if (limit_Up2) {
      MR_RunningStep0(pulse_Step2, dir_Step23, 0, &g_delayStep2);
    }
    //    if (limit_Up3) {
    //      MR_RunningStep0(pulse_Step3, dir_Step23, 0, &g_delayStep3);
    //    }
  }
}
void TestBomThuoc(int xung) {
  //g_Pulse = 20*300;
  g_Pulse = xung;
  while (g_Pulse > 0) {
    if (!limit_Down2 ) {
      MR_RunningStep1(pulse_Step3, dir_Step23, 0, &g_delayStep3, &g_Pulse);
    }
  }

}
void TestXaThuoc() {
  while ( limit_Down3) {
    MR_RunningStep0(pulse_Step3, dir_Step23, 1, &g_delayStep3);
  }
}
void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar != '\n') {
      inputString += inChar;
    }
  }
}
