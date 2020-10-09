#ifndef PAGETEMPLATE_H
#define PAGETEMPLATE_H

#include "parameterbox.h"

class PageTemplate
{
public:
    PageTemplate();
    PageTemplate(int boxTokener);
    PageTemplate(int boxTokener, int offset);
    PageTemplate(ParameterBox box, int boxTokener);
    PageTemplate(int boxTokener, QString title);
    PageTemplate(int boxTokener, QString title, int offset);

    QString get_mainString();
    QString get_TabTitle();
    QString get_basicPage();

    void addCard(MotorParameter in);
    void addCard(MotorParameter in, int boxOffset);
    void addControlledCard(MotorParameter in, int box, int card);

    void addCardOffset(int foffset);
    void resetCardOffset();

private:
    QString qmlcode;
    QString basicPage;
    int boxToken;
    int *cardtokens;
    int cardCount;
    int cardOffset;
    int offset;
    QString Title;
    QString prevID;
    int lastBoxOffset;
};

#endif // PAGETEMPLATE_H
