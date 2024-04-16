#ifndef Func_Main_h
#define Func_Main_h
#include "SIMA7xxx.h"
#include "PIN_CONFIG.h"

class class_FuncMain {
  private:
    bool connectSim();
  public:
    void Setup();
    void Running();
};

#endif
