#ifndef parameter_h
#define parameter_h

#include "PinDefinition.h"
#include "GeneralFunction.h"

#define FIRM_WARE "factory setting"
#define HOST "mqtt.iotoom.com"
#define PORT 1883
#define USER "lab"
#define PASSWORD "IotoomVN"
#define DEVICE_TYPE "pwq"
#define FIRM_WARE "pwq 1.0.0.1"
#define DEVICE_ID "pwq-testing ver new"
#define TOPIC_DEVICE "devices/pwq-47ea6d704a6145be9ad46a4ec4433c37"
#define TOPIC_DEVICE_LOGS "devices/pwq-47ea6d704a6145be9ad46a4ec4433c37/check_logs"

#define MSG_TYPE_CMD "\"message_type\":\"command\""
#define MSG_TYPE_STG "\"message_type\":\"settings\""
#define CMD_ON "\"command\":\"on\""
#define CMD_OFF "\"command\":\"off\""
#define CMD_MEASURE_NOW "\"command\":\"measure_now\""
#define CMD_CALIB_ON "\"command\":\"calib_on\""
#define CMD_CALIB_OFF "\"command\":\"calib_off\""
#define CMD_MANUAL "\"command\":\"manual\""
#define CMD_INIT_TUBE "\"command\":\"init_tube\""
#define CMD_COMPLETED "command_completed"
#define CMD_DENIED "command_denied"
#define CMD_MANUAL_CPLT "manual_completed"
#define MACHINE_STATUS_CONNECTED "CONNECTED"
#define MACHINE_STATUS_DISCONNECTED "DISCONNECTED"
#define MACHINE_STATUS_RUNNING "RUNNING"
#define MACHINE_STATUS_CALIB "CALIB"
#define MACHINE_STATUS_MANUAL "MANUAL"
#define MACHINE_STATUS_ERROR "ERROR"
#define MACHINE_STATUS_WARNING "WARNING"
#define POND_STATUS_ON "ON"
#define POND_STATUS_OFF "OFF"
#define POND_STATUS_WARNING "WARNING"
#define POND_STATUS_ERROR "ERROR"

#define NUM_POND 5
#define NUM_REAGENT 5
#define NUM_SENSOR 4
#define MAX_ID_LEN 64
#define MAX_MTYPE_LEN 32
#define MAX_SCHE_LEN 2048
#define MAX_NAME_LEN 20      // Tên sensor, reagent
#define MAX_TEST_STR_LEN 40  // Test ID, Test Type
#define MAX_CALIB_POINTS 5   // Tối đa 5 điểm calib
#define MAX_VIALS 4          // Tối đa 4 lọ trong 1 bộ hóa chất (Pos, ML, Stir...)
#define MAX_COEF 2           // Hệ số A, B cho sensor
enum CmdCodeLinear {         // CODE LINEAR STARTS FROM 10X
  CMD_LINEAR_REAGENT = 101,
  CMD_LINEAR_POND = 102,
  CMD_LINEAR_VIM = 103,
  // CMD_LINEAR_REAGENT_HOME = 101,
  // CMD_LINEAR_POND_HOME = 102,
  // CMD_LINEAR_VIM_HOME = 103,
  // CMD_LINEAR_REAGENT_UP = 104,
  // CMD_LINEAR_REAGENT_DOWN = 105,
  // CMD_LINEAR_POND_UP = 106,
  // CMD_LINEAR_POND_DOWN = 107,
  // CMD_LINEAR_VIM_UP = 108,
  // CMD_LINEAR_VIM_DOWN = 109,
};
enum CmdCodeTurnTable {  // CODE TURNTABLE STARTS FROM 20X
  CMD_TURNTABLE = 201
  // CMD_TURNTABLE_HOME = 201,
  // CMD_TURNTABLE_CW = 202,
  // CMD_TURNTABLE_CCW = 203,
};
enum CmdCodePump {  // CODE PUMP STARTS FROM 30X
  CMD_PUMP_STEPPER_UP = 301,
  CMD_PUMP_STEPPER_DOWN = 302,
  CMD_PUMP_POND = 303,
  CMD_PUMP_PURITY = 304,
  CMD_PUMP_VIM = 305,
  CMD_PUMP_OUT_CUVETTE = 306,
  CMD_PUMP_ADD_COOLING = 307,
  CMD_PUMP_CYCLE_COOLING = 308,
  CMD_PUMP_OUT_TANK = 309,
  CMD_PUMP_CLEAN_SENSOR = 310,
  CMD_PUMP_NEAR_MACHINE = 311,
};
enum CmdSensor {  // CODE SENSOR STARTS FROM 40X
  CMD_PH = 401,
  CMD_SANILITY = 402,
  CMD_COLOR = 410,
  // CMD_DO = 403,
  // CMD_TEMP = 404,
  // CMD_PH_CALIB_1 = 405,
  // CMD_PH_CALIB_2 = 406,
  // CMD_SANILITY_CALIB_1 = 407,
  // CMD_SANILITY_CALIB_2 = 408,
  // CMD_DO_CALIB_1 = 409,
  // CMD_DO_CALIB_2 = 410,
  // CMD_TEMP_CALIB_1 = 411,
  // CMD_TEMP_CALIB_2 = 412,
  // CMD_CHECK_RS485 = 499,
};
enum CmdStir {  // CODE STIR STARTS FROM 50X
  CMD_STIR_CUVETTE = 501,
  CMD_STIR_VIAL = 502,
  CMD_STIR_VIM = 503,
};
enum CmdOther {  // CODE OTHER STARTS FROM 60X
  CMD_LIGHTING = 601,
  CMD_FAN = 602,
  CMD_COOLING = 603,
};
enum CmdFuncMachine {  // CODE FUNC MACHINE STARTS FROM 90X
  CMD_WATER_IN_CUVETTE = 901,
  CMD_WATER_OUT_CUVETTE = 902,
  CMD_CLEAN_CUVETTE = 903,
  CMD_WASH = 904,
  CMD_GET_REAGENT = 905,
  CMD_DROP_REAGENT = 906,
  CMD_COLOR_SENSOR = 907,
  CMD_DROPS_COUNTER = 908,
  CMD_MEASURE_KH = 910,
  CMD_MEASURE_NO2 = 911,
  CMD_MEASURE_NH3 = 912,
  CMD_MEASURE_CA = 913,
  CMD_MEASURE_MG = 914,
};
struct SettingDevice {
  char current_state[20];
  bool is_measuring;
  bool is_init_tube;
  bool new_water;
  bool lora_working;
  uint8_t amount_clean_water[2];
  uint8_t amount_vim[2];
  int8_t current_reagent;
  int8_t current_pond;
  int8_t current_pos;
  int8_t current_drop;
  float temp_machine;
  int16_t err_code;
};
struct ParamCalib {
  uint8_t num_index;
  uint8_t leds[MAX_CALIB_POINTS];
  float coefs[MAX_CALIB_POINTS];
  float inters;
};
struct ParamReagent {
  bool enable;
  const char *name_reagent;
  const uint8_t amount;
  // uint8_t *pos;
  // const uint8_t *method;
  // const float *drops_will_use;
  // const float ml_water;
  // const float diluting_rate;
  // const uint16_t *time_stir;
  // float *max_ml;
  // float *remaining;
  // const uint8_t *led_detect;
  // Các thông số thay đổi theo từng lọ (vial)
  uint8_t pos[MAX_VIALS];
  uint8_t method[MAX_VIALS];  // Giả sử method đi theo lọ
  float drops_will_use[MAX_VIALS];
  float ml_water;
  float diluting_rate;
  uint16_t time_stir[MAX_VIALS];
  float max_ml[MAX_VIALS];
  float remaining[MAX_VIALS];
  uint8_t led_detect[MAX_VIALS];  // LED dùng để check màu lọ đó

  ParamCalib paramCalib;
};
struct ParamSensor {
  bool has_in_machine;  // use for sensor inside the machine
  bool err_in_machine;
  const char *name_sensor;
  bool enable_each_pond[NUM_POND];
  int16_t err_each_pond[NUM_POND];
  uint16_t err_code;
  float coefficient[MAX_COEF];
  const uint8_t eeprom_point_calib[MAX_COEF];
  const float raw_value[MAX_COEF];
  float value;
};

struct ParamPond {
  bool enable;
  bool err;
  char pond_module_id[MAX_ID_LEN];
  char pond_id[MAX_ID_LEN];
  char sche[MAX_SCHE_LEN];
  char module_type[MAX_MTYPE_LEN];
  uint16_t pump_duration;
  uint16_t err_code;
};
struct ListReagentNow {
  uint8_t num_reagent_now;
  int8_t pond;
  int8_t list_reagent[NUM_REAGENT + NUM_SENSOR];
  char test_type[MAX_TEST_STR_LEN];
  char test_id[MAX_TEST_STR_LEN];
  uint8_t point_index;
};
struct ListErr {
  const char *name_err;
  const uint16_t err_code;
  const uint8_t times_err;
};
struct ControlManual {
  int16_t cmd_code;
  uint16_t time;
  bool is_run;
};
#endif
