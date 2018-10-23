import QtQuick 2.0
import com.syberos.basewidgets 2.0
import "../Common"
import "../Common/constants.js" as UI
CommonPage {
    id: videoPlayPage
    statusBarHoldEnabled: true
    statusBarHoldItemColor: "black"
    orientationPolicy: CPageOrientation.LockPortrait
    /*playVideo: 预览播放录音　　addVideo:添加录音*/
    property string actionType: ""
    property string filePath: ""
    property string fileTitle: ""
    property string fileRemark: ""
    property string fileDate: ""
    /** type:var  祖先控件的id*/
    property var parentPage
    contentAreaItem:  Rectangle {
        anchors.fill: parent
        color:"black"
        Image{
            id:recordImage
            width: parent.width
            height: 650
            anchors.top:parent.top
            sourceSize: Qt.size(parent.width, 650)
            source:"image://nemoThumbnail/" + "file://" + videoPlayPage.filePath;
            Item{
                width: 100
                height: 100
                anchors.centerIn: parent
                Image{
                    id:playIcon
                    anchors.fill: parent
                    source:"qrc:/icon/multimedia_vedio.png"
                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            console.log("playIcon 00000000000000000    ");
                            //pageStack.push("qrc:/qml/Multimedia/VideoPlay.qml", {"videoPath" : videoPlayPage.filePath});
                            gApp.openUrl("video://play?fileuri=file://" + videoPlayPage.filePath);
                        }
                    }
                }
            }
        }
        Item{
            id:previewRectTitleAndRemark
            anchors.bottom: parent.bottom
            anchors.top: recordImage.bottom
            anchors.left: parent.left
            anchors.leftMargin: 20
            anchors.right: parent.right
            anchors.rightMargin: 20
            visible: videoPlayPage.actionType == "playVideo"
            Column{
                width: parent.width
                spacing: 5
                Text{
                    id:titleText
                    font.pixelSize: 22
                    color: "white"
                    wrapMode: Text.WrapAnywhere
                    text:videoPlayPage.fileTitle
                }
                Text{
                    id:dateText
                    font.pixelSize: 18
                    color:"#e0e0e0"
                    text:videoPlayPage.fileDate
                }
                Text{
                    id:remarkText
                    font.pixelSize: 18
                    color:"#e0e0e0"
                    width: parent.width
                    wrapMode: Text.WrapAnywhere
                    text:videoPlayPage.fileRemark
                }
            }
        }

        Rectangle{
            id:imgEditAction
            anchors.bottom: parent.bottom
            anchors.top: recordImage.bottom
            width: parent.width
            color: "black"
            visible:videoPlayPage.actionType == "addVideo"
            Item{
                width:lastStepText.width * 1.5
                height: parent.height * 0.8
                anchors.left: parent.left
                anchors.leftMargin: 40
                anchors.verticalCenter: parent.verticalCenter
                Text {
                    id:lastStepText
                    text: "重拍"
                    font.pixelSize: 24
                    color:"#ffffff"
                    anchors.verticalCenter: parent.verticalCenter
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        ensureSubmitDlg.type = "lastStep"
                        ensureSubmitDlg.show();
                    }
                }
            }

            Item{
                width:saveText.width
                height: parent.height * 0.8
                anchors.right: parent.right
                anchors.rightMargin: 40
                anchors.verticalCenter: parent.verticalCenter
                Text {
                    id:saveText
                    text: "使用视频"
                    font.pixelSize: 24
                    color:"#ffffff"
                    anchors.verticalCenter: parent.verticalCenter
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        console.log("save path is  ", videoPlayPage.filePath)
                        pageStack.push("qrc:/qml/Multimedia/MultimediaAddNotePage.qml", {
                                           "filePath":videoPlayPage.filePath,
                                           "multimediaType": "video",
                                           "parentPage":videoPlayPage.parentPage});
                    }
                }
            }
        }
    }

    Connections{
        target: rootWindow
        onKeyBackTriggered:{
            if(videoPlayPage.actionType == "playVideo"){
                pageStack.pop();
            } else if(videoPlayPage.actionType == "addVideo"){
                console.log("fault report   onKeyBackTriggered")
                ensureSubmitDlg.type = "lastStep"
                ensureSubmitDlg.show();
            }
        }
    }

    CDialog{
        id:ensureSubmitDlg
        property string type: ""
        titleText : "提示"
        messageText:"你尚未保存,确定退出吗?"
        acceptedButtonText: "确定"
        rejectButtonText: "取消"
        onAccepted:{
            if(type == "lastStep"){
                if(videoPlayPage.actionType == "addVideo"){
                    var imgList = [];
                    imgList.push(videoPlayPage.filePath);
                    videoPlayPage.parentPage.removeImage(imgList);
                    pageStack.pop();
                }
            }
        }
    }
    onStatusChanged: {
            if (status == CPageStatus.WillShow) {
                rootWindow.dialogShow = true;
                gScreenInfo.setStatusBarStyle("black");
                gScreenInfo.setStatusBar(true)
            } else if(status == CPageStatus.WillHide){
                rootWindow.dialogShow = false;
            }
        }
}
