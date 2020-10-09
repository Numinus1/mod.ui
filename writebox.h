#ifndef WRITEBOX_H
#define WRITEBOX_H

#include "motorparameter.h"


class WriteBox
{
public:
    WriteBox();
    
//public functions:
public:
    QString get_mainString();
    void addButton(int boxToken, MotorParameter in);
    
    
//private functions:
private:
    
//private data:
private:

    QString qmlcode;
    int buttonCount;
    QString prevId;
    int prevToken;
};

#endif // WRITEBOX_H
