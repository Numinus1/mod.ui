#include "sercomm.h"

SerComm::SerComm(QObject *parent) :
    QObject(parent)
{
    //initialize QSerialPort
    serial = new QSerialPort;
    serial->setFlowControl(QSerialPort::NoFlowControl);
    serial->setDataBits(QSerialPort::Data8);

    //initialize FSM
    sc_State = scDisconnected;

    //initialize state flags
    sc_replyReady = false;

    //intialize Flags
    g_sc_Error = scNoError;
    g_sc_Write = false;
    g_sc_Discon = false;



    //initialize other vars
    transbuff = NULL;
    inSize = 0;
    outSize = 0;

    reseter = false;
    localError = false;

    payload = NULL;
    payloadLength = 0;
}

bool SerComm::serviceCOMM()
{
    if (sc_State == scIdle){
        return false;
    }
    else{
        return true;
    }
}

void SerComm::reset(bool local)
{
    if (serial->isOpen())
        serial->close();

        g_sc_Error = scNoError;
        g_sc_Write = false;
        g_sc_Discon = false;

        if (transbuff != NULL)
            delete transbuff;

        transbuff = NULL;

        reseter = true;
        Receive();

        reseter = false;
        localError = true;

        sc_State = scDisconnected;
        sc_replyReady = false;

        delete serial;
        serial = new QSerialPort;
        serial->setFlowControl(QSerialPort::NoFlowControl);
        serial->setDataBits(QSerialPort::Data8);


}

void SerComm::setCOM(QString portName)
{
    serial->setPortName(portName);
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);

    if (serial->open(QIODevice::ReadWrite))
    {
        qDebug() << "port #" << portName << " opened";
    }
}

bool SerComm::openCOMPort(QString portN, QString baudR,
                 QString parity, QString stopB)
{
    if (sc_State == scDisconnected){
        qDebug() << "port: " << portN << endl << "baud: " << baudR << endl
                 << "parity: " << parity << endl << "stops: " << stopB << endl;

        //set name
        serial->setPortName(portN);

        //set baudrate
        if (baudR == "9600")
            serial->setBaudRate(QSerialPort::Baud9600);
        else if (baudR == "19200")
            serial->setBaudRate(QSerialPort::Baud19200);
        else if (baudR == "38400")
            serial->setBaudRate(QSerialPort::Baud38400);
        else if (baudR == "115200")
            serial->setBaudRate(QSerialPort::Baud115200);

        //set parity
        if (parity == "No Parity")
            serial->setParity(QSerialPort::NoParity);
        else if (parity == "Even Parity")
            serial->setParity(QSerialPort::EvenParity);
        else if (parity == "Odd Parity")
            serial->setParity(QSerialPort::OddParity);

        //set stop bits
        if (stopB == "1")
            serial->setStopBits(QSerialPort::OneStop);
        else if (stopB == "2")
            serial->setStopBits(QSerialPort::TwoStop);

        if (serial->open(QIODevice::ReadWrite))
        {
            sc_State = scIdle;
            qDebug() << "opened";
            return true;
        }
        else
        {
            qDebug() << "closed";
            return false;
        }
    }
    else{
        return false;
    }
}

bool SerComm::closeCOMPort()
{
    if (serial->isOpen()){
        serial->close();
        sc_State = scDisconnected;
        reset(true);
        return true;
    }
    else{
        return false;
    }
}

int SerComm::showState()
{
    return sc_State;
}

int SerComm::showError()
{
    return g_sc_Error;
}

void SerComm::load_transbuff(unsigned char *cmd, int size)
{
    outSize = size;

    if (transbuff != NULL){
        delete transbuff;
        transbuff = NULL;
    }

    transbuff = new unsigned char[outSize];

    for (int i = 0; i < outSize; i++)
    {
        transbuff[i] = cmd[i];
    }

    if (transbuff[1] == 0x10){
        expectedSize = 8;
    }
    else{
        expectedSize = (int) transbuff[5];
        expectedSize *= 2;
        expectedSize += 5;
    }

}

bool SerComm::setWrite(unsigned char* modcmd, int size)
{
    if (sc_State == scIdle){
        load_transbuff(modcmd, size);
        g_sc_Write = true;
        sc_State = scReady;
        return true;
    }
    else{
        return false;
    }
}

bool SerComm::fauxTransmit()
{
    if (outSize > 250)
        qDebug() << "fauxTransmit(): massive outSize";

    return true;
}

bool SerComm::Transmit()
{
    int success = serial->write((char*)transbuff, (qint64)outSize);
    if (success == outSize)
    {
        if (transbuff[1] == 0x10){
        char *testReply = ucharToHexStr(transbuff, outSize);
        qDebug() << "Sent ModSTR: " << testReply;
        delete testReply;
        }
        return true;
    }
    else
    {
        char *testReply = ucharToHexStr(transbuff, outSize);
        qDebug() << "SerComm::Transmit(): transmit failed: " << testReply;
        delete testReply;
        return false;
    }
}

//ensures that the registers read/written are the same in the modbus command and reply
bool SerComm::checkDestRegs()
{
    if (transbuff[1] == 0x10)
    {
        if (recvbuff[1] == 0x10)
        {
            if ((transbuff[3] == recvbuff[3]) && (transbuff[5] == recvbuff[5])){
                return true;
            }
            else{
                g_sc_Error = scRegisterMismatchError;
                return false;
            }

        }
        else
        {
            g_sc_Error = scMODCODEMismatchError;
            return false;
        }
    }
    else if (transbuff[1] == 0x04)
    {
        if (recvbuff[1] == 0x04)
        {
            int dataSize = (int)transbuff[5] * 2;
            if (dataSize == (int)recvbuff[2]){
                return true;
            }
            else{
                g_sc_Error = scRegisterMismatchError;
                return false;
            }
        }
        else
        {
            g_sc_Error = scMODCODEMismatchError;
            return false;
        }
    }
    else
    {
        g_sc_Error = scMODCODEUnknownError;
        return false;
    }
}

//ensures crc is correct
bool SerComm::checkCRC()
{
    unsigned char* temp = new unsigned char[inSize];
    for (int i = 0; i < inSize; i++)
        temp[i] = recvbuff[i];

    if (CRC_Check(temp, inSize)){
        return true;
    }
    else{
        g_sc_Error = scCRCError;
        return false;
    }

}

int SerComm::fauxReceive()
{
    if (payload != NULL){
        delete payload;
        payload = NULL;
    }

    payload = new unsigned char[8];

    for (int i = 0; i < 8; i++)
    {
        payload[i] = (unsigned char) qrand();
    }

    payloadLength = 8;

    return 1;
}

int SerComm::Receive()
{
    static int state = 0;
    static int delayCount = 0;

    if (reseter||localError){
        state = 0;
        delayCount = 0;
        return -1;
    }

    if (!serial->isOpen())
        return -2;

    switch (state) {

    //wait for reply
    case 0:
        if (serial->bytesAvailable())
        {
            delay(10);
            delayCount = 0;
            state = 1;
            inSize = 0;
            return 0;
        }
        else
        {
            if (delayCount < 10)
            {
            delay(10);
            delayCount += 1;
            state = 0;
            return 0;
            }
            else{
                state = 0;
                delayCount = 0;
                qDebug() << "reply timeout";
                g_sc_Error = scTimeoutError;
                sc_State = scError;
                return -1;
            }
        }
    break;

    //take in reply and check registers in response & CRC
    case 1:

        while (serial->bytesAvailable())
         {
             if (serial->getChar((char*)recvbuff + inSize)){
                 inSize += 1;
             }
         }
        if (inSize == expectedSize)
        {
            if (checkCRC()&& checkDestRegs()){
                state = 0;

                unsigned char* tester0 = new unsigned char[inSize];

                for (int i = 0; i < inSize; i++)
                {
                    tester0[i] = recvbuff[i];
                }
                if (tester0[1] == 0x10){
                char* tester1 = ucharToHexStr(tester0, inSize);
                qDebug() << "Received Reply: " << tester1 << endl;


                delete tester1;
                }

                delete tester0;

                if (payload != NULL){
                    delete payload;
                    payload = NULL;
                }

                payload = new unsigned char[inSize];

                for (int i = 0; i < inSize; i++)
                {
                    payload[i] = recvbuff[i];
                }

                payloadLength = inSize;

                return 1;
            }
            else{
                state = 0;
                qDebug() << "reply size error";
                return -1;
            }

        }
        else
        {
            if (delayCount < 100)
            {
                delay(10);
                state = 1;
                delayCount += 1;
                return 0;
            }
            else
            {
                delayCount = 0;
                state = 0;
                g_sc_Error = scTimeoutError;
                return -1;
            }
        }
    break;

    default:
        return -10;
        break;
    }

}



unsigned char* SerComm::generate_payload()
{
    unsigned char* str = new unsigned char[inSize];

    for (int i = 0; i < inSize; i++)
    {
        str[i] = recvbuff[i];
    }

    sc_replyReady = false;

    return str;
}

unsigned char* SerComm::get_payload()
{
    return generate_payload();
}

void SerComm::squelchReadyFlag()
{
    sc_replyReady = false;
}

int SerComm::getInSize()
{
    return inSize;
}

int SerComm::getError()
{
    int returner = g_sc_Error;
    //g_sc_Error = scNoError;
    return returner;
}

int SerComm::get_scState()
{
    return sc_State;
}

bool SerComm::getReplyReady()
{
    return sc_replyReady;
}

bool SerComm::getWriteFlag()
{
    return g_sc_Write;
}

bool SerComm::getContinueError()
{
    if (localError){
        Receive();
        localError = false;
        return true;
    }
    else
    {
        return false;
    }
}

void SerComm::exeFSM()
{

    int err = serial->error();
    if (err == QSerialPort::NoError){
    sc_State = fsm();
    static int lastResult = -999;
    //qDebug() << "executed comm_fsm";

    if (lastResult != sc_State){
        lastResult = sc_State;
        //qDebug() << "scState: " << sc_State;
    }
    }
    else{
        reset(false);
        localError = true;
    }
}

int SerComm::fsm()
{
    if ((sc_State != scDisconnected) && (!serial->isOpen())){
        reset(false);
        return scDisconnected;
    }

    switch (sc_State)
    {
        case scDisconnected:
            if (serial->isOpen())
                return scIdle;
            else
                return scDisconnected;
        break;

    case scIdle:
            return scIdle;
        break;

        case scReady:

            if (!sc_replyReady)
            {
                if (g_sc_Write){
                    g_sc_Write = false;
                    return scTransmit;
                }
                else{
                    return scReady;
                }
            }
            else{
                return scReady;
            }
        break;

        case scTransmit:
            if (Transmit())
                return scListen;
            else
                return scError;
        break;

        case scListen:
        {
        int rec = Receive();
            if (rec == 0){
                return scListen;
            }
            else if (rec == 1)
            {
                sc_replyReady = true;
                return scIdle;
            }
            else{
                qDebug() << "returning error in Receive()";
                return scError;
            }
        }
        break;

        case scError:
        if (scError > 1){
            qDebug() << "scERROR: " << g_sc_Error;
            localError = true;
            return scReady;
        }
        break;
    }
}
