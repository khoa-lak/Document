/*
   EEPROM Read

   Reads the value of each byte of the EEPROM and prints it
   to the computer.
   This example code is in the public domain.
*/

#include <EEPROM.h>
char pond[] = "pond/pond-d7fd097fc29c49058067a909abdf4999";
// start reading from the first byte (address 0) of the EEPROM
int address = 0;
byte value;

void setup() {
  // initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  //  EEPROM.write(0, 5);
  //  EEPROM.write(1, 177);
  //  EEPROM.write(5, 'k');
  for (int i = 0 ; i < 255 ; i++) {
    if (i < strlen(pond))
      EEPROM.write(i,  pond[i]);
    else
      EEPROM.write(i, 0);
  }
  delay(5);

  Serial.println("Read byte from EEPROM memory...");
  // Đọc giá trị từ EEPROM và in ra Serial Monitor
  for (int i = 0 ; i < 255 ; i++) {
    //  value = EEPROM.read(0);
    //  Serial.print(value, DEC);
    byte data = EEPROM.read(i);
    Serial.print(char(data));
  }

}

void loop() {
  delay(500);
  Serial.println("!!!!!!!!!!!");
}
