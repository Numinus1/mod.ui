#ifndef CONSTANTS_H
#define CONSTANTS_H

//Backend
#define bkPreConfig 0
#define bkPreConnect 1
#define bkTryConnect 2
#define bkPrelim 3
#define bkPoll 4
#define bkPost 5
#define bkWrite 6
#define bkWriteButton 7
#define bkSaveConfig 8
#define bkLoadConfig 9
#define bkReset 10
#define bkTest 11

//SerComm
    //FSM States
    #define scDisconnected 0
    #define scIdle 1
    #define scReady 2
    #define scTransmit 3
    #define scListen 4
    #define scError 5

    //Error States
    #define scNoError 0
    #define scSendError 1
    #define scTimeoutError 2
    #define scCRCError 3
    #define scMODCODEMismatchError 4
    #define scMODCODEUnknownError 5
    #define scRegisterMismatchError 6

//ModbusModule
    //FSM
    #define mbConnector 0
    #define mbIdle 1
    #define mbReady 2
    #define mbWrite 3
    #define mbRead 4
    #define mbError 5

//Parameter Type
    #define paramInt 0
    #define paramQ24 1
    #define paramQ24_1k 2
    #define paramQ20 3

//Box Type
    #define boxStable 0
    #define boxMutable 1
    #define boxButton 2
    #define boxConfig -99

//UI
    #define maxCards 7



#endif // CONSTANTS_H
