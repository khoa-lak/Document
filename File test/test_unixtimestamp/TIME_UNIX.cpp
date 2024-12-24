#include "TIME_UNIX.h"
bool CLASS_TIME_UNIXSTAMP::isLeapYear(uint16_t year) {
  if ((year % 4 == 0 && year % 100 != 0 ) || (year  % 400 == 0)) {
    Serial.println("leap year: " + String(year));
    return true;
  }
  else {
    Serial.println("normal year: " + String(year));
    return false;
  }
}
uint32_t CLASS_TIME_UNIXSTAMP::secondsFromEpoch(uint16_t current_year) {
  uint32_t seconds = 0;
  for (uint16_t i = year_epoch ; i < current_year ; i++) {
    if (isLeapYear(i))
      seconds += (leap_year * hour * minute * second);
    else
      seconds += (normal_year * hour * minute * second);
  }
  //Serial.println(seconds);
  return seconds;
}
uint32_t CLASS_TIME_UNIXSTAMP::secondFromFirstYear(uint8_t month, uint16_t current_year) {
  uint32_t second_from_first_year = 0;
  switch (month) {
    case 1: case 3: case 5: case 7: case 8: case 10: case 12:
      second_from_first_year = 31 * second * minute * hour;
      break;
    case 4: case 6: case 9: case 11:
      second_from_first_year = 30 * second * minute * hour;
      break;
    case 2:
      if (isLeapYear(current_year))
        second_from_first_year = 29 * second * minute * hour;
      else
        second_from_first_year = 28 * second * minute * hour;
      break;
  }
  return second_from_first_year;
}
uint8_t CLASS_TIME_UNIXSTAMP::currentMonth(uint32_t &second_sub_epoch, uint16_t current_year) {
  uint8_t month = 1;
  uint32_t second_from_first_year = 0;
  while (second_from_first_year < second_sub_epoch) {
    second_from_first_year = secondFromFirstYear(month, current_year);
    if (second_from_first_year < second_sub_epoch) {
      second_sub_epoch -= second_from_first_year;
      month++;
    }
  }
  //Serial.println(second_from_first_year);
  return month;
}
void CLASS_TIME_UNIXSTAMP::getUnixTime(uint32_t epoch, uint8_t &sec, uint8_t &mins, uint8_t &hrs, uint8_t &day, uint8_t &month, uint16_t &year) {
  uint16_t time_zone = gmt * minute * second;
  uint32_t second_day = hour * minute * second;
  uint32_t second_year = leap_year * second_day;
  epoch += time_zone;
  uint16_t current_year = year_epoch + (epoch / second_year);
  uint32_t second_sub_epoch = epoch - secondsFromEpoch(current_year);
  year = current_year;
  month = currentMonth(second_sub_epoch, current_year);
  day = 1 + (second_sub_epoch / second_day);
  hrs = (second_sub_epoch % second_day) / (minute * second);
  mins = (second_sub_epoch % (minute * second)) / second;
  sec = (second_sub_epoch % second);
  //  Serial.println(year);
  //  Serial.println(month);
  //  Serial.println(day);
  //  Serial.println(hrs);
  //  Serial.println(mins);
  //  Serial.println(sec);
  return;
}
uint32_t CLASS_TIME_UNIXSTAMP::epoch(uint8_t sec, uint8_t mins, uint8_t hrs, uint8_t day, uint8_t month, uint16_t year) {
  //  Serial.println(year);
  //  Serial.println(month);
  //  Serial.println(day);
  //  Serial.println(hrs);
  //  Serial.println(mins);
  //  Serial.println(sec);
  uint32_t second_from_epoch = secondsFromEpoch(year);
  uint32_t second_from_first_year =  0;
  uint8_t m = 1;
  while (m < month) {
    second_from_first_year += secondFromFirstYear(m, year);
    m++;
  }
  //Serial.println(second_from_first_year);
  second_from_epoch += second_from_first_year;
  second_from_epoch += (day - 1) * hour * minute * second;
  second_from_epoch += hrs * minute * second;
  second_from_epoch += mins * second;
  second_from_epoch += sec;
  // Serial.println(second_from_epoch);
  return second_from_epoch;
}
char* CLASS_TIME_UNIXSTAMP::date(uint8_t day, uint8_t month, uint16_t year) {
  uint8_t m = month;
  uint16_t y = (m < 3) ? year - 1 : year;
  m = (m < 3) ? m + 12 : m;
  uint8_t first_year = y % 100;
  uint8_t second_year = y / 100;
  uint8_t order = (day + 13 * (m + 1) / 5 + first_year + first_year / 4 + second_year / 4 + 5 * second_year) % 7;
  char* days[] = {"Saturday", "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday"};
  char* dayOfWeek = days[order];
  return dayOfWeek;
}
void CLASS_TIME_UNIXSTAMP::handleTxtDate(char* txt, uint8_t &sec, uint8_t &mins, uint8_t &hrs, uint8_t &day, uint8_t &month, uint16_t &year) {
  //Serial.println(txt);
  uint8_t len = strlen(txt);
  char txt_temp[len] ;
  strcpy(txt_temp, txt);
  char* token = strtok(txt_temp, "/,:+");
  int count = 0;
  while (token != NULL) {
    switch (count) {
      case 0:
        year = static_cast<uint16_t>(atoi(token));
        break;
      case 1:
        month = static_cast<uint8_t>(atoi(token));
        break;
      case 2:
        day = static_cast<uint8_t>(atoi(token));
        break;
      case 3:
        hrs = static_cast<uint8_t>(atoi(token));
        break;
      case 4:
        mins = static_cast<uint8_t>(atoi(token));
        break;
      case 5:
        sec = static_cast<uint8_t>(atoi(token));
        break;
    }
    token = strtok(NULL, "/,:+");
    count++;
  }
  year = year + 2000;
  return;
}
uint32_t CLASS_TIME_UNIXSTAMP::convertToUnix(char* txt) {
  uint8_t sec = 0;
  uint8_t mins = 0;
  uint8_t hrs = 0;
  uint8_t day = 0;
  uint8_t month = 0;
  uint16_t year = 0;
  //Serial.println(txt);
  handleTxtDate(txt, sec, mins, hrs, day, month, year);
  uint32_t unix = epoch(sec, mins, hrs, day, month, year) - (gmt * minute * second);
  return unix;
}
char* CLASS_TIME_UNIXSTAMP::convertToDate(char* txt) {
  uint8_t sec = 0;
  uint8_t mins = 0;
  uint8_t hrs = 0;
  uint8_t day = 0;
  uint8_t month = 0;
  uint16_t year = 0;
  handleTxtDate(txt, sec, mins, hrs, day, month, year);
  //  Serial.println(year);
  //  Serial.println(month);
  //  Serial.println(day);
  //  Serial.println(hrs);
  //  Serial.println(mins);
  //  Serial.println(sec);
  return date(day, month, year);
}
char* CLASS_TIME_UNIXSTAMP::convertToDay(uint32_t epoch) {
  Serial.println(epoch);
  uint8_t sec = 0;
  uint8_t mins = 0;
  uint8_t hrs = 0;
  uint8_t day = 0;
  uint8_t month = 0;
  uint16_t year = 0;
  getUnixTime(epoch, sec, mins, hrs, day, month, year);
  Serial.println(year);
  Serial.println(month);
  Serial.println(day);
  Serial.println(hrs);
  Serial.println(mins);
  Serial.println(sec);
  year = year % 100;
  char txtDate[21];
  sprintf(txtDate, "%d/%d/%d,%d:%d:%d+%d", year, month, day, hrs, mins, sec, gmt);
  //Serial.println(txtDate);
  char* txt = new char[strlen(txtDate)];
  strcpy(txt, txtDate);
  return txt;
}
