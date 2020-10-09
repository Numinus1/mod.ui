#include "motorparameter.h"

MotorParameter::MotorParameter()
{
    Name = "none provided";
    Size = 0;
    Factor = 0;
    Addr = 0;
    Description = "none provided";
    Value = "n/a";
    Transient = "none";

    giIntReversal = true;
}

MotorParameter::MotorParameter(QString fname, int fsize, int ffactor, unsigned char faddr)
{
    Name = fname;
    Size = fsize;
    Factor = ffactor;
    Addr = faddr;
    Description = "No Description Provided";
    Value = "n/a";
    Transient = "none";

    giIntReversal = true;
}

MotorParameter::MotorParameter(QString fname, int fsize, int ffactor, unsigned char faddr, QString fdescription)
{
    Name = fname;
    Size = fsize;
    Factor = ffactor;
    Addr = faddr;
    Description = fdescription;
    Value = "n/a";
    Transient = "none";


    giIntReversal = true;
}


void MotorParameter::copyParam(MotorParameter *in)
{
    setName(in->getName());
    setSize(in->getSize());
    setFactor(in->getFactor());
    setAddr(in->getAddr());
    setDescription(in->getDescription());
    setValue(in->getValue());
    setTransient(in->getTransient());
}

void MotorParameter::copyParam(MotorParameter in)
{
    setName(in.getName());
    setSize(in.getSize());
    setFactor(in.getFactor());
    setAddr(in.getAddr());
    setDescription(in.getDescription());
    setValue(in.getValue());
    setTransient(in.getTransient());
}

QString MotorParameter::getName()
{
    return Name;
}

QString MotorParameter::getDescription()
{
    return Description;
}

int MotorParameter::getSize()
{
    return Size;
}

int MotorParameter::getFactor()
{
    return Factor;
}

unsigned char MotorParameter::getAddr()
{
    return Addr;
}

QString MotorParameter::getValue()
{
    return Value;
}

QString MotorParameter::getTransient()
{
    return Transient;
}

void MotorParameter::setName(QString fname)
{
    Name = fname;
}

void MotorParameter::setDescription(QString fdescription)
{
    Description = fdescription;
}

void MotorParameter::setSize(int fsize)
{
    Size = fsize;
}

void MotorParameter::setFactor(int ffactor)
{
    Factor = ffactor;
}

void MotorParameter::setAddr(unsigned char faddr)
{
    Addr = faddr;
}

void MotorParameter::setValue(QString fvalue)
{
    Value = fvalue;
}

void MotorParameter::setTransient(QString ftransient)
{
    Transient = ftransient;
}

void MotorParameter::assignValue(unsigned char *data)
{
    if (Size == 1)
    {
        if (Factor == 0)
        {
            int raw = i16bits_fromuchar(data);
//            char *testReply = ucharToHexStr(data, 2);
//            qDebug() << "raw string: " << testReply;
//            delete testReply;

//            qDebug() << "raw int: " << raw;
            setValue(QString::number(raw));/*
            qDebug() << "assigned value: " << Value;*/
        }
    }
    else if (Size == 2)
    {
        if (Factor == 0)
        {
            int raw = fromuchar(data, giIntReversal);
            setValue(QString::number(raw));

        }
        else if (Factor == 1)
        {
            float raw = (float) fromuchar(data, giIntReversal);
            raw /= 16777216;
            setValue(QString::number(raw));
        }
        else if (Factor == 2)
        {
            float raw = (float) fromuchar(data, giIntReversal);
            raw /= 16777216;
            raw *= 1000;
            setValue(QString::number(raw));
        }
        else if (Factor == 3)
        {
            float raw = (float) fromuchar(data, giIntReversal);
            raw /= 1048576;
            setValue(QString::number(raw));
        }
    }
}

unsigned char* MotorParameter::assembleWritableValue(int *length)
{
    unsigned char* refData = NULL;
    char *cData;

    if (Size == 1)
    {
        *length = 2;

        if (Factor == 0)
        {
            cData = QStrtochar(Value);
            int Data = chartoint(cData);

            int tempi;

            refData = new unsigned char [2];

            tempi = Data & 0x000000FF;
            refData[1] = (unsigned char) tempi;

            tempi = Data & 0x0000FF00;
            tempi = tempi >> 8;
            tempi = tempi & 0x000000FF;
            refData[0] = (unsigned char) tempi;
        }
    }
    else if (Size == 2)
    {
        *length = 4;

        if (Factor == 0)
        {
            cData = QStrtochar(Value);
            int Data = chartoint(cData);

            int tempi;

            refData = new unsigned char [4];
            tempi = Data & 0x000000FF;
            refData[1] = (unsigned char) tempi;

            tempi = Data & 0x0000FF00;
            tempi = tempi >> 8;
            tempi = tempi & 0x000000FF;
            refData[0] = (unsigned char) tempi;

            refData[2] = 0x00;
            refData[3] = 0x00;
        }
        else if (Factor == 1)
        {
            cData = QStrtochar(Value);
            float Data = atof(cData);
            Data *= 16777216;

            refData = int_touchar(Data, giIntReversal);
        }
        else if (Factor == 2)
        {
            cData = QStrtochar(Value);
            float Data = atof(cData);
            Data *= 16777216;
            Data /= 1000;

            refData = int_touchar(Data, giIntReversal);
        }
        else if (Factor == 3)
        {
            cData = QStrtochar(Value);
            float Data = atof(cData);
            Data *= 1048576;

            refData = int_touchar(Data, giIntReversal);
        }
    }

    return refData;
}

unsigned char* MotorParameter::assembleWritableTransient(int *length)
{
    unsigned char* refData;
    char *cData;

    if (Size == 1)
    {
        *length = 2;

        if (Factor == 0)
        {
            cData = QStrtochar(Transient);
            int Data = chartoint(cData);

            int tempi;

            refData = new unsigned char [2];

            tempi = Data & 0x000000FF;
            refData[1] = (unsigned char) tempi;

            tempi = Data & 0x0000FF00;
            tempi = tempi >> 8;
            tempi = tempi & 0x000000FF;
            refData[0] = (unsigned char) tempi;
            }
    }
    else if (Size == 2)
    {
        *length = 4;

        if (Factor == 0)
        {
            cData = QStrtochar(Transient);
            int Data = chartoint(cData);

            int tempi;

            refData = new unsigned char [4];
            tempi = Data & 0x000000FF;
            refData[1] = (unsigned char) tempi;

            tempi = Data & 0x0000FF00;
            tempi = tempi >> 8;
            tempi = tempi & 0x000000FF;
            refData[0] = (unsigned char) tempi;

            refData[2] = 0x00;
            refData[3] = 0x00;
        }
        else if (Factor == 1)
        {
            cData = QStrtochar(Transient);
            float Data = atof(cData);
            Data *= 16777216;

            refData = int_touchar(Data, giIntReversal);
        }
        else if (Factor == 2)
        {
            cData = QStrtochar(Transient);
            float Data = atof(cData);
            Data *= 16777216;
            Data /= 1000;

            refData = int_touchar(Data, giIntReversal);
        }
        else if (Factor == 3)
        {
            cData = QStrtochar(Transient);
            float Data = atof(cData);
            Data *= 1048576;

            refData = int_touchar(Data, giIntReversal);
        }
    }

    return refData;
}

void MotorParameter::printContents()
{
    qDebug() << "###NODE###";
    qDebug() << "Name: " << Name;
    qDebug() << "Size: " << Size;
    qDebug() << "Factor: " << Factor;
    qDebug() << "Address: " << Addr;
    qDebug() << "Description: " << Description;
    qDebug() << "Value: " << Value;
    qDebug() << "###ENDNODE###" << endl;
}
