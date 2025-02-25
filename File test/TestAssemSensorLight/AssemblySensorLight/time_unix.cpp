#include "time_unix.h"
bool UnixStamp::isLeapYear(uint16_t year) {
  if ((year % 4 == 0 && year % 100 != 0 ) || (year  % 400 == 0))
    return true;
  else
    return false;
}
uint32_t UnixStamp::secondsFromEpoch(uint16_t current_year) {
  uint32_t seconds = 0;
  for (uint16_t i = yearEpoch ; i < current_year ; i++) {
    if (isLeapYear(i))
      seconds += (leapYear * hour * minute * second);
    else
      seconds += (normalYear * hour * minute * second);
  }
  return seconds;
}
uint32_t UnixStamp::secondFromFirstYear(uint8_t month, uint16_t current_year) {
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
uint8_t UnixStamp::currentMonth(uint32_t &second_sub_epoch, uint16_t current_year) {
  uint8_t month = 1;
  uint32_t second_from_first_year = 0;
  while (second_from_first_year < second_sub_epoch) {
    second_from_first_year = secondFromFirstYear(month, current_year);
    if (second_from_first_year < second_sub_epoch) {
      second_sub_epoch -= second_from_first_year;
      month++;
    }
  }
  return month;
}
void UnixStamp::getUnixTime(uint32_t epoch, uint8_t &sec, uint8_t &mins, uint8_t &hrs, uint8_t &day, uint8_t &month, uint16_t &year) {
  uint16_t time_zone = gmt * minute * second;
  uint32_t second_day = hour * minute * second;
  uint32_t second_year = leapYear * second_day;
  epoch += time_zone;
  uint16_t current_year = yearEpoch + (epoch / second_year);
  uint32_t second_sub_epoch = epoch - secondsFromEpoch(current_year);
  year = current_year;
  month = currentMonth(second_sub_epoch, current_year);
  day = 1 + (second_sub_epoch / second_day);
  hrs = (second_sub_epoch % second_day) / (minute * second);
  mins = (second_sub_epoch % (minute * second)) / second;
  sec = (second_sub_epoch % second);
  return;
}
uint32_t UnixStamp::epoch(uint8_t sec, uint8_t mins, uint8_t hrs, uint8_t day, uint8_t month, uint16_t year) {
  uint32_t second_from_epoch = secondsFromEpoch(year);
  uint32_t second_from_first_year =  0;
  uint8_t m = 1;
  while (m < month) {
    second_from_first_year += secondFromFirstYear(m, year);
    m++;
  }
  second_from_epoch += second_from_first_year;
  second_from_epoch += (day - 1) * hour * minute * second;
  second_from_epoch += hrs * minute * second;
  second_from_epoch += mins * second;
  second_from_epoch += sec;
  return second_from_epoch;
}
char* UnixStamp::date(uint8_t day, uint8_t month, uint16_t year) {
  const char* days[] = {"Sat", "Sun", "Mon", "Tue", "Wed", "Thu", "Fri"};
  uint8_t m = month;
  uint16_t y = (m < 3) ? year - 1 : year;
  m = (m < 3) ? m + 12 : m;
  uint8_t first_year = y % 100;
  uint8_t second_year = y / 100;
  uint8_t order = (day + 13 * (m + 1) / 5 + first_year + first_year / 4 + second_year / 4 + 5 * second_year) % 7;
  char* dayOfWeek = new char[strlen(days[order]) + 1];
  strcpy(dayOfWeek, days[order]);
  return dayOfWeek;
}
void UnixStamp::handleTxtDate(char* txt, uint8_t &sec, uint8_t &mins, uint8_t &hrs, uint8_t &day, uint8_t &month, uint16_t &year) {
  char input[strlen(txt) + 1];
  strcpy(input, txt);
  char* token = strtok(input, "/,:+");
  uint8_t count = 0;
  while (token != NULL) {
    //Serial.println(String(count) + token);
    switch (count) {
      case 0:
        year = atoi(token);
        break;
      case 1:
        month = atoi(token);
        break;
      case 2:
        day = atoi(token);
        break;
      case 3:
        hrs = atoi(token);
        break;
      case 4:
        mins = atoi(token);
        break;
      case 5:
        sec = atoi(token);
        break;
    }
    token = strtok(NULL, "/,:+");
    count++;
  }
  // Serial.println("year: " + String(year) + " -tháng: " + String(month) + " -ngày: " + String(day) + " -giờ: " + String(hrs) + " -phút: " + String(mins)+ " -giây: " + String(sec));
  year = year + 2000;
  return;
}
uint32_t UnixStamp::convertToUnix(char* txt) {
  if (txt == nullptr)
    return 0;
  uint8_t sec = 0;
  uint8_t mins = 0;
  uint8_t hrs = 0;
  uint8_t day = 0;
  uint8_t month = 0;
  uint16_t year = 0;
  handleTxtDate(txt, sec, mins, hrs, day, month, year);
  uint32_t unix = epoch(sec, mins, hrs, day, month, year) - gmt * minute * second;
  return unix;
}
char* UnixStamp::convertToDate(char* txt, uint32_t &hours) {
  uint8_t sec, mins, hrs, day, month = 0;
  uint16_t year = 0;
  handleTxtDate(txt, sec, mins, hrs, day, month, year);
  hours = hrs * minute * second + mins * second + sec;
  if (day % 2 != 0)
    hours += 86400;
  return date(day, month, year);
}
char* UnixStamp::convertToDay(uint32_t epoch) {
  uint8_t sec, mins, hrs, day, month = 0;
  uint16_t year = 0;
  getUnixTime(epoch, sec, mins, hrs, day, month, year);
  year = year % 100;
  char txtDate[21];
  sprintf(txtDate, "%d/%d/%d,%d:%d:%d+%d", year, month, day, hrs, mins, sec, gmt);
  char* txt = new char[strlen(txtDate)];
  strcpy(txt, txtDate);
  return txt;
}
