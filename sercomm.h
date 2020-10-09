#ifndef SERCOMM_H
#define SERCOMM_H

#include "paramwarehouse.h"

class SerComm : public QObject
{
    Q_OBJECT

public:
    explicit SerComm(QObject *parent = 0);

private://data

    //FSM
    int sc_State;
    int fsm();

    //private data members
    QSerialPort *serial;


    //Buffers
    unsigned char recvbuff[250];
    int inSize;
    int expectedSize;

    unsigned char* transbuff;
    int outSize;

    //flags
    bool reseter;
    bool localError;

    //**outgoing flags
    int g_sc_Error;
    bool sc_replyReady;

    //**incoming flags
    bool g_sc_Write;
    bool g_sc_Discon;

private: //functions

    //data channels
    void load_transbuff(unsigned char* , int);
    unsigned char* generate_payload();

    //transmit & receive
    bool Transmit();
    bool fauxTransmit();
    int Receive();
    int fauxReceive();

    //parse reply
    bool checkDestRegs();
    bool checkCRC();

public:
    //execute
    void exeFSM();
    bool serviceCOMM();

    //data channels
    unsigned char* get_payload();
    int getInSize();
    int get_scState();
    bool getReplyReady();
    bool getWriteFlag();
    int getError();
    bool getContinueError();
    bool setWrite(unsigned char *modcmd, int size);
    void squelchReadyFlag();

    //outward data buffer
    unsigned char* payload;
    int payloadLength;

    //flags
    int showState();
    int showError();

    //formerly slots, now data 'settors'
public:
    bool openCOMPort(QString portN, QString baudR,
                     QString parity, QString stopB);
    bool closeCOMPort();
    void reset(bool local);
    void setCOM(QString portName);
};

#endif // SERCOMM_H
