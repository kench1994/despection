import QtQuick 2.0
import com.syberos.basewidgets 2.0
import "../Common"
import "../Common/constants.js" as UI
CommonPage {
    id: audioPlayPage
    statusBarHoldEnabled: true
    statusBarHoldItemColor: "black"
    orientationPolicy: CPageOrientation.LockPortrait
    /*playAudio: 预览播放录音　　addAudio:添加录音*/
    property string actionType: ""
    property string filePath: ""
    property string fileTitle: ""
    property string fileRemark: ""
    property string fileDate: ""
    /** type:var  祖先控件的id*/
    property var parentPage

    Connections {
        target: gWindow
        onActiveChanged: {
            console.log("onActiveChanged gWindow.active ", gWindow.active)
            if (!gWindow.active) {
                if(multimediaAduioRecorder.getPlayState() == "playing"){
                    multimediaAduioRecorder.stopPlayAudio();
                }
            }
        }
    }

    contentAreaItem:  Rectangle {
        anchors.fill: parent
        color:"black"
        Rectangle{
            id:recordBgRect
            width: parent.width
            height: 650
            anchors.top: parent.top
            color:"#000033"
            VolumeLine{
                id:volumeLine
                anchors.centerIn: parent
            }
            Image{
                id:playIcon
                width: 100
                height: 100
                anchors.centerIn: parent
                visible: audioPlayPage.actionType == "playAudio"
                source:"qrc:/icon/multimedia_audio_play_start1_icon.png"
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        console.log("playIcon 00000000000000000    ", multimediaAduioRecorder.getPlayState());
                        if(multimediaAduioRecorder.getPlayState() != "playing"){
                            multimediaAduioRecorder.startPlayAudio(audioPlayPage.filePath);
                            volumeLine.startDrawLine();
                            playIcon.visible = false;
                        } else {
                            multimediaAduioRecorder.stopPlayAudio();
                        }
                    }
                }
            }
        }
        Item{
            id:previewRectTitleAndRemark
            anchors.bottom: parent.bottom
            anchors.top: recordBgRect.bottom
            anchors.left: parent.left
            anchors.leftMargin: 20
            anchors.right: parent.right
            anchors.rightMargin: 20
            visible: audioPlayPage.actionType == "playAudio"
            Column{
                width: parent.width
                spacing: 5
                Text{
                    id:titleText
                    font.pixelSize: 22
                    color: "white"
                    wrapMode: Text.WrapAnywhere
                    text:audioPlayPage.fileTitle
                }
                Text{
                    id:dateText
                    font.pixelSize: 18
                    color:"#e0e0e0"
                    text:audioPlayPage.fileDate
                }
                Text{
                    id:remarkText
                    font.pixelSize: 18
                    color:"#e0e0e0"
                    width: parent.width
                    wrapMode: Text.WrapAnywhere
                    text:audioPlayPage.fileRemark
                }
            }
        }

        Item{
            id:imgEditAction
            anchors.bottom: parent.bottom
            anchors.top: recordBgRect.bottom
            width: parent.width
            visible:audioPlayPage.actionType == "addAudio"
            Item{
                width:lastStepText.width * 1.5
                height: parent.height * 0.8
                anchors.left: parent.left
                anchors.leftMargin: 40
                anchors.verticalCenter: parent.verticalCenter
                Text {
                    id:lastStepText
                    text: "重录"
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

            Image{
                id:startPlayIcon
                width: 34
                height: 34
                anchors.centerIn: parent
                source:"qrc:/icon/multimedia_audio_play_start_icon.png"
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        console.log("playIcon 00000000000000000    ", multimediaAduioRecorder.getPlayState());
                        if(multimediaAduioRecorder.getPlayState() != "playing"){
                            multimediaAduioRecorder.startPlayAudio(audioPlayPage.filePath);
                            volumeLine.startDrawLine();
                        } else {
                            multimediaAduioRecorder.stopPlayAudio();
                        }
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
                    text: "使用录音"
                    font.pixelSize: 24
                    color:"#ffffff"
                    anchors.verticalCenter: parent.verticalCenter
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        console.log("save path is  ", audioPlayPage.filePath)
                        pageStack.push("qrc:/qml/Multimedia/MultimediaAddNotePage.qml", {
                                           "filePath":audioPlayPage.filePath,
                                           "multimediaType": "audio",
                                           "parentPage":audioPlayPage.parentPage});
                    }
                }
            }
        }
    }
    Connections{
        target: multimediaAduioRecorder
        onVolumeChanged:{
            var soundVolume = Math.floor(volume * 1000);
            if(soundVolume == 0)
                volumeLine.setVolume(0);
            else{
                if(soundVolume == 1)
                    soundVolume = 1.5;
                volumeLine.setVolume(Math.floor(Math.log(soundVolume * soundVolume * soundVolume  * soundVolume) * 8.6));
            }
        }
        onSoundPlayStateChanged:{
            if(state == "playing"){
                startPlayIcon.source = "qrc:/icon/multimedia_audio_play_stop_icon.png";
            } else {
                volumeLine.stopDrawLine();
                startPlayIcon.source = "qrc:/icon/multimedia_audio_play_start_icon.png";
                if(audioPlayPage.actionType == "playAudio"){
                    playIcon.visible = true;
                }
            }
        }
    }
    Connections{
        target: rootWindow
        onKeyBackTriggered:{
            if(audioPlayPage.actionType == "playAudio"){
                pageStack.pop();
            } else if(audioPlayPage.actionType == "addAudio"){
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
                if(audioPlayPage.actionType == "addAudio"){
                    var imgList = [];
                    imgList.push(audioPlayPage.filePath);
                    audioPlayPage.parentPage.removeImage(imgList);
                    pageStack.pop();
                }
            }
        }
    }

    onStatusChanged: {
        if (status == CPageStatus.Show) {
            gScreenInfo.setStatusBarStyle("black");
            gScreenInfo.setStatusBar(true)
            rootWindow.dialogShow = true;
        } else if (status == CPageStatus.WillHide){
            rootWindow.dialogShow = false;
            if(multimediaAduioRecorder.getPlayState() == "playing"){
                multimediaAduioRecorder.stopPlayAudio();
            }
        }
    }
}
