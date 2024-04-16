
// Định nghĩa các chân nối
#define LED PA0
#define S0 PB5
#define S1 PB4
#define S2 PC15
#define S3 PC14
#define sensorOut PA8
bool (*runningLED)();
bool (*runningColor)();
uint32_t delayLED;
uint32_t delayColor;
uint32_t delayStartColor = 0;
uint32_t tru = 0;
// Các tần số được đọc bởi các màu
int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;
char char1 = '0';
void setup() {
  // Cấu hình chân

  pinMode(LED, OUTPUT);
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(PA0, OUTPUT);
  // sensorOut là input
  pinMode(sensorOut, INPUT);

  // Thang đo tần số là  20%
  digitalWrite(PC3,  0);
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);
  digitalWrite(PC0, LOW);
  // Begins serial communication
  Serial.begin(9600);
}
void LEDIn(uint8_t intensity) {
  uint8_t state = digitalRead(LED);
  uint16_t timedelay = 0;

  switch (intensity) {
    case 1:
      if (state) timedelay = 500;
      else if (!state) timedelay = 900;
      break;
    case 2:
      if (state) timedelay = 900;
      else if (!state) timedelay = 200;
      break;
    default:
      break;
  }
  timedelay = timedelay - tru;
  //Serial.println(tru);
  if (micros() - delayLED > timedelay) {
    digitalWrite(LED, !state);
    delayLED = micros();
  }
}
uint32_t Func_Color(uint8_t _S2, uint8_t _S3) {
  digitalWrite(S2, _S2);
  digitalWrite(S3, _S3);
  delay(200);
  uint32_t frequency_PulseIn = pulseIn(sensorOut, LOW);
  return frequency_PulseIn;
}
bool Func_ReadColor(uint8_t num) {

  if ( millis() - delayColor > 100) {
    delayColor = millis();
    uint8_t s2 = 0;
    uint8_t s3 = 0;
    uint32_t RGB = 0;
    switch (num) {
      case 1:
        s2 = 0;
        s3 = 0;
        break;
      case 2:
        s2 = 1;
        s3 = 1;
        break;
      case 3:
        s2 = 0;
        s3 = 1;
        break;
      case 4:
        s2 = 1;
        s3 = 0;
        break;
      default:
        break;
    }
    for (int i = 0 ; i < 5 ; i ++) {
      RGB   += Func_Color(s2, s3);
    }
    RGB = RGB / 5;
    Serial.println(RGB);
    tru = millis() - delayColor;
    delayColor = millis();
    return 1;
  } else {
    tru = 0;
  }
  return 0;
}
uint8_t numColor = 1;
void loop() {
  //  LEDIn(1);
  //  if (millis() - delayStartColor > 2000) {
  //    if (!digitalRead(LED)) {
  //      if (Func_ReadColor(numColor)) {
  //        numColor ++;
  //        if (numColor > 4) {
  //          numColor = 1;
  //          Serial.println("---------------------");
  //          digitalWrite(LED, 1);
  //          delay(2000);
  //          delayStartColor = millis();
  //        }
  //      }
  //    }
  //  } else {
  //    tru = 0;
  //  }
  digitalWrite(LED, 0);
  delay(500);
  Serial.println(Func_Color(0,0));
  Serial.println(Func_Color(1, 1));
  Serial.println(Func_Color(0, 1));
  Serial.println(Func_Color(1, 0));
  Serial.println("--------------------");
  digitalWrite(LED, 1);
  delay(1000);
}
