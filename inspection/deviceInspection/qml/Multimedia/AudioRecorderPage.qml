/*!
* \file DeviceInspectionForm.qml
* \brief 设备巡检提交详情页面
*
*此界面是设备巡检提交详情页面，用户在该页面进行操作，将填报数据提交服务器
*
*\author 桓乐乐 huanlele@syberos.com
*\version 1.0
*\date 2015/9/7
*/
import QtQuick 2.0
import com.syberos.basewidgets 2.0
import "../Common"
import "../Common/constants.js" as UI
CommonPage {
    id: audioRecorderPage
    statusBarHoldEnabled: true
    statusBarHoldItemColor: "black"
    orientationPolicy: CPageOrientation.LockPortrait
    property var parentPage
    property string multimediaType: ""
    property string filePath: ""
    property int recordTotalTime: 0
    onStatusChanged: {
        if (status == CPageStatus.WillShow) {
            gScreenInfo.setStatusBarStyle("black");
            gScreenInfo.setStatusBar(true)
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
            Item{
                id:speakItem
                width: parent.width
                height: 480
                anchors.centerIn: parent
                VolumeLine{
                    id:volumeLine
                    anchors.fill: parent
                }
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
        Item{
            id:buttonItem
            anchors.top: recordBgRect.bottom
            anchors.bottom: parent.bottom
            width: parent.width
            CProgressBar{
                id:recorderProgressBar
                anchors.top:parent.top
                width:parent.width
                value: 0
                minimum: 0
                maximum: audioRecorderPage.recordTotalTime * 1000
            }
            Image{
                width: 100
                height: 100
                anchors.centerIn: parent
                source : recordMouse.pressed ?
                             "qrc:/icon/multimedia_audio_record_action_pressed_icon.png" : "qrc:/icon/multimedia_audio_record_action_icon.png"
                MouseArea{
                    id:recordMouse
                    anchors.fill: parent
                    property bool cancelRecord: false
                    onMouseYChanged: {
                        if(mouseY < -50)
                            recordMouse.cancelRecord = true;
                        else
                            recordMouse.cancelRecord = false;
                    }
                    onPressedChanged: {
                        console.log("onPressed   ", pressed, "   recorderTimer.running  ", recorderTimer.running);
                        if(pressed){
                            multimediaAduioRecorder.startAudioRecorder();
                            recorderTimer.start();
                            volumeLine.startDrawLine();
                        } else {
                            if(recordMouse.cancelRecord){
                                if(recorderTimer.running == true)
                                    multimediaAduioRecorder.cancelAudioRecorder();
                            } else {
                                if(recorderTimer.recordedTime <= 3000){
                                    multimediaAduioRecorder.cancelAudioRecorder();
                                    CTOAST.requestToast("时间太短", "", "");
                                } else
                                    multimediaAduioRecorder.stopAudioRecorder();
                            }
                            audioRecorderPage.reset();
                            recordMouse.cancelRecord = false;
                        }
                    }
                }
            }
        }
    }
    Timer{
        id:recorderTimer
        interval: 100
        running: false
        repeat: true
        property int  recordedTime: 0
        onTriggered: {
            if(recordedTime >= audioRecorderPage.recordTotalTime * 1000){
                if(recordMouse.cancelRecord){
                    multimediaAduioRecorder.cancelAudioRecorder();
                } else {
                    multimediaAduioRecorder.stopAudioRecorder();
                }
                audioRecorderPage.reset();
            } else {
                recordedTime += 100;
                recorderProgressBar.value = recordedTime;
            }
        }
    }

    Connections{
        target: multimediaAduioRecorder
        onAudioRecorderFinished:{
            pageStack.push("qrc:/qml/Multimedia/AudioPlayPage.qml", {
                               "filePath":filePath,
                               "actionType": "addAudio",
                               "parentPage":audioRecorderPage.parentPage});
        }
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
    }
    function reset(){
        recorderTimer.stop();
        recorderTimer.recordedTime = 0;
        recorderProgressBar.value = 0;
        console.log("reset volumeLine.stopDrawLine()")
        volumeLine.stopDrawLine();
    }
}
