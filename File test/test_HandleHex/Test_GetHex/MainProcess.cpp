#include "MainProcess.h"
SimA7600 mainSim;
SDCard_ mainSD;
void MainProcess_::funcSetup() {
  refeshVariable();
  mainSim.setupSim();
  mainSD.sdSetup();
  return;
}
void MainProcess_::refeshVariable() {
  delaySimStable = millis();
  return;
}
bool MainProcess_::funcMain() {
  if (connectSim())
    return true;
  return false;
}
bool MainProcess_::connectSim() {
  if (millis() - delaySimStable >= 1 * 60000) {
    if (mainSim.initSim()) {
      mainSim.followTopic();
      if (mainSim.runCMD())
        return true;
    }
  }
  return false;
}
bool MainProcess_::pubMessage() {
  char* mes = "gửi mes lên topic";
  mainSim.pubStatus(mes);
  return true;
}
bool MainProcess_::getFile() {
  mainSim.getFileHex();
  return true;
}
void MainProcess_::funcWriteFlash() {
  char fileHex[9000] = {'0'};
  mainSD.hexFile(fileHex);
  Serial.println(fileHex);

  return;
}
