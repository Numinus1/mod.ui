import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.1
import QtQuick.Window 2.1
import QtQuick.Dialogs 1.0

Item{

    id: rootNoder

    function showFileDialog(x){
        console.log("showFileDialog: begun");
        fileDialog.intent = x;

        if (x === 0){
            fileDialog.title = "Save Config File";
            fileDialog.selectExisting = false;
        }
        else if (x === 1){
            fileDialog.title = "Load Config File";
            fileDialog.selectExisting = true;
        }
        else if (x === 2){
            fileDialog.title = "Choose Tab Setup File";
            fileDialog.selectExisting = true;
        }

        fileDialog.visible = true;
    }

    ApplicationWindow {
        id: rootWin
        visible: true
        width: 2 * Screen.width/5
        height: 4 * Screen.height/5 + 100
        title: qsTr("Mod.UI v1.2 (Stable)")

        Component.onCompleted: {
            setX(0);
            setY(35);
        }

        function primeViewIndexPing(count, index)
        {
            if (count == 0)
            {
                //auxSwipeView.currentIndex = ((index == 0) ? 0 : 1);
            }
        }

        Connections{
            target: ark

            onSpawnStableTabs: {
                stableView.addTabs(pageList, titleList, pageCount);
            }

            onSpawnMutableTabs: {
                mutableView.addTabs(pageList, titleList, pageCount);
            }

            onSpawnButtonPanel: {
                flagView.addPanel(qmlString);
            }

        }

        header: ToolBar{
            Material.elevation: 5

            ToolButton{
                id:tabfileButt
                text: qsTr("Load Setup")

                ToolTip.visible: hovered;
                ToolTip.text: qsTr("Load csv Setup File");




                onClicked: {
                    rootNoder.showFileDialog(2);
                }
            }

            ToolButton{
                id: cleanButt
                text: qsTr("Clean")

                ToolTip.visible: hovered;
                ToolTip.text: qsTr("Removes all panels added by setup files");

                anchors.left: tabfileButt.right
                anchors.leftMargin: 5

                onClicked: {
                    ark.promptClean();
                }

            }

            ToolButton{
                id: manualButt
                text: qsTr("Manual-Connection");

                ToolTip.visible: hovered;
                ToolTip.text: qsTr("Manually select an open a COM Port");

                anchors.left: cleanButt.right
                anchors.leftMargin: 5

                onClicked: {
                    ark.refreshPorts();
                    comwin.visibility = 1;
                }
            }
            ToolButton{
                id:refreshButt
                text: qsTr("Refresh")

                ToolTip.visible: hovered;
                ToolTip.text: qsTr("Refresh Static Parameters");

                anchors.left: manualButt.right
                anchors.leftMargin: 5

                onClicked: {
                    ark.refresh_statics();
                    background.color = "yellow";
                }
            }

            ToolButton{
                id: writeButt
                text: qsTr("Write")

                ToolTip.visible: hovered;
                ToolTip.text: qsTr("Write user-entered values to Device");

                anchors.left: refreshButt.right
                anchors.leftMargin: 5

                onClicked: {
                    ark.promptWrite();
                }
            }
            /*
            ToolButton{
                id: testButt
                text: qsTr("Test")

                anchors.left: writeButt.right
                anchors.leftMargin: 5

                onClicked: {
                    ark.fauxWrite();
                }
            }*/

        }


        PrimeView {
            id: stableView
            height: parent.height * .5
            width: 3 * parent.width / 4
            anchors.left: parent.left
            anchors.top: parent.top

            Component.onCompleted: {
                stableView.insertCommPage();
                stableView.setPrimeViewCount(0);
            }

        }

        Rectangle{
            id: separator

            height: parent.height
            width: 5

            anchors.left: stableView.right
            anchors.top: parent.top
            color: "#9E9E9E"
        }

        Page{
            id: uprightPage
            height: parent.height * .5
            width: parent.width / 4
            anchors.left: separator.right
            anchors.top: parent.top

            header: TabBar{

                id: auxTabBar
                Material.accent: Material.Indigo

                TabButton{
                    text: qsTr("Device")
                }
            }

            SwipeView{
                id: auxSwipeView
                anchors.fill: parent
                currentIndex: 0//mainTabBar.currentIndex
                clip: true

                DriveBPage{
                    id:driveBPage
                }

                ParamBPage{
                    id:paramPage
                }

            }
        }

//        InertiaBPage{
//            id:flagView
//            height: parent.height * .5
//            width: parent.width / 4
//            anchors.left: separator.right
//            anchors.top: uprightPage.bottom
//        }

        FlagView{
            id: flagView
            height: parent.height * .5
            width: parent.width / 4
            anchors.left: separator.right
            anchors.top: uprightPage.bottom

        }

        PrimeView{
            id: mutableView

            height: parent.height * .5
            width: 3 * parent.width / 4

            anchors.bottom: parent.bottom
            anchors.left: parent.left

            Component.onCompleted: {
                mutableView.setPrimeViewCount(1);
            }


        }

       /* Page{
            id: botPage

            height: parent.height * .5
            width: parent.width
            anchors.bottom: parent.bottom
            anchors.left: parent.left

            header: TabBar{
                id: pollTabBar
                Material.accent: Material.Indigo

                TabButton{
                    text: qsTr("Operating Parameters")
                }
            }

            PollPage{
                anchors.fill: parent
            }

        }*/
    }

    FileDialog {
        id: fileDialog
        title: "Please choose a file"
        folder: "///C:/Users/ASUS/Documents/FrontEndSetups/OldFrontEnd"
        visible: false
        nameFilters: [ "Comma Separated Values (*.csv)", "All files (*)" ]

        //0 -> save config file; 1 -> load config file;
        //2 -> load setup file
        property int intent: -1

        onAccepted: {
            if (fileDialog.intent === 0){
                ark.saveConfigFile(fileDialog.fileUrl);
            }
            else if (fileDialog.intent === 1){
                ark.loadConfigFile(fileDialog.fileUrl);
            }
            else if (fileDialog.intent === 2){
                ark.altloadSetupFile(fileDialog.fileUrl);
            }

            //ark.loadSetupFile(fileDialog.fileUrl);
            //console.log("fileDialog - You chose: " + fileDialog.fileUrl);
            fileDialog.visible = false;
        }
        onRejected: {
            fileDialog.visible = false;
            intent = -1;
        }
    }

    Window {
        id:comwin
        visible: false
        title: "Choose COM Port"

        width: 450
        height: 300

        x: 100
        y: 100

        GridLayout{
            id: sergrid1
            columns: 2

            height: parent.height
            width: parent.width / 2

            anchors.left: parent.left
            anchors.top: parent.top
            anchors.margins: 10

            Connections{
                target:ark
                onPortList:
                {
                    console.log("onPortList:");
                    commodel.clear();
                    var i;
                    for (i = 0; i < size; i++)
                    {
                        console.log(list[i]);
                        commodel.append({text: list[i]});

                    }
                    if (size > 0)
                    {
                        combox.currentIndex = 0;
                    }
                }
            }

            Connections{
                target: ark
                onManReport:
                {
                    if (result){
                        comwin.visible = 0;
                        closeportbtn.enabled = true;
                        openportbtn.enabled = false;
                        comstatus.text = "Status: " + pname + " open";
                    }
                    else{
                        closeportbtn.enabled = false;
                        openportbtn.enabled = true;
                        comstatus.text = "Status: " + pname + " failed to open";
                    }
                }

                onManClosed:
                {
                    if (!result){
                        comstatus.text = "Error Closing Port";
                    }
                }
            }

            Label{
                text: "COM Ports:"
            }

            ComboBox{
                id: combox
                editable: true
                model: ListModel{
                    id: commodel
                }

            }
            Label{
                text: "Baudrate:"
            }
            ComboBox{
                id: baudbox
                model: ["9600"]
            }
            Label{
                text: "Parity:"
            }
            ComboBox{
                id: paritybox
                model: ["No Parity", "Even Parity", "Odd Parity"]

            }
            Label{
                text: "Stop Bits:"

            }
            ComboBox{
                id: stopbox
                model: ["1", "2"]

            }
        }

        Column{
            id: sergrid2

            height: parent.height / 2
            width: parent.width / 2 - 20

            anchors.left: sergrid1.right
            anchors.top: parent.top
            anchors.margins: 10

            Button{
                id: refportbtn
                text: qsTr("Refresh Ports")
                onClicked: {
                    ark.refreshPorts();
                }
            }

            Button{
                id: closeserwin
                text: qsTr("Close        ")
                onClicked: {
                    comwin.visibility = 0;
                }
            }

            Button{
                id: openportbtn
                text: qsTr("Open Port    ")
                onClicked:{
                    if (combox.currentIndex != -1){

                    ark.openCOMPort(combox.currentText,
                                     baudbox.currentText,
                                     paritybox.currentText,
                                     stopbox.currentText
                                     );
                    }
                    else{
                        comstatus.text = "No COM Port Selected";
                    }
                }
            }

            Button{
                id: closeportbtn
                text: qsTr("Close Port   ")
                enabled: false
                onClicked: {
                    openportbtn.enabled = true;
                    closeportbtn.enabled = false;
                    comstatus.text = "Status: Closed";
                    ark.closeCOMPort();
                }
            }

            Rectangle{
                color: "white"
                opacity: 0
                width: parent.width
                height: 70
            }

            Label{
                id: comstatus
                font.bold: true
                text: qsTr("Status: Closed")
            }
        }
    }
}
