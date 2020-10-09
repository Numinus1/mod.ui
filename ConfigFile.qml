import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.1

Rectangle {

    color: "#9E9E9E"

    Card{
        id: opCard

        anchors.top: parent.top
        Component.onCompleted: {
            setName("Selected Operation: ");
            setText("Save Configuration");
            setFontSize(10);
            setViewTool("The current Parameters in the Drive will be Saved to File")
        }
    }

    Card{
        id: pathCard

        anchors.top: opCard.bottom
        Component.onCompleted: {
            setName("File Path: ");
            setText("//");
            setFontSize(10);
            setViewTool("Path to the selected file");
        }
    }

    Card{
        id: countCard

        anchors.top: pathCard.bottom
        Component.onCompleted: {
            setName("Parameter Count: ");
            setText("");
            setFontSize(10);
            setViewTool("Number of Parameters that will be Saved From/Loaded To the Drive")
        }
    }

    Card{
        id: reportCard

        anchors.top: countCard.bottom
        Component.onCompleted: {
            setName("Operation Status: ");
            setText("Uninitiated");
            setFontSize(10);
            setViewTool("Status of the current Save or Load operation")
        }
    }

}
