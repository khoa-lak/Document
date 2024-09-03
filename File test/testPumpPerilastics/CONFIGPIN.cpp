#include "CONFIGPIN.h"
void CLASS_CONFIG::FUNC_CONFIGPIN() {
  pinMode(X1 ,    INPUT_PULLUP);
  pinMode(X2 ,    INPUT_PULLUP);
  pinMode(X3 ,    INPUT_PULLUP);
  pinMode(X4,    INPUT_PULLUP);
  pinMode(X5,    INPUT_PULLUP);
  //pinMode(X6,  INPUT_PULLUP);
  // pinMode(X7,   INPUT_PULLUP);

  pinMode(pumpIN, OUTPUT);
  pinMode(pumpOUT, OUTPUT);
  pinMode(valveDir, OUTPUT);
  pinMode(valvePond, OUTPUT);
  pinMode(thermCooler, OUTPUT);
  pinMode(pumpCooling, OUTPUT);
  pinMode(pumpVial, OUTPUT);
  pinMode(pumpDis, OUTPUT);
  pinMode(motorMix, OUTPUT);
  pinMode(fan, OUTPUT);
  //pinMode(lampRed, OUTPUT);
  pinMode(stir , OUTPUT);
  pinMode(pinPos, OUTPUT);
  pinMode(horn, OUTPUT);
  pinMode(outnone, OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(en485, OUTPUT);
  pinMode(enUART, OUTPUT);

  pinMode(enRot, OUTPUT);
  pinMode(dirRot, OUTPUT);
  pinMode(pulseRot, OUTPUT);
  pinMode(enLinear, OUTPUT);
  pinMode(dirLinear, OUTPUT);
  pinMode(pulseLiRe, OUTPUT);
  pinMode(pulseLiWa, OUTPUT);
  pinMode(enPeris, OUTPUT);
  pinMode(dirPeris, OUTPUT);
  pinMode(pulsePerisRe, OUTPUT);
  pinMode(pulsePerisWa, OUTPUT);

  digitalWrite(pumpIN, 1);
  digitalWrite(pumpOUT, 1);
  digitalWrite(valveDir, 1); //in
  digitalWrite(valvePond,  1); //
  digitalWrite(thermCooler,  1);
  digitalWrite(pumpCooling, 1);
  digitalWrite(pumpVial, 1);
  digitalWrite(pumpDis, 1);
  digitalWrite(motorMix, 1);
  digitalWrite(fan, 1);
  //digitalWrite(lampRed, 1);
  digitalWrite(stir , 1);
  digitalWrite(pinPos, 1);
  digitalWrite(horn, 1);
  digitalWrite(outnone, 1);
  digitalWrite(en485, 1);
  digitalWrite(enUART, 1);

  digitalWrite(enRot, 1);
  digitalWrite(dirRot, 1);
  digitalWrite(pulseRot, 1);
  digitalWrite(enLinear, 1);
  digitalWrite(dirLinear, 1);
  digitalWrite(pulseLiRe, 1);
  digitalWrite(pulseLiWa, 1);
  digitalWrite(enPeris, 1);
  digitalWrite(dirPeris, 1);
  digitalWrite(pulsePerisRe, 1);
  digitalWrite(pulsePerisWa, 1);

}
