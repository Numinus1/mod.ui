#ifndef PARAMWAREHOUSE_H
#define PARAMWAREHOUSE_H

#include "pagetemplate.h"


class ParamWarehouse
{
public:
    ParamWarehouse();

    int loadConfigFile(QString fileName);  
    int loadSetupFile(QString fileName);
    int loadTabFile(QString fileName, QString title, int ptype);

    int getSetSize();
    ParameterBox getBox(int index);
    int appendSet(ParameterBox *newload, int range);
    int getBoxType(int index);
    bool getWriteQuery(int boxt, int cardt);
    void setWriteQuery(int boxt, int cardt);
    int getBoxSize(int index);
    MotorParameter getParameter(int boxt, int cardt);

    void setBox(ParameterBox newBox, int index);

    void upTransient (int boxToken, int cardCount, QString ftransient);
    void toggleBinaryTransient(int boxToken, int cardCount);
    void flushBinaryTransients();
    void cleanWarehouse();
private:
    ParameterBox *boxSet;
    int setSize;
    bool **writeCheckList;
};

#endif // PARAMWAREHOUSE_H
