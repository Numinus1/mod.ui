#ifndef MODSTRGEN_H
#define MODSTRGEN_H

#include "sercomm.h"

class ModStrGen
{

public:
    ModStrGen();

    unsigned char* prepReadCommand(ParameterBox in, unsigned char DriveAddress);
    unsigned char* prepWriteCommand(MotorParameter in, unsigned char DriveAddress, int* length);
};

#endif // MODSTRGEN_H
