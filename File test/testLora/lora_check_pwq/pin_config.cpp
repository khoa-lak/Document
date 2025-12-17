#include "pin_config.h"
void PinConfig::setupPin() {
  //  pinMode(X0,  INPUT_PULLUP);
  //  pinMode(X1,  INPUT_PULLUP);
  //  pinMode(X2,  INPUT_PULLUP);
  //  pinMode(X3,  INPUT_PULLUP);
  //  pinMode(X4,  INPUT_PULLUP);
  //
  //  pinMode(Y0,  OUTPUT);
  //  pinMode(Y1,  OUTPUT);
  //  pinMode(Y2,  OUTPUT);
  //  pinMode(Y3,  OUTPUT);
  //  pinMode(Y4,  OUTPUT);
  //  pinMode(Y5,  OUTPUT);
  //  pinMode(Y6,  OUTPUT);
  //  pinMode(Y7,  OUTPUT);
  //  pinMode(Y8,  OUTPUT);
  //  pinMode(Y9,  OUTPUT);
  //  pinMode(Y10, OUTPUT);
  //  pinMode(Y11, OUTPUT);
  //  pinMode(Y12, OUTPUT);
  //  pinMode(Y13, OUTPUT);
  //  pinMode(Y14, OUTPUT);
  //  pinMode(Y15, OUTPUT);
  //  pinMode(Y16, OUTPUT);
  //  pinMode(Y17, OUTPUT);
  //  pinMode(Y18, OUTPUT);
  //  pinMode(Y19, OUTPUT);
  //  pinMode(Y20, OUTPUT);
  //
  //  digitalWrite(Y0, 1);
  //  digitalWrite(Y1, 1);
  //  digitalWrite(Y2, 1);
  //  digitalWrite(Y3, 1);
  //  digitalWrite(Y4, 1);
  //  digitalWrite(Y5, 1);
  //  digitalWrite(Y6, 1);
  //  digitalWrite(Y7, 1);
  //  digitalWrite(Y8, 1);
  //  digitalWrite(Y9, 1);
  //  digitalWrite(Y10, 0);
  //  digitalWrite(Y11, 1);
  //  digitalWrite(Y12, 1);
  //  digitalWrite(Y13, 0);
  //  digitalWrite(Y14, 1);
  //  digitalWrite(Y15, 1);
  //  digitalWrite(Y16, 1);
  //  digitalWrite(Y17, 0);
  //  digitalWrite(Y18, 1);
  //  digitalWrite(Y19, 1);
  //  digitalWrite(Y20, 1);
  //
  //
  //  pinMode(en_UART, OUTPUT);
  //  pinMode(led, OUTPUT);
  pinMode(rst_lora, OUTPUT);
  digitalWrite(rst_lora, 1);
  //digitalWrite(en_UART, 0);
}
