import QtQuick 2.0

Item{
   width: parent.width
   height: deviceName.height + 20
   property string name: ""
   Text {
       id: deviceName
       anchors.left: parent.left
       anchors.leftMargin: 20
       anchors.right: parent.right
       anchors.rightMargin: 40
       //anchors.verticalCenter: parent.verticalCenter
       anchors.bottom: parent.bottom
       //anchors.bottomMargin: 5
       wrapMode: Text.WrapAnywhere
       font.pixelSize: 20
       text: name
   }
   Component.onDestruction: {
       console.log("HeadTitle   destruction")
   }
}
