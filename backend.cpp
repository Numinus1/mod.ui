#include "backend.h"

Backend::Backend(QObject *parent) :
    QObject(parent)
{
    storage = new ParamWarehouse;
    modb = new ModbusModule;

    bkState = bkPreConfig;
    paged = false;
    connstat = 0;
    pauseCommunication = false;

    liftIndex = 0;

    refresh = true;
    refreshCount = 0;
    writeTime = false;
    writeButtonTime = false;
    loadConfigTime = false;

    timer = new QTimer;
    connect(timer, SIGNAL(timeout()),
            this, SLOT(EXEfsm()));


    //test
    oneTime = 0;
    lockFSM = false;

    //ui component load-completion check
    componentComplete = 0;
    componentCount = 0;
}

void Backend::ark_reset()
{

    bkState = bkPreConnect;
    paged = true;
    connstat = 0;
    pauseCommunication = false;

    liftIndex = 0;

    refresh = true;
    refreshCount = 0;
    writeTime = false;
    writeButtonTime = false;
    loadConfigTime = false;

    //test
    oneTime = 0;
    lockFSM = false;

}

void Backend::pauseComm()
{
    pauseCommunication = true;
}

void Backend::EXEfsm()
{
    //static int mux = 0;

    static int lastResult = -999;

    if (lastResult != bkState){
        lastResult = bkState;
        //qDebug() << "bkState: " << bkState;
    }

    if (componentComplete >= componentCount){
        if (!modb->bkError){
            if (modb->getWriteStatus()){
                emit upWriteStatus(1);
                modb->setWriteStatus();
            }
            modb->EXEfsm();
            bkState = fsm();
        }
        else{
            qDebug() << "modb->bkError: true";
            modb->bkError = false;
            ark_reset();
        }
    }
    else{
        qDebug() << "incomplete components";
    }
}

void Backend::startSweep()
{
    qDebug() << "starting sweep";
    //modb->reset();
    connstat = 1;
    timer->start(20);
}

void Backend::refresh_statics()
{
    qDebug() << "refreshing";
    refresh = true;
    emit upWriteStatus(1);
}

void Backend::incrementComponents()
{
    componentComplete += 1;
    qDebug() << "component created: " << componentComplete<< "/" << componentCount ;

}

int Backend::fsm()
{

    switch (bkState) {
    case bkPreConfig:

        if (paged)
        {
            return bkPreConnect;
        }
        else{
            return bkPreConfig;
        }

        break;

    case bkPreConnect:

        if (connstat == 0)
        {
            return bkPreConnect;
        }
        else if (connstat == 1)
        {
            return bkTryConnect;
        }
        else
        {
            connstat = 0;
            return bkPreConnect;
        }

        break;

    case bkTryConnect:

        if (connstat == 1)
        {
            int result = modb->getSweepResult();
            static int lastResult = 99191919;

            if (lastResult != result){
                //qDebug() << "TryConnect result: " << result;
                lastResult = result;
            }

            if (result == -1)
            {
                //no drive found
                return bkPreConnect;
            }
            else if (result == 0)
            {
                //drive search in progress
                return bkTryConnect;
            }
            else if (result == 1)
            {
                //drive found
                connstat = 0;
                emit deviceConnected();
                return bkPrelim;
            }
        }
        else{
            return bkPreConnect;
        }
        break;

    //checks if stable variables need to be updated, sets the counter
    case bkPrelim:
    {
        if (refresh){
            refreshCount = 0;
            refresh = false;
            for (int i = 0; i < storage->getSetSize(); i++)
            {
                ParameterBox temp = storage->getBox(i);
                //qDebug() << "this box is type: " << temp.getType();
                int ftype = temp.getType();
                if (ftype == boxStable)
                    refreshCount += 1;
            }
        }

        return bkPoll;

    }
        break;
    //iterates through all parameter-boxes
    case bkPoll:
    {


        if (liftIndex >= storage->getSetSize()){
            liftIndex = 0;
            return bkPrelim;
        }
        else{
            if (modb->serviceMODB()){
                //qDebug() << "waiting on modb service";
                return bkPoll;
            }
            else{
                int type = storage->getBoxType(liftIndex);
                //if the box is stable-type and refresh has been asserted
                if ((type == 0)&&(refreshCount)){
                    refreshCount -= 1;
                    modb->setReadFlag(storage->getBox(liftIndex));
                    return bkPost;

                }
                //box is stable-type
                else if ((type == 1)||(type == 2)){

                    modb->setReadFlag(storage->getBox(liftIndex));
                    return bkPost;

                }
                //box is mutable but refresh has not been asserted
                else{
                    liftIndex++;
                    return bkPoll;
                }
            }
        }
    }
        break;

    case bkPost:
    {
        if (modb->get_scError())
        {
            return bkPoll;
        }
        else
        {
            //reply ready (with data)
            if (modb->getReadResultFlag())
            {
                lockFSM = true;
                ParameterBox temper = modb->getReadBox();
                storage->setBox(temper, liftIndex);

                for (int i = 0; i < temper.getCount(); i++)
                {
                    MotorParameter temppar = temper.getMotorParameter(i);
                    emit upChildVal(temper.getToken(), i, temppar.getValue());
                }

                liftIndex += 1;
                lockFSM = false;

                if (pauseCommunication){
                    pauseCommunication = false;
                    writeTime = false;
                    writeButtonTime =false;
                    return bkTryConnect;
                }
                else if ((writeTime)||(writeButtonTime)){
                    return bkWrite;
                }
                else
                    return bkPoll;
            }
            //still awaiting reply
            else
            {
                return bkPost;
            }
        }
    }
        break;

    case bkWrite:
    {
        static int box = 0;
        static int card = 0;
        static int permitType = -1;

        if (permitType == -1){
            if (writeButtonTime){
                writeButtonTime = false;
                permitType = boxButton;
            }
            else{
                if (writeTime){
                    writeTime = false;
                    permitType = boxStable;
                }
                else{
                    writeTime = false;
                    writeButtonTime = false;
                    permitType = -1;
                    return bkPrelim;
                }
            }
        }


        if (!modb->serviceMODB()){
            int boxType = storage->getBoxType(box);
            if (box == storage->getSetSize())
            {
                if (permitType == boxStable)
                    refresh = true;
                box = 0;
                card = 0;
                permitType = -1;
                modb->squelch_SC_Flag();
                return bkPrelim;
            }
            else if (boxType != permitType){
                box += 1;
                card = 0;
                return bkWrite;
            }
            else{
                int cardLimit = storage->getBoxSize(box);
                if (card == cardLimit)
                {
                    card = 0;
                    box += 1;
                    return bkWrite;
                }
                else{
                    if (storage->getWriteQuery(box, card)){
                        if (modb->serviceMODB()){
                            return bkWrite;
                        }
                        else{
                            modb->squelch_SC_Flag();
                            storage->setWriteQuery(box, card);
                            modb->setWriteFlag(storage->getParameter(box, card));
                            card += 1;
                            return bkWrite;
                        }
                    }
                    else{
                        card += 1;
                        return bkWrite;
                    }
                }
            }
        }
        else{
            return bkWrite;
        }
    }
        break;

    default:
        break;
    }




    return bkState;
}

void Backend::altloadSetupFile(QString path)
{
    if (storage->loadSetupFile(path)){

        paged = true;

        QList<QString> stablePageList;
        QList<QString> stableTitleList;

        QList<QString> mutablePageList;
        QList<QString> mutableTitleList;

        WriteBox panel;

        int stablePageCount = 0;
        int mutablePageCount = 0;
        int i = 0;

        ParameterBox* tset = NULL;

        while (i < storage->getSetSize()){

            int pageRange = 1;
            int pageIndex = 0;



            for (int x = i; x < storage->getSetSize(); x++)
            {
                ParameterBox tbox = storage->getBox(x);
                if (x == i){
                    pageIndex = tbox.getPage();
                }
                else{
                    if (pageIndex == tbox.getPage()){
                        pageRange += 1;
                    }
                }
            }

            tset = new ParameterBox[pageRange];
            int paramCount = 0;
            int boxCount = 0;
            int cardCount = 0;



            for (int x = 0; x < pageRange; x++)
            {
                tset[x].overwriteBox(storage->getBox(i + x));
                paramCount += tset[x].getCount();
            }



            int type = tset[0].getType();

            if ((type == boxStable)||(type == boxMutable)){

                qDebug() << "working on: " << tset[0].getTitle();

                for (int x = 0; x < pageRange; x++)
                {
                    qDebug() << "box#" << x;
                    for (int y = 0; y < tset[x].getCount(); y++)
                    {
                        MotorParameter temp = tset[x].getMotorParameter(y);
                        qDebug() << temp.getName();
                    }
                }

                qDebug() << "pageRange: " << pageRange;

                int fullCount = (paramCount < maxCards) ? 0 : (paramCount / maxCards);
                int partialCount = paramCount % maxCards;



                for (int x = 0; x < fullCount; x++){

                    QString tempTitle = tset[0].getTitle();

                    if (fullCount > 0){
                    tempTitle.append(" - ");
                    tempTitle.append(QString::number(x + 1));
                    }

                    PageTemplate newPage(tset[x].getToken(), tempTitle, 0);
                    componentCount += 1;

                    for (int y = 0; y < maxCards; y++){

                        MotorParameter temp = tset[boxCount].getMotorParameter(cardCount);
                        newPage.addControlledCard(temp, boxCount + i, cardCount);
                        cardCount += 1;
                        componentCount += 1;

                        if (cardCount == tset[boxCount].getCount()){
                            boxCount += 1;
                            cardCount = 0;
                        }


                    }

                    if (type == 0){
                        stablePageList.append(newPage.get_mainString());
                        stableTitleList.append(newPage.get_TabTitle());
                        stablePageCount += 1;
                    }
                    else{
                        mutablePageList.append(newPage.get_mainString());
                        mutableTitleList.append(newPage.get_TabTitle());
                        mutablePageCount += 1;
                    }
                }
                    if (partialCount){
                        qDebug() << "partialCount: " << partialCount;
                        QString tempTitle = tset[0].getTitle();

                        if (fullCount > 0){
                        tempTitle.append(" - ");
                        tempTitle.append(QString::number(fullCount + 1));
                        }
                        PageTemplate newPage(tset[0].getToken(), tempTitle, 0);
                        componentCount += 1;

                        for (; boxCount < pageRange; boxCount++){

                            for (; cardCount < tset[boxCount].getCount(); cardCount++){
                                MotorParameter temp = tset[boxCount].getMotorParameter(cardCount);
                                newPage.addControlledCard(temp, boxCount + i, cardCount);
                                componentCount += 1;
                            }
                            cardCount = 0;
                        }


                        if (type == 0){
                            stablePageList.append(newPage.get_mainString());
                            stableTitleList.append(newPage.get_TabTitle());
                            stablePageCount += 1;
                        }
                        else{
                            mutablePageList.append(newPage.get_mainString());
                            mutableTitleList.append(newPage.get_TabTitle());
                            mutablePageCount += 1;
                        }
                    }

            }
            else if (type == boxButton){
                for (int j = 0; j < tset[0].getCount(); j++){
                    MotorParameter temp;
                    temp.copyParam(tset[0].getMotorParameter(j));
                    panel.addButton(i, temp);
                }
            }
            //reset params for next loop
            i += pageRange;
            delete tset;
            tset = NULL;
        }

        QString panelPage = panel.get_mainString();
        storage->flushBinaryTransients();


        emit spawnButtonPanel(panelPage);
        emit spawnStableTabs(stablePageList, stableTitleList, stablePageCount);
        emit spawnMutableTabs(mutablePageList, mutableTitleList, mutablePageCount);
    }
}

void Backend::loadSetupFile(QString path)
{
    qDebug() << "Backend::loadSetupFile(): ";
    int counter = storage->loadSetupFile(path);
    if (counter > 0)
    {

        paged = true;

        QList<QString> stablePageList;
        QList<QString> stableTitleList;
        int stablePageCount = 0;
        QList<QString> mutablePageList;
        QList<QString> mutableTitleList;
        int mutablePageCount = 0;

        WriteBox panel;
        componentCount += 1;

        int i = 0;

        while (i < storage->getSetSize())
        {
            qDebug() << "starting on box#" << i;
            ParameterBox tptr;
            tptr = storage->getBox(i);
            int offset = 0;
            int pageCount = 0;

            int pageRange = 0;
            int pageIndex = tptr.getPage();
            unsigned char lowAddr = tptr.getAddr();
            int ftype = tptr.getType();
            int findex = tptr.getToken();

            for (int x = i + 1; x < storage->getSetSize(); x++)
            {
                ParameterBox nextBox = storage->getBox(x);

                if (pageIndex == nextBox.getPage()){
                    pageRange += 1;
                }
                else{
                    break;
                }
            }

            qDebug() << "with a range of: " << pageRange;

            int paramCount = 0;
            int *paramSet = new int[pageRange];

            for (int x = i; x < i + pageRange + 1; x++){
                ParameterBox nextBox = storage->getBox(x);
                paramSet[x - i] = nextBox.getCount();
                paramCount += nextBox.getCount();
            }

            ParameterBox boxptr;

            qDebug() << "paramCount: " << paramCount;
            for (int a = 0; a < pageRange; a++)
            {
                qDebug() << "paramSet[" << a << "]: " << paramSet[a];
            }


            if (pageRange > 0){

                boxptr.setUp(paramCount, lowAddr, findex, ftype);
                boxptr.setTitle(tptr.getTitle());
                int offset = 0;
                for (int x = i; x < i + pageRange + 1; x++){
                    ParameterBox medBox = storage->getBox(x);
                    int counter = medBox.getCount();
                    for (int y = 0; y < counter; y++){
                        boxptr.setMotorParameter(medBox.getMotorParameter(y), offset + y);
                    }
                    offset += counter;
                }
            }
            else{
                boxptr.overwriteBox(storage->getBox(i));
            }

            qDebug() << "boxptr count: " << boxptr.getCount();

            int valCount = boxptr.getCount();
            int type = boxptr.getType();

            int boxOffset;

            if ((type == 0)||(type == 1)){
                int offsetIndex = 0;
                int offsetCounter = 0;

                if (valCount > 7)
                {
                    int fullCount = valCount / 7;
                    int partialCount = valCount % 7;
                    int cardOffset = 0;

                    for (int j = 0; j < fullCount; j++)
                    {
                        QString tempTitle = boxptr.getTitle();
                        tempTitle.append(" - ");
                        tempTitle.append(QString::number(j + 1));

                        PageTemplate newPage(boxptr.getToken(), tempTitle, cardOffset);

                        componentCount += 1;

                        int cardCount = 0;

                        if (type == 0){
                            stablePageList.append(newPage.get_mainString());
                            stableTitleList.append(newPage.get_TabTitle());
                            stablePageCount += 1;
                        }
                        else if (type == 1){
                            mutablePageList.append(newPage.get_mainString());
                            mutableTitleList.append(newPage.get_TabTitle());
                            mutablePageCount += 1;
                        }
                    }
                    if (partialCount != 0){
                        QString tempTitle = boxptr.getTitle();
                        tempTitle.append(" - ");
                        tempTitle.append(QString::number(offset + 1));

                        PageTemplate newPage(boxptr.getToken(), tempTitle, cardOffset);
                        if (cardOffset){
                            cardOffset = 0;
                        }
                        componentCount += 1;

                        for (int k = 0; k < partialCount; k++)
                        {
                            int loopOffset = 0;
                            if (k - loopOffset < paramSet[offsetIndex]){

                            }
                            else{
                                offsetCounter += paramSet[offsetIndex];
                                loopOffset += paramSet[offsetIndex];
                                offsetIndex += 1;
                                newPage.resetCardOffset();
                            }

                                MotorParameter temp;
                                temp.copyParam(boxptr.getMotorParameter(k + (offset*7)));
                                newPage.addCard(temp, offsetIndex);
                                componentCount += 1;

                        }
                            if (type == 0){
                                stablePageList.append(newPage.get_mainString());
                                stableTitleList.append(newPage.get_TabTitle());
                                stablePageCount += 1;
                            }
                            else{
                                mutablePageList.append(newPage.get_mainString());
                                mutableTitleList.append(newPage.get_TabTitle());
                                mutablePageCount += 1;
                            }
                    }

                }
                else
                {
                    PageTemplate newPage(boxptr.getToken(), boxptr.getTitle());
                    componentCount += 1;

                    for (int j = 0; j < boxptr.getCount(); j++)
                    {
                        int loopOffset = 0;
                        if (j - loopOffset < paramSet[offsetIndex]){

                        }
                        else{
                            offsetCounter += paramSet[offsetIndex];
                            loopOffset += paramSet[offsetIndex];
                            offsetIndex += 1;
                        }

                        MotorParameter temp;
                        temp.copyParam(boxptr.getMotorParameter(j));
                        newPage.addCard(temp, offsetIndex);
                        componentCount += 1;
                    }
                    if (type == 0){
                        stablePageList.append(newPage.get_mainString());
                        stableTitleList.append(newPage.get_TabTitle());
                        stablePageCount += 1;
                    }
                    else if (type == 1){
                        mutablePageList.append(newPage.get_mainString());
                        mutableTitleList.append(newPage.get_TabTitle());
                        mutablePageCount += 1;
                    }
                }
            }
            else if (type == 2){
                //qDebug() << "in type 2";
                for (int j = 0; j < boxptr.getCount(); j++){
                    MotorParameter temp;
                    temp.copyParam(boxptr.getMotorParameter(j));
                    panel.addButton(i, temp);
                    componentCount += 1;
                    //qDebug() << "added button";
                }
            }

            i += pageRange + 1;
        }

        QString panelPage = panel.get_mainString();
        componentCount += 1;
        storage->flushBinaryTransients();


        emit spawnButtonPanel(panelPage);
        emit spawnStableTabs(stablePageList, stableTitleList, stablePageCount);
        emit spawnMutableTabs(mutablePageList, mutableTitleList, mutablePageCount);

    }
}

void Backend::loadConfigFile(QString path)
{
    int counter = storage->loadTabFile(path, "Config", boxConfig);
    loadConfigTime = true;
}

void Backend::saveConfigFile(QString path)
{

}

void Backend::fauxUpValues()
{
    emit upChildVal(2, 0, "first");
    emit upChildVal(2, 1, "second");
    emit upChildVal(3, 0, "third");
    emit upChildVal(3, 2, "fourth");
}

void Backend::fauxWrite()
{

    int setSize = storage->getSetSize();

    for (int i = 0; i < setSize; i++)
    {
        ParameterBox tbox = storage->getBox(i);

        int cardCount = tbox.getCount();
        qDebug() << "listing box#" << i << " of type " << tbox.getType() << "and page " << tbox.getPage();

        for (int j = 0; j < cardCount; j++)
        {
            MotorParameter tpar = tbox.getMotorParameter(j);
            qDebug() << i << "." << j << ": " << tpar.getName();
        }
    }
    /*ModStrGen tgen;
    int setSize = storage->getSetSize();
    qDebug() << "fauxWrite(): ";
    QString debugString;
    unsigned char* str;

    for (int i = 0; i < setSize; i++)
    {
        ParameterBox new0 = storage->getBox(i);
        int cardCount = new0.getCount();
        int ptype = new0.getType();
        qDebug() << "Box#" << i << " of Type " << ptype;

        for (int j = 0; j <cardCount; j++)
        {
            MotorParameter newp = new0.getMotorParameter(j);
            if (storage->getWriteQuery(i, j)||(ptype == boxButton)){
                qDebug() << "W " << i << ":" << j << " -> " << newp.getValue() << " && " << newp.getTransient() << " @ " << new0.getAddr();
                int len;
                str = tgen.prepWriteCommand(newp, 0x01, &len);
                char *testReply = ucharToHexStr(str, len);
                qDebug() << "W ModSTR: " << testReply;
                delete testReply;
                delete str;

            }
            else{
                qDebug() << "NW " << i << ":" << j << " -> " << newp.getValue() << " && " << newp.getTransient() << " @ " << new0.getAddr();

            }
            }
        qDebug() << endl;
    }*/
}

void Backend::promptWrite()
{
    if (writeTime){
    }
    else{
    }
    writeTime = true;
}

void Backend::promptClean()
{
    storage->cleanWarehouse();
    emit cleanUp();
}

void Backend::upTransient(int boxToken, int cardCount, QString ftransient)
{
    if (bkState >= bkPrelim){
        qDebug() << "update transient: " << boxToken << ":" << cardCount << " = " << ftransient;
        storage->upTransient(boxToken, cardCount, ftransient);
    }
    else{
        qDebug() << "upTransient(): Debug Enabled";
        ParameterBox tempBox = storage->getBox(boxToken);
        MotorParameter tempParam = tempBox.getMotorParameter(cardCount);
        qDebug() << "Name: " << tempParam.getName();
        qDebug() << "Address(" << tempParam.getSize() << "): " << tempParam.getAddr();
        ModStrGen tempGen;
        qDebug() << "Old Value: " << tempParam.getValue();
        qDebug() << "Old Transient: " << tempParam.getTransient();
        qDebug() << "New Value: " << ftransient;
        tempParam.setValue(ftransient);
        tempParam.setTransient(ftransient);
        qDebug() << "param Value: " << tempParam.getValue();
        qDebug() << "param Transient: " << tempParam.getTransient();

        int len;
        unsigned char *tempStr = tempGen.prepWriteCommand(tempParam, 0x01, &len);
        char *testReply = ucharToHexStr(tempStr, len);
        qDebug() << "param factor: " << tempParam.getFactor();
        qDebug() << "Write Str: " << testReply;
        delete testReply;
        qDebug() << "Exiting upTransient() Debug";
    }
}

void Backend::upButtonTransient(int boxToken, int cardCount)
{
    if (bkState >= bkPrelim){
        qDebug() << "toggling: " << boxToken << ":" << cardCount;
        storage->toggleBinaryTransient(boxToken, cardCount);
        writeButtonTime = true;
    }
    else{
        qDebug() << "upButtonTransient(): Debug Enabled";
        ParameterBox tempBox = storage->getBox(boxToken);
        MotorParameter tempParam = tempBox.getMotorParameter(cardCount);
        ModStrGen tempGen;
        QString tval = tempParam.getValue();
        qDebug() << "Name: " << tempParam.getName();
        qDebug() << "Address(" << tempParam.getSize() << "): " << tempParam.getAddr();
        if (tval == "0")
        {
            tempParam.setValue("1");
            tempParam.setTransient("1");
            qDebug() << "set Value & Transient to 0";
        }
        else if (tval == "1")
        {
            tempParam.setValue("0");
            tempParam.setTransient("0");
            qDebug() << "set Value & Transient to 1";
        }
        else
        {
            qDebug() << "Value/Temp Atypical: ";
            qDebug() << "Value: " << tempParam.getValue();
            qDebug() << "Transient: " << tempParam.getTransient();
            qDebug() << "Setting both to 0";
            tempParam.setValue("0");
            tempParam.setTransient("0");
            qDebug() << "Values reset";
        }
        int len;
        unsigned char *tempStr = tempGen.prepWriteCommand(tempParam, 0x01, &len);
        char *testReply = ucharToHexStr(tempStr, len);
        qDebug() << "Binary Type";
        qDebug() << "Write Str: " << testReply;
        delete testReply;
        qDebug() << "Exiting upButtonTransient() Debug";
    }
}

void Backend::flushButtonTransients()
{
    if (bkState >= bkPrelim){
    qDebug() << "flushing buttons";
    storage->flushBinaryTransients();
    }

}

void Backend::refreshPorts()
{
    QList<QSerialPortInfo> com_ports = QSerialPortInfo::availablePorts();
    QSerialPortInfo port;

    QList<QString> list;
    foreach(port, com_ports)
    {
        list.append(port.portName());
    }

    for (int i = 0; i < list.size(); i++)
    {
        qDebug() << list[i];
    }

    emit portList(list, list.size());
}

void Backend::openCOMPort(QString portN, QString baudR, QString parity, QString stopB)
{
    bool result;

    if (modb->manOpenPort(portN, baudR, parity, stopB))
    {
        result = true;
    }
    else
    {
        result = false;
    }

    emit manReport(portN, result);

}

void Backend::closeCOMPort()
{
    bool result;

    if (modb->manClosePort()){
        result = true;
    }
    else{
        result = false;
    }

    emit manClosed(result);
}


