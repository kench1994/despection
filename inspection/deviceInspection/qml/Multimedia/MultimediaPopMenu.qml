import QtQuick 2.0
import "../Common"
PopDialog{
    id:popDialog
    property string showType: "addPhoto"
    signal clicked(int type)
    MouseArea{
        anchors.bottom: parent.bottom
        width: parent.width
        height: 120
        Rectangle{
            id:menu
            width: parent.width
            height: 120
            Row{
                anchors.centerIn: parent
                spacing: 60
                MultimediaMenuItem{
                    name: "拍照"
                    imageSource: "qrc:/icon/multimedia_addPicture.png"
                    onClicked: {
                        popDialog.hide();
                        console.log("拍照11111111")
                        popDialog.clicked(1);
                    }
                }
                MultimediaMenuItem{
                    name: "视频"
                    imageSource: "qrc:/icon/multimedia_addVideo.png"
                    visible: showType != "associatedPhoto"
                    onClicked: {
                        popDialog.hide();
                        console.log("视频11111111")
                        popDialog.clicked(2);
                    }
                }
                MultimediaMenuItem{
                    name: "录音"
                    imageSource: "qrc:/icon/multimedia_addAudio.png"
                    visible: showType != "associatedPhoto"
                    onClicked: {
                        popDialog.hide();
                        console.log("录音11111111")
                        popDialog.clicked(3);
                    }
                }
            }
        }
    }
}
