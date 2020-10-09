import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.1

Rectangle {

    color: "#9E9E9E"

    Card{
        id: slideCard

        anchors.top: parent.top
        Component.onCompleted: {
            setName("Stiffness: ");
            setText("0");
            setFontSize(10);
            setViewTool("Value of the Stiffness Parameter of Slider");
        }
    }

    Rectangle{
        width: parent.width
        height: 50
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.top: slideCard.bottom
        anchors.topMargin: 10

        Slider{
        id: stiffSlider

        anchors.left: parent.left
        anchors.top: parent.top

        width: parent.width

        onValueChanged: {
            slideCard.setText(value.toString());
        }
    }
    }

}
