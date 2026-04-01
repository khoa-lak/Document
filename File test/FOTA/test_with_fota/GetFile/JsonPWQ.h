#ifndef json_pwq_h
#define json_pwq_h
#include <ArduinoJson.h>
#include "Parameter.h"
#define MSG_MIN 255
#define MSG_MID 511
#define MSG_MAX 2048
class JsonPWQ {
private:
  char _sharedBufferMin[MSG_MIN];
  char _sharedBufferMid[MSG_MID];
  char _sharedBufferMax[MSG_MAX];
  char _txBuffer[MAX_MSG_SIZE];  // Buffer tĩnh để tạo JSON gửi đi
public:
  bool isJson(const char* js);
  void handleMsgSettingDevice(char* msg, ModuleSetting& moduleStg, ParamSensor paramSensor[NUM_SENSOR]);
  bool handleMsgTurnPump(char* msg, ModuleSetting& moduleStg);
  char* msgCMDCplt(const char* msg_type);
  char* msgCMDDenied(const char* msg_type);
  char* msgStateError(uint16_t error_code);
  char* msgResultCalib(const char* state);

  char* msgStateMachine(ModuleSetting moduleStg, ParamSensor paramSensor[NUM_SENSOR]);
  char* msgStatus(char* state);
  char* msgValueSensor(char* pond_id, ParamSensor paramSensor);
  char* commandRespond(char* cmd_fb);
};
#endif
//msg receive
//{"module_id":"pond-module-06d857966e3b452eb27c03ff35b0c1b4","msg_type":"update_state","state":"calib||running"}
//{"module_id":"pond-module-06d857966e3b452eb27c03ff35b0c1b4","msg_type":"get_value","payload":[{"name":"DO","enable":false},{"name":"TEMP","enable":false}]} -> msg get value sensor
//{"module_id":"pond-module-06d857966e3b452eb27c03ff35b0c1b4","msg_type":"calib","name":"DO","point_index":1}
//{"module_id":"pond-module-06d857966e3b452eb27c03ff35b0c1b4","msg_type":"control_pump","enable":"true":"time":400}

//msg send
//{"module_id":"pond-module-06d857966e3b452eb27c03ff35b0c1b4","msg_type":"control_pump","command":"completed"}
//{"module_id":"pond-module-06d857966e3b452eb27c03ff35b0c1b4","msg_type":"calib","command":"completed"}
//{"module_id":"pond-module-06d857966e3b452eb27c03ff35b0c1b4","msg_type":"get_value","payload":[{"name":"DO","value":7.85},{"name":"TEMP","value":35.9}]}
