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
// #define rstLora PB11
// #define d0Lora  PC8
// #define nssLora PB12
// #define mosi    PB15
// #define miso    PB14
// #define clk     PB13
#define rstLora PB11
#define d0Lora  PB10
#define nssLora PB12
#define mosi    PB15
#define miso    PB14
#define clk     PB13
#define led PC13
// Singleton instance of the radio driver
//SPIClass definePin(PB15, PB14, PB13, PB12);
RH_RF95 rf95(nssLora, d0Lora);
uint32_t intervalBlinkLed = 0;
uint32_t delayLora = millis();
void setup()
{
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Started");
  IWatchdog.begin(10000000);
  pinMode(led, OUTPUT);
  SPI.setMISO(miso);
  SPI.setMOSI(mosi);
  SPI.setSCLK(clk);
  //SPI.setSSEL(PB12);
  pinMode(rstLora, OUTPUT);
  digitalWrite(rstLora , 1);

  if (!rf95.init())
    Serial.println("init failed");
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
}

void loop()
{
  blink_led();
  if (millis() - delayLora > 5000) {
    //    String data =
    //      "\"13:52:30 - 111111111111111111111111111111111111111111111111111111\"\r\n"
    //      "\"13:53:30 - 222222222222222222222222222222222222222222222222222222\"\r\n"
    //      "\"13:54:30 - 333333333333333333333333333333333333333333333333333333\"\r\n"
    //      "\"13:54:30 - 333333333333333333333333333333333333333333333333333333\"\r\n"
    //      "\"13:54:30 - 333333333333333333333333333333333333333333333333333333\"\r\n"
    //      "\"13:54:30 - 333333333333333333333333333333333333333333333333333333\"\r\n"
    //      "\"13:54:30 - 333333333333333333333333333333333333333333333333333333\"\r\n"
    //      "\"13:54:30 - 333333333333333333333333333333333333333333333333333333\"\r\n"
    //      "\"13:54:30 - 333333333333333333333333333333333333333333333333333333\"\r\n"
    //      "\"13:54:30 - 333333333333333333333333333333333333333333333333333333\"\r\n"
    //      "\"13:54:30 - 333333333333333333333333333333333333333333333333333333\"\r\n"
    //      "\"13:54:30 - 333333333333333333333333333333333333333333333333333333\"\r\n"
    //      "\"13:54:30 - 333333333333333333333333333333333333333333333333333333\"\r\n"
    //      "\"13:54:30 - 333333333333333333333333333333333333333333333333333333\"\r\n"
    //      "\"13:54:30 - 333333333333333333333333333333333333333333333333333333\"\r\n"
    //      "\"13:54:30 - 333333333333333333333333333333333333333333333333333333\"\r\n"
    //      "\"13:54:30 - 333333333333333333333333333333333333333333333333333333\"\r\n"
    //      "\"13:54:30 - 44444444444444444444444\"\r\n";
    //    sendDataOverLoRa(data);
    Serial.println("Sending to rf95_server");
    // Send a message to rf95_server
    uint8_t data[] = "HelloWorld";
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
    delayLora = millis();
  }
}

void sendDataOverLoRa(String data) {
  // Chia chuỗi thành các hàng
  uint8_t cmdStart[] = "START SEND LOGS: \r\n";
  uint8_t cmdEnd[] = "END SEND LOGS\r\n";
  uint16_t startIndex = 0;
  uint8_t lineCount = 0;
  uint8_t totalLines = 0; // Đếm số hàng
  rf95.send(cmdStart, sizeof(cmdStart));
  rf95.waitPacketSent();

  while (startIndex < data.length()) {
    // Tìm vị trí kết thúc dòng
    uint16_t endIndex = data.indexOf('\n', startIndex);
    if (endIndex == -1) endIndex = data.length(); // Nếu không có \n, lấy đến cuối

    // Lấy một hàng
    String line = data.substring(startIndex, endIndex);
    if (line.length() == 0) break; // Thoát nếu dòng rỗng

    // Thêm header
    //    uint8_t* packet = new uint8_t[line.length()];
    //    strcpy(packet, (uint8_t*)line.c_str());
    uint8_t packet[RH_RF95_MAX_MESSAGE_LEN] = {0};
    memcpy(packet, line.c_str(),  line.length() > RH_RF95_MAX_MESSAGE_LEN ? RH_RF95_MAX_MESSAGE_LEN :  line.length());
    packet[line.length() - 1] = '\n';
    rf95.send(packet, RH_RF95_MAX_MESSAGE_LEN);
    rf95.waitPacketSent();
    startIndex = endIndex + 1; // Chuyển sang dòng tiếp theo
    lineCount++;
    delay(50); // Đợi để tránh xung đột
  }
  rf95.send(cmdEnd, sizeof(cmdEnd));
  rf95.waitPacketSent();
  delay(2000);
  Serial.println("Hoan tat gui du lieu!");
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
