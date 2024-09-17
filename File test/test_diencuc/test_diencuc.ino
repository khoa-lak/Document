#define X0   PB7
#define X1   PB6
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(X0, INPUT_PULLUP);
  pinMode(X1, INPUT_PULLUP);

}

void loop() {
  in1();
  in2();
  delay(1000);
}
void in1() {
  if (!digitalRead(X0))
    Serial.println("x0 on");
}
void in2() {
  if (!digitalRead(X1))
    Serial.println("x1 on");
}
