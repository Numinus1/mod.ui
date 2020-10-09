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

    Rectangle{

        id: opBox
        height: ((parent.height * .4) < 120) ? 120 : (parent.height * .4)
        width: parent.width - 20
        anchors.left: parent.left
        anchors.leftMargin: 7
        anchors.top: parent.top
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
                text: qsTr("Mode:")
            }

            RadioButton {
                id: opSave
                text: qsTr("PC Slider Input")
                checked: true
            }

            RadioButton {
                id: opLoad
                text: qsTr("Analog Pot")
            }

        }
    }

    Button{
        id: modeButt
        width: parent.width - 20
        height: 50
        anchors.left: parent.left
        anchors.leftMargin: 7

        anchors.top: opBox.bottom
        anchors.topMargin: 5

        Material.background: "white"

        text:"Set Mode"

        ToolTip.visible: hovered
        ToolTip.text: qsTr("Choose Input Mode")
    }
}
}
