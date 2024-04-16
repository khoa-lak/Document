#include <RHReliableDatagram.h>
#include <RH_RF95.h>
#include <SPI.h>

#define Master 1
#define Slave 2

RH_RF95 driver;
RHReliableDatagram manager(driver, Slave);

class TruyenLoraWQ {
  public:
    unsigned long _timeStart;
    String _MesReceiver; //Chuỗi nhận json từ master
    uint8_t _mesMaster[10];
    int _stateLora;
    uint8_t _mesFBack[10];
    void ham_Run() {
      _mesFBack[0] = Slave;
      if (manager.available())
      {
        uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
        uint8_t len = sizeof(buf);
        uint8_t from;
        if (manager.recvfromAck(buf, &len, &from))
        {
          _MesReceiver = buf;
          
        }
        if (!manager.sendtoWait(_mesFBack, sizeof(_mesFBack), Master ))
        {
          _stateLora = 2;
          _timeStart = millis();
        }
        else {
          _stateLora = 1;
          _timeStart = millis();
        }
      }

    }

};
