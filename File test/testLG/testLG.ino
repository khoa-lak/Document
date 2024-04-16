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
void setup() {
  Serial.begin(9600);
  pinMode(y0, OUTPUT);
  pinMode(y1, OUTPUT);
  pinMode(y2, OUTPUT);
  pinMode(y3, OUTPUT);
  pinMode(y4, OUTPUT);
  pinMode(y5, OUTPUT);
  pinMode(y6, OUTPUT);
  pinMode(x0,INPUT);
  pinMode(x1,INPUT);
  pinMode(x2,INPUT);
  pinMode(x3,INPUT);
  pinMode(x4,INPUT);
  pinMode(x5,INPUT);

 
}

void loop() {
digitalWrite(3, HIGH);
  digitalWrite(y1, HIGH);
  digitalWrite(y2, HIGH);
  digitalWrite(y3, HIGH);
  digitalWrite(y4, HIGH);
  digitalWrite(y5, HIGH);
  digitalWrite(y6, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
   digitalWrite(y0, LOW);
  digitalWrite(y1, LOW);
  digitalWrite(y2, LOW);
  digitalWrite(y3, LOW);
  digitalWrite(y4, LOW);
  digitalWrite(y5, LOW);
  digitalWrite(y6, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);       

}
