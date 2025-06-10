#define analogPin  PC4
#include <SimpleKalmanFilter.h>
SimpleKalmanFilter simpleKalmanFilter(2, 2, 0.01);
uint32_t timePrint = millis();
float temperature = 0.0;
uint32_t delayRead = millis();
void setup() {
  Serial.begin(9600);

}

void loop() {
  uint32_t value = 0;
  for (uint8_t i = 0 ; i < 10 ; i++) {
    value += analogRead(analogPin);
    delay(100);
  }
  //value /= 10;
  //float temperature = (value/10.0)*0.245 -104.271;
  float valueFake = value / 10.0;
  temperature = 0.00037 * pow(valueFake, 2) - 0.1828 * valueFake + 20.0;
  float realTemp = simpleKalmanFilter.updateEstimate(temperature);
  if (millis() - delayRead > 1000)
    Serial.println("value: " + String(realTemp) + " " + String(valueFake));
}
