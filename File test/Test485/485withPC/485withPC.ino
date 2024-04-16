#include <SoftwareSerial.h>
#define en485 PA8
SoftwareSerial serial485(PA10, PA9);
String inputString;
void setup() {
  Serial.begin(9600);
  serial485.begin(9600);
  serial485.setTimeout(200);
  pinMode(en485, OUTPUT);
  digitalWrite(en485, 0);

}

void loop() {
  if (inputString == "gui") {
    Serial.println("gui du lieu");
    byte request[] = {0x05, 0x03, 0x26, 0x00, 0x00, 0x06, 0xCF, 0x04}; //Disen
    digitalWrite(en485, 1);
    delay(5);
    serial485.write(request, sizeof(request)); // Gửi dữ liệu đi
    delay(5);
    inputString = "";
    digitalWrite(en485, 0);
  }
  while (serial485.available()) {
    Serial.print("Response: ");
    for (int j = 0; j < 30; j++) {
      Serial.print(serial485.read());
      Serial.print(" ");
    }
    Serial.println();
    Serial.println("////// ");
  }
  delay(500); // Chờ 1 giây trước khi gửi yêu cầu tiếp theo
}


void serialEvent() {
  inputString = "";
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar != '\n' && inChar != ' ' && inChar != '\r') {
      inputString += inChar;
    }
  }
  Serial.println(inputString);
}
