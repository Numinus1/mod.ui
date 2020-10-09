import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.1

Rectangle {

    color: "#9E9E9E"

    Card{
        id: connCard

        anchors.top: parent.top
        Component.onCompleted: {
            setName("Connection Status: ");
            setText("Not Connected");
            setFontSize(10);
            setViewTool("State of Connection with a JEDL Drive");
        }
    }

    Card{
        id: addCard

        anchors.top: connCard.bottom
        Component.onCompleted: {
            setName("Drive Address: ");
            setText("00");
            setFontSize(10);
            setViewTool("Destination Address of Transmissions")
        }
    }

    Card{
        id: pathCard

        anchors.top: addCard.bottom
        Component.onCompleted: {
            setName("File Path: ");
            setText("//");
            setFontSize(10);
            setViewTool("Path to the selected file");
        }
    }

    Card{
        id: reportCard

        anchors.top: pathCard.bottom
        Component.onCompleted: {
            setName("Operation Status: ");
            setText("Uninitiated");
            setFontSize(10);
            setViewTool("Status of the current Save or Load operation")
        }
    }



}
