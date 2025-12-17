#define adc1 PA4
// #define adc2 PA5
// #define adc3 PA6
//#define interruptPin1 PC0
void setup() {
  Serial.begin(9600);
  //  pinMode(interruptPin1, INPUT);
  //  attachInterrupt(digitalPinToInterrupt(interruptPin1), hasDrop, RISING);
}

void loop() {
  uint32_t num1 = analogRead(adc1);
  // uint32_t num2 = analogRead(adc2);
  // uint32_t num3 = analogRead(adc3);
  //  uint32_t num2 = 0;
  //  uint32_t num3 = 0;
  //if (num1 < 2300  || num2 < 2100  || num3 < 2100 ) {
  Serial.println("điện áp vào: " + String(num1));  // + " " + String(num2) + " " + String(num3)

  //}
  delay(100);
}
//void hasDrop() {
//  Serial.println("has drop");
//}
