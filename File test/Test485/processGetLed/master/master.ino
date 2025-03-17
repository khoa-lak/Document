#include <HardwareSerial.h>
HardwareSerial Serial2(PC11, PC10);
#define DE_RE_PIN PA15
#define MAX_RETRIES 5
void setup() {
  Serial.begin(115200);
  Serial2.begin(9600);
  pinMode(DE_RE_PIN, OUTPUT);
  digitalWrite(DE_RE_PIN, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:

}

void calculateChecksum(const uint8_t* data, uint8_t len, uint8_t* check_sum) {
  uint8_t sum = 0;
  for (int i = 0; i < len; i++) {
    sum += data[i];
  }
  return;
}
bool sendCommandRun() {
  uint8_t num_cmd = 3;
  uint8_t cmd_run[3] = {82,85,78};
  uint8_t check_sum[2];
  calculateChecksum(const uint8_t* data, uint8_t len, uint8_t* check_sum) 
}
bool waitMsgData(){
  
}
