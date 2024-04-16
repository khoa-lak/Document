#define y0 3
#define y1 E5
#define y2 5
#define y3 6
#define y4 7
#define y5 8
#define y6 9


#define x0 E0
#define x1 E1
#define x2 E2
#define x3 E3
#define x4 C0
#define x5 C1

String inputString = "";         // a String to hold incoming data


void setup() {
  Serial.begin(9600);
  pinMode(3, OUTPUT);
  pinMode(y1, OUTPUT);
  pinMode(y2, OUTPUT);
  pinMode(y3, OUTPUT);
  pinMode(y4, OUTPUT);
  pinMode(y5, OUTPUT);
  pinMode(y6, OUTPUT);
  pinMode(x0, INPUT);
  pinMode(x1, INPUT);
  pinMode(x2, INPUT);
  pinMode(x3, INPUT);
  pinMode(x4, INPUT);
  pinMode(x5, INPUT);
}

void loop() {
  if (inputString == "1") {
    while (digitalRead(x0) == HIGH) {
      digitalWrite(3, HIGH);
      delay(10);
      digitalWrite(3, LOW);
      delay(10);
    }


  }
  if (inputString == "2") {
    for (int i = 0; i <= 15 ; i++) {
      digitalWrite(3, HIGH);
      delay(10);
      digitalWrite(3, LOW);
      delay(10);
    }
  }

}


void serialEvent() {
  inputString = "";
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar != '\n') {
      inputString += inChar;
    }


  }
}
