// rf95_reliable_datagram_server.pde
// -*- mode: C++ -*-
// Example sketch showing how to create a simple addressed, reliable messaging server
// with the RHReliableDatagram class, using the RH_RF95 driver to control a RF95 radio.
// It is designed to work with the other example rf95_reliable_datagram_client
// Tested with Anarduino MiniWirelessLoRa

#include <RHReliableDatagram.h>
#include <RH_RF95.h>
#include <SPI.h>
#define SERVER_ADDRESS 1
#define CLIENT_ADDRESS 9


// Singleton instance of the radio driver
RH_RF95 driver;

// Class to manage message delivery and receipt, using the driver declared above
RHReliableDatagram manager(driver, SERVER_ADDRESS);

void setup()
{
  Serial.begin(9600);
  if (!manager.init())
    Serial.println("init failed");
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
}

uint8_t data[4]  ;
// Dont put this on the stack:
uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];

String inputString = "";         // a String to hold incoming data
String stringComplete = "" ; // whether the string is complete

void loop()
{
  manager.sendtoWait(data, sizeof(data), CLIENT_ADDRESS);
  if (manager.available())
  {
    // Wait for a message addressed to us from the client
    uint8_t len = sizeof(buf);
    uint8_t from;
    if (manager.recvfromAck(buf, &len, 2000, &from))
    {
      Serial.print("got request from : 0x");
      Serial.print(from);
      Serial.print(": ");
      Serial.println((char*)buf);
      stringComplete = (char*)buf;
    }
    if (stringComplete == "done") {
      delay(200);
      for (int i = 0 ; i < 11 ; i++) {
        data[i] = 0;
      }
    }

  }
}
  void serialEvent() {
    int i = 0;
    while (Serial.available()) {
      // get the new byte:
      char inChar = (char)Serial.read();
      if (inChar != '\n') {
        data[i] = inChar;

        i =i+1;
      }
      if (inChar == '\n') {

      }
    }

  }
