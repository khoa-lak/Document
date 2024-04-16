#include <SoftwareSerial.h>
#include <ModbusRtu.h>

uint8_t u8state;

SoftwareSerial mySerial(PC11, PC10); // RX, TX
Modbus master(0, mySerial);
modbus_t telegram;
unsigned long u32wait;
#define En485 PA15
void setup() {
  mySerial.begin(9600);
  master.start();
  master.setTimeOut( 2000 ); // if there is no answer in 2000 ms, roll over
  u32wait = millis() + 1000;
  u8state = 0;
  pinMode(En485, OUTPUT);
}

void loop() {
  uint16_t au16data[6] = {0};
  switch ( u8state ) {
    case 0:
      if (millis() > u32wait) u8state++; // wait state
      break;
    case 1:
      telegram.u8id = 1; // slave address
      telegram.u8fct = 3; // function code (this one is registers read)
      telegram.u16RegAdd = 0; // start address in slave
      telegram.u16CoilsNo = 6; // number of elements (coils or registers) to read
      telegram.au16reg = au16data; // pointer to a memory array in the Arduino
      digitalWrite(En485, 1);
      master.query( telegram ); // send query (only once)
      u8state++;
      delay(5);
      break;
    case 2:
      digitalWrite(En485, 0);
      master.poll(); // check incoming messages
      if (master.getState() == COM_IDLE) {
        u8state = 0;
        u32wait = millis() + 10;
        for (int i = 0 ; i < 6 ; i++) {
          Serial.print(au16data[i], HEX); //Or do something else!
          Serial.print(" ");
        }
        Serial.println(" ");
      }
      break;
  }
}
