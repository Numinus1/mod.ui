import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.1

Rectangle{
    color: "#9E9E9E"

    property int primeViewCount: 0

    function addPanel(panelcode)
    {
        primePage.spawnCustomPanel(panelcode);
    }



Page {
    id: primePage
    anchors.left: parent.left
    anchors.top: parent.top
    height: parent.height
    width: parent.width


    function spawnCustomPanel(panelcode)
    {

            prime_SwipeView.addPage(prime_SwipeView.createPage(panelcode));
    }


    header: TabBar{
        id: prime_tabbar
        Material.accent: Material.Indigo

        TabButton{
            text:qsTr("Custom");
        }

    }

    SwipeView{
        id: prime_SwipeView
        anchors.fill: parent
        currentIndex: prime_tabbar.currentIndex
        clip: true

        property int childCount: 1

        function addPage(page){
                addItem(page);
                childCount += 1;
                page.visible = true;
        }

        function createPage(qmlString){
                    //console.log(qmlString);
                    var page = Qt.createQmlObject(qmlString, rootNoder, "whatevs.txt");
                    rootNoder.data = page;
                    return page;
                }


    }
}
}
