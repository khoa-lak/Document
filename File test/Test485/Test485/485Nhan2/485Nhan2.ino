#include <ModbusRtu.h>
#include <SoftwareSerial.h>
#define LED 13

//SoftwareSerial Serial_Slave(3,5);

uint16_t au16data[16] = {
    3, 1415, 9265, 4, 2, 7182, 28182, 8, 0, 0, 0, 0, 0, 0, 1, 1};

Modbus slave(1,Serial,4); // this is slave @1 and RS-232 or USB-FTDI

void setup()
{
  pinMode(LED, OUTPUT);
  Serial.begin( 19200, SERIAL_8N1 ); // baud-rate at 19200
  slave.start();
  digitalWrite(LED, HIGH);
}

void loop()
{
  slave.poll(au16data, 16);
  if (au16data[0] == 1)
  {
    digitalWrite(LED, LOW);
  }
  else
  {
    digitalWrite(LED, HIGH);
  }
}
