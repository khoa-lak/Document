//// rf95_client.pde
//// -*- mode: C++ -*-
//// Example sketch showing how to create a simple messageing client
//// with the RH_RF95 class. RH_RF95 class does not provide for addressing or
//// reliability, so you should only use RH_RF95 if you do not need the higher
//// level messaging abilities.
//// It is designed to work with the other example rf95_server
//// Tested with Anarduino MiniWirelessLoRa
//
#include <SPI.h>
#include <RH_RF95.h>
#define rstLora PA2
#define d0Lora  PA3
#define nssLora PA4
#define mosi    PA7
#define miso    PA6
#define clk     PA5
#define led PC13
// Singleton instance of the radio driver
//SPIClass SPIClass(PB15, PB14, PB13, PB12);
RH_RF95 rf95(nssLora, d0Lora);

uint32_t intervalBlinkLed = 0;
void setup()
{
  pinMode(led, OUTPUT);
  //pinMode(d0Lora, INPUT_PULLUP);
  SPI.setMISO(miso);
  SPI.setMOSI(mosi);
  SPI.setSCLK(clk);
  //SPI.setSSEL(PB12);
  pinMode(rstLora, OUTPUT);
  digitalWrite(rstLora , 1);
  Serial.begin(9600);
  if (!rf95.init())
    Serial.println("init failed");
  //attachInterrupt(digitalPinToInterrupt(d0Lora), lora, FALLING);
  //  SPCR |= _BV(SPE);
  //  SPI.attachInterrupt();
}
void lora() {
  Serial.println("in");

}
void loop() {
  blink_led();
  //  if (rf95.available())
  //  {
  //    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  //    uint8_t len = sizeof(buf);
  //    if (rf95.recv(buf, &len))
  //    {
  //      digitalWrite(led, 0);
  //      Serial.print("got request: ");
  //      Serial.println((char*)buf);
  uint8_t data[] = "And hello back to you1111111111111";
  rf95.send(data, sizeof(data));
  rf95.waitPacketSent();
  Serial.println("Sent a reply");
  //    }
  //    else
  //    {
  //      digitalWrite(led, 1);
  //      digitalWrite(rstLora , 0);
  //      Serial.println("recv failed");
  //      delay(500);
  //      digitalWrite(rstLora , 1);
  //    }
  //delay(3000);
  HAL_Delay(3000);
  //}

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
