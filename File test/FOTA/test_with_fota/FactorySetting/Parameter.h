#ifndef parameter_h
#define parameter_h
#include <IWatchdog.h>
#include "PinDefinition.h"
#include "GeneralFunction.h"
#define FLASH_PAGE_SIZE 0x800        // 2KB mỗi Page
#define OTA_STORAGE_ADDR 0x08028000  // Địa chỉ vùng OTA bạn đã chọn
#define FLAG_ADDR 0x0803F800         // Page cuối cùng lưu Flag
#define MAX_MSG_SIZE 4096

#define HOST "mqtt.iotoom.com"
#define PORT 1883
#define USER "lab"
#define PASSWORD "IotoomVN"
#define DEVICE_TYPE "pwq"
#define FIRM_WARE "firm ware - 1111"
#define DEVICE_ID "pond-module-14a454a86fe64701b7c72e650627187e"
#define TOPIC_DEVICE "devices/pond-module-14a454a86fe64701b7c72e650627187e"
#define TOPIC_DEVICE_LOGS "devices/pond-module-14a454a86fe64701b7c72e650627187e/check_logs"

#define MSG_TYPE_CMD "\"message_type\":\"command\""
#define MSG_TYPE_STG "\"message_type\":\"settings\""
#define CMD_ON "\"command\":\"on\""
#define CMD_OFF "\"command\":\"off\""
#define CMD_MEASURE_NOW "\"command\":\"measure_now\""
#define CMD_CALIB_ON "\"command\":\"calib_on\""
#define CMD_CALIB_OFF "\"command\":\"calib_off\""
#define CMD_MANUAL "\"command\":\"manual\""
#define CMD_INIT_TUBE "\"command\":\"init_tube\""
#define CMD_CONTROL_PUMP "\"command\":\"control_pump\""
#define CMD_COMPLETED "command_completed"
#define CMD_DENIED "command_denied"
#define CMD_MANUAL_CPLT "manual_completed"
#define CMD_UPLOAD_CODE "\"command\":\"upload_code\""
#define CMD_FACTORY_SETTING "\"command\":\"return_factory_setting\""
#define CMD_WHILE "\"command\":\"while\""
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

#define MAX_STATE_LEN 32
#define MAX_ID_LEN 64
#define NUM_SENSOR 4

#define ERROR_CODE_SENSOR(type, index) (type + (index) + 1)
enum ListErrModule {
  ERR_SENSOR_ENABLE = 100,
  ERR_SENSOR_UNPLUGGED = 200,
  ERR_SENSOR_ERROR = 300,
  ERR_PUMP_UNPLUGGED = 401
};
struct ModuleSetting {
  char current_state[MAX_STATE_LEN];
  char pond_id[MAX_ID_LEN];
  bool jack_pump_plug;
  bool is_pump_run;
  uint16_t time_pump;
  uint32_t pump_start_time;
  int8_t sensor_calib;
  int8_t point_calib;
  uint16_t err_code;
};
struct ParamSensor {
  bool enable;
  bool err;
  bool jack_plug;
  const char* name_sensor;
  uint16_t err_code;
  uint16_t interval_read;
  uint32_t previous_time;
  float coefficient[2];
  uint8_t eeprom_point_calib[2];
  float raw_value[2];
  float value;
};

#endif
