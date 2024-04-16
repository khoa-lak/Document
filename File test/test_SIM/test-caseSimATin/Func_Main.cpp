#include "Func_Main.h"
CLASS_SIMA7xxx simA76xxx;
void class_FuncMain::Setup() {
  simA76xxx.Func_SetupSim();
  return;
}
void class_FuncMain::Running() {
  connectSim();
  return;
}
bool class_FuncMain::connectSim() {
  if (simA76xxx.Func_InitSim())
    simA76xxx.Func_FollowTopic();
  //bool done = simA76xxx.ui8_commandRun;
  return 1;
}
