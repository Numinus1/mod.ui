import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.1

Rectangle{
    color: "#9E9E9E"

    property int primeViewCount: 0

    function addTabs(pageList, titleList, pageCount)
    {
        primePage.spawnCustomTabs(pageList, titleList, pageCount);
    }

    function insertPage(qmlString)
    {
        primePage.insertPage(qmlString);
    }

    function insertCommPage()
    {
        prime_SwipeView.addPage(prime_SwipeView.createPage("import QtQuick 2.7 \nimport QtQuick.Controls 2.2 \nimport QtQuick.Controls.Material 2.1 \nRectangle { \n   color: \"#9E9E9E\" \n     Connections{ \n        target: ark \n        onUpWriteStatus: {\n \n    if (status === 1){ \n         writeCard.message = \"Write Successful\"; \n    }\n     else{\n         writeCard.message = \"Write Failure\";\n     }\n     writeTimer.counter = 0;\n     writeTimer.restart();\n \n }\n onDeviceConnected:{ \n            console.log(\"connected to device\"); \n            connCard.setText(\"Connected\"); \n        } \n    } Card{ \n        id: connCard \n        anchors.top: parent.top \n        Component.onCompleted: { \n            setName(\"Connection Status: \"); \n            setText(\"Not Connected\"); \n            setFontSize(10); \n            setViewTool(\"State of Connection with Device\"); \n        } \n    } \n    Card{ \n        id: addCard \n        anchors.top: connCard.bottom \n        Component.onCompleted: { \nsetName(\"Device Address: \"); \n            setText(\"01\"); \n            setFontSize(10); \n            setViewTool(\"Destination Address of Transmissions\") \n        } \n    } \n    Card{ \n        id: pathCard \n        anchors.top: addCard.bottom \n        Component.onCompleted: { \n            setName(\"File Path: \"); \n            setText(\"//\"); \n            setFontSize(10); \n            setViewTool(\"Path to the selected file\");\n         } \n    } \n    Card{ \n        id: reportCard \n        anchors.top: pathCard.bottom \n        Component.onCompleted: { \n            setName(\"Operation Status: \"); \n            setText(\"Uninitiated\"); \n            setFontSize(10); \n            setViewTool(\"Status of the current Save or Load operation\") \n        } \n    }  \nCard{ \n    id: writeCard \n    property string message: \"\"\n \n    anchors.top: reportCard.bottom \n\n    Timer{\n         id: writeTimer \n        interval: 1000 \n        running: false\n         repeat: true \n        property int counter: 0 \n        onTriggered: {\n             counter += 1;\n             writeCard.setText(writeCard.message + \" \" + counter + \" sec ago\");\n         }\n     }\n     Component.onCompleted: { \n        setName(\"Write Status: \"); \n        setText(\"\"); \n        setFontSize(10); \n        setViewTool(\"Status of last Write Command\"); \n    } \n}\n} \n"));
        prime_tabbar.insertTabButton(prime_tabbar.createTabButton("import QtQuick 2.7 \nimport QtQuick.Controls 2.2 \nimport QtQuick.Controls.Material 2.1 \nTabButton{ \n       text: qsTr(\"Device\") \n ToolTip.visible: hovered \n ToolTip.text: \"Device\"} "));
    }

    function setPrimeViewCount(index)
    {
        primeViewCount = index;
    }



Page {
    id: primePage
    anchors.left: parent.left
    anchors.top: parent.top
    height: parent.height
    width: parent.width



    function insertPage(page)
    {
        prime_SwipeView.addPage(page);
    }

    function getIndex()
    {
        return prime_tabbar.currentIndex;
    }

    function addCustomTab(qmlString, title)
    {

        prime_SwipeView.addPage(prime_SwipeView.createPage(qmlString));
        prime_tabbar.insertTabButton(prime_tabbar.createTabButton(title));

    }

    function spawnCustomTabs(pageList, titleList, pageCount)
    {
        for (var i = 0; i < pageCount; i++)
        {
            prime_SwipeView.addPage(prime_SwipeView.createPage(pageList[i]));
            prime_tabbar.insertTabButton(prime_tabbar.createTabButton(titleList[i]));
        }
    }


    header: TabBar{
        id: prime_tabbar
        Material.accent: Material.Indigo

        property int childCount: 0

        function insertTabButton(button)
        {
            addItem(button);
            childCount += 1;
            button.visible = true;
        }

        function createTabButton(title)
        {
            var button = Qt.createQmlObject(title, rootNoder, "whatevs.txt");
            rootNoder.data = button;
            return button;
        }

        onCurrentIndexChanged: {
            rootWin.primeViewIndexPing(primeViewCount, prime_tabbar.currentIndex);
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

        onCurrentIndexChanged: {
            prime_tabbar.currentIndex = prime_SwipeView.currentIndex;
        }


    }
}
}
