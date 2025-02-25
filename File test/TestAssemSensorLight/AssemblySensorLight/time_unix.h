#ifndef time_unix_h
#define time_unix_h
#include <Arduino.h>
class UnixStamp {
  private:
    uint8_t second = 60;
    uint8_t minute = 60;
    uint8_t hour = 24;
    uint16_t normalYear = 365;
    uint16_t leapYear = 366;
    uint8_t gmt = 7;
    const uint16_t yearEpoch = 1970;
    bool isLeapYear(uint16_t year) ;
    char* date(uint8_t day, uint8_t month, uint16_t year) ;
    uint8_t currentMonth(uint32_t &second_sub_epoch, uint16_t current_year);
    uint32_t secondsFromEpoch(uint16_t current_year) ;
    uint32_t secondFromFirstYear(uint8_t month, uint16_t current_year) ;
    uint32_t epoch(uint8_t sec, uint8_t mins, uint8_t hrs, uint8_t day, uint8_t month, uint16_t year) ;
    void getUnixTime(uint32_t epoch, uint8_t &sec, uint8_t &mins, uint8_t &hrs, uint8_t &day, uint8_t &month, uint16_t &year);
    void handleTxtDate(char* date, uint8_t &sec, uint8_t &mins, uint8_t &hrs, uint8_t &day, uint8_t &month, uint16_t &year);
  public:
    char* convertToDate(char* txt,uint32_t &hours) ;
    char* convertToDay(uint32_t epoch) ;
    uint32_t convertToUnix(char* txt);
};
#endif
