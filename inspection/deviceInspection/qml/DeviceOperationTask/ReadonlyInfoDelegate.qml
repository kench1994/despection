import QtQuick 2.0
import com.syberos.basewidgets 2.0
import "../Common"
import "../Common/constants.js" as UI
Rectangle{
    id :detaildelegate
    width :parent.width
    height: Math.max(label.height, contentText.height) + 5
    Text {
        id:label
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin:40
        font.pixelSize: UI.FONT_SIZE_16
        color:"#999999"
        text:name
    }
    Text {
        id: contentText
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.left: label.right
        anchors.rightMargin: 20
        anchors.leftMargin: 20
        wrapMode: Text.WrapAnywhere
        font.pixelSize: UI.FONT_SIZE_16
        color:"#999999"
        text: value
    }
    CLine {
        width:parent.width
    }
}
