#include "parameterbox.h"

ParameterBox::ParameterBox()
{
    ptr = NULL;
    count = 0;
    StartAddr = 0x00;
    byteCount = 0;
    token = -1;
    pollType = 0;
    tabTitle = "no_title_given";
    page = -1;
}

ParameterBox::ParameterBox(MotorParameter *fptr, int fcount, unsigned char saddr, unsigned char fbyteCount, int tokener)
{
    count = fcount;
    StartAddr = saddr;
    byteCount = fbyteCount;

    ptr = new MotorParameter[count];

    for (int i = 0; i < count; i++){
        ptr[i].setName(fptr[i].getName());
        ptr[i].setDescription(fptr[i].getDescription());
        ptr[i].setFactor(fptr[i].getFactor());
        ptr[i].setSize(fptr[i].getSize());
    }

    token = tokener;

    pollType = 0;
    tabTitle = "no_title_given";
    page = -1;
}

ParameterBox::ParameterBox(MotorParameter *fptr, int fcount, int tokener)
{
    count = fcount;
    fptr = new MotorParameter[count];
    token = tokener;

    for (int i = 0; i < count; i++){
        ptr[i].setName(fptr[i].getName());
        ptr[i].setDescription(fptr[i].getDescription());
        ptr[i].setFactor(fptr[i].getFactor());
        ptr[i].setSize(fptr[i].getSize());
    }

    StartAddr = ptr[0].getAddr();

    byteCount = 0;

    for (int j = 0; j < count; j++)
    {
        byteCount += ptr[j].getSize();
    }

    pollType = 0;
    tabTitle = "no_title_given";
    page = -1;

}

int ParameterBox::getToken()
{
    return token;
}

MotorParameter* ParameterBox::getPtr()
{
    return ptr;
}


int ParameterBox::getCount()
{
    return count;
}

unsigned char ParameterBox::getAddr()
{
    return StartAddr;
}

QString ParameterBox::getTitle()
{
    return tabTitle;
}

int ParameterBox::getType()
{
    return pollType;
}

void ParameterBox::popParams(MotorParameter *fptr, int fcount, int tokener)
{
    count = fcount;
    ptr = new MotorParameter[count];
    token = tokener;

    for (int i = 0; i < count; i++){
        ptr[i].setName(fptr[i].getName());
        ptr[i].setDescription(fptr[i].getDescription());
        ptr[i].setFactor(fptr[i].getFactor());
        ptr[i].setSize(fptr[i].getSize());
    }

    StartAddr = ptr[0].getAddr();

    byteCount = 0;

    for (int j = 0; j < count; j++)
    {
        byteCount += ptr[j].getSize();
    }

}

void ParameterBox::setMotorParameter(MotorParameter param, int position)
{
    ptr[position].setName(param.getName());
    ptr[position].setDescription(param.getDescription());
    ptr[position].setFactor(param.getFactor());
    ptr[position].setAddr(param.getAddr());
    ptr[position].setSize(param.getSize());
    ptr[position].setValue(param.getValue());
    ptr[position].setTransient(param.getTransient());
}

void ParameterBox::genParams(int scount)
{
    ptr = new MotorParameter[scount];
}

void ParameterBox::setUp(int scount, unsigned char addr, int boxIndex, int ftype)
{
    count = scount;
    StartAddr = addr;
    token = boxIndex;
    genParams(scount);
    pollType = ftype;
}

void ParameterBox::setByteCount()
{
    byteCount = 0;

    for (int i = 0; i < count; i++)
    {
        byteCount += 2 * ptr[i].getSize();
    }
}

int ParameterBox::getByteCount()
{
    return byteCount;
}

void ParameterBox::assignValues(unsigned char *cData, int cLength)
{
    int offset = 3;

    for (int i = 0; i < count; i++)
    {
        ptr[i].assignValue(cData + offset);
        offset += (ptr[i].getSize()*2);
    }

    delete cData;
}

void ParameterBox::upTransient(int cardCount, QString ftransient)
{
    ptr[cardCount].setTransient(ftransient);
}

MotorParameter ParameterBox::getMotorParameter(int position)
{
    MotorParameter returner;
    returner.setName(ptr[position].getName());
    returner.setDescription(ptr[position].getDescription());
    returner.setSize(ptr[position].getSize());
    returner.setFactor(ptr[position].getFactor());
    returner.setValue(ptr[position].getValue());
    returner.setTransient(ptr[position].getTransient());
    returner.setAddr(ptr[position].getAddr());
    return returner;
}

void ParameterBox::setPage(int tpage)
{
    page = tpage;
}

int ParameterBox::getPage()
{
    return page;
}

void ParameterBox::copyBox(ParameterBox *in)
{
    setUp(in->getCount(), in->getAddr(), in->getToken(), in->getType());
    setTitle(in->getTitle());

    for (int i = 0; i < in->getCount(); i++)
    {
        setMotorParameter(in->getMotorParameter(i), i);
    }
    setByteCount();
    setPage(in->getPage());
}

void ParameterBox::copyBox(ParameterBox in)
{
    setUp(in.getCount(), in.getAddr(), in.getToken(), in.getType());
    setTitle(in.getTitle());

    for (int i = 0; i < in.getCount(); i++)
    {
        setMotorParameter(in.getMotorParameter(i), i);
    }
    setByteCount();
    setPage(in.getPage());
}

void ParameterBox::overwriteBox(ParameterBox newBox)
{
    if (ptr != NULL){
        ptr = NULL;
        delete ptr;
    }

    copyBox(newBox);
}

void ParameterBox::overwriteBox(ParameterBox *newBox)
{
    if (ptr != NULL){
        ptr = NULL;
        delete ptr;
    }

    copyBox(newBox);
}

bool ParameterBox::resetBinaries()
{
    if (pollType == 2){
        for (int i = 0; i < count; i++)
        {
            ptr[i].setValue("0");
            ptr[i].setTransient("0");
        }
        return true;
    }
    else{
        return false;
    }
}

void ParameterBox::printContents()
{
    qDebug() << endl << "###ParameterBox###";
    qDebug() << "Printing Box#" << token;
    qDebug() << "Starting Address: " << StartAddr;
    qDebug() << "Count: " << count;
    qDebug() << "Bytecount: " << byteCount << endl;
    qDebug() << "Title: " << tabTitle;
    qDebug() << "Type: " << pollType;

    for (int i = 0; i < count; i++)
    {
        ptr[i].printContents();
    }
    qDebug() << "###PARAMETERBOX###";
}

void ParameterBox::setToken(int tokener)
{
    token = tokener;
}

void ParameterBox::setTitle(QString title)
{
    tabTitle = title;
}

void ParameterBox::setType(int ftype)
{
    pollType = ftype;
}
