#ifndef SDCard_h
#define SDCard_h
#include <SPI.h>
#include <SD.h>
class SDCard_ {
  private:
  public:
    void sdSetup() {
      SPI.begin();
      if (!SD.begin(PA4)) {
        return;
      }
      return;
    }
    void hexFile(char* file) {
      File myFile;
      myFile = SD.open("FileData.txt", FILE_READ);
      if (myFile) {
        //char file[9000] = {'0'};
        uint16_t i = 0;
        while (myFile.available()) {
          file[i] = (char)myFile.read();
          i++;
        }
        uint16_t len = strlen(file);
        Serial.println(len);
        Serial.println(file);
        Serial.println("====================");

//        if (len > 0) {
//          char* fileHex = new char[len + 1];
//          strcpy(fileHex, file);
//          myFile.close();
//          return fileHex;
//        }
        myFile.close();
        return ;
      }
      return ;
    }
};
#endif
