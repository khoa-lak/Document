// rf95_server.pde
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messageing server
// with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// reliability, so you should only use RH_RF95  if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example rf95_client
// Tested with Anarduino MiniWirelessLoRa

#include <IWatchdog.h>
#include <SPI.h>
#include <RH_RF95.h>
#define rstLora PC6
#define d0Lora  PC7
#define nssLora PB12
#define mosi    PB15
#define miso    PB14
#define clk     PB13
#define led PD2
// Singleton instance of the radio driver
//SPIClass definePin(PB15, PB14, PB13, PB12);
RH_RF95 rf95(nssLora, d0Lora);
uint32_t intervalBlinkLed = 0;
uint32_t delayLora = millis();
void setup()
{
  IWatchdog.begin(10000000);
  pinMode(led, OUTPUT);
  SPI.setMISO(miso);
  SPI.setMOSI(mosi);
  SPI.setSCLK(clk);
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
  blink_led();
  if (millis() - delayLora > 5000) {

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
      { digitalWrite(led, 0);
        Serial.print("got reply: ");
        Serial.println((char*)buf);
        //      Serial.print("RSSI: ");
        //      Serial.println(rf95.lastRssi(), DEC);
      }
      else
      {
        digitalWrite(led, 1);
        digitalWrite(rstLora , 0);
        Serial.println("recv failed");
        delay(500);
        digitalWrite(rstLora , 1);
      }
    }
    else
    {
      digitalWrite(led, 1);
      Serial.println("No reply, is rf95_server running?");
    }
    delay(2000);
    delayLora = millis();
  }
}

void blink_led() {
  uint16_t tim = 500;
  uint8_t en = digitalRead(led);
  if (millis() - intervalBlinkLed >= tim) {
    IWatchdog.reload();
    digitalWrite(led, !en);
    intervalBlinkLed = millis();
  }
  return;
}
