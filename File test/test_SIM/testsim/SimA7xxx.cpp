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
    SerialAT.setTimeout(200);
    pRunningConnect = &SimA7xxx::connectSim;
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
  mqtt.setServer(HOST, PORT);        // Thay đổi: Thiết lập server MQTT bằng PubSubClient (lưu ý: Port 8883 cho TLS; PubSubClient cần hỗ trợ TLS thêm nếu yêu cầu).
  mqtt.setKeepAlive(300);            // Thay đổi: Thiết lập keep-alive.
  mqtt.setCallback(staticCallback);  // Thay đổi: Sử dụng static wrapper thay vì lambda để tránh lỗi compile.

  char *willMsg = json.msgStatus(STATUS_DISCONNECTED);  // Thay đổi: Chuẩn bị will message.
  mqtt.setBufferSize(MAX_MSG_SIZE);
  bool connected = mqtt.connect(DEVICE_ID, USER, PASSWORD, TOPIC_DEVICE, 1, false, willMsg);  // Thay đổi: Kết nối MQTT với credentials và will.
  delete[] willMsg;

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
  char mes[length + 1];
  memcpy(mes, payload, length);
  mes[length] = '\0';
  if (mqttMsgCallback)
    mqttMsgCallback(topic, mes);
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

