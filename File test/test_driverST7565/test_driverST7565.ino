
#include "ST7565.h"
//1-CS, 2-RST, 3-A0, 4-SCK, 5-SDA/MOSI, 6-VCC+3V, 7-Gnd, 8-Led-, 9-Led+3V
// Define ST7565 LCD pin connections
#define PIN_SCK     PA5   // SCK pin 
#define PIN_MOSI    PA7   // MOSI pin
#define PIN_CS      PA4   // CS pin
#define PIN_RST     PA3   // RST pin
#define PIN_A00      PA2   // A0 pin
ST7565 lcd = ST7565(PIN_MOSI, PIN_SCK, PIN_A00, PIN_RST, PIN_CS);
void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ;
  }
  lcd.begin(0x64);
  lcd.display();
  lcd.clear();
  lcd.drawstring(5, 0, "IOTOOM VN");
  lcd.drawstring(0, 1, "VIP PRO NUM 1 VN");
  lcd.drawstring(0, 2, "MAY DO CHI SO");
  lcd.drawstring(0, 3, "DRUM FILTER");
  lcd.drawstring(0, 4, "BIO CYCLO");
  lcd.drawstring(0, 5, "XE CHAY DUOI NUOC");
  lcd.display();
  delay(2000);
  lcd.clear();

}

void loop() {

}
