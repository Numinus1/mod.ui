#include "paramwarehouse.h"

ParamWarehouse::ParamWarehouse()
{
    boxSet = NULL;
    setSize = 0;
}

int ParamWarehouse::getSetSize()
{
    return setSize;
}

ParameterBox ParamWarehouse::getBox(int index)
{
    ParameterBox returner;
    if ((boxSet != NULL) && (index < setSize)){
        returner.setUp(boxSet[index].getCount(), boxSet[index].getAddr(),boxSet[index].getToken(), boxSet[index].getType());
        returner.setTitle(boxSet[index].getTitle());

        for (int i = 0; i < boxSet[index].getCount(); i++)
        {
            returner.setMotorParameter(boxSet[index].getMotorParameter(i), i);
        }

        returner.setByteCount();
        returner.setPage(boxSet[index].getPage());
    }

    return returner;


}

int ParamWarehouse::getBoxType(int index)
{
    return boxSet[index].getType();
}

void ParamWarehouse::setBox(ParameterBox newBox, int index)
{
    boxSet[index].overwriteBox(newBox);
}

bool ParamWarehouse::getWriteQuery(int boxt, int cardt)
{
    return writeCheckList[boxt][cardt];
}

void ParamWarehouse::setWriteQuery(int boxt, int cardt)
{
    writeCheckList[boxt][cardt] = false;
}

int ParamWarehouse::getBoxSize(int index)
{
    return boxSet[index].getCount();
}

MotorParameter ParamWarehouse::getParameter(int boxt, int cardt)
{
    return boxSet[boxt].getMotorParameter(cardt);
}

void ParamWarehouse::upTransient(int boxToken, int cardCount, QString ftransient)
{
    QString tValue;
    MotorParameter tParam;
    tParam = boxSet[boxToken].getMotorParameter(cardCount);
    tValue = tParam.getValue();
    if (tValue != ftransient){
        boxSet[boxToken].upTransient(cardCount, ftransient);
        writeCheckList[boxToken][cardCount] = true;
    }
}

void ParamWarehouse::toggleBinaryTransient(int boxToken, int cardCount)
{
    MotorParameter tempParam = boxSet[boxToken].getMotorParameter(cardCount);

    QString oldValue = tempParam.getValue();
    if (oldValue == "0"){
        QString newTransient = "1";
        boxSet[boxToken].upTransient(cardCount, newTransient);
        qDebug() << "new transient: " << newTransient;
    }
    else if (oldValue == "1"){
        QString newTransient = "0";
        boxSet[boxToken].upTransient(cardCount, newTransient);
        qDebug() << "new transient: " << newTransient;
    }
    else{
        MotorParameter temp2 = boxSet[boxToken].getMotorParameter(cardCount);
        temp2.setTransient("0");
        temp2.setValue("0");
        boxSet[boxToken].setMotorParameter(temp2, cardCount);
        qDebug() << "button value/transient both reset";
    }

    writeCheckList[boxToken][cardCount] = true;
}

void ParamWarehouse::flushBinaryTransients()
{
    //qDebug() << "flushBinaryTransients(): in; setSize: " << setSize;
    for (int i = 0; i < setSize; i++)
    {
        if (boxSet[i].resetBinaries()){
            //qDebug() << "reset box#" << i;
        }
        else{
            //qDebug() << "did not reset box#" << i;
        }
    }
}

int ParamWarehouse::loadTabFile(QString fileName, QString title, int ptype)
{
    QFile file(fileName);
    QString fileError;
    static int pageCount = 0;

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        //error code - failed to open file
        fileError = file.errorString();
        qDebug() << fileError;
        return -1;
    }

    //data containers to hold intake data
    QString inLine;
    QList<QString> lines;
    QList<QStringList> cutLines;

    //read 'file' as a stream of bytes
    QTextStream in(&file);

    //swallow entire file and close file
    while (!in.atEnd())
    {
        inLine = in.readLine();
        lines.append(inLine);
    }

    file.close();

    qDebug() << "loadTabFile(): lineCount: " << lines.count();


    //validate linecount greater than 0
    if (!lines.count())
    {
        //error code - file validation
        return 1;
    }

    //create MotorParameter arrays
    MotorParameter **unordList = new MotorParameter*[lines.count() - 1];
    int unordIndex = 0;

    MotorParameter **ordList = new MotorParameter*[lines.count() - 1];
    int ordIndex = 0;

    unsigned lowestAddr = 0xFF;
    int lowIndex = 0;
    int checkList[lines.count()];

    for (int i = 0; i < lines.count(); i++)
    {
        checkList[i] = 0;
    }

    //split lines by comma

    for (int i = 0; i < lines.count(); i++)
    {
        cutLines.append(lines[i].split(','));
    }

    //create an unordered linked list of motor parameters

    QString fname;
    int fsize;
    int ffactor;
    unsigned char faddr;
    QString fdesc;

    for (int j = 1; j < lines.count(); j++)
    {
        //scrub whitespaces from the QStrings


        /*for (int k = 1; k < 5; k++)
        {
            for (int l = 0; l < cutLines[j][k].size(); l++)
            {
                if (cutLines[j][k][l] == ' ')
                {
                    cutLines[j][k].remove(l, 1);
                    l -= 1;
                }
            }
        }*/

        if (cutLines[j][0][0] == ' ')
            cutLines[j][0].remove(0, 1);

        if (cutLines[j][4][0] == ' ')
            cutLines[j][4].remove(0, 1);

        //Extract data into distinct variables
        //-Name
        fname = cutLines[j][0];
        //-Size
        fsize = intFromQStr(cutLines[j][1]);
        //-Factor
        ffactor = intFromQStr(cutLines[j][2]);
        //-Address
        faddr = (unsigned char) intFromQStr(cutLines[j][3]);
        //-Description
        fdesc = cutLines[j][4];

        unordList[unordIndex] = new MotorParameter(fname, fsize,
                                        ffactor, faddr, fdesc);
        unordIndex += 1;
    }
    qDebug() << "created unordList";
    //sort list
    for (int i = 0; i < unordIndex; i++)
    {
        for (int j = 0; j < unordIndex; j++)
        {
            if (checkList[j] == 0){
                if (lowestAddr >= unordList[j]->getAddr()){
                    lowestAddr = unordList[j]->getAddr();
                    lowIndex = j;
                }
            }
        }
        ordList[i] = new MotorParameter(unordList[lowIndex]->getName(),
                                        unordList[lowIndex]->getSize(),
                                        unordList[lowIndex]->getFactor(),
                                        unordList[lowIndex]->getAddr(),
                                        unordList[lowIndex]->getDescription());
        checkList[lowIndex] = 1;
        lowestAddr = 0xFF;
    }

    qDebug() << "Created ord list";

    MotorParameter *tmptr = new MotorParameter[unordIndex];
    qDebug() << "created tmptr array";
    for (int i = 0; i < unordIndex; i++)
    {
        tmptr[i].copyParam(ordList[i]);
    }
    qDebug() << "populated tmptr array";

    for (int i = 0; i < lines.count(); i++)
    {
        checkList[i] = 0;
    }

    qDebug() << "set checkList to all zero";
    int range = 1;

    int lineCount = lines.count();
    for (int j = 0; j < unordIndex - 1; j++)
    {
        checkList[j + 1] = range;

        int lb = (int) ordList[j]->getAddr();
        int ub = (int) ordList[j + 1]->getAddr();


        if ((lb + ordList[j]->getSize()) == ub)
        {

        }
        else
        {
            range += 1;
        }
    }
    qDebug() << "done range magic";

    if (lineCount > 2){
    int lb = (int) ordList[unordIndex - 2]->getAddr();
    int ub = (int) ordList[unordIndex - 1]->getAddr();

    qDebug() << "done more range magic";
    if ((lb + ordList[unordIndex - 2]->getSize()) == ub)
    {
        checkList[unordIndex] = checkList[unordIndex - 1];
    }
    else
    {
        checkList[unordIndex] = checkList[unordIndex - 1] + 1;
    }
    }
    else{
        checkList[1] = 1;
    }

    int boxSize[range];

    for (int i = 0; i < range; i++)
    {
        boxSize[i] = 0;
    }

    for (int i = 1; i < lines.count(); i++)
    {
        boxSize[checkList[i] - 1] += 1;
    }

    int index = 0;
    ParameterBox* boxedSet;
    boxedSet = new ParameterBox[range];
    for (int i = 0; i < range; i++)
    {
       for (int j = 1; j < lines.count(); j++)
       {
           if (checkList[j] == i + 1)
           {

               index = j - 1;
               break;
           }
       }

        boxedSet[i].setUp(boxSize[i], ordList[index]->getAddr(), i + setSize, ptype);
        boxedSet[i].setTitle(title);

        MotorParameter temper;

        for (int k = 0; k < boxSize[i]; k++)
        {
            temper.copyParam(ordList[index + k]);
            boxedSet[i].setMotorParameter(temper, k);
        }

        boxedSet[i].setByteCount();
        boxedSet[i].setPage(pageCount);

    }

    appendSet(boxedSet, range);

    delete boxedSet;



    qDebug() << "setSize: " << setSize;
    pageCount += 1;
    return range;
}

int ParamWarehouse::loadSetupFile(QString fileName)
{
    fileName.remove(0, 8);
    QFile filePrime(fileName);
    QString fileError;

    qDebug() << "file path:";
    qDebug() << fileName;
    if (!filePrime.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        //error code - failed to open file
        fileError = filePrime.errorString();
        qDebug() << fileError;
        return -1;
    }

    //data containers to hold intake data
    QString inLine;
    QList<QString> lines;
    QList<QStringList> cutLines;

    //read 'file' as a stream of bytes
    QTextStream in(&filePrime);

    //swallow entire file and close file
    while (!in.atEnd())
    {
        inLine = in.readLine();
        lines.append(inLine);
    }

    filePrime.close();

    //validate linecount greater than 0
    if (!lines.count())
    {
        //error code - file validation
        return -1;
    }

    //split lines by comma

    for (int i = 0; i < lines.count(); i++)
    {
        cutLines.append(lines[i].split(','));
    }

    //validate inputs
    if (!(cutLines[0][0] == "title"))
    {
        if (!(cutLines[0][1] == "type"))
        {
            if (!(cutLines[0][2] == "path"))
            {
                //exit due to validation error
                qDebug() << "validation error in " << fileName;
                return -1;
            }
        }
    }

    int returner = 0;
    for (int i = 1; i < lines.count(); i++){


        QString tabTitle = cutLines[i][0];
        int pollType = intFromQStr(cutLines[i][1]);
        QString listPath = cutLines[i][2];
        qDebug() << "ptype: " << pollType;

        int stopper = fileName.size() - 1;

        while (fileName[stopper] != '/')
        {
            stopper -= 1;
        }

        //extract parameter list filepath
        QString secondaryPath = fileName;
        secondaryPath.remove(stopper + 1, fileName.size() - stopper - 1);
        secondaryPath.append(listPath);

        returner += loadTabFile(secondaryPath, tabTitle, pollType);
    }

    return returner;


}

int ParamWarehouse::appendSet(ParameterBox *newload, int range)
{

    if (setSize == 0)
    {
        boxSet = new ParameterBox[range];
        writeCheckList = new bool*[range];

        for (int i = 0; i < range; i++)
        {
            boxSet[i].copyBox(newload[i]);
            writeCheckList[i] = new bool [boxSet[i].getCount()];
            for (int writer = 0; writer < boxSet[i].getCount(); writer++)
            {

                writeCheckList[i][writer] = false;
            }
        }



    }
    else
    {
        ParameterBox* temper = new ParameterBox[setSize];
        bool** tempList = new bool*[setSize];
        for (int i = 0; i < setSize; i++)
        {
            temper[i].copyBox(boxSet[i]);
            tempList[i] = new bool[temper[i].getCount()];

            for (int writer = 0; writer < temper[i].getCount(); writer++)
            {

                tempList[i][writer] = writeCheckList[i][writer];
            }

            delete writeCheckList[i];
        }

        delete boxSet;
        delete writeCheckList;


        boxSet = new ParameterBox[range + setSize];
        writeCheckList = new bool*[range + setSize];

        for (int j = 0; j < setSize + range; j++)
        {
            if (j < setSize)
            {
                boxSet[j].copyBox(temper[j]);
                writeCheckList[j] = new bool[boxSet[j].getCount()];

                for (int writer = 0; writer < boxSet[j].getCount(); writer++)
                {
                    writeCheckList[j][writer] = tempList[j][writer];
                }
                delete tempList[j];

            }
            else
            {
                boxSet[j].copyBox(newload[j - setSize]);
                writeCheckList[j] = new bool[boxSet[j].getCount()];

                for (int writer = 0; writer < boxSet[j].getCount(); writer++)
                {
                    writeCheckList[j][writer] = false;
                }
            }
        }

        delete temper;
        delete tempList;
    }

    setSize += range;
    //print boxSet
    /*for (int i = 0; i < setSize; i++)
    {
        boxSet[i].printContents();
    }*/

    return range;
}

void ParamWarehouse::cleanWarehouse()
{
    delete[] boxSet;
    for (int i= 0; i < setSize; i++){
        delete [] writeCheckList[i];
    }
    delete[] writeCheckList;
    setSize = 0;
}
