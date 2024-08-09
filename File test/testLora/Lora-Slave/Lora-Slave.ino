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
#define rstLora PB11
#define led PC13
// Singleton instance of the radio driver
//SPIClass SPIClass(PB15, PB14, PB13, PB12);
RH_RF95 rf95(PB12, PC8);

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


}

void loop() {
  //Serial.println("11111111111111");
  delay(5000);
  blink_led();
  Serial.println("Sending to rf95_server");
  // Send a message to rf95_server
  uint8_t data[] = "HelloWorldaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab";
  //uint8_t data[] = "{\"schedules\":[{\"name\":\"NH3\",\"time\":[0,200,400,600,800,1000,1200,1400]},{\"name\":\"DO\",\"time\":[]},{\"name\":\"CA\",\"time\":[0,200,400,600,800,1000,1200,1400]},"
  //                  "{\"name\":\"KH\",\"time\":[0,200,400,600,800,1000,1200,1400]},{\"name\":\"MG\",\"time\":[0,200,400,600,800,1000,1200,1400]},{\"name\":\"NO2\",\"time\":[0,200,400,600,800,1000,1200,1400]},{\"name\":\"NO3\",\"time\":[480]},"
  //                  "{\"name\":\"H2S\",\"time\":[480]},{\"name\":\"GH\",\"time\":[480]},{\"name\":\"K\",\"time\":[480]},{\"name\":\"CL\",\"time\":[480]},"
  //                  "{\"name\":\"P\",\"time\":[480]},{\"name\":\"PH\",\"time\":[0,200,400,600,800,1000,1200,1400]},{\"name\":\"ACIDITY\",\"time\":[]},{\"name\":\"SALINITY\",\"time\":[]}]}";

  rf95.send(data, sizeof(data));

  rf95.waitPacketSent();
  // Now wait for a reply
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  if (rf95.waitAvailableTimeout(3000))
  {
    // Should be a reply message for us now
    if (rf95.recv(buf, &len))
    {
      Serial.print("got reply: ");
      Serial.println((char*)buf);
      //      Serial.print("RSSI: ");
      //      Serial.println(rf95.lastRssi(), DEC);
    }
    else
    {
      digitalWrite(rstLora , 0);
      Serial.println("recv failed");
      delay(500);
      digitalWrite(rstLora , 1);
    }
  }
  else
  {
    Serial.println("No reply, is rf95_server running?");
  }
  delay(400);
}
//#include <SPI.h>
//#include <LoRa_STM32.h>
//#define rstLora PB11
//#define led PC13
////SPIClass SPI_2(PB15, PB14, PB13, PB12);
//uint32_t intervalBlinkLed = 0;
//#define SS PB12
//#define RST PB11
//#define DI0 PB10
//#define TX_P 17
//#define BAND 433E6
//#define ENCRYPT 0x78
//void setup() {
//  pinMode(led, OUTPUT);
//  SPI.setMISO(PB14);
//  SPI.setMOSI(PB15);
//  SPI.setSCLK(PB13);
//  //SPI.setSSEL(PB12);
////  pinMode(rstLora, OUTPUT);
////  digitalWrite(rstLora , 1);
//  Serial.begin(9600);
//  while (!Serial);
//  //SPI_2.begin();
//  LoRa.setTxPower(TX_P);
//  LoRa.setSyncWord(ENCRYPT);
//  LoRa.setPins(SS, RST, DI0);
//  Serial.println("LoRa Receiver");
//
//  if (!LoRa.begin(433E6)) {
//    Serial.println("Starting LoRa failed!");
//    while (1);
//  }
//}
//
//void loop() {
//  // try to parse packet
//  blink_led();
//  int packetSize = LoRa.parsePacket();
//  if (packetSize) {
//    // received a packet
//    Serial.print("Received packet '");
//
//    // read packet
//    while (LoRa.available()) {
//      Serial.print((char)LoRa.read());
//    }
//
//    // print RSSI of packet
//    Serial.print("' with RSSI ");
//    Serial.println(LoRa.packetRssi());
//  }
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
