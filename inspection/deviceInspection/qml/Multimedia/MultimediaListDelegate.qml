import QtQuick 2.0
import "../Common"
import "../Common/constants.js" as UI
Rectangle{
    id: delegateRect
    property string multimediaFilePath: ""
    property string multimediaIcon: ""
    property string multimediaType: ""
    property string multimediaDescription: ""
    property string multimediaTitle: ""
    property string multimediaDate: ""
    property string error :""
    property bool  canDelete: true
    property bool dataOk:true
    color: mousearea.pressed ? UI.LIST_ITEM_BGCOLOR : "white"
    width: parent.width
    height: 100
    signal clicked()
    signal endImageClicked()
    MouseArea{
        id:mousearea
        anchors.fill: parent
        onClicked: {
            if(dataOk)
                delegateRect.clicked();
        }
    }
    Rectangle{
        id:downloadRect
        width: 90
        height: 90
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 20
        visible: !dataOk
        Text {
            id: downloadText
            anchors.centerIn: parent
            font.pixelSize: UI.FONT_SIZE_16
            text:  error == "" ?  "下载中..." : error
        }
    }

    Image {
        id:iconImage
        width: 90
        height: 90
        visible: dataOk
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 20
        sourceSize: Qt.size(parent.width - 20, parent.height - 20)
        smooth: true
        cache:false
        source: multimediaIcon
        Image{
            width: parent.width/2
            height: parent.width/2
            anchors.centerIn: parent
            visible: multimediaType == "video"
            source: "qrc:/icon/multimedia_vedio.png"
        }
    }
    Column{
        anchors.left: iconImage.right
        anchors.leftMargin: 20
        anchors.right: endImageItem.left
        anchors.rightMargin: 20
        anchors.verticalCenter: parent.verticalCenter
        spacing: 10
        Text{
            font.pixelSize: UI.LIST_TITLE_SIZE
            color: UI.LIST_TITLE_COLOR
            width: parent.width
            elide: Text.ElideRight
            text:multimediaTitle
        }
        Text{
            font.pixelSize: UI.LIST_SUBTILE_SIZE
            color:UI.LIST_SUBTITLE_COLOR
            width: parent.width
            elide:Text.ElideRight
            text:multimediaDate
        }
        Text{
            font.pixelSize: UI.LIST_SUBTILE_SIZE
            color:UI.LIST_SUBTITLE_COLOR
            width: parent.width
            elide:Text.ElideRight
            text:multimediaDescription
        }
    }
    Item{
        id:endImageItem
        width: 80
        height: 80
        visible: canDelete && dataOk
        anchors.right: parent.right
        anchors.rightMargin: 5
        anchors.verticalCenter: parent.verticalCenter
        Image{
            anchors.centerIn: parent
            source: "qrc:/icon/multimedia_audio_record_cancel_icon.png"
        }
        MouseArea{
            anchors.fill: parent
            onClicked: {
                delegateRect.endImageClicked();
            }
        }
    }
    Rectangle {
        id: bottomSeparator
        width: parent.width
        height: UI.LIST_SEPARETOR_LINE_HEIGHT
        anchors.bottom: parent.bottom
        color: UI.LIST_SEPARETOR_LINE_COLOR
    }
}
