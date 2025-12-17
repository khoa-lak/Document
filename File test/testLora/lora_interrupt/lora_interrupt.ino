#include <SPI.h>
#include <RH_RF95.h>
#define di0_lora PC8
#define rst_lora PB11
#define nss_lora PB12
#define sck_lora PB13
#define mis_lora PB14
#define mos_lora PB15
#define led PC13
RH_RF95 rf95(nss_lora, di0_lora);
bool packetReceived = false;
uint32_t intervalBlinkLed = 0;
void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Started");
  SPI.setMISO(mis_lora);
  SPI.setMOSI(mos_lora);
  SPI.setSCLK(sck_lora);
  pinMode(rst_lora, OUTPUT);
  digitalWrite(rst_lora, 1);
  while (!rf95.init()) {
    Serial.println("init failed");
    delay(1000);
  }
  pinMode(led, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(di0_lora), isr0, RISING);
}
void isr0() {
  Serial.println("Sent a reply");
}
void loop() {
  blink_led();
//  if (rf95.available())
//  {
//    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
//    uint8_t len = sizeof(buf);
//    Serial.print("got request: ");
//    if (rf95.recv(buf, &len))  {
//      Serial.println((char*)buf);
//      uint8_t data[] = "And hello back to you1111111111111";
//      rf95.send(data, sizeof(data));
//      rf95.waitPacketSent();
//      Serial.println("Sent a reply");
//    }
//    else
//    {
//      digitalWrite(rst_lora , 0);
//      Serial.println("recv failed");
//      delay(500);
//      digitalWrite(rst_lora , 1);
//    }
//  }
}
void blink_led() {
  uint16_t tim = 500;
  uint8_t en = digitalRead(led);
  if (millis() - intervalBlinkLed >= tim) {
    digitalWrite(led, !en);
    intervalBlinkLed = millis();
  }
  return;
}
