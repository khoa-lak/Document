#include "SIMA7xxx.h"
#include <time.h>
HardwareSerial SerialAT(UART_RX, UART_TX);
TinyGsm modem(SerialAT);
TinyGsmClient client(modem);
PubSubClient mqtt(client);
SimA7xxx *SimA7xxx::instance = nullptr;
void SimA7xxx::staticCallback(char *topic, uint8_t *payload, unsigned int length) {
  if (instance) {
    instance->mqttCallback(topic, payload, length);
  }
}
void SimA7xxx::setupSim() {
  pinMode(UART_PEN, OUTPUT);
  digitalWrite(UART_PEN, HIGH);
  instance = this;
  rstVariable();
  return;
}
void SimA7xxx::rstVariable() {
  pRunningConnect = &SimA7xxx::configSim;
  return;
}
bool SimA7xxx::initSim() {
  bool done = (this->*pRunningConnect)();
  return done;
}
bool SimA7xxx::configSim() {
  uint32_t baud = 0;
  baud = TinyGsmAutoBaud(SerialAT);
  if (!baud) {
    Serial.println(F(" Module does not respond!"));
    delay(500);
  } else {
    Serial.println("baudrate la: " + String(baud));
    SerialAT.begin(baud);
    SerialAT.setTimeout(300);
    pRunningConnect = &SimA7xxx::connectSim;
    //pRunningConnect = &SimA7xxx::connectDone;
  }
  return false;
}
bool SimA7xxx::connectSim() {
  if (!modem.init()) {
    Serial.println("Modem init failed");
    return false;
  }
  if (!modem.waitForNetwork(60000)) {  // Thay đổi: Sử dụng waitForNetwork() thay vì AT+CSQ, AT+CREG, v.v.
    Serial.println("Network connection failed");
    return false;
  }
  Serial.println("Signal quality: " + String(modem.getSignalQuality()));
  if (!modem.gprsConnect("internet")) {  // Thay đổi: Sử dụng gprsConnect() với APN (thay "internet" bằng APN thực tế nếu cần).
    Serial.println("GPRS connect failed");
    return false;
  }
  Serial.println("GPRS connected: " + String(modem.isGprsConnected()));
  pRunningConnect = &SimA7xxx::connectMQTT;
  return false;
}

bool SimA7xxx::connectMQTT() {
  mqtt.setServer(HOST, PORT);                                   // Thay đổi: Thiết lập server MQTT bằng PubSubClient (lưu ý: Port 8883 cho TLS; PubSubClient cần hỗ trợ TLS thêm nếu yêu cầu).
  mqtt.setKeepAlive(300);                                       // Thay đổi: Thiết lập keep-alive.
  mqtt.setCallback(staticCallback);                             // Thay đổi: Sử dụng static wrapper thay vì lambda để tránh lỗi compile.
  char *willMsg = json.msgStatus(MACHINE_STATUS_DISCONNECTED);  // Thay đổi: Chuẩn bị will message.
  mqtt.setBufferSize(MAX_MSG_SIZE);
  bool connected = mqtt.connect(DEVICE_ID, USER, PASSWORD, TOPIC_DEVICE, 1, false, willMsg);  // Thay đổi: Kết nối MQTT với credentials và will.
  if (connected) {
    pRunningConnect = &SimA7xxx::subTopicDevice;
    return false;
  } else {
    Serial.println("MQTT connect failed, state: " + String(mqtt.state()));
    pRunningConnect = &SimA7xxx::disConnectMQTT;
    return false;
  }
}

bool SimA7xxx::subTopicDevice() {
  if (mqtt.subscribe(TOPIC_DEVICE)) {
    pRunningConnect = &SimA7xxx::connectDone;
    return false;
  } else {
    Serial.println("Subscribe failed");
    pRunningConnect = &SimA7xxx::disConnectMQTT;
    return false;
  }
}

bool SimA7xxx::connectDone() {
  static uint32_t lastCheck = 0;  // THÊM: Timer check signal.
  if (!mqtt.connected()) {
    disConnectMQTT();
    return false;
  }
  // THÊM: Check signal/GPRS mỗi 5min, reconnect nếu yếu.
  if (millis() - lastCheck > 150000) {
    int signal = modem.getSignalQuality();
    Serial.println("Periodic check: Signal = " + String(signal) + ", GPRS = " + String(modem.isGprsConnected()));
    if (signal < 10 || !modem.isGprsConnected()) {  // Signal yếu (< -93dBm) hoặc GPRS mất.
      disConnectMQTT();
    }
    lastCheck = millis();
  }
  return true;
}

bool SimA7xxx::disConnectMQTT() {
  int retry = 0;
  bool mqttDisconnected = false;
  bool gprsDisconnected = false;
  while (retry < 5) {
    mqtt.disconnect();                          // PubSubClient::disconnect() returns void, so we just call it
    gprsDisconnected = modem.gprsDisconnect();  // Returns bool
    if (gprsDisconnected) {
      break;
    }
    retry++;
    delay(100);  // Wait 100ms between retries
  }
  if (!gprsDisconnected) {
    Serial.println("GPRS disconnect failed after 5 retries");
    resetSim();
    // pRunningConnect = &SimA7xxx::connectSim;
    return false;
  }
  pRunningConnect = &SimA7xxx::connectMQTT;  // Set up to reconnect MQTT
  return false;
}

bool SimA7xxx::resetSim() {
  // modem.restart();  // Thay đổi: Sử dụng modem.restart() để reset module.
  rstVariable();
  SerialAT.println("AT+CRESET\r\n");
  delay(100);
  return false;
}

void SimA7xxx::followMsgAnyTopic() {
  // Serial.println("oke 2");
  mqtt.loop();
  // Serial.println("oke 3");
  return;
}
void SimA7xxx::setMqttMsgCallback(MqttMsgCallback cb) {
  mqttMsgCallback = cb;
}
void SimA7xxx::mqttCallback(char *topic, byte *payload, unsigned int length) {
  if (length > MAX_MSG_SIZE) {
    pubLogs("msg gửi tới quá lớn");
    return;
  }
  memcpy(this->_rxBuffer, payload, length);
  this->_rxBuffer[length] = '\0';  // Null termination
  if (mqttMsgCallback)
    mqttMsgCallback(topic, this->_rxBuffer);
  // String logs_str = "msg nhận từ [" + String(topic) + "]: " + String(mes);
  // pubLogs((char *)logs_str.c_str());
  return;
}

bool SimA7xxx::followTopic(char *topic) {
  return mqtt.subscribe(topic);  // Thay đổi: Sử dụng mqtt.subscribe() cho topic.
}

bool SimA7xxx::unfollowTopic(char *topic) {
  return mqtt.unsubscribe(topic);  // Thay đổi: Sử dụng mqtt.unsubscribe() cho topic.
}
void SimA7xxx::pubStatus(char *stt) {
  pubMsg(stt, TOPIC_DEVICE);
  return;
}
void SimA7xxx::pubMsgMQTT(char *stt, char *topic) {
  char topic_pond[80];
  sprintf(topic_pond, "ponds/%s", topic);
  pubMsg(stt, topic_pond);
  return;
}
void SimA7xxx::pubLogs(char *logs) {
  if (logs == nullptr || strlen(logs) == 0)
    return;
  pubMsg(logs, TOPIC_DEVICE_LOGS);
  return;
}
void SimA7xxx::pubMsg(char *mesPub, char *topic) {
  Serial.println("SEND MSG MQTT:");
  Serial.print("lenght: ");
  Serial.println(strlen(mesPub));
  Serial.print("  - ");
  Serial.println(topic);
  Serial.print("  - ");
  Serial.println(mesPub);
  int retry = 0;
  while (retry < 5) {
    if (mqtt.publish(topic, mesPub)) {
      return;
    }
    retry++;
    delay(100);  // chờ 100ms giữa các lần thử lại
  }
  Serial.println("Publish failed after 5 retries");
  rstVariable();
  return;
}
void SimA7xxx::subTopicPond(char *pond_id) {
  char topic_pond[100];
  sprintf(topic_pond, "ponds/%s", pond_id);
  mqtt.subscribe(topic_pond);
  return;
}


char *SimA7xxx::getDate() {
  if (!modem.isGprsConnected()) {
    Serial.println("GPRS not connected, cannot fetch date-time");
    return nullptr;
  }
  const char *ntpServer = "pool.ntp.org";
  modem.NTPServerSync(ntpServer, 7 * 4);              // UTC+7 offset in quarters of an hour (7 * 4 = 28)
  String dateTime = modem.getGSMDateTime(DATE_FULL);  // Format: YY/MM/DD,HH:MM:SS+TZ
  if (dateTime.length() == 0) {
    Serial.println("Failed to get date-time from modem");
    return nullptr;
  }
  strcpy(this->_dateBuffer, dateTime.c_str());
  Serial.println("Date-Time: " + String(this->_dateBuffer));
  return this->_dateBuffer;
}

uint32_t SimA7xxx::getUnix() {
  if (!modem.isGprsConnected()) {
    Serial.println("GPRS not connected, cannot fetch Unix time");
    return 0;
  }
  uint32_t nowMillis = millis();
  const uint32_t SYNC_INTERVAL = 3600000;
  if (_lastSyncUnix != 0 && (nowMillis - _lastSyncMillis < SYNC_INTERVAL)) {
    // Thời gian trôi qua (giây) = (millis hiện tại - millis lúc sync) / 1000
    uint32_t secondsPassed = (nowMillis - _lastSyncMillis) / 1000;
    return _lastSyncUnix + secondsPassed;
  }
  Serial.println("Syncing NTP...");
  const char *ntpServer = "pool.ntp.org";
  modem.NTPServerSync(ntpServer, 7 * 4);              // UTC+7 offset (28 quarters of an hour)
  String dateTime = modem.getGSMDateTime(DATE_FULL);  // Format: YY/MM/DD,HH:MM:SS+TZ
  if (dateTime.length() == 0) {
    Serial.println("Failed to get date-time from modem");
    return 0;
  }
  int year, month, day, hour, minute, second, tz;
  if (sscanf(dateTime.c_str(), "%d/%d/%d,%d:%d:%d+%d",
             &year, &month, &day, &hour, &minute, &second, &tz)
      != 7) {
    Serial.println("Failed to parse date-time: " + dateTime);
    return 0;
  }
  struct tm t = { 0 };
  t.tm_year = year + 2000 - 1900;  // Year since 1900
  t.tm_mon = month - 1;            // Months are 0-based
  t.tm_mday = day;
  t.tm_hour = hour;
  t.tm_min = minute;
  t.tm_sec = second;
  t.tm_isdst = 0;
  time_t unixTime = mktime(&t);
  // Adjust for timezone (tz in quarters of an hour, e.g., +28 for UTC+7)
  unixTime -= tz * 15 * 60;
  if (unixTime < 0) {
    Serial.println("Invalid Unix time calculated");
    return 0;
  }
  _lastSyncUnix = (uint32_t)unixTime;
  _lastSyncMillis = millis();  // Lưu lại thời điểm millis lúc này
  return (uint32_t)unixTime;
  //return 1;
}

uint32_t SimA7xxx::getCurrentTime(uint32_t unixTime) {
  if (unixTime == 0) {
    unixTime = getUnix();
    if (unixTime == 0) {
      Serial.println("Cannot calculate current time without valid Unix time");
      return 0;
    }
  }
  // Convert Unix time to UTC+7 (add 7 hours = 25200 seconds)
  time_t localTime = (time_t)unixTime + 25200;
  struct tm *t = gmtime(&localTime);  // Treat as UTC+7
  if (!t) {
    Serial.println("Failed to convert Unix time to struct tm");
    return 0;
  }
  // Check if the current day is even in UTC+7
  int currentDay = t->tm_mday;
  int daysToSubtract = (currentDay % 2 == 0) ? 0 : 1;
  // Calculate the start of the most recent even day in UTC+7
  struct tm evenDay = *t;
  evenDay.tm_mday -= daysToSubtract;
  evenDay.tm_hour = 0;
  evenDay.tm_min = 0;
  evenDay.tm_sec = 0;
  evenDay.tm_isdst = 0;
  // Convert back to Unix time (adjust for UTC+7 to UTC)
  time_t evenDayTime = mktime(&evenDay) - 25200;
  if (evenDayTime == -1) {
    Serial.println("Failed to normalize even day time");
    return 0;
  }
  // Calculate seconds since the start of the even day
  uint32_t currentTime = unixTime - (uint32_t)evenDayTime;
  //Serial.println("Current time (seconds since even day in UTC+7): " + String(currentTime));
  return currentTime;
}
// const int http_port = 80;
// const char *http_resource = "http://upcode.iotoom.com/khoa%20test/BootloaderFile.ino.bin";
// uint32_t current_addr = 0x0801D000;
bool SimA7xxx::downloadToFlash(uint32_t start_addr) {
  const int http_port = 80;
  const char *http_resource = "https://upcode.iotoom.com/khoa%20test/BootLoader.ino.hex";
  if (!modem.isGprsConnected()) return false;

  HttpClient http(client, HOST, http_port);
  if (http.get(http_resource) != 0 || http.responseStatusCode() != 200) return false;

  long contentLength = http.contentLength();
  uint32_t currentAddr = OTA_STORAGE_ADDR;
  uint32_t totalRead = 0;

  // // 1. Xóa Flash (Giữ nguyên logic cũ nhưng bọc trong disable_irq)
  HAL_FLASH_Unlock();
  // __disable_irq();
  FLASH_EraseInitTypeDef EraseInitStruct;
  uint32_t PageError = 0;
  EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
  EraseInitStruct.PageAddress = OTA_STORAGE_ADDR;
  EraseInitStruct.NbPages = (contentLength + 2047) / 2048;
  Serial.println("Erasing Flash...");
  if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) != HAL_OK) {
    Serial.println("Erase Error!");
    HAL_FLASH_Lock();
    return false;
  }
  // __enable_irq();
  const uint16_t MAX_HEX_LINE_LEN = 100;
  char hexBuffer[MAX_HEX_LINE_LEN];
  int bufferIdx = 0;
  while (http.connected() || http.available()) {
    if (http.available()) {
      char c = http.read();

      // Kiểm tra ký tự kết thúc dòng
      if (c == '\n' || c == '\r') {
        if (bufferIdx > 0) {
          hexBuffer[bufferIdx] = '\0';  // Kết thúc chuỗi
          parseHexLine(hexBuffer);
          bufferIdx = 0;  // Reset để đọc dòng mới
        }
      } else if (bufferIdx < MAX_HEX_LINE_LEN - 1) {
        hexBuffer[bufferIdx++] = c;
      }
    }
  }
  // uint8_t b1, b2;
  // while (totalRead < contentLength) {
  //   if (http.available() >= 2) {
  //     b1 = http.read();
  //     b2 = http.read();

  //     // In dữ liệu dưới dạng HEX để kiểm tra cấu trúc file .bin
  //     if (b1 < 0x10) Serial.print("0");
  //     Serial.print(b1, HEX);
  //     Serial.print(" ");
  //     if (b2 < 0x10) Serial.print("0");
  //     Serial.print(b2, HEX);
  //     Serial.print(" ");

  //     uint16_t data16 = (uint16_t)(b2 << 8) | b1;

  //     // KHÓA NGẮT ĐỂ GHI FLASH (Tránh đơ máy)
  //     __disable_irq();
  //     HAL_StatusTypeDef status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, currentAddr, data16);
  //     __enable_irq();
  //     if (status != HAL_OK) {
  //       Serial.printf("\n[ERROR] Write failed at 0x%08X\n", currentAddr);
  //       HAL_FLASH_Lock();
  //       return false;
  //     }
  //     currentAddr += 2;
  //     totalRead += 2;
  //     // Xuống dòng mỗi 16 byte để dễ nhìn
  //     if (totalRead % 16 == 0) {
  //       Serial.printf(" | Total: %ld/%ld\n", totalRead, contentLength);
  //     }
  //   }
  //   // Nếu đứng máy quá lâu không có dữ liệu từ Modem
  //   static uint32_t lastData = millis();
  //   if (http.available() > 0) lastData = millis();
  //   if (millis() - lastData > 10000) {
  //     Serial.println("\n[TIMEOUT] Modem stopped sending data.");
  //     break;
  //   }
  // }
  HAL_FLASH_Lock();
  Serial.println("\n--------------------------");
  Serial.println("Download Finished.");
  return true;
}
void SimA7xxx::parseHexLine(char *line) {
  if (line[0] != ':') return;  // Mỗi dòng HEX phải bắt đầu bằng dấu ':'

  // Sử dụng sscanf để tách các thành phần của dòng HEX
  // Định dạng: :llaaaatt[dd...]cc
  // ll: độ dài, aaaa: địa chỉ, tt: loại bản ghi
  unsigned int len, addr, type;
  if (sscanf(line + 1, "%02x%04x%02x", &len, &addr, &type) != 3) return;
  uint32_t targetAddr = OTA_STORAGE_ADDR + addr;
  if (type == 00) {  // Record Type 00: Dữ liệu (Data)
    for (int i = 0; i < len; i += 2) {
      char hexByte[3] = { 0 };
      unsigned int bLow = 0, bHigh = 0;

      // Trích xuất byte thấp
      hexByte[0] = line[9 + (i * 2)];
      hexByte[1] = line[10 + (i * 2)];
      bLow = strtoul(hexByte, NULL, 16);

      // Trích xuất byte cao
      if (i + 1 < len) {
        hexByte[0] = line[9 + ((i + 1) * 2)];
        hexByte[1] = line[10 + ((i + 1) * 2)];
        bHigh = strtoul(hexByte, NULL, 16);
      } else {
        bHigh = 0xFF;  // Trường hợp lẻ byte (hiếm gặp ở file HEX STM32)
      }
      uint16_t data16 = (uint16_t)(bHigh << 8) | bLow;
      Serial.printf("[0x%08X] <- %04X (%02X | %02X) \n", targetAddr + i, data16, bLow, bHigh);
      // Ghi vào Flash
      //__disable_irq();
      HAL_StatusTypeDef status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, targetAddr + i, data16);
      // __enable_irq();
      if (status != HAL_OK) {
        Serial.printf("FAILED at 0x%08X | Status: %d\n", targetAddr + i, status);
        HAL_FLASH_Lock();
        return;
      }
    }
    // Serial.printf("Written 0x%08X, Len: %d\n", targetAddr, len);
    // HAL_FLASH_Lock();
  } else if (type == 04) {  // Record Type 04: Địa chỉ mở rộng (Extended Linear Address)
    unsigned int upperAddr;
    sscanf(line + 9, "%04x", &upperAddr);
    Serial.printf("--- Base Address Changed to: 0x%04X0000 ---\n", upperAddr);
  } else if (type == 01) {  // Record Type 01: Kết thúc file
    Serial.println("--- End of File ---");
  }
}