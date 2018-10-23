/*!
* \file BarCodeScan.qml
* \brief 一二维码扫描页面
*
*此界面一二维码扫描页面，调用相机取帧，并对取到的帧进行解析，如果解析成功，返回解析数据
*
*\author 桓乐乐 huanlele@syberos.com
*\version 1.0
*\date 2015/9/7
*/
import QtQuick 2.1
import QtMultimedia 5.0
import com.syberos.basewidgets 2.0
import com.syberos.decodeWorkSpace 1.0
/*!
  \class BarCodeScan
  \inModule 公共控件
  \brief 一二维码扫描页面
  \ingroup common
  此界面一二维码扫描页面，调用相机取帧，并对取到的帧进行解析，如果解析成功，返回解析数据
*/
CommonPage {
    id: barCodeScanPage

    property var parentPage
    statusBarHoldEnabled: true
    statusBarHoldItemColor: "black"
    orientationPolicy: CPageOrientation.LockPortrait
    enabled:false
    property string strDecodeContent: ""
    property bool pageWillPop : false
    /**
     *解码完成信号
     * @param 解析数据
     */
    signal decodeFinished(string decodeContent)
    Connections {
        target: gWindow
        onActiveChanged: {
            console.log("onActiveChanged gWindow.active ", gWindow.active)
            if (gWindow.active) {
                QtCamera.startCameraAndSetZoom();
            } else{
                QtCamera.stopCamera();
            }
        }
    }

    Connections{
        target: rootWindow
        onKeyBackTriggered:{
            console.log("barcodePage  hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh onKeyBackTriggered");
            if(QtCamera.status.toString() == "8")
              barCodeScanPage.popAndClearStatus();
        }
    }
    function popAndClearStatus(){
        if(barCodeScanPage.pageWillPop == true){
            console.log("barCodeScanPage. pageWillPop== true  return;")
            return;
        }
        barCodeScanPage.pageWillPop = true;
        QtCamera.stopCamera();
        pageStack.pop()
    }

    contentAreaItem:Rectangle {
        anchors.fill: parent
        CommonTitlebar{
            id: pageHead
            width: parent.width
            leftItemEnabled: true
            titleItemEnabled: true
            rightItemEnabled: false
            leftItemText:""
            onLeftItemTriggered:  {
                popAndClearStatus()
            }
            anchors.top: parent.top
            onLeftItemChanged:leftItem.backgroundEnabled = false
            leftItemBackgroundEnabled: false
            titleText:os.i18n.ctr(qsTr("BarCode"))
        }
        Timer{
            id:searchLockTimer
            interval: 2000
            running: false
            repeat: true
            triggeredOnStart:true
            onTriggered: {
                console.log("worksp.camera   status is ", QtCamera.status)
                if(QtCamera.status.toString() == "8"){
                    console.log("huanlele  9999999999999999999999999999999")
                    QtCamera.searchAndLock();
                }
            }
        }

        DecodeWorkSpace{
            id:worksp
            onDecodeFinished: {
                console.log("33333imagepath is ", decodeImagePath,
                            "content is " , decodeContent, "  success  ", success, "  parentPage ", parentPage);
                if(barCodeScanPage.pageWillPop == true){
                    console.log("onDecodeFinished  barCodeScanPage.pageWillPop == true return; ")
                    return;
                }
                barCodeScanPage.strDecodeContent = decodeContent.toLowerCase();
                barCodeScanPage.popAndClearStatus();
            }
            onChangedCameraStatus:{
                console.log("huanlilele  camere status is " , status);
                if(QtCamera.status.toString() == "8"){
                    barCodeScanPage.enabled = true;
                    centerRect.visible = false;
                    searchLockTimer.start();
                    scanLineRunAnm.start();
                }else {
                    barCodeScanPage.enabled = false;
                }
            }
        }
        VideoOutput {
            id:videopt
            anchors.top: parent.top
            width: parent.width
            anchors.bottom: parent.bottom
            orientation:-90
            source: QtCamera
            fillMode:VideoOutput.Stretch
            focus : visible // to receive focus and capture key events when visible
            onHeightChanged: {
                console.log("videoOutput height is  ", height)
            }
            onWidthChanged: {
                console.log("videoOutput width is  ", width)
            }
        }

        Rectangle{
            id:topRect
            color:"black"
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: centerRect.top
            opacity: 0.5
        }
        Rectangle{
            id:btmRect
            color:"black"
            anchors.top: centerRect.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            opacity: 0.5
            Text {
                id: tip
                anchors.top: parent.top
                anchors.topMargin: 40
                anchors.horizontalCenter: parent.horizontalCenter
                color:"#ffffff"
                font.pixelSize: 18
                text: os.i18n.ctr(qsTr("Show bar code in the box, can automatically scan"))
            }
        }
        Rectangle{
            id:leftRect
            color:"black"
            anchors.top: topRect.bottom
            anchors.left: parent.left
            anchors.right: centerRect.left
            anchors.bottom: btmRect.top
            opacity: 0.5
        }
        Rectangle{
            id:rightRect
            color:"black"
            anchors.top: topRect.bottom
            anchors.left: centerRect.right
            anchors.right: parent.right
            anchors.bottom: btmRect.top
            opacity: 0.5
        }

        Rectangle{
            id:centerRect
            width: 300
            height: 300
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            opacity: 0.5
            color:"black"
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    if(QtCamera.status == 8){
                        console.log("huanlele  3333333333333333333")
                        QtCamera.searchAndLock();
                    }
                }
            }
        }

        Rectangle{
            anchors.top: centerRect.top
            anchors.left: centerRect.left
            height: 40
            color: "#00ff36"
            width: 4
        }
        Rectangle{
            anchors.bottom: centerRect.bottom
            anchors.left: centerRect.left
            height: 40
            color: "#00ff36"
            width: 4
        }
        Rectangle{
            anchors.top: centerRect.top
            anchors.right: centerRect.right
            height: 40
            color: "#00ff36"
            width: 4
        }
        Rectangle{
            anchors.bottom: centerRect.bottom
            anchors.right: centerRect.right
            height: 40
            color: "#00ff36"
            width: 4
        }
        Rectangle{
            anchors.left: centerRect.left
            anchors.top: centerRect.top
            width: 40
            color: "#00ff36"
            height: 4
        }
        Rectangle{
            anchors.right: centerRect.right
            anchors.top: centerRect.top
            width: 40
            color: "#00ff36"
            height: 4
        }
        Rectangle{
            anchors.left: centerRect.left
            anchors.bottom: centerRect.bottom
            width: 40
            color: "#00ff36"
            height: 4
        }
        Rectangle{
            anchors.right: centerRect.right
            anchors.bottom: centerRect.bottom
            width: 40
            color: "#00ff36"
            height: 4
        }
        Image{
            property int scanX:(parent.width - 300)/2
            id:scaLine
            source: "qrc:/icon/SAOMIAO_56.png"
            width: centerRect.width
            height: sourceSize.height
            x:scanX
            y:250
            NumberAnimation on y {
                id: scanLineRunAnm
                running: false
                from: 250; to: 250 + 300
                loops:Animation.Infinite
                duration: 4000
            }
        }
    }
    Timer{
        id:startCameraTm
        interval: 300
        running: false
        repeat: false
        onTriggered: {
            console.log("QtCamera.startCameraAndSetZoom();")
            QtCamera.startCameraAndSetZoom();
        }
    }

    onStatusChanged:{
        if(status == CPageStatus.Show){
            console.log("BarCodeScan.qml status == CPageStatus.Show  startCameraTimer start")
            rootWindow.dialogShow = true;
            startCameraTm.start();
        }else if(status == CPageStatus.WillHide){
            if(barCodeScanPage.parentPage && barCodeScanPage.parentPage.refresh)
                barCodeScanPage.parentPage.refresh = false;
            rootWindow.dialogShow = false;
            scanLineRunAnm.stop();
            searchLockTimer.stop();
            startCameraTm.stop();
        }
    }
    Component.onCompleted: {
        rootWindow.isScaningBarCode = 1;
        rootWindow.closeKeyboard()
        gScreenInfo.setStatusBarStyle("transwhite");
        //gScreenInfo.setStatusBar(true)
        console.log("barcode scan  qml  completed--------------")
    }
    Component.onDestruction: {
        gScreenInfo.setStatusBarStyle("transblack");
        if(barCodeScanPage.strDecodeContent != "")
            barCodeScanPage.decodeFinished(barCodeScanPage.strDecodeContent);
        rootWindow.isScaningBarCode = 0;
        console.log("barcode scan  qml  destruction--------------")
    }

}
