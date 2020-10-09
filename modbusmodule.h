#ifndef MODBUSMODULE_H
#define MODBUSMODULE_H

#include "modstrgen.h"

class ModbusModule : public QObject
{
    Q_OBJECT

public:
    explicit ModbusModule(QObject *parent = 0);
    void reset();
    int fsm();
    void EXEfsm();
    void setDeviceAddress(int Address);


    void setWriteFlag(MotorParameter param);
    bool getWriteFlag();
    void setReadFlag(ParameterBox box);
    bool getReadFlag();
    void squelch_SC_Flag();

    int getReadResult();
    int getWriteResult();

    int get_scError();

    bool getWriteStatus();
    void setWriteStatus();


    int getSweepResult();

    void assembleReadResult(unsigned char* replyStr, int size);

    ParameterBox getReadBox();
    bool getReadResultFlag();
    bool serviceMODB();

    //manual serial port
    bool manOpenPort(QString portN, QString baudR,
                     QString parity, QString stopB);
    bool manClosePort();

    //test
    void testWrite(MotorParameter testParam);


private:
    //Drive
    unsigned char DeviceAddress;
    int ComPing();
    int ComSweep();
    SerComm *comm;
    int connTrial;

    int mb_State;

    bool writeFlag;
    bool writeDone;
    MotorParameter outParam;

    bool readFlag;
    ParameterBox inBox;

    ParameterBox outBox;
    bool readresultFlag;

    ModStrGen gen;


public:
    bool extError;
    bool localError;
    bool connPrompt;
    bool bkError;


};
#endif // MODBUSMODULE_H
