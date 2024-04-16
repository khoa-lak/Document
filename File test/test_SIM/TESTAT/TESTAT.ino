// this sketch is used for testing LoNet with Arduino

// Connect VIO to +5V
// Connect GND to Ground
// Connect RX (data into SIM808) to Digital 11
// Connect TX (data out from SIM808) to Digital 10



//SoftwareSerial Serial3(PA3, PA2); // RX, TX
//SoftwareSerial Serial1(PA10, PA9);
#include <SoftwareSerial.h>

//SoftwareSerial mod(PC11, PC10);
//HardwareSerial  SerialAT(PA10, PA9);
//HardwareSerial SerialAT(PD2, PC12);
// SerialAT(PA10, PA9);
#define SerialAT Serial1
//#define led PC13
#define led PC13
unsigned long time1 = millis();
unsigned long time2 = millis();
byte ledStatus = LOW;
//const char* at = "AT+CGACT?";
String inputString = "";
char input[10];
void (*Func_Running)();

void callSerial(String AT) {
  inputString = "";
  int i = 0;
  int index = AT.length();
  SerialAT.println(AT);
  delay(1000);
  //SerialAT.print((char)26);
  while (SerialAT.available()) {
    char inChar = (char)SerialAT.read();
    if ((inChar != '\n') && (inChar != '\r')) {
      inputString += inChar;
      //      input[i] = inChar;
      //      i++;

    }

  }
  //  for ( int j = 0 ; j <= 10 ; j++) {
  //    Serial.println(j);
  //    Serial.println(input[j]);
  //  }

  Serial.println(index);
  String inputString1 = inputString.substring(index, inputString.length());
  inputString = "";
  inputString += inputString1;
}

void setupSim() {
  String AT = "";

  if (inputString == "") {
    Serial.println("111111111");
    AT = "AT+CSQ";
  } else if (inputString == "+CSQ: 24,99OK" || inputString == "+CSQ: 25,99OK" ) {
    Serial.println("2222222222");
    AT = "AT+CREG?";
  } else if (inputString == "ERROR") {
    AT = "";
    inputString = "";
    Serial.println("33333333");
  }
  if (AT != "") {
    Serial.println(AT);
    callSerial(AT);
    Serial.println(inputString);
  }


}

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  SerialAT.begin(115200);
  pinMode(led, OUTPUT);
  digitalWrite(led, ledStatus);
  Func_Running = &setupSim;
}

void test() {

}

void loop() // run over and over
{
  if (millis() - time1 > 100) {
    ledStatus = !ledStatus;
    digitalWrite(led, ledStatus);   // turn the LED on (HIGH is the voltage level)
    time1 = millis();
  }
  if (millis() - time2 > 3000) {
    //Func_Running();
    time2 = millis();
  }
  if (SerialAT.available()) {
    Serial.write(SerialAT.read());
  }
  if (Serial.available()) {
    SerialAT.write(Serial.read());
  }
  delay(0);
}
