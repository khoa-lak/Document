#define adc1 PC0
#define adc2 PC1
#define adc3 PC2
// #define adc2 PA5
// #define adc3 PA6
//#define interruptPin1 PC0
void setup() {
  Serial.begin(9600);
  //  pinMode(interruptPin1, INPUT);
  //  attachInterrupt(digitalPinToInterrupt(interruptPin1), hasDrop, RISING);
}
uint32_t g_num1 = 0;
uint32_t g_num2 = 0;
uint32_t g_num3 = 0;
uint32_t time_read = millis();
uint32_t time_read_drop = millis();
uint32_t drop = 0;
void loop() {
  uint32_t num1 = analogRead(adc1);
  uint32_t num2 = analogRead(adc2);
  uint32_t num3 = analogRead(adc3);
  // g_num1 += num1;
  // g_num2 += num2;
  // g_num3 += num3;
  // uint32_t num2 = analogRead(adc2);
  // uint32_t num3 = analogRead(adc3);
  //  uint32_t num2 = 0;
  //  uint32_t num3 = 0;
  if (num1 < 700 || num2 < 700 || num3 < 590) {
    if (millis() - time_read_drop > 200) {
      Serial.println("điện áp vào: " + String(num1) + " " + String(num2) + " " + String(num3));  // + " " + String(num2) + " " + String(num3)
      drop++;
      time_read_drop = millis();
    }
  }

  if (millis() - time_read > 10000) {
    Serial.println("số giọt là : " + String(drop));
    time_read = millis();
  }
  // Serial.println("điện áp vào: " + String(num1) + " " + String(num2) + " " + String(num3));  // + " " + String(num2) + " " + String(num3)


  // delay(500);
}
//void hasDrop() {
//  Serial.println("has drop");
//}
