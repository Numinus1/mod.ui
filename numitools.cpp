#include "numitools.h"


//Generates CRC16 for MODBUS RTU Protocol

//pData: string of unsigned chars containing transmission data
//length: length of pData;

//return: unsigned short (16 bits)
unsigned short crc16(unsigned char *pData, size_t length )
    {
    unsigned short crc = 0xFFFF;
    for(size_t n=0;n<length;++n)
    {
        unsigned char comb_val = (crc>>8) ^ pData[n];
        crc = (crc<<8) ^ modbus_16_tbl[comb_val & 0x00FF];
    }
    return crc;
    }


//Converts unsigned short to two unsigned char

//temper: unsigned short input

//return: pointer to unsigned char array[2]
unsigned char* ushort_touchar(unsigned short temper)
{
    unsigned short temp0 = temper;
    unsigned short temp1 = temper;

    temp0 = temp0 >> 8;
    temp1 = temp1 & 0x00FF;
    unsigned char *bytes = new unsigned char[2];
    bytes[1] = (unsigned char)temp1;
    bytes[0] = (unsigned char)temp0;
    return bytes;
}


//Essentially makes an array of of 1 byte integers in
//the guise of unsigned chars whose values are determined by each
//set of 2 chars in the input string; also returns their length

//a: user input

//&lengther: length of return

//return: HEX-converted unsigned char array

//Example: a = {'A', 'F', 'C', '3'}; array of 4 chars A, F, C and 3
// --> unsigned char return = {0xAF, 0xC3}; array of 2 unsigned chars representing numerical value of input
unsigned char* chartohexer (char *a, int *lengther)
{

    int lengthin = stringlength(a);

    //return NULL and 0 length if string is empty
    if (lengthin == 0)
    {
        *lengther = 0;
        return NULL;
    }

    int length = lengthin;

    if (length%2)
        length++;

    length = length/2;

    unsigned char *result = new unsigned char[length];

    unsigned char first;
    unsigned char second;

    int j = 0;
    char *temp = new char[stringlength(a) + 2];
    char *throwin = new char[2];
    throwin[0] = '0';
    throwin[1] = '\0';

    if (lengthin%2)
    {
        stringcopy(temp, throwin);
        stringcnct(temp, a);
    }
    else
        stringcopy(temp, a);

    for (int i = 0; i < lengthin; i+=2)
    {


        if ((temp[i] > 47)&&(temp[i]<58))
        {
            first = temp[i] - 48;
        }
        else
        {
            first = temp[i] - 55;
        }

        if ((temp[i+1] > 47)&&(temp[i+1] < 58))
        {
            second = temp[i+1] - 48;
        }
        else if (temp[i+1] == '\0')
        {
            second = 0;

        }
        else
        {
            second = temp[i+1] - 55;
        }
        result[j] = first*16 + second;
        j++;

    }

    *lengther = length;
    delete temp;
    return result;
}

//Confirms that all characters are either 0 to 9, a to f, or A to F;
//converts any lowercase letters to uppercase

//a: user input as char array

//return: -1 if a empty; 0 if chars do not meet input requirements; 1 if all is well
//note: also capitalises any lower-case chars in the input array
int withinrange(char *a)
{
    int len = stringlength(a);
    if (len == 0)
        return -1;
    else
    {
        for (int i = 0; i < len; i++)
        {
            if ((a[i] < 48)||((a[i] > 57)&&(a[i] < 65))||((a[i] > 70)&&(a[i] < 97))||(a[i] > 102))
                return 0;

            if ((a[i] > 96)&&(a[i] < 103))
                a[i] = a[i] - 32;
        }
    }
    return 1;
}

//Calculates exponential product

//base: base number
//exponent: exponent number

//return: base to the power of exponent
int expo(int base, int exponent)
{
    int result = 1;

    for (int i = 0; i < exponent; i++)
    {
        result *= base;
    }

    return result;
}

float negexpo(int base, int exponent)
{
    float result = 1;

    for (int i = 0; i > exponent; i--)
    {
        result /= base;
    }
}

//Calculates length of char array

//input: input char array

//return: length of input NOT including null terminator
int stringlength(char* input)
{
    if (input == NULL)
        return 0;

    int i = 0;

    while (input[i] != '\0')
    {
        i++;
    }

    return i;
}

//Copies one string to the other

//[return]destination: string to copy to
//source: source string to copy from

//note: assumes destination is already declared and of sufficient length
void stringcopy(char* destination, char* source)
{
    int i = 0;

    while (source[i] != '\0')
    {
        destination[i] = source[i];
        i++;
    }

    destination[i] = '\0';
}

//Concatenates one string to the other

//[return]destination: string to concatenate to
//source: string to concatenate from

//note: assumes destination is already declared and of sufficient length
void stringcnct(char* destination, char* source)
{
    int a = stringlength(destination);

    int i = 0;
    while (source[i] != '\0')
    {
        destination[i+a] = source[i];
        i++;
    }
    destination[i+a] = '\0';

}

//Converts an integer represented as chars into a true int

//stringer: input string

//return: returns true int
//Example: stringer = {'2', '3'}
//--> int return = 23;
//note: does NOT parse input for incorrect input
//therefore, chars other than '1' to '9' will give erroneous return
unsigned int chartouint(char *stringer)
{
    int calen = stringlength(stringer);

    unsigned int truerep = 0;


    for (int i = 0; i < calen; i++)
    {
        truerep += (stringer[i] - 48)*expo(10, (calen - 1 - i));
    }

    return truerep;
}

//Converts an integer represented as chars into a true int

//stringer: input string

//return: returns true int
//Example: stringer = {'2', '3'}
//--> int return = 23;
//note: does NOT parse input for incorrect input
//therefore, chars other than '1' to '9' will give erroneous return
int chartoint(char *stringer)
{

    bool negval = false;

    if (stringer[0] == '-')
    {
        negval = true;
    }

    int calen = stringlength(stringer);

    int truerep = 0;

    if (!negval)
    {
        for (int i = 0; i < calen; i++)
        {
            truerep += (stringer[i] - 48)*expo(10, (calen - 1 - i));
        }
    }
    else
    {
        for (int i = 1; i < calen; i++)
        {
            truerep += (stringer[i] - 48)*expo(10, (calen - 1 - i));
        }

        truerep *= -1;
    }

    delete stringer;

    return truerep;
}

float chartofloat(char *stringer)
{
    int calen = stringlength(stringer);

    float truerep = 0;

    int i = 0;
    int wholes = 0;
    int decimals = 0;

    while (stringer[i] != '.')
    {
        i++;
    }

    wholes = i;
    decimals = calen - wholes - 1;

    for (int j = 0; j < wholes; j++)
    {
        truerep += (stringer[j] - 48)*expo(10, (wholes - 1 - j));
    }

    i = 0;

    for (int k = wholes + 1; k < calen; k++)
    {
        i--;
        truerep += (stringer[k] - 48)*negexpo(10, i);
    }
    qDebug() << "truerep is" << truerep;
    return truerep;
}

//Converts QString to char*

//temp: input QString

//return: char* converted from temp
char* QStrtochar(QString temp)
{
     if (temp.isEmpty())
         return NULL;
     QByteArray ba = temp.toLatin1();
     char *c_str = ba.data();
     char *returner = new char [stringlength(c_str)+1];
     stringcopy(returner, c_str);
     delete c_str;
     return returner;

}

//Creates a QString out of a char*

//instr: input char array
//len: length of instr

//return: newly constructed QString object
QString chartoQStr(char* instr, int len)
{
    if (len > 0){
        char *tempc = new char[len + 1];
        for (int i = 0; i < len; i++){
            tempc[i] = instr[i];
        }
        tempc[len] = '\0';
        QString str = tempc;
        return str;
    }

    return NULL;


}

//Converts an int into it's character representation

//input: input int

//return: char array representation
//Example: input = (int) 375
//-->char *return -> {'3', '7', '5'}
char* inttochar (int input)
{
        int i = 0;
        int div = 1;
        bool triggered = false;

        while (!triggered)
        {
            if (input%div == input)
            {
                triggered = true;
            }
            else
            {
                i++;
                div *= 10;
            }
        }

        char *spear = new char[i+1];
        spear[i] = '\0';

        for (int k = i - 1; k >= 0; k--)
        {
            spear[k] = (char) (48 + input%10);
            input /= 10;
        }
        return spear;
}


//Compares two char arrays

//*first: first input char array
//*second: second input char array

//int return = 0 if false, 1 if true
//note: simplified version that only returns true IFF
//first and second are exactly identical, else false
int stringcmp(char *first, char *second)
{
    int k = stringlength(first);
    if (k != stringlength(second))
        return 0;

    for (int i = 0; i < k; i++)
    {
        if (first[k] != second[k])
            return 0;
    }
    return 1;

}

//Conversion between char types for when it doesn't matter:
unsigned char *unsignmychar(char *str)
    { return (unsigned char *)str; }

char *signmychar(unsigned char *str)
    { return (char *)str; }

//converts signed integer to unsigned char array

//value: value slated for conversion

//unsigned char *return: same data split into bytes
unsigned char *int_touchar(int value, bool reversal)
{
    int temper;
    unsigned char *data = new unsigned char[4];
    unsigned char *returner = new unsigned char[4];

    temper = value;
    temper = temper & 0x000000FF;
    data[3] = (unsigned char) temper;

    temper = value;
    temper = temper >> 8;
    temper = temper & 0x000000FF;
    data[2] = (unsigned char) temper;

    temper = value;
    temper = temper >> 16;
    temper = temper & 0x000000FF;
    data[1] = (unsigned char) temper;

    temper = value;
    temper = temper >> 24;
    temper = temper & 0x000000FF;
    data[0] = (unsigned char)temper;

    if (reversal)
    {
        returner[0] = data[2];
        returner[1] = data[3];
        returner[2] = data[0];
        returner[3] = data[1];
    }
    else
    {
        returner[0] = data[0];
        returner[1] = data[1];
        returner[2] = data[2];
        returner[3] = data[3];
    }

    delete data;

    return returner;
}

unsigned char *float_touchar(float value, bool reversal)
{
    unsigned char *data = new unsigned char[4];
    unsigned char *returner = new unsigned char[4];
    unsigned char temper[sizeof(float)];

    memcpy(temper, &value, sizeof(value));

    data[0] = temper[0];
    data[1] = temper[1];
    data[2] = temper[2];
    data[3] = temper[3];

    if (reversal)
    {
        returner[0] = data[2];
        returner[1] = data[3];
        returner[2] = data[0];
        returner[3] = data[1];
    }
    else
    {
        returner[0] = data[0];
        returner[1] = data[1];
        returner[2] = data[2];
        returner[3] = data[3];
    }

    delete data;

    return returner;
}

//converts unsigned integer to unsigned char array

//value: value slated for conversion

//unsigned char *return: same data split into bytes
unsigned char *uint_touchar(unsigned int value)
{
    unsigned int temper;
    unsigned char *data = new unsigned char[4];

    temper = value;
    temper = temper & 0x000000FF;
    data[3] = (unsigned char) temper;

    temper = value;
    temper = temper >> 8;
    temper = temper & 0x000000FF;
    data[2] = (unsigned char) temper;

    temper = value;
    temper = temper >> 16;
    temper = temper & 0x000000FF;
    data[1] = (unsigned char) temper;

    temper = value;
    temper = temper >> 24;
    temper = temper & 0x000000FF;
    data[0] = (unsigned char)temper;

    return data;
}

unsigned char *special_fromuint(unsigned int value)
{

    unsigned int temper;
    unsigned char *data = new unsigned char[4];

    temper = value;
    temper = temper >> 24;
    temper = temper & 0x000000FF;
    data[2] = (unsigned char)temper;

    temper = value;
    temper = temper >> 16;
    temper = temper & 0x000000FF;
    data[3] = (unsigned char)temper;

    temper = value;
    temper = temper >> 8;
    temper = temper & 0x000000FF;
    data[0] = (unsigned char)temper;

    temper = value;
    temper = temper & 0x000000FF;
    data[1] = (unsigned char)temper;

    return data;

}

//Extracts 16-bit integer contained within 2 unsigned chars

//space: unsigned char array of 2 bytes

//int return: extracted integer

int i16bits_fromuchar(unsigned char *space)
{
    int TempW = 0x000000FF & space[0];
    TempW = (TempW << 8);
    TempW = (TempW & 0x0000FF00);
    TempW = (TempW | space[1]);
    TempW = (TempW & 0x0000FFFF);

    return TempW;

}

//Extracts integer contained within 4 unsigned chars

//space: unsigned char array of size 4 containing integer
//reversal: true IF first and second 16 bits need to be swapped

//int return: extracted integer
int fromuchar(unsigned char *space, bool reversal)
{
    unsigned char hiTemp[2];
    unsigned char loTemp[2];
    int TempW;

    if (reversal){
        hiTemp[0] = space[2];
        hiTemp[1] = space[3];
        loTemp[0] = space[0];
        loTemp[1] = space[1];
    }
    else{
        hiTemp[0] = space[0];
        hiTemp[1] = space[1];
        loTemp[0] = space[2];
        loTemp[1] = space[3];
    }

        TempW = (0x000000FF & hiTemp[0]);
        TempW = (TempW << 8);
        TempW = (TempW & 0x0000FF00);
        TempW = (TempW | hiTemp[1]);
        TempW = (TempW << 8);
        TempW = (TempW & 0x00FFFF00);
        TempW = (TempW | loTemp[0]);
        TempW = (TempW << 8);
        TempW = (TempW & 0xFFFFFF00);
        TempW = (TempW | loTemp[1]);

        return TempW;
}

//Extracts unsigned integer contained within 4 unsigned chars

//space: unsigned char array of size 4 containing integer
//reversal: true IF first and second 16 bits need to be swapped

//int return: extracted unsigned integer
unsigned int ufromuchar(unsigned char *space, bool reversal)
{
    unsigned char hiTemp[2];
    unsigned char loTemp[2];
    unsigned int TempW;

    if (reversal){
        hiTemp[0] = space[2];
        hiTemp[1] = space[3];
        loTemp[0] = space[0];
        loTemp[1] = space[1];
    }
    else{
        hiTemp[0] = space[0];
        hiTemp[1] = space[1];
        loTemp[0] = space[2];
        loTemp[1] = space[3];
    }

        TempW = (0x000000FF &hiTemp[0]);
        TempW = (TempW << 8);
        TempW = (TempW & 0x0000FF00);
        TempW = (TempW | hiTemp[1]);
        TempW = (TempW << 8);
        TempW = (TempW & 0x00FFFF00);
        TempW = (TempW | loTemp[0]);
        TempW = (TempW << 8);
        TempW = (TempW & 0xFFFFFF00);
        TempW = (TempW | loTemp[1]);

        return TempW;
}

//checks CRC16 (MODBUS) of unsigned char array

//scanr: input unsigned char array
//len: length of scanr

//bool return: true IF CRC is correct, ELSE false
bool CRC_Check(unsigned char *scanr, int len)
{
    unsigned short crcref = crc16(scanr, len - 2);
    unsigned char *crc = ushort_touchar(crcref);

    if ((crc[0] == scanr[len-2])&&(crc[1] == scanr[len - 1])){
        return true;
    }
    else{
        return false;
    }
}

//Creates new unsigned char array with data identical to
//unsigned char array inputted to function for data isolation

//input: unsigned char array to copy
//len: length of input

//unsigned char return: copied unsigned char array
unsigned char *deepcpy(unsigned char *input, int len)
{
    unsigned char *returner = new unsigned char [len];

    for (int i = 0; i < len; i++)
        returner[i] = input[i];

    return returner;
}

//Extracts int values contained in unsigned char array (one
//per byte) and converts to 'viewable' string (char array)

//input: unsigned char array containing integers
//size: size of input

//char* return: viewable data

//Note: returns as HEX viewables
//Example: input[] = {0xFB, 0xF8}
//-->return[] = "FB F8\0"
char *ucharToHexStr(unsigned char *input, int size)
{
    int newsize = (size*3);

    char *returner = new char [newsize];

    unsigned char hi;
    unsigned char lo;
    int j = 0;

    for (int i = 0; i < size; i++)
    {
        j = i*3;
        hi = (input[i] & 0xF0);
        hi = (hi >> 4);
        hi = (hi & 0x0F);
        lo = (input[i] & 0x0F);

        if (hi < 0x0A)
            returner[j] = hi + 48;
        else
            returner[j] = hi + 55;

        if (lo < 0x0A)
            returner[j + 1] = lo + 48;
        else
            returner[j + 1] = lo + 55;

        returner[j + 2] = ' ';
    }
    returner[newsize - 1] = '\0';

    return returner;
}

unsigned char up8_fromushort(unsigned short a)
{
    a = a >> 8;
    a = a & 0x00FF;
    unsigned char b = (unsigned char) a;
    return b;
}

unsigned char down8_fromushort(unsigned short a)
{
    a = a & 0x00FF;
    unsigned char b = (unsigned char) a;
    return b;
}

unsigned char *scanranger (unsigned short ubound, unsigned short lbound)
{
    unsigned short range = ubound - lbound;
    range += 1;
    unsigned char *ranger = new unsigned char[2];
    ranger[0] = up8_fromushort(range);
    ranger[1] = down8_fromushort(range);
    return ranger;
}

void delay(int msToWait)
{
    QTime dieTime = QTime::currentTime().addMSecs( msToWait );
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }
}

int intFromQStr(QString input)
{
    char* cData = QStrtochar(input);
    int returner = chartoint(cData);
    delete cData;

    return returner;
}
