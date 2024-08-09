// rf95_server.pde
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messageing server
// with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// reliability, so you should only use RH_RF95  if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example rf95_client
// Tested with Anarduino MiniWirelessLoRa


#include <SPI.h>
#include <RH_RF95.h>
#define rstLora PB11
#define led PC13
// Singleton instance of the radio driver
//SPIClass definePin(PB15, PB14, PB13, PB12);
RH_RF95 rf95(PB12,PC8);
uint32_t intervalBlinkLed = 0;

void setup()
{
  pinMode(led, OUTPUT);
  SPI.setMISO(PB14);
  SPI.setMOSI(PB15);
  SPI.setSCLK(PB13);
  //SPI.setSSEL(PB12);
  pinMode(rstLora, OUTPUT);
  digitalWrite(rstLora , 1);
  Serial.begin(9600);
  if (!rf95.init())
    Serial.println("init failed");
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
}

void loop()
{
  //Serial.println("11111111111111");
  blink_led();
  if (rf95.available())
  {
    // Should be a message for us now
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    if (rf95.recv(buf, &len))
    {
      digitalWrite(led, HIGH);
      //      RH_RF95::printBuffer("request: ", buf, len);
      Serial.print("got request: ");
      Serial.println((char*)buf);
      //      Serial.print("RSSI: ");
      //      Serial.println(rf95.lastRssi(), DEC);

      // Send a reply
      uint8_t data[] = "And hello back to you1111111111111";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent();
      Serial.println("Sent a reply");
      digitalWrite(led, LOW);
    }
    else
    {
      digitalWrite(rstLora , 0);
      Serial.println("recv failed");
      delay(500);
      digitalWrite(rstLora , 1);
    }
  }
}
//#include <SPI.h>
//#include <LoRa_STM32.h>
//#define rstLora PB11
//#define led PC13
////SPIClass SPI_2(PB15, PB14, PB13, PB12);
//uint32_t intervalBlinkLed = 0;
//#define SS PB12
//#define RST PB11
//#define DI0 PC8
//#define TX_P 17
//#define BAND 433E6
//#define ENCRYPT 0x78
//int counter = 0;
//void setup() {
//  pinMode(led, OUTPUT);
//  SPI.setMISO(PB14);
//  SPI.setMOSI(PB15);
//  SPI.setSCLK(PB13);
//  //SPI.setSSEL(PB12);
//  //  pinMode(rstLora, OUTPUT);
//  //  digitalWrite(rstLora , 1);
//  Serial.begin(9600);
//  while (!Serial);
//  SPI.begin();
//  LoRa.setTxPower(TX_P);
//  LoRa.setSyncWord(ENCRYPT);
//  LoRa.setPins(SS, RST, DI0);
//  Serial.println("LoRa sender");
//
//  if (!LoRa.begin(433E6)) {
//    Serial.println("Starting LoRa failed!");
//    while (1);
//  }
//}
//
//void loop() {
//  blink_led();
//  Serial.print("Sending packet: ");
//  Serial.println(counter);
//
//  // send packet
//  LoRa.beginPacket();
//  LoRa.print("hello ");
//  LoRa.print(counter);
//  LoRa.endPacket();
//
//  counter++;
//
//  delay(5000);
//}
void blink_led() {
  uint16_t tim = 500;
  uint8_t en = digitalRead(led);
  if (millis() - intervalBlinkLed >= tim) {
    digitalWrite(led, !en);
    intervalBlinkLed = millis();
  }
  return;
}
