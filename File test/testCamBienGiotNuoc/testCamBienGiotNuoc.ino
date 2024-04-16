#include <SimpleKalmanFilter.h>
SimpleKalmanFilter simpleKalmanFilter(2, 2, 0.01);
void setup() {
  pinMode(8, INPUT_PULLUP);
  pinMode(13, OUTPUT);
  Serial.begin(9600);
}
long k = millis();
int cout = 0;
bool c = false;
void loop() {
  //  if ((digitalRead(8) == LOW) && !c) {
  //    if (millis() - k >= 100) {
  //      Serial.println(cout++);
  //      c = true;
  //      k = millis();
  //    }
  //
  //  } else if ((digitalRead(8) == HIGH) ) {
  //    c = false;
  //  }
  //  if ((digitalRead(8) == LOW)) {
  //    digitalWrite(13, HIGH);
  //  }else{
  //    digitalWrite(13, LOW);
  //  }  // read a reference value from A0 and map it from 0 to 100
  int real_value = analogRead(A0);

  // add a noise to the reference value and use as the measured value


  // calculate the estimated value with Kalman Filter
  int estimated_value = simpleKalmanFilter.updateEstimate(real_value);
  //Serial.println(estimated_value);
  if ((estimated_value >= 4) && !c) {
    if (millis() - k >= 100) {
      Serial.println(cout++);
      c = true;
      k = millis();
    }

  } else if (estimated_value < 4 ) {
    c = false;
  }

}
