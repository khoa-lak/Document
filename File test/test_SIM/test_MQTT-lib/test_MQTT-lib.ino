#define TINY_GSM_MODEM_SIM7600
//#define SerialAT Serial1
#include <SoftwareSerial.h>
SoftwareSerial SerialAT(2, 3);
#include <TinyGsmClient.h>
#include <PubSubClient.h>
TinyGsm        modem(SerialAT);
TinyGsmClient client(modem);
PubSubClient  mqtt(client);
const char *c_host = "tcp://mqtt.iotoom.com";
const char *c_user = "lab";
const char *c_pass = "IotoomVN";
char *c_deviceID          = "devicetest";
char *c_topicDeviceID     = "devices/devicetest";
char *c_prexPond      = "ponds/";
char *c_mesLost       = "disconnected";
char *c_mesConnect    = "connected";
char *c_mesRun        = "running";
char *c_mesOff        = "off";
char *c_mesCommand    = "command_completed";
char *c_mesError      = "error";
uint32_t lastReconnectAttempt = 0;
//void mqttCallback(char* topic, byte* payload, unsigned int len) {
//  Serial.print("Message arrived [");
//  Serial.print(topic);
//  Serial.print("]: ");
//  Serial.write(payload, len);
//  Serial.println();
//  // Only proceed if incoming message's topic matches
//  if (String(topic) == c_topicDeviceID ) {
//    mqtt.publish(c_topicDeviceID, "ok con de");
//  }
//}
void mqttCallback(char* topic, byte* payload, unsigned int len) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.write(payload, len);
  Serial.println();

}
boolean mqttConnect() {
  Serial.print("Connecting to ");
  Serial.print(c_host);

  // Connect to MQTT Broker
  //boolean status = mqtt.connect("GsmClientTest");

  // Or, if you want to authenticate MQTT:
  boolean status = mqtt.connect("GsmClientName", c_user, c_pass);

  if (status == false) {
    Serial.println(" fail");
    return false;
  }
  Serial.println(" success");
  mqtt.publish(c_topicDeviceID, "GsmClientTest started");
  mqtt.subscribe(c_topicDeviceID);
  return mqtt.connected();
}
void setup() {
  Serial.begin(9600);
  SerialAT.begin(115200);
  delay(6000);
  modem.restart();
  String modemInfo = modem.getModemInfo();
  Serial.print("Modem Info: ");
  Serial.println(modemInfo);
  if (!modem.waitForNetwork()) {
    Serial.println(" fail");
    delay(5000);
    return;
  }
  Serial.println(" success");
  if (modem.isNetworkConnected()) {
    Serial.println("Network connected");
  }
  mqtt.setServer(c_host, 1883);
  mqtt.setCallback(mqttCallback);
}

void loop() {

  if (!mqtt.connected()) {
    //Serial.println("=== MQTT NOT CONNECTED ===");
    // Reconnect every 10 seconds
    uint32_t t = millis();
    if (t - lastReconnectAttempt > 10000L) {
      lastReconnectAttempt = t;
      if (mqttConnect()) {
        lastReconnectAttempt = 0;
      }
    }
    delay(100);
    return;
  }

  mqtt.loop();

}
