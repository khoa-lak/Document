#include <SoftwareSerial.h>
SoftwareSerial RS485(PC11, PC10);
#define DE PA15
#define BTN1 PB8
#define BTN2 PB9
#define L1 PC13
#define L2 PA0
char request1[] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x06, 0xC5, 0xC8}; // Dữ liệu truyền đi
char request2[] = {0x02, 0x03, 0x21, 0x00, 0x00, 0x02, 0xCE, 0x04}; // Dữ liệu truyền đi
unsigned long was_delayFunc = millis();
uint8_t which = 0;
void setup() {
  Serial.begin(9600);
  RS485.begin(9600); // Khởi tạo kết nối RS485 với cảm biến
  pinMode(DE, OUTPUT);
  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  pinMode(BTN1, INPUT_PULLDOWN);
  pinMode(BTN2, INPUT_PULLDOWN);
}

void loop() {
  if (digitalRead(BTN1) == 1) {
    which = 1;
  }
  if (digitalRead(BTN2) == 1) {
    which = 0;
  }
  if (which == 1) {
    TranARecive485(1, request1, sizeof(request1));
    which = 2;
  } else if (which == 2) {
    TranARecive485(2, request2, sizeof(request2));
    which = 1;
  }
  BlinkLed();
}

void BlinkLed() {
  uint8_t state11 = digitalRead(L1);
  uint8_t state12 = digitalRead(L2);
  if (millis() - was_delayFunc > 1000) {
    digitalWrite(L1, !state11);
    digitalWrite(L2, !state12);
    was_delayFunc = millis();
  }
}

void TranARecive485(int slave, char * request, int sizeOF) {
  byte response[20];
  digitalWrite(DE, 1);
  RS485.write(request, sizeOF); // Gửi dữ liệu đi
  delay(10); // Đợi cho cảm biến xử lý và trả về dữ liệu

  int i = 0;
  digitalWrite(DE, 0);
  while (RS485.available()) {
    response[i++] = RS485.read(); // Nhận dữ liệu từ cảm biến
  }
  char ATCommand[80];
  sprintf(ATCommand, "Response\"%d\": ", slave);
  Serial.print(ATCommand);
  for (int j = 0; j < i; j++) {
    Serial.print(response[j], HEX);
    Serial.print(" ");
  }
  Serial.println();
  Serial.flush();
  delay(4000); // Chờ 1 giây trước khi gửi yêu cầu tiếp theo
}
