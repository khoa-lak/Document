#define LED 13
#define CB 0
void setup() {
  pinMode(LED, OUTPUT);
  pinMode(CB, INPUT);
}

void loop() {
  if (digitalRead(CB))
    digitalWrite(LED, 0);
  else
    digitalWrite(LED, 1);
}
