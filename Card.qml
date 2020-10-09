import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.1

Page{

    function setName(name)
    {
        tag.text = name;
    }

    function setText(texting)
    {
        view.text = texting;
    }

    function setFontSize(x)
    {
        tag.font.pointSize = x;
        view.font.pointSize = x;
    }

    function setViewTool(tip)
    {
        view.ToolTip.text = tip;
    }

    function toggleEnable(enableValue)
    {
        view.enabled = enableValue;
    }

    height: ((parent.height * .2) > 60) ? 60 : (parent.height * .2)
    width: parent.width

    anchors.left: parent.left
    anchors.topMargin: 5

    Label{
        id: tag
        width: parent.width / 2 - 10
        height: parent.height - 10
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 10

        textFormat: Text.RichText

    }
    TextArea{
        id: view

        width: 2 * (parent.width) / 4 - 5
        height: parent.height

        anchors.left: tag.right
        anchors.top: parent.top
        text: ""
        textFormat: Text.RichText

        ToolTip.visible: hovered

    }
}
