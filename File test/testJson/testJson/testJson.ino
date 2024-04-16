#include <ArduinoJson.h>
#include "TruyenLora.h"

char input[] =
  "{\"device_id\":\"water_quality_f9s7dga09sdfas0df\","
  "\"timestamp\": 1634534678,"
  "\"message_type\": \"settings\","
  "\"data\":{\"command\": \"stop\","
  "\"properties\": {"
  "\"culture_animal\": \"whiteleg_shrimp\","
  "\"interval\": 24,"
  " \"params\":["
  "{"
  "\"code\": \"NH3\","
  "\"name\": \"Amoniac\","
  "\"min\": 0.4,"
  "\"unit\": \"mg/l\","
  "\"selected\": true,"
  "\"brand\": \"sera\","
  "\"vials\": [0,1,2]"
  "},"
  "{"
  "\"code\": \"NO2\","
  "\"name\": \"Nitrite NO2\","
  "\"max\": 5,"
  "\"unit\": \"mg/l\","
  "\"brand\": \"sera\","
  "\"vials\": [3,4],"
  "\"selected\": false"
  "},"
  "{"
  "\"code\": \"pH\","
  "\"name\": \"Độ pH\","
  "\"min\": 7.5,"
  "\"max\": 9.0,"
  "\"brand\": \"sera\","
  "\"vials\": [5],"
  "\"selected\": false"
  "}"
  "]"
  "}"
  "}"
  "}";
void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);

}
void loop() {
//  char input[] =
//    "{\"device_id\":\"water_quality_f9s7dga09sdfas0df\","
//    "\"timestamp\": 1634534678,"
//    "\"message_type\": \"settings\","
//    "\"data\":{\"command\": \"stop\","
//    "\"properties\": {"
//    "\"culture_animal\": \"whiteleg_shrimp\","
//    "\"interval\": 24,"
//    " \"params\":["
//    "{"
//    "\"code\": \"NH3\","
//    "\"name\": \"Amoniac\","
//    "\"min\": 0.4,"
//    "\"unit\": \"mg/l\","
//    "\"selected\": true,"
//    "\"brand\": \"sera\","
//    "\"vials\": [0,1,2]"
//    "},"
//    "{"
//    "\"code\": \"NO2\","
//    "\"name\": \"Nitrite NO2\","
//    "\"max\": 5,"
//    "\"unit\": \"mg/l\","
//    "\"brand\": \"sera\","
//    "\"vials\": [3,4],"
//    "\"selected\": false"
//    "},"
//    "{"
//    "\"code\": \"pH\","
//    "\"name\": \"Độ pH\","
//    "\"min\": 7.5,"
//    "\"max\": 9.0,"
//    "\"brand\": \"sera\","
//    "\"vials\": [5],"
//    "\"selected\": false"
//    "}"
//    "]"
//    "}"
//    "}"
//    "}";
//  Serial.println(input);
//  delay(2000);
//  StaticJsonDocument<400> doc;
//  DeserializationError error = deserializeJson(doc, (char*)input);
//
//  if (error) {
//    Serial.print(F("deserializeJson() failed: "));
//    Serial.println(error.f_str());
//
//  }
//  JsonObject _ObjData = doc.as<JsonObject>();
//  const char* device_id = _ObjData["device_id"]; // "water_quality_f9s7dga09sdfas0df"
//  const char* data_command = doc["data"]["command"]; // "stop"
//  JsonObject data_properties = doc["data"]["properties"];
//  const char* data_properties_culture_animal = data_properties["culture_animal"]; // "whiteleg_shrimp"
//
//  //  int data_properties_interval = data_properties["interval"]; // 24
//  //
//  JsonObject data_properties_params_0 = data_properties["params"][0];
//  const char* data_properties_params_0_code = data_properties_params_0["code"]; // "NH3"
//  Serial.println((char*)data_properties_params_0_code);
//  //  const char* data_properties_params_0_name = data_properties_params_0["name"]; // "Amoniac"
//  //  float data_properties_params_0_min = data_properties_params_0["min"]; // 0.4
//  //  const char* data_properties_params_0_unit = data_properties_params_0["unit"]; // "mg / l"
//  //  bool data_properties_params_0_selected = data_properties_params_0["selected"]; // true
//  //  const char* data_properties_params_0_brand = data_properties_params_0["brand"]; // "sera"
//  //
//  //  JsonArray data_properties_params_0_vials = data_properties_params_0["vials"];
//  //  int data_properties_params_0_vials_0 = data_properties_params_0_vials[0]; // 0
//  //  int data_properties_params_0_vials_1 = data_properties_params_0_vials[1]; // 1
//  //  int data_properties_params_0_vials_2 = data_properties_params_0_vials[2]; // 2
  StaticJsonDocument<192> doc1;

  doc1["device_id"] = "water_quality_f9s7dga09sdfas0df";
  doc1["message_type"] = "data";
  doc1["timestamp"] = 1634534678;

  JsonObject data_params_0 = doc1["data"]["params"].createNestedObject();
  data_params_0["code"] = "oxygen";
  data_params_0["value"] = 0.53;
  data_params_0["unit"] = "mg/l";
  char output[192];
  serializeJson(doc1, output);
  Serial.println(output);

}
void ChayLED1() {
  for (int i = 0 ; i <= 10 ; i++) {
    digitalWrite(13, HIGH);
    delay(800);
    digitalWrite(13, LOW);
    delay(800);
  }
}
