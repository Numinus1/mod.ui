#ifndef MOTORPARAMETER_H
#define MOTORPARAMETER_H

#include "numitools.h"

class MotorParameter
{
public:
    MotorParameter();
    MotorParameter(QString fname, int fsize, int ffactor, unsigned char faddr);
    MotorParameter(QString fname, int fsize, int ffactor, unsigned char faddr, QString fdescription);


    QString getName();
    QString getDescription();
    int getSize();
    int getFactor();
    unsigned char getAddr();
    QString getValue();
    QString getTransient();


    void setName(QString fname);
    void setDescription(QString fdescription);
    void setSize(int fsize);
    void setFactor(int ffactor);
    void setAddr(unsigned char faddr);
    void setValue(QString fvalue);
    void setTransient(QString ftransient);

    void assignValue(unsigned char* data);
    unsigned char* assembleWritableValue(int* length);
    unsigned char* assembleWritableTransient(int* length);

    void copyParam(MotorParameter *in);
    void copyParam(MotorParameter in);

    void printContents();

//private data members
private:
    //*>File Order
    QString Name;

    //1 or 2 16-bit fields
    int Size;

    //0 -> int
    //1 -> q24
    //2 -> q24 * 1000
    //3 -> q20
    int Factor;

    //MODBUS Address
    unsigned char Addr;

    //User-entered description
    QString Description;

    QString Value;

    QString Transient;

    bool giIntReversal;
};



#endif // MOTORPARAMETER_H
