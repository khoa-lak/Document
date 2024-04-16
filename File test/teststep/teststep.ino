unsigned long currentMicros, Micros;
//#define en1   PC0
//#define dir1   PC1
//#define pul1   PC2
//#define en2   PC3
//#define dir2   PA0
//#define pul2   PA1
//#define en3   PA2
//#define dir3   PA3
//#define pul3   PA5

#define enpump        PA3
#define dirpump       PA5
#define pulsepump1    PA6
#define pulsepump2    PA7

//#define dir2    PA0
//#define pulse2  PA1
String inputString = "";
char boolRun;
uint8_t stp = 1000;
uint16_t stp2 = 240;
void setup() {
  pinMode(enpump,    OUTPUT);
  pinMode(dirpump,    OUTPUT);
  pinMode(pulsepump1,    OUTPUT);
  pinMode(pulsepump2,    OUTPUT);
  digitalWrite(enpump, 0);
  digitalWrite(dirpump, 0);
  digitalWrite(pulsepump1, 0);
  digitalWrite(pulsepump2, 0);
  //  pinMode(d2,    INPUT_PULLUP);
  //  pinMode(u2,    INPUT_PULLUP);
  //  pinMode(en1,    OUTPUT);
  //  pinMode(dir1,    OUTPUT);
  //  pinMode(pul1,  OUTPUT);
  //  pinMode(en2,    OUTPUT);
  //  pinMode(dir2,    OUTPUT);
  //  pinMode(pul2,  OUTPUT);
  //  pinMode(en3,    OUTPUT);
  //  pinMode(dir3,    OUTPUT);
  //  pinMode(pul3,  OUTPUT);
  //  digitalWrite(en1, 0);
  //  digitalWrite(pul1, 0);
  //  digitalWrite(dir1, 0);
  //  digitalWrite(en2, 0);
  //  digitalWrite(pul2, 0);
  //  digitalWrite(dir2, 0);
  //  digitalWrite(en3, 0);
  //  digitalWrite(pul3, 0);
  //  digitalWrite(dir3, 0);
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
void loop() {
  //  if (inputString == "r0") {
  //    funcStepRotate(0);
  //    inputString = "";
  //  }
  //  else if (inputString == "r1") {
  //    funcStepRotate(1);
  //    inputString = "";
  //  }
  //  else if (inputString == "lr0") {
  //    funcStepLinearRight(0);
  //    inputString = "";
  //  }
  //  else if (inputString == "lr1") {
  //    funcStepLinearRight(1);
  //    inputString = "";
  //  }
  //  else if (inputString == "ll0") {
  //    funcStepLinearLeft(0) ;
  //    inputString = "";
  //  }
  //  else if (inputString == "ll1") {
  //    funcStepLinearLeft(1);
  //    inputString = "";
  //  } else if (inputString == "all") {
  //    funcCommon();
  //  }
  if (inputString == "p10") {
    funcPump1(0);
    inputString = "";
  } else if (inputString == "p11") {
    funcPump1(1);
    inputString = "";
  } else if (inputString == "p20") {
    funcPump2(0);
    inputString = "";
  } else if (inputString == "p21") {
    funcPump2(1);
    inputString = "";
  }
}
void funcPump1(uint8_t dir) {
  uint16_t spe = 200;
  digitalWrite(dirpump, dir);
  digitalWrite(enpump, 1); 
    for (int i = 0 ; i < 2000 ; i++) {
      digitalWrite(pulsepump1, 0);
      delayMicroseconds(spe);
      digitalWrite(pulsepump1, 1);
      delayMicroseconds(spe);
    }
  digitalWrite(enpump, 0);
}
void funcPump2(uint8_t dir) {
  uint16_t spe = 200;
  digitalWrite(dirpump, dir);
  digitalWrite(enpump, 1);
    for (int i = 0 ; i < 2000 ; i++) {
      digitalWrite(pulsepump2, 0);
      delayMicroseconds(spe);
      digitalWrite(pulsepump2, 1);
      delayMicroseconds(spe);
    }
  digitalWrite(enpump, 0);
}
//void funcStepRotate(uint8_t dir) {
//  uint16_t spe = 500;
//  digitalWrite(dir1, dir);
//  digitalWrite(en1, 1);
//  for (int j = 0 ; j < 20 ; j++ ) {
//    for (int i = 0 ; i < 800 ; i++) {
//      digitalWrite(pul1, 0);
//      delayMicroseconds(spe);
//      digitalWrite(pul1, 1);
//      delayMicroseconds(spe);
//    }
//    delay(1200);
//  }
//}
//void funcStepLinearRight(uint8_t dir) {
//  uint16_t spe = 150;
//  digitalWrite(en2, 1);
//  digitalWrite(dir2, dir);
//  for (int i = 0 ; i < 30000 ; i++) {
//    digitalWrite(pul2, 0);
//    delayMicroseconds(spe);
//    digitalWrite(pul2, 1);
//    delayMicroseconds(spe);
//  }
//  digitalWrite(en2, 0);
//}
//void funcStepLinearLeft(uint8_t dir) {
//  uint16_t spe = 150;
//  digitalWrite(en3, 1);
//  digitalWrite(dir3, dir);
//  for (int i = 0 ; i < 20000 ; i++) { //24500: vial
//    digitalWrite(pul3, 0);
//    delayMicroseconds(spe);
//    digitalWrite(pul3, 1);
//    delayMicroseconds(spe);
//  }
//  digitalWrite(en3, 0);
//}
//void funcCommon() {
//  uint16_t spe = 150;
//  digitalWrite(en1, 1);
//  digitalWrite(en2, 1);
//  digitalWrite(en3, 1);
//  digitalWrite(dir1, 0);
//  digitalWrite(dir2, 0);
//  digitalWrite(dir3, 0);
//  for (int i = 0 ; i < 40000 ; i++) {
//    digitalWrite(pul1, 0);
//    digitalWrite(pul2, 0);
//    digitalWrite(pul3, 0);
//    delayMicroseconds(spe);
//    digitalWrite(pul1, 1);
//    digitalWrite(pul2, 1);
//    digitalWrite(pul3, 1);
//    delayMicroseconds(spe);
//  }
//  delay(1000);
//  digitalWrite(dir1, 1);
//  digitalWrite(dir2, 1);
//  digitalWrite(dir3, 1);
//  for (int i = 0 ; i < 40000 ; i++) {
//    digitalWrite(pul1, 0);
//    digitalWrite(pul2, 0);
//    digitalWrite(pul3, 0);
//    delayMicroseconds(spe);
//    digitalWrite(pul1, 1);
//    digitalWrite(pul2, 1);
//    digitalWrite(pul3, 1);
//    delayMicroseconds(spe);
//  }
//  digitalWrite(en1, 0);
//  digitalWrite(en2, 0);
//  digitalWrite(en3, 0);
//  delay(10000);
//}
