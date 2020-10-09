#include "pagetemplate.h"

PageTemplate::PageTemplate()
{
    qmlcode = "import QtQuick 2.0 \n import QtQuick.Controls 2.2\n";
    qmlcode.append("import QtQuick.Controls.Material 2.1\n");
    qmlcode.append("Rectangle { \n color: \"#9E9E9E\" \n");

    boxToken = 0;

    QString pageID = (QString::number(boxToken));
    qmlcode.append("property int token: ");
    qmlcode.append(pageID);
    qmlcode.append(" \n");

    qmlcode.append("Component.onCompleted: { \n                ark.incrementComponents(); \n            } \n");



    cardtokens = NULL;
    cardCount = 0;
    cardOffset = 0;
    offset = 0;
    lastBoxOffset = -1;
}

PageTemplate::PageTemplate(int boxTokener)
{
    qmlcode = "import QtQuick 2.0 \n import QtQuick.Controls 2.2\n";
    qmlcode.append("import QtQuick.Controls.Material 2.1\n");
    qmlcode.append("Rectangle { \n color: \"#9E9E9E\" \n");

    boxToken = boxTokener;

    QString pageID = (QString::number(boxToken));
    qmlcode.append("property int token: ");
    qmlcode.append(pageID);
    qmlcode.append(" \n");

    qmlcode.append("Component.onCompleted: { \n                ark.incrementComponents(); \n            } \n");


    cardtokens = NULL;
    cardCount = 0;
    cardOffset = 0;
    offset = 0;
    lastBoxOffset = -1;
}

PageTemplate::PageTemplate(int boxTokener, QString title)
{
    qmlcode = "import QtQuick 2.0 \n import QtQuick.Controls 2.2\n";
    qmlcode.append("import QtQuick.Controls.Material 2.1\n");
    qmlcode.append("Rectangle { \n color: \"#9E9E9E\" \n");

    boxToken = boxTokener;

    QString pageID = (QString::number(boxToken));
    qmlcode.append("property int token: ");
    qmlcode.append(pageID);
    qmlcode.append(" \n");

    qmlcode.append("Component.onCompleted: { \n                ark.incrementComponents(); \n            } \n");


    cardtokens = NULL;
    cardCount = 0;
    cardOffset = 0;
    offset = 0;

    QString titleString = "import QtQuick 2.7 \nimport QtQuick.Controls 2.2 \nimport QtQuick.Controls.Material 2.1 \n";
    titleString.append("TabButton{ \n                text: qsTr(\"");
    titleString.append(title);
    titleString.append("\") \n ");
    titleString.append("ToolTip.visible: hovered \n ");
    titleString.append("ToolTip.text: qsTr(\"");
    titleString.append(title);
    titleString.append("\") \n ");
    titleString.append("\n } \n");
    Title = titleString;
    lastBoxOffset = -1;
}

PageTemplate::PageTemplate(int boxTokener, QString title, int offsetter)
{
    qmlcode = "import QtQuick 2.0 \n import QtQuick.Controls 2.2\n";
    qmlcode.append("import QtQuick.Controls.Material 2.1\n");
    qmlcode.append("Rectangle { \n color: \"#9E9E9E\" \n");

    boxToken = boxTokener;

    QString pageID = (QString::number(boxToken));
    qmlcode.append("property int token: ");
    qmlcode.append(pageID);
    qmlcode.append(" \n");

    qmlcode.append("Component.onCompleted: { \n                ark.incrementComponents(); \n            } \n");


    cardtokens = NULL;
    cardCount = 0;
    cardOffset = 0;
    offset = offsetter;

    QString titleString = "import QtQuick 2.7 \nimport QtQuick.Controls 2.2 \nimport QtQuick.Controls.Material 2.1 \n";
    titleString.append("TabButton{ \n                text: qsTr(\"");
    titleString.append(title);
    titleString.append("\") \n ");
    titleString.append("ToolTip.visible: hovered \n ");
    titleString.append("ToolTip.text: qsTr(\"");
    titleString.append(title);
    titleString.append("\") \n ");
    titleString.append("\n } \n");
    qDebug() << titleString;
    Title = titleString;
    lastBoxOffset = -1;
}

PageTemplate::PageTemplate(ParameterBox box, int boxTokener)
{
    qmlcode = "import QtQuick 2.0 \n import QtQuick.Controls 2.2\n";
    qmlcode.append("import QtQuick.Controls.Material 2.1\n");
    qmlcode.append("Rectangle { \n color: \"#9E9E9E\" \n");

    boxToken = boxTokener;

    QString pageID = (QString::number(boxToken));
    qmlcode.append("property int token: ");
    qmlcode.append(pageID);
    qmlcode.append(" \n");

    qmlcode.append ("Component.onCompleted: { \n                ark.incrementComponents(); \n            } \n");

    qmlcode.append ("Connections{ \ntarget: ark \n ");

    qmlcode.append ("onCleanUp: { \n");
    qmlcode.append ("this.destroy(); \n}\n");
    qmlcode.append ("}\n");

    cardtokens = NULL;
    cardCount = 0;
    cardOffset = 0;
    offset = 0;

    for (int i = 0; i < box.getCount(); i++)
    {
        addCard(box.getMotorParameter(i));
    }
    lastBoxOffset = -1;
}

QString PageTemplate::get_mainString()
{
    qmlcode.append(" \n }");
    return qmlcode;
}

QString PageTemplate::get_basicPage()
{
    return basicPage;
}

void PageTemplate::addCard(MotorParameter in)
{
    QString cardID = "card_";
    cardID.append(QString::number(boxToken));
    cardID.append("_");
    cardID.append(QString::number(cardCount + cardOffset));

    QString tagID = "tag_";
    tagID.append(QString::number(boxToken));
    tagID.append("_");
    tagID.append(QString::number(cardCount + cardOffset));

    QString viewID = "view_";
    viewID.append(QString::number(boxToken));
    viewID.append("_");
    viewID.append(QString::number(cardCount + cardOffset));


    qmlcode.append("Page{ \n");


    qmlcode.append("property int boxToken: ");
    qmlcode.append(QString::number(boxToken));
    qmlcode.append(" \n");

    qmlcode.append("property int cardToken: ");
    qmlcode.append(QString::number(cardCount + offset + cardOffset));
    qmlcode.append(" \n");

    qmlcode.append ("Connections{ \ntarget: ark \n ");

    qmlcode.append("onUpChildVal: { \n");
    qmlcode.append("if (");
    qmlcode.append (cardID);
    qmlcode.append(".boxToken === box) \n{ \nif (");
    qmlcode.append (cardID);
    qmlcode.append(".cardToken === card) \n{ \n");
    qmlcode.append(cardID);
    qmlcode.append(".setText(value); \n}}}\n");

    qmlcode.append("onGetChildVal: { \nif (");
    qmlcode.append(cardID);
    qmlcode.append(".boxToken === box){ \n if (");
    qmlcode.append(cardID);
    qmlcode.append(".cardToken === card){ \nark.getVal(box, card, ");
    qmlcode.append(viewID);
    qmlcode.append(".yieldText()); \n}}} \n");
    qmlcode.append("}\n");

    qmlcode.append("function setName(name){");
    qmlcode.append(tagID);
    qmlcode.append(".text = name;} \n");


    qmlcode.append("function setText(texting){");
    qmlcode.append(viewID);
    qmlcode.append(".text = texting;} \n");

    qmlcode.append("function setFontSize(x){");
    qmlcode.append(tagID);
    qmlcode.append(".font.pointSize = x; \n ");
    qmlcode.append(viewID);
    qmlcode.append(".font.pointSize = x;} \n");

    qmlcode.append("function setViewTool(tip){");
    qmlcode.append(viewID);
    qmlcode.append(".ToolTip.text = tip;} \n\n");

    qmlcode.append ("Component.onCompleted: { \nsetFontSize(10); \n                ark.incrementComponents(); \n }");
    qmlcode.append("id: ");
    qmlcode.append(cardID);
    qmlcode.append(" \n");
    qmlcode.append("height: ((parent.height * .2) > 50) ? 50 : (parent.height * .2) \n width: parent.width \n");
    qmlcode.append("anchors.left: parent.left \n ");

    if (cardCount == 0){
        qmlcode.append("anchors.top: parent.top \n anchors.topMargin: 5 \n");
    }
    else{
        qmlcode.append("anchors.top: ");
        qmlcode.append(prevID);
        qmlcode.append(".bottom \n anchors.topMargin: 5 \n ");
    }

    prevID = cardID;

    qmlcode.append("Label{ id: ");
    qmlcode.append(tagID);
    qmlcode.append("\n");
    qmlcode.append("width: parent.width / 2 - 10 \n height: parent.height - 10 \n anchors.left: parent.left \n anchors.leftMargin: 10 \n anchors.top: parent.top \n anchors.topMargin: 10 \n textFormat: Text.RichText \n");
    qmlcode.append("text: \"");
    qmlcode.append(in.getName());
    qmlcode.append("\" } \n");
    qmlcode.append("TextArea{ \n");

    qmlcode.append("onEditingFinished: { \n            ark.upTransient(");
    qmlcode.append(cardID);
    qmlcode.append(".boxToken, ");
    qmlcode.append(cardID);
    qmlcode.append(".cardToken, ");
    qmlcode.append(viewID);
    qmlcode.append(".yieldText()); \n        } \n");
    qmlcode.append ("function yieldText(){ return ");
    qmlcode.append("text;} \n");

    qmlcode.append("id: ");
    qmlcode.append(viewID);
    qmlcode.append("\n width: 2 * (parent.width) / 4 - 5 \n height: parent.height \n anchors.left: ");
    qmlcode.append(tagID);
    qmlcode.append(".right \n anchors.top: parent.top \n text: \"\" \n ToolTip.visible: hovered \n");
    qmlcode.append("ToolTip.text: \"");
    qmlcode.append(in.getDescription());
    qmlcode.append("\" \n } \n");
    qmlcode.append("}\n");

    cardCount += 1;

}

void PageTemplate::addCard(MotorParameter in, int boxOffset)
{
    if (lastBoxOffset != -1){
        if (boxOffset != lastBoxOffset){
            cardCount = 0;
            offset = 0;
        }
    }

    QString cardID = "card_";
    cardID.append(QString::number(boxToken + boxOffset));
    cardID.append("_");
    cardID.append(QString::number(cardCount + offset));

    QString tagID = "tag_";
    tagID.append(QString::number(boxToken + boxOffset));
    tagID.append("_");
    tagID.append(QString::number(cardCount + offset));

    QString viewID = "view_";
    viewID.append(QString::number(boxToken + boxOffset));
    viewID.append("_");
    viewID.append(QString::number(cardCount));

    qDebug() << "Adding " << in.getName() << " @ " << in.getAddr() << " -> " << cardID;
    qDebug() << "boxToken: " << boxToken + boxOffset;
    qDebug() << "cardToken: " << cardCount + offset;

    qmlcode.append("Page{ \n");


    qmlcode.append("property int boxToken: ");
    qmlcode.append(QString::number(boxToken + boxOffset));
    qmlcode.append(" \n");

    qmlcode.append("property int cardToken: ");
    qmlcode.append(QString::number(cardCount + offset));
    qmlcode.append(" \n");

    qmlcode.append ("Connections{ \ntarget: ark \n ");

    qmlcode.append("onUpChildVal: { \n");
    qmlcode.append("if (");
    qmlcode.append (cardID);
    qmlcode.append(".boxToken === box) \n{ \nif (");
    qmlcode.append (cardID);
    qmlcode.append(".cardToken === card) \n{ \n");
    qmlcode.append(cardID);
    qmlcode.append(".setText(value); \n}}}\n");

    qmlcode.append("onGetChildVal: { \nif (");
    qmlcode.append(cardID);
    qmlcode.append(".boxToken === box){ \n if (");
    qmlcode.append(cardID);
    qmlcode.append(".cardToken === card){ \nark.getVal(box, card, ");
    qmlcode.append(viewID);
    qmlcode.append(".yieldText()); \n}}} \n");
    qmlcode.append("}\n");

    qmlcode.append("function setName(name){");
    qmlcode.append(tagID);
    qmlcode.append(".text = name;} \n");


    qmlcode.append("function setText(texting){");
    qmlcode.append(viewID);
    qmlcode.append(".text = texting;} \n");

    qmlcode.append("function setFontSize(x){");
    qmlcode.append(tagID);
    qmlcode.append(".font.pointSize = x; \n ");
    qmlcode.append(viewID);
    qmlcode.append(".font.pointSize = x;} \n");

    qmlcode.append("function setViewTool(tip){");
    qmlcode.append(viewID);
    qmlcode.append(".ToolTip.text = tip;} \n\n");

    qmlcode.append ("Component.onCompleted: { \nsetFontSize(10); \n                ark.incrementComponents(); \n }");
    qmlcode.append("id: ");
    qmlcode.append(cardID);
    qmlcode.append(" \n");
    qmlcode.append("height: ((parent.height * .2) > 50) ? 50 : (parent.height * .2) \n width: parent.width \n");
    qmlcode.append("anchors.left: parent.left \n ");

    if ((cardCount == 0)&&(lastBoxOffset == -1)){
        qmlcode.append("anchors.top: parent.top \n anchors.topMargin: 5 \n");
    }
    else{
        qmlcode.append("anchors.top: ");
        qmlcode.append(prevID);
        qmlcode.append(".bottom \n anchors.topMargin: 5 \n ");
    }

    prevID = cardID;
    lastBoxOffset = boxOffset;

    qmlcode.append("Label{ id: ");
    qmlcode.append(tagID);
    qmlcode.append("\n");
    qmlcode.append("width: parent.width / 2 - 10 \n height: parent.height - 10 \n anchors.left: parent.left \n anchors.leftMargin: 10 \n anchors.top: parent.top \n anchors.topMargin: 10 \n textFormat: Text.RichText \n");
    qmlcode.append("text: \"");
    qmlcode.append(in.getName());
    qmlcode.append("\" } \n");
    qmlcode.append("TextArea{ \n");

    qmlcode.append("onEditingFinished: { \n            ark.upTransient(");
    qmlcode.append(cardID);
    qmlcode.append(".boxToken, ");
    qmlcode.append(cardID);
    qmlcode.append(".cardToken, ");
    qmlcode.append(viewID);
    qmlcode.append(".yieldText()); \n        } \n");
    qmlcode.append ("function yieldText(){ return ");
    qmlcode.append("text;} \n");

    qmlcode.append("id: ");
    qmlcode.append(viewID);
    qmlcode.append("\n width: 2 * (parent.width) / 4 - 5 \n height: parent.height \n anchors.left: ");
    qmlcode.append(tagID);
    qmlcode.append(".right \n anchors.top: parent.top \n text: \"\" \n ToolTip.visible: hovered \n");
    qmlcode.append("ToolTip.text: \"");
    qmlcode.append(in.getDescription());
    qmlcode.append("\" \n } \n");
    qmlcode.append("}\n");

    cardCount += 1;

}

void PageTemplate::addControlledCard(MotorParameter in, int box, int card)
{
    QString cardID = "card_";
    cardID.append(QString::number(box));
    cardID.append("_");
    cardID.append(QString::number(card));

    QString tagID = "tag_";
    tagID.append(QString::number(box));
    tagID.append("_");
    tagID.append(QString::number(card));

    QString viewID = "view_";
    viewID.append(QString::number(box));
    viewID.append("_");
    viewID.append(QString::number(card));

    //qDebug() << "Adding " << in.getName() << " @ " << in.getAddr() << " -> " << cardID;
    //qDebug() << "boxToken: " << box;
    //qDebug() << "cardToken: " << card;

    qmlcode.append("Page{ \n");


    qmlcode.append("property int boxToken: ");
    qmlcode.append(QString::number(box));
    qmlcode.append(" \n");

    qmlcode.append("property int cardToken: ");
    qmlcode.append(QString::number(card));
    qmlcode.append(" \n");

    qmlcode.append ("Connections{ \ntarget: ark \n ");

    qmlcode.append("onUpChildVal: { \n");
    qmlcode.append("if (");
    qmlcode.append (cardID);
    qmlcode.append(".boxToken === box) \n{ \nif (");
    qmlcode.append (cardID);
    qmlcode.append(".cardToken === card) \n{ \n");
    qmlcode.append(cardID);
    qmlcode.append(".setText(value); \n}}}\n");

    qmlcode.append("onGetChildVal: { \nif (");
    qmlcode.append(cardID);
    qmlcode.append(".boxToken === box){ \n if (");
    qmlcode.append(cardID);
    qmlcode.append(".cardToken === card){ \nark.getVal(box, card, ");
    qmlcode.append(viewID);
    qmlcode.append(".yieldText()); \n}}} \n");
    qmlcode.append("}\n");

    qmlcode.append("function setName(name){");
    qmlcode.append(tagID);
    qmlcode.append(".text = name;} \n");


    qmlcode.append("function setText(texting){");
    qmlcode.append(viewID);
    qmlcode.append(".text = texting;} \n");

    qmlcode.append("function setFontSize(x){");
    qmlcode.append(tagID);
    qmlcode.append(".font.pointSize = x; \n ");
    qmlcode.append(viewID);
    qmlcode.append(".font.pointSize = x;} \n");

    qmlcode.append("function setViewTool(tip){");
    qmlcode.append(viewID);
    qmlcode.append(".ToolTip.text = tip;} \n\n");

    qmlcode.append ("Component.onCompleted: { \nsetFontSize(10); \n                ark.incrementComponents(); \n }");
    qmlcode.append("id: ");
    qmlcode.append(cardID);
    qmlcode.append(" \n");
    qmlcode.append("height: ((parent.height * .2) > 50) ? 50 : (parent.height * .2) \n width: parent.width \n");
    qmlcode.append("anchors.left: parent.left \n ");

    if (cardCount == 0){
        qmlcode.append("anchors.top: parent.top \n anchors.topMargin: 5 \n");
    }
    else{
        qmlcode.append("anchors.top: ");
        qmlcode.append(prevID);
        qmlcode.append(".bottom \n anchors.topMargin: 5 \n ");
    }

    prevID = cardID;

    qmlcode.append("Label{ id: ");
    qmlcode.append(tagID);
    qmlcode.append("\n");
    qmlcode.append("width: parent.width / 2 - 10 \n height: parent.height - 10 \n anchors.left: parent.left \n anchors.leftMargin: 10 \n anchors.top: parent.top \n anchors.topMargin: 10 \n textFormat: Text.RichText \n");
    qmlcode.append("text: \"");
    qmlcode.append(in.getName());
    qmlcode.append("\" } \n");
    qmlcode.append("TextArea{ \n");

    qmlcode.append("onEditingFinished: { \n            ark.upTransient(");
    qmlcode.append(cardID);
    qmlcode.append(".boxToken, ");
    qmlcode.append(cardID);
    qmlcode.append(".cardToken, ");
    qmlcode.append(viewID);
    qmlcode.append(".yieldText()); \n        } \n");
    qmlcode.append ("function yieldText(){ return ");
    qmlcode.append("text;} \n");

    qmlcode.append("id: ");
    qmlcode.append(viewID);
    qmlcode.append("\n width: 2 * (parent.width) / 4 - 5 \n height: parent.height \n anchors.left: ");
    qmlcode.append(tagID);
    qmlcode.append(".right \n anchors.top: parent.top \n text: \"\" \n ToolTip.visible: hovered \n");
    qmlcode.append("ToolTip.text: \"");
    qmlcode.append(in.getDescription());
    qmlcode.append("\" \n } \n");
    qmlcode.append("}\n");

    cardCount += 1;

}

void PageTemplate::addCardOffset(int foffset)
{
    offset += foffset;
}

void PageTemplate::resetCardOffset()
{
    offset = 0;
}

QString PageTemplate::get_TabTitle()
{
    return Title;
}
