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
        id: connButt
        width: parent.width - 20
        height: 50
        anchors.left: parent.left
        anchors.leftMargin: 7

        anchors.top: parent.top
        anchors.topMargin: 5

        Material.background: "white"

        text:"Search for Device"

        ToolTip.visible: hovered;
        ToolTip.text: qsTr("Search for the Drive across all COM Ports")

        onClicked: {
            ark.startSweep();
        }
    }

    Button{
        id: canButt
        width: parent.width - 20
        height: 50
        anchors.left: parent.left
        anchors.leftMargin: 7

        anchors.top: connButt.bottom
        anchors.topMargin: 5

        Material.background: "white"

        text:"Disconnect"

        ToolTip.visible: hovered
        ToolTip.text: qsTr("Stop Communicating with Device")

        onClicked: {
            ark.pauseComm();
        }
    }

    Rectangle{

        id: opBox
        height: ((parent.height * .4) < 120) ? 120 : (parent.height * .4)
        width: parent.width - 20
        anchors.left: parent.left
        anchors.leftMargin: 7
        anchors.top: canButt.bottom
        anchors.topMargin: 5

        Material.background: "white"
        ButtonGroup{
            buttons: opChoice.children
        }

        Column{
            id: opChoice
            anchors.left: parent.left
            anchors.leftMargin: 5
            anchors.top: parent.top
            anchors.topMargin: 5

            Material.accent: Material.Green
            Material.background: "white"

            Label{
                text: qsTr("Operation:")
            }

            RadioButton {
                id: opSave
                text: qsTr("Save Configuration")
                //checked: true
            }

            RadioButton {
                id: opLoad
                text: qsTr("Load Configuration")
                checked: true
            }

        }
    }

    Button{
        id: pathButt
        width: parent.width - 20
        height: 50
        anchors.left: parent.left
        anchors.leftMargin: 7

        anchors.top: opBox.bottom
        anchors.topMargin: 5

        Material.background: "white"

        text:"Select File"

        ToolTip.visible: hovered
        ToolTip.text: qsTr("Choose the File Name to Save/Load Configuration")
        onClicked: {
            if(opSave.checked)
            {
                rootNoder.showFileDialog(0);
            }
            else{
                rootNoder.showFileDialog(1);
            }
        }
    }

    Button{
        id: sendButt
        width: parent.width - 20
        height: 50
        anchors.left: parent.left
        anchors.leftMargin: 7

        anchors.top: pathButt.bottom
        anchors.topMargin: 5

        Material.background: "white"

        text:"Execute"

        ToolTip.visible: hovered
        ToolTip.text: qsTr("Write the Selected File to the Drive")
    }

}
}
