#include "ConfigPin-testGiot.h"
ConfigPin configPin;

uint16_t g_Pulse;
bool g_deBa = 0;
unsigned long TongXung;
unsigned long time1 = micros();
unsigned long g_delayStep1 = micros();
unsigned long g_delayStep2 = micros();
unsigned long g_delayStep3 = micros();
unsigned long g_delayFunction = millis();
void (*runningFunc)();
void (*runningFunc1)();
void setup() {
  configPin.Config_InOut() ;
  Serial.begin(115200);
  attachInterrupt(digitalPinToInterrupt(btn_1), Func_InterruptReset, FALLING);
  runningFunc = &Reset;
}
void MR_RunningStep(uint8_t Step, uint8_t Dir, uint8_t HorL, unsigned long *timeDelay ) {//1-Which Step,2-DirofStep,3-Dir1or0,4-TimeDelay
  uint8_t Ena;
  uint16_t Delay;
  if (Step == pulse_Step1) {
    Ena = en_Step1;
    Delay = 3000;
  } else {
    Delay = 500;
    Ena = en_Step23;
  }
  digitalWrite(Ena, 1);
  digitalWrite(Dir, HorL);
  uint8_t stateStep = digitalRead(Step);
  if (micros() - *timeDelay > Delay) {
    digitalWrite(Step , !stateStep);
    *timeDelay = micros();
    TongXung++;
  }
}
void MR_RunningStep1(uint8_t Step, uint8_t Dir, uint8_t HorL, unsigned long *timeDelay ) {//1-Which Step,2-DirofStep,3-Dir1or0,4-TimeDelay
  uint8_t Ena;
  uint16_t Delay;
  if (Step == pulse_Step1) {
    Ena = en_Step1;
    Delay = 3000;
  } else {
    Delay = 5000;
    Ena = en_Step23;
  }
  digitalWrite(Ena, 1);
  digitalWrite(Dir, HorL);
  uint8_t stateStep = digitalRead(Step);
  if (micros() - *timeDelay > Delay) {
    digitalWrite(Step , !stateStep);
    *timeDelay = micros();
    TongXung++;
  }
}
void MR_RunningStep(uint8_t Step, uint8_t Dir, uint8_t HorL, unsigned long *timeDelay, uint16_t *pulse) {//1-Which Step,2-DirofStep,3-Dir1or0,4-TimeDelay
  uint8_t Ena;
  uint16_t Delay;
  if (Step == pulse_Step1) {
    Ena = en_Step1;
    Delay = 3000;
  } else {
    Delay = 500;
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

void Func_InterruptReset() {
  runningFunc = &Reset;
}
void Reset() {
  configPin.Config_InOut();
  if (!btn_Run) {
    g_deBa = false;
    runningFunc = &VeHome;
    runningFunc1 = &DemTongGioiXLNho1;
    // runningFunc1= &DemTongGioiXLLon1;
  }
}
void Run1() {
  runningFunc1();
}
void Run2() {

}
void loop() {
  runningFunc();
  BlinkLed();
}
void BlinkLed() {
  uint8_t state13 = digitalRead(PC13);
  uint8_t state11 = digitalRead(PB11);
  if (millis() - time1 > 1000) {
    digitalWrite(PC13, !state13);
    digitalWrite(PB11, !state11);
    time1 = millis();
  }
}

void DemTongGioiXLNho1() {
    g_Pulse = 320;
    while (g_Pulse > 0) {
      MR_RunningStep(pulse_Step1, dir_Step1, 1, &g_delayStep1, &g_Pulse);
    }
  while (limit_Down2 || limit_Down3) {
    if (limit_Down2) {
      MR_RunningStep(pulse_Step2, dir_Step23, 1, &g_delayStep2);
    }
    if (limit_Down3) {
      MR_RunningStep(pulse_Step3, dir_Step23, 1, &g_delayStep3);
    }
  }
  if (!limit_Down2 && !limit_Down3) {
    g_Pulse = 80;
    while (g_Pulse > 0) {
      if (g_Pulse > 0) {
        MR_RunningStep(pulse_Step2, dir_Step23, 0, &g_delayStep2, &g_Pulse);
      }
      if (g_Pulse > 0) {
        MR_RunningStep(pulse_Step3, dir_Step23, 0, &g_delayStep3, &g_Pulse);
      }
    }
    //BomNC();
    TongXung = 0;
    runningFunc1 = &DemTongGioiXLNho2;
  }
}
void DemTongGioiXLNho2() {
  //  g_Pulse = 10000;
  //  while (g_Pulse > 0 && limit_Up3 ) {
  //    MR_RunningStep(pulse_Step3, dir_Step23, 0, &g_delayStep3, &g_Pulse);
  //  }
  //  delay(10000);
  g_Pulse = 2460;
  while (g_Pulse > 0 && limit_Up3 ) {
    MR_RunningStep(pulse_Step3, dir_Step23, 0, &g_delayStep3, &g_Pulse);
  }
  g_Pulse = 20000;
  while (g_Pulse > 0 && limit_Up2 ) {
    MR_RunningStep(pulse_Step2, dir_Step23, 0, &g_delayStep2, &g_Pulse);
  }
  delay(2000);
  runningFunc1 = &DemTongGioiXLNho3;
}
void DemTongGioiXLNho3() {
  if (limit_Down3) {
    MR_RunningStep1(pulse_Step3, dir_Step23, 1, &g_delayStep3);
  } else {
    g_Pulse = 100;
    while (g_Pulse > 0 ) {
      MR_RunningStep(pulse_Step3, dir_Step23, 1, &g_delayStep3, &g_Pulse);
    }
    while (limit_Up2) {//Ve home step2
      MR_RunningStep(pulse_Step2, dir_Step23, 0, &g_delayStep2);
    }
    Serial.println(TongXung);
    //XaNC();
    runningFunc = &Reset;
  }
}

void VeHome() {
  if (limit_Up2) {//Ve home step2
    MR_RunningStep(pulse_Step2, dir_Step23, 0, &g_delayStep2);
  }
  if (limit_Up3) {//Ve home step3
    MR_RunningStep(pulse_Step3, dir_Step23, 0, &g_delayStep3);
  }
  if (!limit_Up2 && !limit_Up3) {
    if (!g_deBa) {
      g_Pulse = 400;
      while (g_Pulse > 0) {
        MR_RunningStep(pulse_Step1, dir_Step1, 0, &g_delayStep1, &g_Pulse);
      }
      g_deBa = true;
    }
    if (limit_Home1) {//Ve home step1
      MR_RunningStep(pulse_Step1, dir_Step1, 1, &g_delayStep1);
      g_Pulse = 120;
    } else {
      while (g_Pulse > 0) {
        if (g_Pulse > 40 && g_Pulse < 120) {//Scale 3 step
          MR_RunningStep(pulse_Step2, dir_Step23, 0, &g_delayStep2, &g_Pulse);
          MR_RunningStep(pulse_Step3, dir_Step23, 0, &g_delayStep3, &g_Pulse);
        }
        if (g_Pulse > 0)
          MR_RunningStep(pulse_Step1, dir_Step1, 1, &g_delayStep1, &g_Pulse);
      }
      runningFunc = &Run1;
    }
  }
}

void BomNC() {
  g_delayFunction = millis();
  while (millis() - g_delayFunction < 12000) {
    digitalWrite(dir_pump, 0);
    digitalWrite(pump1, 0);
  }
  digitalWrite(dir_pump, 0);
  digitalWrite(pump1, 1);
}
void XaNC() {
  g_delayFunction = millis();
  while (millis() - g_delayFunction < 12000) {
    digitalWrite(dir_pump, 1);
    digitalWrite(pump1, 0);
  }
  digitalWrite(dir_pump, 0);
  digitalWrite(pump1, 1);
}
