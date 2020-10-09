#ifndef PARAMETERBOX_H
#define PARAMETERBOX_H

#include "motorparameter.h"

class ParameterBox
{
public:
    ParameterBox();
    ParameterBox(MotorParameter *fptr, int fcount, unsigned char saddr, unsigned char fbyteCount, int tokener);
    ParameterBox(MotorParameter *fptr, int fcount, int tokener);

    void popParams(MotorParameter *fptr, int fcount, int tokener);
    void setMotorParameter(MotorParameter param, int position);
    void genParams(int scount);
    void setUp(int scount, unsigned char addr, int boxIndex, int ftype);
    void setByteCount();
    void assignValues(unsigned char* cData, int cLength);
    void setToken(int tokener);
    void upTransient(int cardCount, QString ftransient);
    void setTitle(QString title);
    void setType (int ftype);
    void setPage(int tpage);
    bool checkWrite(int boxToken, int cardToken);

    MotorParameter getMotorParameter(int position);
    void copyBox(ParameterBox in);
    void copyBox(ParameterBox *in);
    void overwriteBox(ParameterBox newBox);
    void overwriteBox(ParameterBox *newBox);

    bool resetBinaries();

    void printContents();

    int getCount();
    MotorParameter *getPtr();
    int getToken();
    unsigned char getAddr();
    int getByteCount();
    QString getTitle();
    int getType();
    int getPage();

private:
    MotorParameter *ptr;
    int count;
    unsigned char StartAddr;
    int byteCount;
    int token;
    QString tabTitle;

    //0 -> stable parameterset
    //1 -> mutable parameterset
    //2 -> flag parameterset
    int pollType;
    int page;

};

#endif // PARAMETERBOX_H
