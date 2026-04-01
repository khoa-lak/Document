#ifndef general_function_h
#define general_function_h
#include <Arduino.h>
class GeneralFunction {
public:
  // void deletePointer(char*& pointer);
  // void copyString(char* dest, const char* src, uint16_t max_len);
  // void wrapperPointer(char*& msg1, const char* msg2);
  // void wrapperStatus(const char*& msg1, const char* msg2);
  // void removeWhitespace(const char* msg_input, char msg[], uint16_t length);
  // bool compareTwoCharArray(const char* msg1, const char* msg2);
  static void copyString(char* dest, const char* src, uint16_t max_len);
  static void wrapperStatus(char* dest, const char* src);
  static void removeWhitespace(const char* msg_input, char msg[], uint16_t length);
  static bool compareTwoCharArray(const char* msg1, const char* msg2);
};
#endif
