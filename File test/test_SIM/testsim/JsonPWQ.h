#ifndef json_index_balance_h
#define json_index_balance_h
#include <ArduinoJson.h>
#include "Parameters.h"
#define HEADER_LENGTH 16
#define MSG_MIN 255
#define MSG_MID 511
#define MSG_MAX 1023
class JsonPWQ {
private:
  GeneralFunction generalFunction;
  ParametersMemory paraMem;
  uint16_t lengthToAllocation(const char* js);

public:
  bool isJon(const char* js);
  bool handleMsgSettingDevice(const char* msg, ParamPond paramPond[NUM_POND], ParamReagent paramReagent[NUM_REAGENT], ParamSensor paramSensor[NUM_SENSOR]);
  void handleMsgSettingPond(char* msg, ParamPond paramPond[NUM_POND]);
  void handleMsgMeasureNow(const char* msg, ParamPond paramPond[NUM_POND], ParamReagent paramReagent[NUM_REAGENT], ParamSensor paramSensor[NUM_SENSOR], ListReagentNow& listReagentNow);
  bool handleMsgSchedule(const char* msg, int8_t* data, uint32_t current_time, ParamReagent paramReagent[NUM_REAGENT], ParamSensor paramSensor[NUM_SENSOR]);

  char* msgStateMachine(char* title, uint32_t unix_time, uint16_t total_time, float temp, int8_t pond, ParamPond paramPond[NUM_POND], ParamReagent paramReagent[NUM_REAGENT], ParamSensor paramSensor[NUM_SENSOR], uint8_t param_queue[][3], uint8_t num_queue, char* test_id, const char* state);
  char* commandRespond(char* cmd_fb);
  char* msgStatus(char* state);
  char* msgStatus();
  char* msgStatusError(char* name_err, int16_t err_code, uint8_t grade_err);
  char* msgLogs(char* logs);
  char* msgInforLed(const char* stage, uint16_t* led_data, SettingDevice stgDevice);
  char* msgValueReagent(const char* name_reagent, char* pond_id, float value, uint16_t* led_data, char* test_id, char* test_type, SettingDevice stgDevice);
  char* msgValueSensor(char* pond_id, char* test_id, char* test_type, ParamSensor paramSensor[NUM_SENSOR]);
  //for lora
  char* msgGetValueModule(uint8_t pond, ParamPond paramPond[NUM_POND], ParamSensor paramSensor[NUM_SENSOR]);
  char* handleMsgValueModule(char* msg, uint8_t pond, ParamPond paramPond, ParamSensor paramSensor[NUM_SENSOR]);
  char* msgControlPump(const char* module_id, bool enable, uint16_t time);
  char* msgCalibSensorModule(const char* module_id, const char* name_sensor, uint8_t point_index);
};
#endif
