#include <SPI.h>
#include <LoRa_STM32.h>

#define SS PA4
#define RST PB0
#define DI0 PA1
#define BAND 433E6
#define SYNC_WORD 0x34

byte masterNode = 0xFF;
byte broadcastNode = 0x00;
String masterName = "PWQ-2183894";
byte msgCount = 0;

struct Node {
  String name;
  byte id;
};

Node nodes[] = {
  {"module-pond-213", 0x01},
  {"module-pond-217", 0x02}
};
int nodeCount = sizeof(nodes) / sizeof(nodes[0]);

unsigned long lastTempRequest = 0;
const long tempInterval = 600000;  // 10 phút
unsigned long lastPumpSchedule = 0;
const long pumpScheduleInterval = 3600000;  // 1 giờ (ví dụ)
bool isPumpControlActive = false;  // Trạng thái điều khiển bơm
int currentNodeIndex = 0;  // Chỉ số node đang gọi lấy nhiệt độ
bool waitingForTemp = false;  // Đang chờ nhận nhiệt độ

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println("LoRa Master: " + masterName);
  Serial.println("Commands: 'send [module-name/all] [command]' (e.g., send all light_on, send module-pond-213 pump_on)");

  LoRa.setPins(SS, RST, DI0);
  if (!LoRa.begin(BAND)) {
    Serial.println("LoRa init failed!");
    while (1);
  }
  LoRa.setSyncWord(SYNC_WORD);
}

void loop() {
  onReceive(LoRa.parsePacket());

  unsigned long currentMillis = millis();

  // Điều khiển bơm theo lịch (ví dụ: bật/tắt bơm cho module-pond-213 mỗi giờ)
  if (!isPumpControlActive && currentMillis - lastPumpSchedule >= pumpScheduleInterval) {
    isPumpControlActive = true;
    sendMessage("pump_on", nodes[0].id, masterNode);  // Bật bơm cho module-pond-213
    Serial.println("Scheduled: Sent pump_on to " + nodes[0].name);
    delay(5000);  // Giả lập bơm chạy 5 giây
    sendMessage("pump_off", nodes[0].id, masterNode);
    Serial.println("Scheduled: Sent pump_off to " + nodes[0].name);
    lastPumpSchedule = currentMillis;
    isPumpControlActive = false;
  }

  // Lấy nhiệt độ (không thực hiện nếu đang điều khiển bơm)
  if (!isPumpControlActive && currentMillis - lastTempRequest >= tempInterval) {
    if (!waitingForTemp) {
      // Gửi broadcast power_on
      sendMessage("power_on", broadcastNode, masterNode);
      Serial.println("Broadcasting power_on to all nodes");
      lastTempRequest = currentMillis;
      waitingForTemp = true;
      currentNodeIndex = 0;
      delay(2000);  // Chờ 2 giây để slaves đọc cảm biến
    } else {
      // Gửi get_temp tới từng node
      if (currentNodeIndex < nodeCount) {
        sendMessage("get_temp", nodes[currentNodeIndex].id, masterNode);
        Serial.println("Requesting temp from " + nodes[currentNodeIndex].name);
        currentNodeIndex++;
      } else {
        waitingForTemp = false;  // Hoàn thành lấy nhiệt độ
      }
    }
  }

  // Xử lý lệnh từ Serial (mô phỏng app)
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    if (input.startsWith("send ")) {
      input = input.substring(5);
      int spaceIndex = input.indexOf(' ');
      if (spaceIndex == -1) {
        Serial.println("Error: Invalid command format");
        return;
      }

      String destStr = input.substring(0, spaceIndex);
      String message = input.substring(spaceIndex + 1);

      byte destination;
      if (destStr == "all") {
        destination = broadcastNode;
        Serial.println("Broadcasting from " + masterName + ": " + message);
      } else {
        destination = getNodeId(destStr);
        if (destination == 0) {
          Serial.println("Error: Unknown module " + destStr);
          return;
        }
        Serial.println("Sending from " + masterName + " to " + destStr + ": " + message);
      }

      if (message == "pump_on" || message == "pump_off") {
        isPumpControlActive = true;  // Tạm dừng lấy nhiệt độ
      }
      sendMessage(message, destination, masterNode);
      if (message == "pump_on" || message == "pump_off") {
        delay(5000);  // Giả lập thời gian bơm
        isPumpControlActive = false;  // Tiếp tục lấy nhiệt độ
      }
    }
  }
}

byte getNodeId(String name) {
  for (int i = 0; i < nodeCount; i++) {
    if (nodes[i].name == name) {
      return nodes[i].id;
    }
  }
  return 0;
}

String getNodeName(byte id) {
  for (int i = 0; i < nodeCount; i++) {
    if (nodes[i].id == id) {
      return nodes[i].name;
    }
  }
  return "Unknown";
}

void sendMessage(String outgoing, byte destination, byte sender) {
  LoRa.beginPacket();
  LoRa.write(destination);
  LoRa.write(sender);
  LoRa.write(msgCount++);
  LoRa.write(outgoing.length());
  LoRa.print(outgoing);
  LoRa.endPacket();
}

void onReceive(int packetSize) {
  if (packetSize == 0) return;

  byte recipient = LoRa.read();
  byte sender = LoRa.read();
  byte incomingMsgId = LoRa.read();
  byte incomingLength = LoRa.read();

  String incoming = "";
  while (LoRa.available()) {
    incoming += (char)LoRa.read();
  }

  if (incomingLength != incoming.length() || recipient != masterNode) {
    return;
  }

  String senderName = getNodeName(sender);
  if (incoming.startsWith("ACK:")) {
    Serial.println("ACK from " + senderName + " for msgID " + incoming.substring(4));
  } else {
    Serial.println("Received from " + senderName + " (ID 0x" + String(sender, HEX) + "): " + incoming + " (RSSI: " + LoRa.packetRssi() + ")");
    sendMessage("ACK:" + String(incomingMsgId), sender, masterNode);
  }
}