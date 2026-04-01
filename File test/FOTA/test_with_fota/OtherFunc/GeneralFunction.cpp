#include "GeneralFunction.h"
// void GeneralFunction::deletePointer(char*& pointer) {
//   if (pointer != nullptr) {
//     delete[] pointer;
//     pointer = nullptr;
//   }
//   return;
// }
void GeneralFunction::copyString(char* dest, const char* src, uint16_t max_len) {
  if (dest == nullptr) return;
  if (src == nullptr) {
    dest[0] = '\0';  // Chuỗi rỗng nếu nguồn null
    return;
  }
  // Copy an toàn, đảm bảo không tràn bộ nhớ
  strncpy(dest, src, max_len - 1);
  dest[max_len - 1] = '\0';  // Luôn đảm bảo ký tự kết thúc
}
// void GeneralFunction::wrapperPointer(char*& dest, const char* src) {
//   if (dest != nullptr) {
//     delete[] dest;
//     dest = nullptr;
//   }
//   if (src != nullptr) {
//     dest = new char[strlen(src) + 1];
//     strcpy(dest, src);
//   }
// }
void GeneralFunction::wrapperStatus(char* dest, const char* src) {
  if (dest == nullptr) return;
  if (src == nullptr) {
    // Không làm gì hoặc gán rỗng tùy logic, ở đây giữ nguyên trạng thái cũ nếu src null
    return;
  }
  // Giả sử current_state trong struct SettingDevice có kích thước 20
  strncpy(dest, src, 19);
  dest[19] = '\0';
}
bool GeneralFunction::compareTwoCharArray(const char* msg1, const char* msg2) {
  if (msg1 == nullptr || msg2 == nullptr) return false;
  return (strcmp(msg1, msg2) == 0);
}
void GeneralFunction::removeWhitespace(const char* msg_input, char msg[], uint16_t length) {
  if (msg_input == nullptr) {
    msg[0] = '\0';
    return;
  }

  size_t j = 0;
  for (size_t i = 0; msg_input[i] != '\0' && j < length - 1; i++) {
    if (!isspace(msg_input[i])) {
      msg[j++] = msg_input[i];
    }
  }
  msg[j] = '\0';
  return;
}
