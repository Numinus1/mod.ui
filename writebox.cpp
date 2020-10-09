#include "writebox.h"

WriteBox::WriteBox()
{
    //include statements, open Main Rectangle
    qmlcode = "import QtQuick 2.0 \n import QtQuick.Controls 2.2\n";
    qmlcode.append("import QtQuick.Controls.Material 2.1\n");
    qmlcode.append("Rectangle { \n");
    qmlcode.append("color: \"#9E9E9E\" \n");
    qmlcode.append("Rectangle{ \n");
    qmlcode.append("color: \"white\" \n");
    qmlcode.append("height: parent.height - 10 \n");
    qmlcode.append("width: parent.width \n");
    qmlcode.append("anchors.top: parent.top \n");
    qmlcode.append("anchors.topMargin: 5 \n");
    qmlcode.append("anchors.left: parent.left \n");
    qmlcode.append("anchors.leftMargin: 0 \n");
    qmlcode.append("\n");




    //add signal of component completion
    qmlcode.append("Component.onCompleted: { \n                ark.incrementComponents(); \n ark.flushButtonTransients(); \n           } \n");

    //set initial values
    buttonCount = 0;
    prevToken = -1;
    prevId = "parent";
}

//get entire string to construct component
QString WriteBox::get_mainString()
{
    //close Grey and White Rectangles, return qmlcode
    qmlcode.append(" \n } \n}");
    return qmlcode;
}

//add button to WriteBox
void WriteBox::addButton(int boxToken, MotorParameter in)
{
    bool first = false;
    if (prevToken == -1){
        prevToken = boxToken;
        first = true;
    }
    else{
        if (prevToken != boxToken){
            buttonCount = 0;
        }
    }
    //prep ID
    QString cardID = "card_";
    cardID.append(QString::number(boxToken));
    cardID.append("_");
    cardID.append(QString::number(buttonCount));


    //open Button
    qmlcode.append("Button{ \n");
    qmlcode.append("id: ");
    qmlcode.append(cardID);
    qmlcode.append(" \n");

    //define anchors and dimensions
    qmlcode.append("width: parent.width - 20 \n");
    qmlcode.append("height: 50 \n");
    qmlcode.append("anchors.left: parent.left \n");
    qmlcode.append("anchors.leftMargin: 7 \n");

        //define top anchor
        if (first){
            qmlcode.append("anchors.top: parent.top \n");
        }
        else{
            qmlcode.append("anchors.top: ");
            qmlcode.append(prevId);
            qmlcode.append(".bottom \n");
        }
    prevId = cardID;
    qmlcode.append("anchors.topMargin: 5 \n");

    //add Background and Text
    qmlcode.append("Material.background: \"white\" \n");
    qmlcode.append("text: \"");
    qmlcode.append(in.getName());
    qmlcode.append("\" \n");
    //add ToolTip
    qmlcode.append("ToolTip.visible: hovered \n");
    qmlcode.append("ToolTip.text: \"");
    qmlcode.append(in.getDescription());
    qmlcode.append("\" \n");

    //add Internal Identifiers
    qmlcode.append("property int boxToken: ");
    qmlcode.append(QString::number(boxToken));
    qmlcode.append(" \n");

    qmlcode.append("property int cardToken: ");
    qmlcode.append(QString::number(buttonCount));
    qmlcode.append(" \n");

    //add workhorse
    qmlcode.append("function setValue(value){ ");
    qmlcode.append("Material.background = (value == 1) ? Material.Green : Material.Red;");
    qmlcode.append("} \n");

    //add slots
    qmlcode.append ("Connections{ \ntarget: ark \n ");

    //slot 1
    qmlcode.append("onUpChildVal: { \n");
    qmlcode.append("if (");
    qmlcode.append (cardID);
    qmlcode.append(".boxToken === box) \n{ \nif (");
    qmlcode.append (cardID);
    qmlcode.append(".cardToken === card) \n{ \n");
    qmlcode.append(cardID);
    qmlcode.append(".setValue(value); \n}}}\n");

    qmlcode.append("}\n");

    //add click trigger
    qmlcode.append("onClicked: { \n");
    qmlcode.append("ark.upButtonTransient(");
    qmlcode.append(cardID);
    qmlcode.append(".boxToken");
    qmlcode.append(", ");
    qmlcode.append(cardID);
    qmlcode.append(".cardToken");
    qmlcode.append("); \n");
    qmlcode.append("} \n");

    //close button
    qmlcode.append("} \n");

    buttonCount += 1;

}
