#include <OneWire.h>

// OneWire DS18S20, DS18B20, DS1822 Temperature Example
//
// http://www.pjrc.com/teensy/td_libs_OneWire.html
//
// The DallasTemperature library can do all this work for you!
// https://github.com/milesburton/Arduino-Temperature-Control-Library

#define LED PC13
OneWire  ds(PA7);  // on pin 10 (a 4.7K resistor is necessary)
float f_temp;
void setup(void) {
  pinMode(LED, OUTPUT);
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
}

void loop(void) {
 // Blink_Led();
  Func_Temp();
  //Serial.println("lalalla");
}

void Blink_Led() {
  for (int i = 0; i < 10; i++) {
    digitalWrite(LED, 1);
    delay(200);
    digitalWrite(LED, 0);
    delay(200);
  }
}
bool Func_Temp() {
  byte i;
  byte present = 0;
  byte type_s;
  byte data[9];
  byte addr[8];
  float celsius, fahrenheit;
  if ( !ds.search(addr)) {
    ds.reset_search();
    return 0;
  }
  if (OneWire::crc8(addr, 7) != addr[7]) {
    return 0;
  }
  switch (addr[0]) {
    case 0x10:
      type_s = 1;
      break;
    case 0x28:
      type_s = 0;
      break;
    case 0x22:
      type_s = 0;
      break;
    default:
      return 0;
  }
  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);        // start conversion, with parasite power on at the end
  delay(1000);     // maybe 750ms is enough, maybe not
  present = ds.reset();
  ds.select(addr);
  ds.write(0xBE);         // Read Scratchpad
  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
  }
  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
  }
  celsius = (float)raw / 16.0;
  fahrenheit = celsius * 1.8 + 32.0;
  //  if (celsius > 35)
  //    digitalWrite(CFPin_Fan, 0);
  //  else
  //    digitalWrite(CFPin_Fan, 1);
  if (celsius > 0) {
    f_temp = celsius;
    Serial.println(f_temp);
    return 1;
  } else {
    return 0;
  }
}
