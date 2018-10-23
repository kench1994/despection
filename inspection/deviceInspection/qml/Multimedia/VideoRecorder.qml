import	QtQuick	2.0
import	com.syberos.basewidgets	2.0
import	QtMultimedia	5.0
import "../Common"
CommonPage{
    id:videoRecordPage
    width:parent.width
    height:parent.height
    property var  parentPage
    signal videoRecordFinished(string filePath)
    property string videoFilePath: ""
    property int  recordTotalTime: 0
    property bool lockScreen: false
    property bool firstPushedIn: true
    statusBarHoldEnabled: true
    statusBarHoldItemColor: "black"
    Connections {
        target: gWindow
        onActiveChanged: {
            console.log("onActiveChanged gWindow.active ", gWindow.active, " videoRecordPage.status is  ", videoRecordPage.status)
            if(videoRecordPage.status != CPageStatus.Show)
                return;
            console.log("videoRecordPage.status == CPageStatus.Show")
            if (gWindow.active) {
                __camera.start();
            } else{
                __camera.stop();
            }
        }
    }

    //锁屏时 退出
//    Connections{
//        target:inspectionSystemConfig
//        onDisplayStateChanged:{
//            // 0 on  1 off 2 begin off
//            if(Number(displayState) == 2){
//                console.log("VideoRecorder.qml  displayState Changed   recorderTimer.running  ", recorderTimer.running)
//                videoRecordPage.lockScreen = true;
//            } else if(Number(displayState) == 0){
//                videoRecordPage.lockScreen = false;
//            }
//        }
//    }
    contentAreaItem:Item{
        anchors.fill: parent
        VideoOutput	{
            id:videoOutput
            width:	parent.width
            height:700
            fillMode:	VideoOutput.PreserveAspectCrop
            orientation:-90
            source:__camera
            Rectangle{
                id:recordActionRect
                width: recordActionText.width + 20
                height: recordActionText.height + 10
                anchors.horizontalCenter: videoOutput.horizontalCenter
                anchors.bottom: videoOutput.bottom
                anchors.bottomMargin: 40
                color:pressMouse.cancelRecord?  "red" : "grey"
                radius: 10
                opacity: 0.7
                visible: pressMouse.pressed
                Text{
                    id:recordActionText
                    anchors.centerIn: parent
                    color:"white"
                    text:pressMouse.cancelRecord? "松开取消" : "上滑取消"
                }
            }
        }
        Camera	{
            id:	__camera
            focus.focusMode:	Camera.FocusContinuous
            onCameraStateChanged: {
                console.log("onCameraStateChanged[camera] camera state changed to: " + cameraState )
                if (cameraState == Camera.ActiveState) {
                    console.log("onCameraStateChanged[camera] Activated.")
                }
                if (cameraState == Camera.UnloadedState) {
                    console.log("onCameraStateChanged[camera] Deactivated.")
                }
            }

            onError: {
                console.log("onError[camera] camera error occur: [" + errorCode + "] " +  errorString)
            }

            onCameraStatusChanged: {
                console.log("onCameraStatusChanged[camera] camera status changed to: " + cameraStatus)
                if (cameraStatus == Camera.ActiveStatus) {
                    console.log("onCameraStatusChanged[camera] camera status : Activated.")
                    //recorderImg.enabled = true;
                    enableRecorderTm.start();
                }else
                    recorderImg.enabled = false;
            }

            onLockStatusChanged:	{
                console.log("onLockStatusChanged[camera]	lounck	status	changed:	",	lockStatus)
                if	(lockStatus	==	Camera.Locked)	{
                    console.log("onLockStatusChanged[camera]	focus	locked")
                }	else	if	(lockStatus	==	Camera.Unlocked)	{
                    console.log("onLockStatusChanged[camera]	unlocked")
                }	else	if	(lockStatus	==	Camera.Searching)	{
                    console.log("onLockStatusChanged[camera]	focusing...")
                }
            }
            videoRecorder {
                onRecorderStateChanged:  {
                    console.log("onRecorderStateChanged[camera][videorecorder] update: state changed to : " + __camera.videoRecorder.recorderState)
                }
                onRecorderStatusChanged: {
                    console.log("onRecorderStatusChanged[camera][videorecorder] update: status changed to : " + __camera.videoRecorder.recorderStatus)
                    // 2 -> 3 -> 5 -> 7 -> 2
                    if (__camera.videoRecorder.recorderStatus === CameraRecorder.FinalizingStatus) {
                       console.log("__camera.videoRecorder.recorderStatus === CameraRecorder.FinalizingStatus")
                    }
                }
                onError: {
                    console.log("[camera][videorecorder] error code: " + errorCode + " : " + errorString)
                }
            }
            Component.onCompleted: {
                __camera.stop();
            }
        }

        Rectangle{
            id:pressBtn
            width: parent.width
            anchors.top: videoOutput.bottom
            anchors.bottom: parent.bottom
            color:"black"
            CProgressBar{
                id:recorderProgressBar
                anchors.top:parent.top
                width:parent.width
                value: 0
                minimum: 0
                maximum: videoRecordPage.recordTotalTime * 1000
            }
            Rectangle{
                id:recorderBgRect
                anchors.centerIn: parent
                width: 80
                height: 80
                radius: 40
                color: "grey"
                visible: !recorderImg.enabled
            }

            Image{
                id:recorderImg
                width: 80
                height: 80
                enabled: false
                anchors.centerIn: parent
                source: pressMouse.pressed?
                            "qrc:/icon/multimedia_video_record_action_pressed_icon.png" : "qrc:/icon/multimedia_video_record_action_icon.png"
                MouseArea {
                    id:pressMouse
                    anchors.fill: parent
                    property bool  cancelRecord: false
                    onMouseYChanged: {
                        if(mouseY < -50)
                            pressMouse.cancelRecord = true;
                        else
                            pressMouse.cancelRecord = false;
                    }
                    onPressedChanged: {
                        console.log("onPressed   ", pressed)
                        if(pressed) {
                            if (__camera.captureMode != Camera.CaptureVideo) {
                                __camera.captureMode = Camera.CaptureVideo
                            }
                            recorderTimer.start();
                            videoRecordPage.videoFilePath = multimediaVideoRecorder.createVideoSavePath();
                            console.log("videoRecordPage.videoFilePath   ", videoRecordPage.videoFilePath);
                            __camera.videoRecorder.outputLocation = "file://" + videoRecordPage.videoFilePath ;
                            __camera.videoRecorder.record();												//开始录制视频
                        } else {
                            if(pressMouse.cancelRecord){
                                //防止用户连点
                                recorderImg.enabled = false;
                                enableRecorderTm.start();
                                if(recorderTimer.running == true){
                                    __camera.videoRecorder.stop();
                                    multimediaVideoRecorder.deleteVideoFile();
                                }
                            } else {
                                __camera.videoRecorder.stop();
                                if(recorderTimer.recordedTime <= 3000){
                                    //防止用户连点
                                    recorderImg.enabled = false;
                                    enableRecorderTm.start();
                                    multimediaVideoRecorder.deleteVideoFile();
                                    CTOAST.requestToast("时间太短", "", "");
                                } else if(videoRecordPage.lockScreen){
                                    multimediaVideoRecorder.deleteVideoFile();
                                } else {
                                    pageStack.push("qrc:/qml/Multimedia/VideoPlayPage.qml", {
                                                       "filePath":videoRecordPage.videoFilePath,
                                                       "actionType": "addVideo",
                                                       "parentPage":videoRecordPage.parentPage});
                                }
                            }
                            videoRecordPage.reset();
                        }
                    }
                }
            }
        }//press rectangle
    }

    //防止用户连点定时器
    Timer{
        id:enableRecorderTm
        interval: 500
        running: false
        repeat: false
        onTriggered: {
            console.log("enableRecorderTm  __camera.cameraStatus is ", __camera.cameraStatus);
            if(__camera.cameraStatus == Camera.ActiveStatus && videoRecordPage.status == CPageStatus.Show)
                recorderImg.enabled = true;
        }
    }

    Timer{
        id:recorderTimer
        interval: 100
        running: false
        repeat: true
        property int  recordedTime: 0
        onTriggered: {
            if(recordedTime >= videoRecordPage.recordTotalTime * 1000) {
                __camera.videoRecorder.stop();
                if(pressMouse.cancelRecord){
                    console.log("_camera.videoRecorder.stop()   end    " )
                    multimediaVideoRecorder.deleteVideoFile();
                } else {
                    console.log("recorderTimer  ok  push videoPlayPage.qml")
                    pageStack.push("qrc:/qml/Multimedia/VideoPlayPage.qml", {
                                       "filePath":videoRecordPage.videoFilePath,
                                       "actionType": "addVideo",
                                       "parentPage":videoRecordPage.parentPage});
                }
                videoRecordPage.reset();
            } else {
                recordedTime += 100;
                recorderProgressBar.value = recordedTime;
            }
        }
    }

    function reset(){
        pressMouse.cancelRecord = false;
        recorderTimer.stop();
        recorderTimer.recordedTime = 0;
        recorderProgressBar.value = 0;
    }

    onStatusChanged:{
        if(status == CPageStatus.WillHide){
            __camera.stop();
            console.log("VideoRecorder.qml  onStatusChanged   CPageStatus.WillHide ")
        } else if(status == CPageStatus.Show){
            gScreenInfo.setStatusBarStyle("black");
            gScreenInfo.setStatusBar(true)
            __camera.start();
            console.log("VideoRecorder.qml  onStatusChanged   CPageStatus.Show ")
        }
    }
}
