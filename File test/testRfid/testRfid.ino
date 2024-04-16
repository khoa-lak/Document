#include <SPI.h>
#include <MFRC522.h>

#define rst 6
#define ss 10

MFRC522 mfrc522(ss, rst);

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
}

void loop() {
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }
mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
}
