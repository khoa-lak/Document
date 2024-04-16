#define I14 PA4  //Phao
#define Q7  PA3  //PUMP1
#define Q8  PA5  //PUMP2
#define Q9  PA6  //PUMP3
#define Q10 PA7  //DIRPUMP
#define Q11 PC4  //MAGNETIC
#define Q12 PC5  //Pump
#define Q13 PB0  //Dir
#define Q14 PB1
#define Q15 PB7
#define Q16 PB6
void (*runningFuncMachine)();
uint32_t g_ui32delay = millis();
uint32_t g_ui32time = 0;
void setup() {
  pinMode(I14, INPUT_PULLUP);
  pinMode(Q7, OUTPUT);
  pinMode(Q8, OUTPUT);
  pinMode(Q9, OUTPUT);
  pinMode(Q10, OUTPUT);
  pinMode(Q11, OUTPUT);
  pinMode(Q12, OUTPUT);
  pinMode(Q13, OUTPUT);
  pinMode(Q14, OUTPUT);
  pinMode(Q15, OUTPUT);
  pinMode(Q16, OUTPUT);
  digitalWrite(Q7,  HIGH);
  digitalWrite(Q8,  HIGH);
  digitalWrite(Q9,  HIGH);
  digitalWrite(Q10, HIGH);
  digitalWrite(Q11, HIGH);
  digitalWrite(Q12, HIGH);
  digitalWrite(Q13, HIGH);
  digitalWrite(Q14, HIGH);
  runningFuncMachine = & pump;
  g_ui32delay = millis();
}

void loop() {
  //runningFuncMachine();
  testpump();
}
void testpump(){
  digitalWrite(PB6, 0);
}
void pump() {
  digitalWrite(Q12, 0);
  digitalWrite(Q13, 0);
  digitalWrite(Q7, 0);
  digitalWrite(Q10, 0);
  if (millis() -  g_ui32delay > 10000) {
    digitalWrite(Q7, 1);
    digitalWrite(Q12, 1);
    digitalWrite(Q13, 1);
    digitalWrite(Q10, 1);
    runningFuncMachine = & pump1;
    delay(1000);
    g_ui32delay = millis();
  }
}
void pump1() {
  if (!digitalRead(I14)) {
    digitalWrite(Q12, 0);
  } else {
    digitalWrite(Q12, 1);
    runningFuncMachine = & pump2;
    g_ui32time = millis() - g_ui32delay;
    delay(1000);
    g_ui32delay = millis();
  }
}
void pump2() {
  digitalWrite(Q7, 0);
  if (millis() -  g_ui32delay > 12000) {
    digitalWrite(Q7, 1);
    runningFuncMachine = & pump3;
    g_ui32delay = millis();
  }
}

void pump3() {
  digitalWrite(Q10, 0);
  digitalWrite(Q7, 0);
  if (millis() -  g_ui32delay > 14000) {
    digitalWrite(Q7, 1);
    digitalWrite(Q10, 1);
    runningFuncMachine = & pump4;
    delay(1000);
    g_ui32delay = millis();
  }
}
void pump4() {
  digitalWrite(Q12, 0);
  digitalWrite(Q13, 0);
  if (digitalRead(I14)) {
    g_ui32delay = millis();
  } else {
    Serial.println(g_ui32time);
    if (millis() -  g_ui32delay > g_ui32time) {
      digitalWrite(Q12, 1);
      digitalWrite(Q13, 1);
      runningFuncMachine = & pump5;
      delay(1000);
      g_ui32delay = millis();
    }
  }
}
void pump5() {
  if (millis() -  g_ui32delay > 30000) {
    runningFuncMachine = & pump1;
    g_ui32delay = millis();
    g_ui32time = 0;
  } else {
    digitalWrite(Q7, 1);
    digitalWrite(Q10, 1);
    digitalWrite(Q12, 1);
    digitalWrite(Q13, 1);
  }
}
