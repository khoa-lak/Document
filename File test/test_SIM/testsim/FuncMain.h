#ifndef funcmain_h
#define funcmain_h
#include <EEPROM.h>
#include "SimA7xxx.h"
#include "JsonPWQ.h"
#define NUM_ERR 50
#define ERR_PUMP_IN 101
#define ERR_PUMP_OUT 102
#define ERR_SENSOR_TANK 103
#define ERR_NOT_REAGENT_COLOR 104
#define ERR_NOT_REAGENT_DROP 105
#define ERR_NOT_REAGENT_APP 106
#define ERR_NOT_WATER 107
#define ERR_SS_NOT_USE_MACHINE 121
#define ERR_SS_MACHINE_CALIB_FAILED 122
#define ERR_SS_NOT_ENABLE_MODULE 123
#define ERR_SS_NOT_USE_MODULE 124
#define ERR_SS_MODULE_FAILED 125
#define ERR_SEND_CALIB_MODULE 126
#define ERR_FB_CALIB_MODULE 127
#define ERR_MODULE_NOT_ENBALE 128
#define ERR_INDEX_NOT_ENABLE 129
struct ListErrMachine {
  ListErr listErr;
  const uint8_t grade_err;  //grade_err: 0 - warning, 1 - err (turn off machine)
};
class FuncMain {
private:
  GeneralFunction generalFunction;
  SimA7xxx simA7xxx;
  JsonPWQ jsonPWQ;

  bool _first_run;
  //handle msg mqtt
  bool checkSim();
  void classifyFuncRun();
  void handleMsgMQTT(const char *topic, const char *msg);
  void handleMsgCommand(const char *msg);
  void handleMsgSetting(const char *topic, const char *msg);
  bool handleGetStgPond();
  void pubMsgPond(char *msg, uint8_t pond);
  void pubMsgDevice(char *msg);
  void pubLogsMQTT(char *msg);
  void handleStateConnected();
  void handleStateRunning();
  void handleStateCalib();


  void rstVariables();
  void rstPointer();
public:
  FuncMain();
  ~FuncMain();
  void setupMain();
  void runMain();
  void hasMqttMessage(const char *topic, const char *msg);
  // current_state, is_measuring, new_water, lora_working, current_reagent, current_pond, current_pos,current_drop, temp_machine, err_code;
  SettingDevice stgDevice = { STATUS_DISCONNECTED, false, false, true, -1, -1, -1, -1, 27.0, 0 };
  // enable, name_reagent, amount, pos, method, drops_will_use, ml_water, diluting_rate, time_stir,check_color, max_test, current_count, led_detect, param calib
  ParamReagent paramReagent[NUM_REAGENT] = {
    { false, "KH", 1, (uint8_t[]){ 3 }, (uint8_t[]){ 1 }, (float[]){ 3.0 }, 3.0, 1.0, (uint16_t[]){ 0 }, (bool[]){ false }, 100, 0, (uint8_t[]){ 5 }, { 0, nullptr, nullptr, -1 } },
    { false, "NH3", 3, (uint8_t[]){ 4, 5, 6 }, (uint8_t[]){ 0, 0, 0 }, (float[]){ 3.6, 3.6, 3.6 }, 3.0, 0.5, (uint16_t[]){ 0, 0, 60 }, (bool[]){ false, false, true }, 100, 0, (uint8_t[]){ 3 }, { 0, nullptr, nullptr, -1 } },
    { false, "NO2", 2, (uint8_t[]){ 23, 24 }, (uint8_t[]){ 0, 0 }, (float[]){ 3.0, 3.0 }, 3.0, 0.5, (uint16_t[]){ 0, 60 }, (bool[]){ true, false }, 100, 0, (uint8_t[]){ 2 }, { 0, nullptr, nullptr, -1 } },
    { false, "CA", 3, (uint8_t[]){ 10, 11, 18 }, (uint8_t[]){ 0, 0, 1 }, (float[]){ 2.4, 0.6, 3.0 }, 3.0, 1.0, (uint16_t[]){ 0, 0, 0 }, (bool[]){ false, true, false }, 100, 0, (uint8_t[]){ 2, 4 }, { 0, nullptr, nullptr, -1 } },
    { false, "MG", 3, (uint8_t[]){ 12, 13, 19 }, (uint8_t[]){ 0, 0, 1 }, (float[]){ 4.0, 1.0, 3.0 }, 3.0, 1.0, (uint16_t[]){ 0, 0, 0 }, (bool[]){ false, true, false }, 100, 0, (uint8_t[]){ 2, 4 }, { 0, nullptr, nullptr, -1 } }
  };
  // enable, err, pond_module_id, pond_id, sche, communication_type, pump_duration, err_code;
  ParamPond paramPond[NUM_POND] = {
    { false, false, nullptr, nullptr, nullptr, nullptr, 40, 0 },
    { false, false, nullptr, nullptr, nullptr, nullptr, 40, 0 },
    { false, false, nullptr, nullptr, nullptr, nullptr, 40, 0 },
    { false, false, nullptr, nullptr, nullptr, nullptr, 40, 0 },
    { false, false, nullptr, nullptr, nullptr, nullptr, 40, 0 }
  };
  // has_in_machine, err_in_machine, name_sensor, enable_each_pond, err_each_pond, err_code, coef, eeprom_addres, raw_value, value;
  ParamSensor paramSensor[NUM_SENSOR] = {
    { false, false, "PH", (bool[NUM_POND]){ false }, (int16_t[NUM_POND]){ -1 }, 500, nullptr, nullptr, nullptr, -1 },
    { false, false, "SALINITY", (bool[NUM_POND]){ false }, (int16_t[NUM_POND]){ -1 }, 501, (float[]){ 1.0, 0 }, (uint8_t[]){ 4, 8 }, (float[]){ 0.0, 9.0 }, -1 },
    { false, false, "DO", (bool[NUM_POND]){ false }, (int16_t[NUM_POND]){ -1 }, 502, (float[]){ 1.0, 0 }, (uint8_t[]){ 12, 16 }, (float[]){ 4.0, 8.0 }, -1 },
    { false, false, "TEMP", (bool[NUM_POND]){ false }, (int16_t[NUM_POND]){ -1 }, 503, (float[]){ 1.0, 0 }, (uint8_t[]){ 20, 24 }, (float[]){ 0.0, 30.0 }, -1 }
  };
  // num_reagent_now, pond, list_reagent, test_type, test_id, point_index;
  ListReagentNow listReagentNow = { 0, -1, (int8_t[NUM_POND + NUM_SENSOR]){ -1 }, nullptr, nullptr, 0 };
  // name_err, err_code, times_err;
  ListErrMachine listErrMachine[NUM_ERR] = {
    { { "pump in err", ERR_PUMP_IN, 1 }, 1 },
    { { "pump out err", ERR_PUMP_OUT, 2 }, 1 },
    { { "float sensor err", ERR_SENSOR_TANK, 2 }, 1 },
    { { "color detect not reagent ", ERR_NOT_REAGENT_COLOR, 2 }, 0 },
    { { "drops counter detect not reagent ", ERR_NOT_REAGENT_DROP, 1 }, 0 },
    { { "app detect not reagent ", ERR_NOT_REAGENT_APP, 1 }, 0 },
    { { "not water ", ERR_NOT_WATER, 2 }, 0 },
    { { "sensor not use at the machine", ERR_SS_NOT_USE_MACHINE, 1 }, 0 },
    { { "calib sensor at the machine failed", ERR_SS_MACHINE_CALIB_FAILED, 1 }, 0 },
    { { "sensor not enable at module", ERR_SS_NOT_ENABLE_MODULE, 1 }, 0 },
    { { "sensor not use at module", ERR_SS_NOT_USE_MODULE, 1 }, 0 },
    { { "sensor module failed", ERR_SS_MODULE_FAILED, 1 }, 0 },
    { { "send cmd calib failed", ERR_SEND_CALIB_MODULE, 1 }, 0 },
    { { "module not feedback", ERR_FB_CALIB_MODULE, 1 }, 0 },
    { { "module is disable", ERR_MODULE_NOT_ENBALE, 1 }, 0 },
    { { "index is disable", ERR_INDEX_NOT_ENABLE, 1 }, 0 }
  };
};
#endif
