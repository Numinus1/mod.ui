import QtQuick 2.9
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.1
import QtQuick.Window 2.2

Rectangle {

    Connections{
        target: ark

        onUpChildVal: {
        }
    }

    function addTab(qmlString, title)
    {

    }

    function addCustomPage(qmlString)
    {
        console.log("adding custom page: ");
        //console.log(qmlString);

        viewSwipe.addPage(viewSwipe.createPage(qmlString));
    }

    function iterateChildren()
    {
        viewSwipe.iterateChildren();
    }

    function upChild(box, card, value)
    {
        viewSwipe.upChild(box, card, value);
    }

    color: "#9E9E9E"
    property int currIndex: 0

    SwipeView{
        id: viewSwipe

        width: parent.width
        height: parent.height * .7

        anchors.top: parent.top
        anchors.topMargin: parent.height * .05
        anchors.left: parent.left

        currentIndex: 0

        property int childCount: 0

        Component.onCompleted: {
        }

        function addPage(page){
                addItem(page);
                childCount += 1;
                page.visible = true;
        }

        function createPage(qmlString){
                    var page = Qt.createQmlObject(qmlString, viewSwipe, "whatevs.txt");

                    return page;
                }

        function iterateChildren()
        {
            for (var i = 0; i < childCount; i++)
            {
                console.log(viewSwipe.itemAt(i).token);
            }
        }

        function upChild(box, card, value)
        {
            for (var i = 0; i < childCount; i++)
            {
                if (viewSwipe.itemAt(i).token === box){

                    var pager = viewSwipe.itemAt(i);

                    console.log(pager.count);
                    for (var j = 0; j < pager.count; j++)
                    {

                        console.log(pager.itemAt(j).cardToken);
                    }
                }
            }
        }



    }

    PageIndicator{
        id: erer
        count: viewSwipe.count
        currentIndex: viewSwipe.currentIndex
        interactive: true
        onCurrentIndexChanged: viewSwipe.currentIndex = currentIndex

        width: parent.width
        height: parent.height * .1
        anchors.top: viewSwipe.bottom
        anchors.topMargin: parent.height * .05
        anchors.left: parent.left

    }
}
