import QtQuick 2.0

Item{
    id:root
    width: 80
    height: rectImg.height + tipText.height
    property string imageSource: ""
    property string  name: ""
    signal clicked()
    Rectangle{
        id:rectImg
        width: parent.width
        height: width
        border.color: "#777777"
        radius: 10
        color:mouse.pressed? "#dddddd":"white"
        Image{
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            width: 70
            height: 70
            source: imageSource
        }
        MouseArea{
            id:mouse
            anchors.fill: parent
            onClicked: {
                root.clicked();
            }
        }
    }
    Text{
        id:tipText
        anchors.top: rectImg.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        text:name
    }
    Component.onCompleted: {
        //console.log("rectImg.height is ", rectImg.height , "   tipText height is ", tipText.height)
    }
}

