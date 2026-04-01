#include "GeneralFunction.h"
void GeneralFunction::deletePointer(char*& pointer) {
  if (pointer != nullptr) {
    delete[] pointer;
    pointer = nullptr;
  }
  return;
}
void GeneralFunction::wrapperPointer(char*& msg1, const char* msg2) {
  if (msg1 != nullptr) {
    delete[] msg1;
    msg1 = nullptr;
  }
  msg1 = new char[strlen(msg2) + 1];
  strcpy(msg1, msg2);
  return;
}
void GeneralFunction::wrapperStatus(const char*& msg1, const char* msg2) {
  if (msg2 == nullptr) {
    msg1 = nullptr;
    return;
  }
  msg1 = (char*)msg2;
  return;
}
bool GeneralFunction::compareTwoCharArray(const char* msg1, const char* msg2) {
  return (strcmp(msg1, msg2) == 0);
}
void GeneralFunction::removeWhitespace(const char* msg_input, char msg[], uint16_t length) {
  if (msg_input == nullptr) {
    Serial.println("Input is null");
    msg[0] = '\0';
    return;
  }

  size_t j = 0;
  for (size_t i = 0; msg_input[i] != '\0' && j < length; i++) {
    if (!isspace(msg_input[i])) {
      msg[j++] = msg_input[i];
    }
  }
  msg[j] = '\0';
  return;
}
