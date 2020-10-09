import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.1

Rectangle {

    color: "#9E9E9E"
    Rectangle {
        color: "white"
        height: parent.height - 10
        width: parent.width

        anchors.top: parent.top
        anchors.topMargin: 5

        anchors.left: parent.left
        anchors.leftMargin: 0

    Button{
        id: writeButt
        width: parent.width - 20
        height: 50
        anchors.left: parent.left
        anchors.leftMargin: 7

        anchors.top: parent.top
        anchors.topMargin: 5

        Material.background: "white"

        text:"Write"

        ToolTip.visible: hovered;
        ToolTip.text: qsTr("Write Values")

        onClicked: {
            ark.promptWrite();
        }
    }

}
}
