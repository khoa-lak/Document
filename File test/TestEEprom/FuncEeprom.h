#include <EEPROM.h>

uint8_t _takeData[12];
char _sendData[12] = {1, 'a', 3, 2, 5, 2, 7, 8, 9, 0, 11, 12};
void FuncTakeData() {
  for (int i = 0 ; i < 12 ; i++) {
    _takeData[i]  = EEPROM.read(i);
    Serial.println(EEPROM.read(i));
  }
}

void FuncSendData() {
  for (int i = 0 ; i < 12 ; i++) {
    EEPROM.write(i, _sendData[i]);
    Serial.println(EEPROM.read(i));
  }
}
