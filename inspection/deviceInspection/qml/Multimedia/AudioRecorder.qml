import QtQuick 2.0
import "../Common"
PopDialog{
    id:popDialog
    /*recorder： 录音； play：播放录音*/
    property string  actionType: "recorder"
    property string audioFilePath: ""
    property var parentPage
    property int  maxVolume: 0
    signal audioRecorderFinished(string audioFilePath)
    MouseArea{
        id: recordMouse
        anchors.bottom: parent.bottom
        width: parent.width * 0.6
        height: width * 1.5
        anchors.centerIn: parent
        visible: actionType == "recorder"
        property bool cancelRecord: false
            Item{
                id:audioIconRect
                width: parent.width
                height: parent.height * 0.8
                Image{
                    id:phoneBg
                    anchors.fill: parent
                    source: "qrc:/icon/multimedia_audio_record_interphone_bg.png"
                }
                Item{
                    id:speakItem
                    width: parent.width
                    height: phone.height + 20
                    anchors.top: parent.top
                    anchors.topMargin: 40
                    visible: !recordMouse.cancelRecord
                    VolumeLine{
                        id:volumeLine
                        anchors.fill: parent
                    }
                }
                Image{
                    id:cancelIcon
                    visible: recordMouse.cancelRecord
                    anchors.centerIn: parent
                    source: "qrc:/icon/multimedia_audio_record_cancel_icon.png"
                }
                Rectangle{
                    id:speackActionBg
                    color: "red"
                    visible: recordMouse.cancelRecord
                    width: speakActionTxt.width + 30
                    height: speakActionTxt.height +20
                    anchors.centerIn: speakActionTxt
                }
                Text{
                    id:speakActionTxt
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 50
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: "white"
                    text:  recordMouse.cancelRecord ?  "松开手指，取消发送" : "手指上滑，取消发送"
                }
            }
            Rectangle{
                width: parent.width
                height: parent.height * 0.2
                anchors.top:audioIconRect.bottom
                color: speakMouse.pressed ? "blue" : "white"
                Text{
                    text:"按住说话"
                    anchors.centerIn: parent
                }
                MouseArea{
                    id:speakMouse
                    anchors.fill: parent
                    onMouseYChanged: {
                        if(mouseY < -50)
                            recordMouse.cancelRecord = true;
                        else
                            recordMouse.cancelRecord = false;
                    }
                    onPressedChanged: {
                        console.log("onPressed   ", pressed)
                        if(pressed)
                            multimediaAduioRecorder.startAudioRecorder();
                        else {
                            if(recordMouse.cancelRecord){
                                multimediaAduioRecorder.cancelAudioRecorder();
                            } else {
                                multimediaAduioRecorder.stopAudioRecorder();
                            }
                            popDialog.reset();
                        }
                    }
                }
            }
    } //recorder

    MouseArea{
        id:playMouse
        height: 100
        width: parent.width
        anchors.centerIn: parent
        visible: actionType == "play"
        Rectangle{
            anchors.fill: parent
            Image{
                id:audioSpeak
                width: 80
                height: 80
                source: "qrc:/icon/multimedia_audio_speak.png"
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 20
            }

            Text{
                id:audioName
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: audioSpeak.right
                anchors.leftMargin: 10
                anchors.right: parent.right
                anchors.rightMargin: 20
                text:playMouse.getFileName(audioFilePath)
                wrapMode: Text.WrapAnywhere
            }
        }
        onClicked: {
            multimediaAduioRecorder.startPlayAudio(audioFilePath);
        }
        function getFileName(filePath){
            var pos = String(filePath).lastIndexOf("/");
            var fileName = String(filePath).substring(pos + 1);
            return fileName;
        }
    } //play

    Connections{
        target: multimediaAduioRecorder
        onAudioRecorderFinished:{
            parentPage.saveMultimediaFile(filePath, "audio", "description")
            console.log("filePath is ", filePath, " result is ", result, "  description is ", description, " maxVolume is ", popDialog.maxVolume);
            popDialog.hide();
        }
        onVolumeChanged:{
            var soundVolume = Math.floor(volume * 1000);
            if(soundVolume > popDialog.maxVolume)
                popDialog.maxVolume = soundVolume;
        }
    }
    onCanceled: {
        if(popDialog.actionType == "play"){
            multimediaAduioRecorder.stopPlayAudio();
        }
    }

    function reset(){
        recordMouse.cancelRecord = false;
        volumeIcon.source = "qrc:/icon/multimedia_audio_record_decibel1.png";
    }
}
