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
