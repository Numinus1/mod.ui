import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.1

Rectangle {

    color: "#9E9E9E"

    property int currIndex: 0

    SwipeView{
        id: viewSwipe

        width: parent.width
        height: parent.height * .8

        anchors.top: parent.top
        anchors.topMargin: parent.heing * .05

        anchors.left: parent.left

        currentIndex: 0

        Component.onCompleted: {
            currIndex = viewSwipe.currentIndex;
        }

        function addPage(page){
            console.log("adding new page");
            addItem(page);
            page.visible = true;
        }

        /*function popPage(list){
            var namer = "pager" + currIndex.toString();
            var component = Qt.createComponent("SlatePage.qml")
            var page = component.createObject(viewSwipe,
                                              {"id: " namer });


        }*/
    }

}
