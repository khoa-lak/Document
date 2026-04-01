#ifndef parameter_h
#define parameter_h

#include "PinDefinition.h"
#include "GeneralFunction.h"
#include "ParametersMemory.h"

// --- MEMORY MAP STM32F103VET6 (512KB) ---
// 1. Bootloader: 0x08000000 -> 0x08008000 (32KB)
// 2. Main App:   0x08008000 -> 0x08028000 (128KB) - Chạy chính
// 3. Factory:    0x08028000 -> 0x08048000 (128KB) - Backup an toàn
// 4. Storage:    0x08048000 -> 0x08068000 (128KB) - Tải OTA về đây
#define APP_ADDR       0x08008000
#define FACTORY_ADDR   0x08028000
#define STORAGE_ADDR   0x08048000
#define APP_SIZE       (128 * 1024)
#define STORAGE_SIZE   (128 * 1024)
#define FLAG_ADDR         0x0807F800
#define FLAG_UPDATE_REQ   0xAAAAAAAA 
#define BKP_ERR_COUNT_REG 1 // Dùng thanh ghi Backup DR1 để đếm lỗi
//info FOTA
#define URL_FIRMWARE "https://s3.ap-southeast-1.amazonaws.com/iotoom.com/hardware/test_OTA.txt"
//info MQTT
#define HOST "mqtt.iotoom.com"
#define PORT 1883
#define USER "lab"
#define PASSWORD "IotoomVN"
#define DEVICE_TYPE "pwq"
#define DEVICE_ID "pwq-testing"
#define TOPIC_DEVICE "devices/pwq-testing"
#define TOPIC_DEVICE_LOGS "devices/pwq-testing/check_logs"

#define MSG_TYPE_CMD "\"message_type\":\"command\""
#define MSG_TYPE_STG "\"message_type\":\"settings\""
#define MSG_TYPE_FIRMWARE "\"message_type\":\"update_firmware\""
#define CMD_ON "\"command\":\"on\""
#define CMD_OFF "\"command\":\"off\""
#define CMD_MEASURE_NOW "\"command\":\"measure_now\""
#define CMD_CALIB_ON "\"command\":\"calib_on\""
#define CMD_CALIB_OFF "\"command\":\"calib_off\""
#define CMD_INIT_TUBE "\"command\":\"init_tube\""
#define CMD_UPDATE_STG_NOW "\"command\":\"update_setting_now\""
#define CMD_COMPLETED "command_completed"
#define CMD_DENIED "command_denied"
#define STATUS_CONNECTED "CONNECTED"
#define STATUS_DISCONNECTED "DISCONNECTED"
#define STATUS_RUNNING "RUNNING"
#define STATUS_CALIB "CALIB"
#define STATUS_ERROR "ERROR"
#define STATUS_WARNING "WARNING"
#define STATUS_ON "ON"
#define STATUS_OFF "OFF"

#define NUM_POND 5
#define NUM_REAGENT 5
#define NUM_SENSOR 4
enum CmdCodeLinear {  // CODE LINEAR STARTS FROM 10X
  CMD_LINEAR_REAGENT_HOME = 101,
  CMD_LINEAR_POND_HOME = 102,
  CMD_LINEAR_VIM_HOME = 103,
  CMD_LINEAR_REAGENT_UP = 104,
  CMD_LINEAR_REAGENT_DOWN = 105,
  CMD_LINEAR_POND_UP = 106,
  CMD_LINEAR_POND_DOWN = 107,
  CMD_LINEAR_VIM_UP = 108,
  CMD_LINEAR_VIM_DOWN = 109,
};
enum CmdCodeTurnTable {  // CODE TURNTABLE STARTS FROM 20X
  CMD_TURNTABLE_HOME = 201,
  CMD_TURNTABLE_CW = 202,
  CMD_TURNTABLE_CCW = 203,
};
enum CmdCodePump {  // CODE PUMP STARTS FROM 30X
  CMD_PUMP_STEPPER_UP = 301,
  CMD_PUMP_STEPPER_DOWN = 302,
  CMD_PUMP_POND = 303,
  CMD_PUMP_PURITY = 304,
  CMD_PUMP_VIM = 305,
  CMD_PUMP_ADD_COOLING = 306,
  CMD_PUMP_CYCLE_COOLING = 307,
  CMD_PUMP_DIS = 308,
};
enum CmdSensor {  // CODE SENSOR STARTS FROM 40X
  CMD_PH = 401,
  CMD_SANILITY = 402,
  CMD_DO = 403,
  CMD_TEMP = 404,
  CMD_PH_CALIB_1 = 405,
  CMD_PH_CALIB_2 = 406,
  CMD_SANILITY_CALIB_1 = 407,
  CMD_SANILITY_CALIB_2 = 408,
  CMD_DO_CALIB_1 = 409,
  CMD_DO_CALIB_2 = 410,
  CMD_TEMP_CALIB_1 = 411,
  CMD_TEMP_CALIB_2 = 412,
  CMD_CHECK_RS485 = 499,
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
  const char *current_state;
  bool is_measuring;
  bool new_water;
  bool lora_working;
  int8_t current_reagent;
  int8_t current_pond;
  int8_t current_pos;
  int8_t current_drop;
  float temp_machine;
  int16_t err_code;
};

struct ParamCalib {
  uint8_t num_index;
  uint8_t *leds;
  float *coefs;
  float inters;
};
struct ParamReagent {
  bool enable;
  const char *name_reagent;
  const uint8_t amount;
  const uint8_t *pos;
  const uint8_t *method;
  const float *drops_will_use;
  const float ml_water;
  const float diluting_rate;
  const uint16_t *time_stir;
  const bool *check_color;
  uint8_t max_test;
  uint8_t current_count;
  const uint8_t *led_detect;
  ParamCalib paramCalib;
};
struct ParamSensor {
  bool has_in_machine;  // use for sensor inside the machine
  bool err_in_machine;
  const char *name_sensor;
  bool *enable_each_pond;
  int16_t *err_each_pond;
  uint16_t err_code;
  float *coefficient;
  const uint8_t *eeprom_point_calib;
  const float *raw_value;
  float value;
};
struct ParamPond {
  bool enable;
  bool err;
  char *pond_module_id;
  char *pond_id;
  char *sche;
  char *communication_type;
  uint16_t pump_duration;
  uint16_t err_code;
};
struct ListReagentNow {
  uint8_t num_reagent_now;
  int8_t pond;
  int8_t *list_reagent;
  char *test_type;
  char *test_id;
  uint8_t point_index;
};
struct ListErr {
  const char *name_err;
  const uint16_t err_code;
  const uint8_t times_err;
};
#endif
