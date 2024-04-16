// rf95_reliable_datagram_client.pde
// -*- mode: C++ -*-
// Example sketch showing how to create a simple addressed, reliable messaging client
// with the RHReliableDatagram class, using the RH_RF95 driver to control a RF95 radio.
// It is designed to work with the other example rf95_reliable_datagram_server
// Tested with Anarduino MiniWirelessLoRa

#include <RHReliableDatagram.h>
#include <RH_RF95.h>
#include <SPI.h>


#define SERVER_ADDRESS 1
#define CLIENT_ADDRESS 9
// Singleton instance of the radio driver
RH_RF95 driver;

// Class to manage message delivery and receipt, using the driver declared above
RHReliableDatagram manager(driver, CLIENT_ADDRESS);
void (*runningFunc)();
void setup()
{
  Serial.begin(9600);
  if (!manager.init())
    Serial.println("init failed");
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
   pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  runningFunc = &resetFunc;
}

uint8_t data[] = "done";
// Dont put this on the stack:
uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];

void loop()
{
  runningFunc();

}
void resetFunc() {
  digitalWrite(5, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);
  digitalWrite(8, HIGH);
  runningFunc = &hamNhan;
}

void hamNhan() {
  if (manager.available()) {
    uint8_t len = sizeof(buf);
    uint8_t from;
    if (manager.recvfromAckTimeout(buf, &len, &from))
    {
      Serial.println((char*)buf);
      runningFunc = &hamOnOff;
    }
    else
    {
      runningFunc = &hamNhan;
    }
  }
   
}

void hamGui() {
 
  if (!manager.sendtoWait(data, sizeof(data), SERVER_ADDRESS))
  {
    runningFunc = &hamNhan;
  }
  else {
    runningFunc = &hamGui;
  }
  
}
void hamOnOff() {
  if ((char)buf[0] == '1') {
    digitalWrite(5, LOW);
  }
  else if ((char)buf[0] == '0') {
    digitalWrite(5, HIGH);
  }

  if ((char)buf[1] == '1') {
    digitalWrite(6, LOW);
  }
  else if ((char)buf[1] == '0') {
    digitalWrite(6, HIGH);
  }
    if ((char)buf[2] == '1') {
    digitalWrite(7, LOW);
  }
  else if ((char)buf[2] == '0') {
    digitalWrite(7, HIGH);
  }
    if ((char)buf[3] == '1') {
    digitalWrite(8, LOW);
  }
  else if ((char)buf[3] == '0') {
    digitalWrite(8, HIGH);
  }
  delay(200);
  runningFunc = &hamGui;
 
}
