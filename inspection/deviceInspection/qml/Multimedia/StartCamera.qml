/*!
* \file StartCamera.qml
* \brief 调用相机拍照页面
*
*此界面调用相机拍照页面，并保存图片
*
*\author 桓乐乐 huanlele@syberos.com
*\version 1.0
*\date 2015/9/7
*/
import QtQuick 2.0
import QtMultimedia 5.0
import com.syberos.basewidgets 2.0
import "../Common"
/*!
  \class StartCamera
  \inModule 公共控件
  \brief 调用相机拍照页面
  \ingroup common
  此界面调用相机拍照页面，并保存图片
*/
CommonPage {
    id: camearImagPage
    /** type:var 父控件id，以便子控件能够找到父控件 */
    property var parentPage
    /*设备关联照片，*/
    property string actionType: ""

    Connections {
        target: gWindow
        onActiveChanged: {
            console.log("startCamera onActiveChanged gWindow.active ", gWindow.active, "   camearImagPage.status  is ", camearImagPage.status)
            if(camearImagPage.status != CPageStatus.Show)
                return;
            if (gWindow.active) {
                QtCamera.start()
            } else{
                QtCamera.stopCamera();
            }
        }
    }

    contentAreaItem:Item {
        anchors.fill: parent
        Item{
            id:showCameraItem
            anchors.fill: parent
            enabled: false
            Timer{
                id:startCameraTime
                interval: 500
                running: false
                repeat: false
                onTriggered: {
                    console.log("camera start   status is  ", QtCamera.status)
                    QtCamera.start();
                }
            }
            Item{
                id:cameraItem
                anchors.fill: parent
                Connections{
                    target: QtCamera
                    onSavedImage:{
                        console.log("onImageCaptublack  id  ", id, "  filename  ", fileName, "actionType is ", camearImagPage.actionType)
                        pageStack.push("qrc:/qml/Multimedia/EditImage.qml", {
                                           "editImagePath": fileName,
                                           "actionType": camearImagPage.actionType,
                                           "parentPage":camearImagPage.parentPage});
                    }
                    onStatusChanged:{
                        console.log("onStatusChanged 666666   ", QtCamera.status);
                        //QCamera::ActiveStatus	8	The camera has been started and can produce data.
                        //                          The viewfinder displays video frames in active state.
                        if(QtCamera.status.toString() == "8"){
                            showCameraItem.enabled = true;
                            rootWindow.dialogShow = false;
                            var flashMode = QtCamera.getCameraFlashMode();
                            console.log("camera.getCameraFlashMode(): flashMode = ", flashMode);
                            switch(flashMode) {
                            case "flashOn" : { shanguangOn.visible = true; break; }
                            case "flashAuto" : { shanguangAuto.visible = true; break; }
                            case "flashOff" : { shanguangOff.visible = true; break; }
                            default : { shanguangOn.visible = true; }
                            }
                        }else {
                            showCameraItem.enabled = false;
                            rootWindow.dialogShow = true;
                        }
                    }
                }
            }

            Rectangle{
                id:bgRect
                anchors.top:parent.top
                height: 704
                width: parent.width
                color:"black"
                Item{
                    id: shanguangItem
                    property bool isWaitSelect: true
                    width: 50
                    height: 190
                    anchors.top: parent.top
                    anchors.topMargin: 30
                    anchors.left: parent.left
                    anchors.leftMargin: 20
                    z:100

                    Image {
                        id: shanguangOn
                        width: 50
                        height: 50
                        anchors.top: parent.top
                        source: "qrc:/icon/SHANGUANG_03.png"
                        visible: false
                        Behavior on visible {
                            NumberAnimation { duration: 200}
                        }

                        MouseArea{
                            anchors.fill: parent
                            onClicked: {
                                if(!shanguangItem.isWaitSelect) {
                                    shanguangAuto.visible = false;
                                    shanguangOff.visible = false;
                                    console.log("shanguang on")
                                    QtCamera.setCameraFlashMode("flashOn");
                                } else {
                                    shanguangAuto.visible = true;
                                    shanguangOff.visible = true;
                                }
                                shanguangItem.isWaitSelect = !shanguangItem.isWaitSelect;
                            }
                        }
                    }
                    Image {
                        id: shanguangAuto
                        width: 50
                        height: 50
                        anchors.top: parent.top
                        anchors.topMargin: shanguangItem.isWaitSelect ? 0: 70
                        Behavior on anchors.topMargin {
                            NumberAnimation { duration: 200; easing.type: "Linear" }
                        }

                        source: "qrc:/icon/SHANGUANG_18.png"
                        visible: false
                        Behavior on visible {
                            NumberAnimation { duration: 200}
                        }

                        MouseArea{
                            anchors.fill: parent
                            onClicked: {
                                if(!shanguangItem.isWaitSelect)
                                {
                                    shanguangOn.visible = false;
                                    shanguangOff.visible = false;
                                    console.log("shanguang auto");
                                    QtCamera.setCameraFlashMode("flashAuto");
                                }
                                else
                                {
                                    shanguangOn.visible = true;
                                    shanguangOff.visible = true;
                                }

                                shanguangItem.isWaitSelect = !shanguangItem.isWaitSelect;
                            }
                        }
                    }
                    Image {
                        id: shanguangOff
                        width: 50
                        height: 50
                        anchors.top: parent.top
                        anchors.topMargin: shanguangItem.isWaitSelect? 0 : 140
                        Behavior on anchors.topMargin {
                            NumberAnimation { duration: 200; easing.type: "Linear" }
                        }

                        source: "qrc:/icon/SHANGUANG_24.png"
                        visible: false
                        Behavior on visible {
                            NumberAnimation { duration: 200}
                        }

                        MouseArea{
                            anchors.fill: parent
                            onClicked: {
                                if(!shanguangItem.isWaitSelect){
                                    shanguangOn.visible = false;
                                    shanguangAuto.visible = false;
                                    console.log("shanguang off")
                                    QtCamera.setCameraFlashMode("flashOff");
                                } else {
                                    shanguangOn.visible = true;
                                    shanguangAuto.visible = true;
                                }
                                shanguangItem.isWaitSelect = !shanguangItem.isWaitSelect;
                            }
                        }
                    }
                }

                VideoOutput {
                    id:videopt
                    anchors.top: parent.top
                    anchors.topMargin: 90
                    anchors.bottom: parent.bottom
                    width: parent.width
                    orientation:-90
                    source: QtCamera
                    fillMode:VideoOutput.Stretch
                    focus : visible // to receive focus and capture key events when visible
                }
            }
            Rectangle{
                id:actionRect
                anchors.bottom:parent.bottom
                anchors.top:bgRect.bottom
                width: parent.width
                color:"black"
                Image {
                    id: photoGraphImg
                    width: 100
                    height: 100
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    source: "qrc:/icon/PAIZHAO_81.png"
                    MouseArea{
                        id:encodeMouse
                        anchors.fill: parent
                        onReleased: {
                            console.log("get photo  actionRect.enabled   ", actionRect.enabled)
                            if(shanguangAuto.visible && shanguangOff.visible && shanguangOn.visible){
                                var flashMode = QtCamera.getCameraFlashMode();
                                console.log("camera.getCameraFlashMode(): flashMode = ", flashMode);
                                shanguangOn.visible = false;
                                shanguangAuto.visible = false;
                                shanguangOff.visible = false;
                                switch(flashMode){
                                case "flashOn" : { shanguangOn.visible = true; break; }
                                case "flashAuto" : { shanguangAuto.visible = true; break; }
                                case "flashOff" : { shanguangOff.visible = true; break; }
                                default : { shanguangOn.visible = true; }
                                }
                                shanguangItem.isWaitSelect = !shanguangItem.isWaitSelect;
                            }
                            showCameraItem.enabled = false;
                            rootWindow.dialogShow = true;
                            QtCamera.setImageCapture();
                        }
                    }
                }
                Item{
                    id:cancelBtn
                    width:cancelText.width * 1.5
                    height: cancelText.height * 1.5
                    anchors.left: parent.left
                    anchors.leftMargin: 48
                    anchors.verticalCenter: parent.verticalCenter
                    Text {
                        id:cancelText
                        text: "取消"
                        font.pixelSize: 24
                        color:"#ffffff"
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    MouseArea{
                        id:cancelBtnMouse
                        anchors.fill: parent
                        onClicked: {
                            pageStack.pop();
                        }
                    }
                }
            }
        }
    }

    onStatusChanged:{
        if(status == CPageStatus.Show){
            rootWindow.dialogShow = true;
            camearImagPage.statusBarHoldEnabled = false
            gScreenInfo.setStatusBarStyle("transblack");
            gScreenInfo.setStatusBar(false)
            startCameraTime.start();
        } else if(status == CPageStatus.WillHide){
            console.log("StartCamera.qml  onStatusChanged  willHide")
            QtCamera.stopCamera();
            rootWindow.dialogShow = false;
        }
    }
    Component.onCompleted: {
        console.log("StartCamera.qml  Component.onCompleted ")
        rootWindow.cameraIsRuning = 1;
    }
    Component.onDestruction: {
        console.log("StartCamera.qml destruction")
        rootWindow.cameraIsRuning = 0;
    }
}
