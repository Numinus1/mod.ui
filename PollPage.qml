import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.1

Rectangle {

    color: "#9E9E9E"

    Card{
        id: speedCard

        anchors.top: parent.top
        Component.onCompleted: {
            setName("Speed: ");
            setText("");
            setFontSize(10);
            setViewTool("The current Speed of the Motor")
        }
    }

    Card{
        id: currCard

        anchors.top: speedCard.bottom
        Component.onCompleted: {
            setName("Current: ");
            setText("");
            setFontSize(10);
            setViewTool("The Motor Current")
        }
    }

    Card{
        id: voltCard

        anchors.top: currCard.bottom
        Component.onCompleted: {
            setName("Voltage: ");
            setText("");
            setFontSize(10);
            setViewTool("The current Terminal Voltage")
        }
    }

    Card{
        id: heatCard

        anchors.top: voltCard.bottom
        Component.onCompleted: {
            setName("Temperature: ");
            setText("");
            setFontSize(10);
            setViewTool("The current Temperature of the Drive (not Motor)")
        }
    }

}
