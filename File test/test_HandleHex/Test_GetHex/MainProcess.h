#ifndef MainProcess_h
#define MainProcess_h
#include "simA7600.h"
#include "SDCard.h"
class MainProcess_ {
  private:
    void refeshVariable();
    bool connectSim();
    uint32_t delaySimStable = millis();
  public:
    bool allowed_call_sim = false;
    bool pubMessage();
    bool getFile();
    bool funcMain();
    void funcSetup();
    void funcWriteFlash();
};
#endif
