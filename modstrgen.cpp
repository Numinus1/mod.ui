#include "modstrgen.h"

ModStrGen::ModStrGen()
{

}

unsigned char* ModStrGen::prepReadCommand(ParameterBox in, unsigned char DriveAddress)
{
    unsigned char* modstr;

    modstr = new unsigned char [8];
    modstr[0] = DriveAddress;
    modstr[1] = 0x04;

    unsigned char downAdd = in.getAddr();
    unsigned char upAdd = (unsigned char)((int)downAdd + (in.getByteCount()/2) - 1);
//    qDebug() << "lowAdd: " << downAdd;
//    qDebug() << "upAdd: " << upAdd;
    modstr[2] = up8_fromushort(downAdd);
    modstr[3] = down8_fromushort(downAdd);

    unsigned char *scanrange = scanranger(upAdd, downAdd);
    modstr[4] = scanrange[0];
    modstr[5] = scanrange[1];

    unsigned short crcref = crc16(modstr, 6);
    unsigned char *crc = ushort_touchar(crcref);

    modstr[6] = crc[0];
    modstr[7] = crc[1];

    delete scanrange;
    delete crc;

    return modstr;
}

unsigned char* ModStrGen::prepWriteCommand(MotorParameter in, unsigned char DriveAddress, int* length)
{
    qDebug() << "ModStrGen::prepWriteCommand(): ";
    unsigned char downAdd = in.getAddr();
    qDebug() << "in.getAddr() result: " << in.getAddr();
    qDebug() << "downAdd: " << downAdd;
    unsigned char upAdd = (unsigned char)((int) downAdd + in.getSize() - 1);
    qDebug() << "upAdd: " << upAdd;
    unsigned char *scanrange = scanranger(upAdd, downAdd);
    int datalen;

    unsigned char* data = in.assembleWritableTransient(&datalen);
    int cmdlen = datalen + 9;

    *length = cmdlen;

    unsigned char* modstr = new unsigned char[cmdlen];

    modstr[0] = DriveAddress;
    modstr[1] = 0x10;
    modstr[2] = up8_fromushort(downAdd);
    modstr[3] = down8_fromushort(downAdd);


    modstr[4] = scanrange[0];
    modstr[5] = scanrange[1];





    modstr[6] = (unsigned char) datalen;

    for (int i = 0; i < datalen; i++)
    {
        modstr[7 + i] = data[i];
    }

    unsigned short crcref = crc16(modstr, cmdlen - 2);
    unsigned char *crc = ushort_touchar(crcref);

    modstr[7 + datalen] = crc[0];
    modstr[7 + datalen + 1] = crc[1];

    delete data;
    delete scanrange;
    delete crc;

    return modstr;
}
