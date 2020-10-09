#include "modbusmodule.h"

ModbusModule::ModbusModule(QObject *parent) :
    QObject(parent)
{
    comm = new SerComm;

    DeviceAddress = 0x01;

    connPrompt = false;
    connTrial = 0;
    writeFlag = false;
    writeDone = false;
    readFlag = false;
    mb_State = mbConnector;

    extError = false;
    localError = false;
    bkError = false;

    inBox.setUp(1, 0x00, 0, 0);
    inBox.setTitle("MODB inBox");

    MotorParameter temper;
    temper.setSize(1);
    temper.setFactor(0);
    temper.setAddr(0x00);
    temper.setValue("0");
    temper.setTransient("0");

    inBox.setMotorParameter(temper, 0);

    inBox.setByteCount();

}

bool ModbusModule::serviceMODB()
{
    if (mb_State == mbIdle){
        return false;
    }
    else{
        return true;
    }
}

void ModbusModule::reset()
{
    if (comm != NULL)
    {
        delete comm;
        comm = NULL;
    }
    comm = new SerComm;

    DeviceAddress = 0x01;

    connPrompt = false;
    connTrial = 0;
    writeFlag = false;
    readFlag = false;
    mb_State = mbConnector;

    extError = true;
    ComPing();
    ComSweep();
    getReadResult();
    getWriteResult();
    extError = false;
    localError = false;
    bkError = false;

    comm->reset(true);
}

void ModbusModule::setWriteFlag(MotorParameter param)
{
    qDebug() << "ModbusModule::setWriteFlag(): Setting Write Flag";
    writeFlag = true;
    outParam.copyParam(param);
    mb_State = mbReady;
    qDebug() << "Write flag set";
}

bool ModbusModule::getWriteFlag()
{
    return writeFlag;
}

void ModbusModule::setReadFlag(ParameterBox box)
{
    readFlag = true;
    inBox.overwriteBox(box);
    readresultFlag = false;
    mb_State = mbReady;
}

bool ModbusModule::getReadFlag()
{
    return readFlag;
}

void ModbusModule::squelch_SC_Flag()
{
    comm->squelchReadyFlag();
}

void ModbusModule::assembleReadResult(unsigned char *replyStr, int size)
{
    outBox.overwriteBox(inBox);
    outBox.assignValues(replyStr, size);
    readresultFlag = true;

    delete replyStr;
}

bool ModbusModule::getReadResultFlag()
{
    return readresultFlag;
}

int ModbusModule::get_scError()
{
    return comm->getError();
}

ParameterBox ModbusModule::getReadBox()
{
    ParameterBox returner;
    returner.overwriteBox(outBox);
    readresultFlag = false;

    return returner;
}

int ModbusModule::getReadResult()
{

    static int state = 0;
    static unsigned char* modstr = NULL;

    if ((extError)||(localError)){
        state = 0;
        return -1;
    }

    switch (state) {
    case 0:
        if (modstr != NULL){
            delete modstr;
            modstr = NULL;
        }

        modstr = gen.prepReadCommand(inBox, DeviceAddress);
        state = 1;
        break;

    case 1:
        if (comm->setWrite(modstr, 8)){
            state = 2;
        }
        else
            state = 1;
        break;

    case 2:
        if (comm->getReplyReady())
        {
            //unsigned char* newData =
            assembleReadResult(comm->get_payload(), comm->getInSize());
            state = 0;
            return 1;
        }
        else
        {
            state = 2;
            return 0;
        }
        break;

    default:
        state = 0;
        break;
    }

    return 0;
}

int ModbusModule::getWriteResult()
{

    static int state = 0;
    static unsigned char* modstr = NULL;
    static int modlen;

    qDebug() << "ModbusModule::getWriteResult(): ";

    if ((extError)||(localError)){
        state = 0;
        return -1;
    }

    switch (state) {
    case 0:
    {
        if (modstr != NULL)
            delete modstr;

        modstr = gen.prepWriteCommand(outParam, DeviceAddress, &modlen);
        state = 1;
        char* tester = ucharToHexStr(modstr, modlen);
        qDebug() << "modstr(" << modlen << "): " << tester;
        delete tester;
    }
        break;

    case 1:
        if (comm->setWrite(modstr, modlen)){
            state = 2;
        }
        else
            state = 1;
        break;

    case 2:
        if (comm->getReplyReady())
        {
            writeFlag = false;
            state = 0;
            writeDone = true;
            return 1;
        }
        else
        {
            state = 2;
            return 0;
        }
        break;

    default:
        state = 0;
        break;
    }

    return 0;
}

int ModbusModule::ComPing()
{
    static int state = 0;
    static int delayCount = 0;

    if (extError||localError){
        state = 0;
        delayCount = 0;
        return -1;
    }

    if (delayCount > 20){
        delayCount = 0;
        state = 0;
        localError = true;
        getReadResult();
        return -1;
    }



    switch (state) {

    case 0:{
        int done = getReadResult();
        state = ((done == 0) ? 0 : 1);
        if (done == 0){
            delay(20);
            delayCount += 1;
        }
    }
        break;

    case 1:{
        state = 0;
        delayCount = 0;
        //DriveAddress = scReply[0];
        return 1;
    }
        break;

    default:
        state = 0;
        break;
    }

    return 0;
}

int ModbusModule::ComSweep()
{
    static int state = 0;

    static int i = 0;
    static int index = 0;
    static QString pnames[20];

    int scState = comm->get_scState();

    if (extError||localError){
        state = 0;
        index = 0;
        i = 0;
        return -1;
    }
    switch (state) {

    case 0:{

        QList<QSerialPortInfo> com_ports = QSerialPortInfo::availablePorts();
        QSerialPortInfo port;
        foreach (port, com_ports)
        {
            pnames[index] = port.portName();
            //qDebug() << pnames[index];
            index++;
        }
        state += 1;

        if (index == 0)
            return -1;
    }
        break;

    case 1:{
        if (scState != scDisconnected)
        {
            comm->closeCOMPort();
        }
        state += 1;
    }
        break;

    case 2:{
        if (scState == scDisconnected)
        {
            state += 1;
            comm->setCOM(pnames[i]);
        }
        else
        {
            comm->closeCOMPort();
        }
    }
    case 3:{
        int connStatus = ComPing();
        if (connStatus == -1)
        {
            if (i == (index - 1)){
                state = 0;
                i = 0;
                index = 0;
                return -1;
            }
            else{
                i += 1;
                state = 1;
            }
        }
        else if (connStatus == 0)
        {
            state = 3;
        }
        else if (connStatus == 1)
        {
            state = 0;
            i = 0;
            index = 0;
            return 1;
        }
    }
        break;

    default:
        state = 0;
        i = 0;
        index = 0;
        break;
    }

    return 0;
}

int ModbusModule::getSweepResult()
{
    connPrompt = true;
    return connTrial;
}

void ModbusModule::setDeviceAddress(int Address)
{
    DeviceAddress = (unsigned char) Address;
}

void ModbusModule::EXEfsm()
{
    int sc_status = comm->get_scState();

    static int lastResult = -999;

    if (lastResult != mb_State){
        lastResult = mb_State;
        //qDebug() << "mbState: " << mb_State;
    }

    if (comm->getContinueError())
    {
        localError = true;
        getReadResult();
        getWriteResult();
        ComPing();
        ComSweep();
        mb_State = mbConnector;
        localError = false;
        bkError = true;
        connPrompt = false;
    }

    if (sc_status == scDisconnected)
    {
        comm->exeFSM();
        mb_State = fsm();
    }
    else if (comm->serviceCOMM()){
        comm->exeFSM();
    }
    else{
        mb_State = fsm();
    }
//    if (sc_status == scDisconnected)
//        mb_State = fsm();
//    else if ((sc_status == scReady)&&(!(comm->getWriteFlag())))
//        mb_State = fsm();
//    else
//        comm->exeFSM();

}

int ModbusModule::fsm()
{

    switch (mb_State) {

    case mbConnector:
    {
        int conStat;
        if (connPrompt)
        {
            conStat = ComSweep();

            if (conStat == -1){
                connPrompt = false;
                connTrial = -1;
                //no drive found code
                return mbConnector;
            }
            else if (conStat == 0){
                connPrompt = true;
                connTrial = 0;
                return mbConnector;
            }
            else{
                connPrompt = false;
                connTrial = 1;
                return mbIdle;
            }
        }
        else
        {
            return mbConnector;
        }

        break;
    }

    case mbIdle:
        return mbIdle;
        break;

    case mbReady:
    {
        int scState = comm->get_scState();

        if (scState == scIdle)
        {
            if (writeFlag){
                return mbWrite;
            }
            else if (readFlag){
                return mbRead;
            }
            else{
                return mbIdle;
            }
        }
        else
        {
            return mbReady;
        }
        break;
    }

    case mbWrite:
    {

        writeFlag = false;
        int writeResult = getWriteResult();

        if (writeResult == 0)
        {
            return mbWrite;
        }
        else if (writeResult == 1)
        {
            return mbIdle;
        }

        break;
    }

    case mbRead:
    {

        readFlag = false;

        int readResult = getReadResult();

        if (readResult == 0)
        {
            return mbRead;
        }
        else if (readResult == 1)
        {
            return mbIdle;
        }
    }

    default:
        return mbConnector;
        break;
    }

    return mbConnector;
}

void ModbusModule::testWrite(MotorParameter testParam)
{
    int modlen;
    unsigned char* modstr = gen.prepWriteCommand(testParam, DeviceAddress, &modlen);
    char *testReply = ucharToHexStr(modstr, modlen);
    qDebug() << "modlen: " << modlen;
    qDebug() << "Synthesized ModSTR: " << testReply;
    delete testReply;
    delete modstr;
}

bool ModbusModule::manOpenPort(QString portN, QString baudR,
                               QString parity, QString stopB)
{
    if ((mb_State == mbConnector) || (mb_State == mbIdle)){
        if (comm->openCOMPort(portN, baudR, parity, stopB))
        {
            connTrial = true;
            mb_State = mbIdle;
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

bool ModbusModule::manClosePort()
{
    reset();
    if (comm->closeCOMPort()){
        return true;
    }
    else{
        return false;
    }

}

bool ModbusModule::getWriteStatus()
{
    return writeDone;
}

void ModbusModule::setWriteStatus()
{
        writeDone = false;
}

