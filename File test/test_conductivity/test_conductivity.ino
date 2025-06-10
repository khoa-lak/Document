#define conductivityPin  PA7
#define conductivityPin2  PA6
uint32_t timePrint = millis();
void setup() {
  Serial.begin(9600);

}

void loop() {
  uint16_t value = analogRead(conductivityPin);
  uint16_t value2 = analogRead(conductivityPin2);
  if (millis() - timePrint > 1000) {
    Serial.println("value: " + String(value) + " " + String(value2));
    timePrint = millis();
  }
}
