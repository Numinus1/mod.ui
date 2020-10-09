import QtQuick 2.0
 import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.1
Rectangle{
color: "white"
height: parent.height - 10
width: parent.width
anchors.top: parent.top
anchors.topMargin: 5
anchors.left: parent.left
anchors.leftMargin: 0

Component.onCompleted: {
                ark.incrementComponents();
 ark.flushButtonTransients();
           }
Button{
id: card_6_0
width: parent.width - 20
height: 50
anchors.left: parent.left
anchors.leftMargin: 7
anchors.top: parent.top
anchors.topMargin: 5
Material.background: "white"
text: "Enable"
ToolTip.visible: hovered
ToolTip.text: "Enable/Disable Drive"
property int boxToken: 6
property int cardToken: 0
function setValue(value){ Material.background = (value == 1) ? Material.Green : Material.Red;}
Connections{
target: ark
 onUpChildVal: {
if (card_6_0.boxToken === box)
{
if (card_6_0.cardToken === card)
{
card_6_0.setValue(value);
}}}
}
onClicked: {
ark.upButtonTransient(card_6_0.boxToken, card_6_0.cardToken);
}
}
Button{
id: card_7_0
width: parent.width - 20
height: 50
anchors.left: parent.left
anchors.leftMargin: 7
anchors.top: card_6_0.bottom
anchors.topMargin: 5
Material.background: "white"
text: "Detect Inertia"
ToolTip.visible: hovered
ToolTip.text: "Set Detect Inertia Flag in Drive"
property int boxToken: 7
property int cardToken: 0
function setValue(value){ Material.background = (value == 1) ? Material.Green : Material.Red;}



onWriteStatus: {

    if (status === 1){
        writeCard.message = "Write Successful";
    }
    else{
        writeCard.message = "Write Failure";
    }
    writeTimer.counter = 0;
    writeTimer.restart();

}

onUpWriteStatus(status){ \n\n    if (status === 1){ \n        writeCard.message = \"Write Successful\"; \n    } \n    else{ \n        writeCard.message = \"Write Failure\"; \n    } \n    writeTimer.counter = 0; \n    writeTimer.restart(); \n\n } \n
"
Card{
    id: writeCard

    property string NewMessage: "None"

    anchors.top: reportCard.bottom

    Timer{
        id: writeTimer
        interval: 1000
        running: false
        repeat: true
        property int counter: 0
        property bool qul: false

        onTriggered: {
            counter += 1;
            writeCard.setText(message + " " + counter + " sec ago");
        }
    }

    Component.onCompleted: {
        setName("Write Status: ");
        setText(writeCard.message);
        setFontSize(10);
        setViewTool("Status of last Write Command");
    }
}

Card{ \n    id: writeCard \n    property string message: \"\"\n \n    anchors.top: reportCard.bottom \n\n    Timer{\n         id: writeTimer \n        interval: 1000 \n        running: false\n         repeat: true \n        property int counter: 0 \n        onTriggered: {\n             counter += 1;\n             writeCard.setText(message + \" \" + counter + \" sec ago\");\n         }\n     }\n     Component.onCompleted: { \n        setName(\"Write Status: \"); \n        setText(\"\"); \n        setFontSize(10); \n        setViewTool(\"Status of last Write Command\"); \n    } \n}\n
"
Connections{
target: ark
 onUpChildVal: {
if (card_7_0.boxToken === box)
{
if (card_7_0.cardToken === card)
{
card_7_0.setValue(value);
}}}
}
onClicked: {
ark.upButtonTransient(card_7_0.boxToken, card_7_0.cardToken);
}
}

 }


