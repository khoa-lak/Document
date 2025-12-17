#include <SPI.h>
#include <LoRa_STM32.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define SS PA4
#define RST PB0
#define DI0 PA1
#define BAND 433E6
#define SYNC_WORD 0x34

#define ONE_WIRE_BUS PB1
#define PUMP_PIN PA0
#define LIGHT_PIN PA2
#define SENSOR_POWER_PIN PA3  // Pin cấp nguồn cho DS18B20

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

byte masterNode = 0xFF;
byte broadcastNode = 0x00;
byte myNode = 0x01;                 // 0x01 cho module-pond-213, 0x02 cho module-pond-217
String myName = "module-pond-213";  // Thay đổi tương ứng
byte msgCount = 0;

float lastTemp = 0.0;               // Lưu giá trị nhiệt độ
unsigned long sensorStartTime = 0;  // Thời gian bật cảm biến
const long sensorTimeout = 300000;  // 5 phút
bool sensorPowered = false;         // Trạng thái nguồn cảm biến
const int maxRetries = 3;
const long ackTimeout = 1000;

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;
  Serial.println("LoRa Slave: " + myName);

  LoRa.setPins(SS, RST, DI0);
  if (!LoRa.begin(BAND)) {
    Serial.println("LoRa init failed!");
    while (1)
      ;
  }
  LoRa.setSyncWord(SYNC_WORD);

  sensors.begin();
  pinMode(PUMP_PIN, OUTPUT);
  pinMode(LIGHT_PIN, OUTPUT);
  pinMode(SENSOR_POWER_PIN, OUTPUT);
  digitalWrite(PUMP_PIN, LOW);
  digitalWrite(LIGHT_PIN, LOW);
  digitalWrite(SENSOR_POWER_PIN, LOW);  // Cảm biến tắt ban đầu
}

void loop() {
  // Tắt cảm biến nếu quá 5 phút
  if (sensorPowered && millis() - sensorStartTime >= sensorTimeout) {
    digitalWrite(SENSOR_POWER_PIN, LOW);
    sensorPowered = false;
    Serial.println(myName + ": Sensor powered OFF (timeout)");
  }

  onReceive(LoRa.parsePacket());
}

void readTemperature() {
  digitalWrite(SENSOR_POWER_PIN, HIGH);  // Bật nguồn cảm biến
  sensorPowered = true;
  sensorStartTime = millis();
  delay(1000);  // Chờ cảm biến ổn định
  sensors.requestTemperatures();
  lastTemp = sensors.getTempCByIndex(0);
  if (lastTemp == DEVICE_DISCONNECTED_C) {
    lastTemp = 0.0;  // Lỗi cảm biến
  }
  Serial.println(myName + ": Read temp = " + String(lastTemp));
}

void sendTemperature() {
  String message = String(myNode, HEX) + ":" + String(lastTemp);
  bool success = false;
  int retries = 0;

  while (retries < maxRetries && !success) {
    byte sentMsgId = msgCount;
    sendMessage(message, masterNode, myNode);
    Serial.println("Sent from " + myName + ": " + message + " (Attempt " + String(retries + 1) + ")");

    unsigned long startTime = millis();
    while (millis() - startTime < ackTimeout) {
      if (checkACK(sentMsgId)) {
        success = true;
        digitalWrite(SENSOR_POWER_PIN, LOW);  // Tắt cảm biến sau khi gửi thành công
        sensorPowered = false;
        Serial.println(myName + ": Sensor powered OFF");
        break;
      }
    }
    retries++;
    if (!success && retries < maxRetries) {
      delay(random(100, 500));  // Delay ngẫu nhiên tránh collision
    }
  }

  if (!success) {
    Serial.println(myName + ": Failed to send temperature after " + String(maxRetries) + " retries");
  }
}

bool checkACK(byte sentMsgId) {
  int packetSize = LoRa.parsePacket();
  if (packetSize == 0) return false;

  byte recipient = LoRa.read();
  byte sender = LoRa.read();
  byte incomingMsgId = LoRa.read();
  byte incomingLength = LoRa.read();

  String incoming = "";
  while (LoRa.available()) {
    incoming += (char)LoRa.read();
  }

  if (incomingLength != incoming.length() || recipient != myNode || sender != masterNode) {
    return false;
  }

  if (incoming.startsWith("ACK:") && incoming.substring(4).toInt() == sentMsgId) {
    Serial.println(myName + ": ACK received for msgID " + String(sentMsgId));
    return true;
  }
  return false;
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

  if (incomingLength != incoming.length() || sender != masterNode) {
    return;
  }

  bool isForMe = (recipient == myNode || recipient == broadcastNode);
  if (!isForMe) {
    return;
  }

  String type = (recipient == broadcastNode) ? "Broadcast" : "Unicast";
  Serial.println("Received at " + myName + " (" + type + "): " + incoming);

  if (incoming == "power_on" && recipient == broadcastNode) {
    readTemperature();
    sendMessage("ACK:" + String(incomingMsgId), masterNode, myNode);
  } else if (incoming == "get_temp" && recipient == myNode) {
    sendTemperature();
  } else if (incoming == "pump_on" && recipient == myNode) {
    digitalWrite(PUMP_PIN, HIGH);
    Serial.println(myName + ": Pump ON");
    sendMessage("ACK:" + String(incomingMsgId), masterNode, myNode);
  } else if (incoming == "pump_off" && recipient == myNode) {
    digitalWrite(PUMP_PIN, LOW);
    Serial.println(myName + ": Pump OFF");
    sendMessage("ACK:" + String(incomingMsgId), masterNode, myNode);
  } else if (incoming == "light_on") {  // Broadcast hoặc unicast
    digitalWrite(LIGHT_PIN, HIGH);
    Serial.println(myName + ": Light ON");
    if (recipient == myNode) sendMessage("ACK:" + String(incomingMsgId), masterNode, myNode);
  } else if (incoming == "light_off") {
    digitalWrite(LIGHT_PIN, LOW);
    Serial.println(myName + ": Light OFF");
    if (recipient == myNode) sendMessage("ACK:" + String(incomingMsgId), masterNode, myNode);
  }
}