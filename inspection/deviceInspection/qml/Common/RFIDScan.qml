/*!
* \file RFIDScan.qml
* \brief RFID扫描页面
*
*RFID扫描页面: 用于执行RFID扫描操作
*
*\author 陈瑞华 chenruihua@syberos.com
*\version 1.0
*\date 2015/9/8
*/
import QtQuick 2.0
import QtQuick 2.2
import com.syberos.basewidgets 2.0
import com.syberos.generateForm 1.0
import "./constants.js" as UI
/*!
  \class RFIDScan
  \inModule 公共控件
  \brief 公共控件，RFID扫描页面
  \ingroup common
  公共控件，RFID扫描页面:执行RFID扫描操作,显示并发送扫描结果
*/
CommonPage {
        id: scanRFIDPage
        property string strFfid: ""
        property bool pageWillPop : false
        property var parentPage
        statusBarHoldEnabled: true
        statusBarHoldItemColor: "white"
        orientationPolicy: CPageOrientation.LockPortrait
        clip: true

        /**
         *RFID扫描完成信号
         * @param 扫描结果
         */
        signal sendRFIDResult(string resultStr);
        signal sendRFIDTimeout();


        Connections{
            target: rootWindow
            onKeyBackTriggered:{
                console.log("scanRFIDPage   onKeyBackTriggered");
                scanRFIDPage.popAndClearStatus();
            }
        }

        Connections {
            target: gWindow
            onActiveChanged: {
                console.log("onActiveChanged gWindow.active ", gWindow.active, " scanRFIDPage.status is  ", scanRFIDPage.status)
                if (!gWindow.active && scanRFIDPage.status == CPageStatus.Show) {
                    scanRFIDPage.popAndClearStatus();
                }
            }
        }

        contentAreaItem:  Item {
            id: content
            CommonTitlebar {
                id: pageHead
                width: parent.width
                leftItemEnabled: true
                titleItemEnabled: true
                rightItemEnabled: false
                leftItemText: ""
                titleText: os.i18n.ctr( qsTr("RFIDScan") )
                onLeftItemTriggered: {
                    scanRFIDPage.popAndClearStatus();
                }
                anchors.top: parent.top
                onLeftItemChanged: leftItem.backgroundEnabled = false
                leftItemBackgroundEnabled: false
            }

            Rectangle {
                id: titleSeparateLine
                width: parent.width; height: 1
                color: "#000000"
                opacity: 0.1
                z: parent.z + 1
                anchors.top: pageHead.bottom
            }

            Rectangle {
                id: backGroundRect
                width: parent.width
                color: "#eeeff1"
                anchors.top: titleSeparateLine.bottom
                anchors.bottom: parent.bottom

                Text {
                    id: remindingText
                    text: os.i18n.ctr(qsTr("Scan the Rfid lable"))//请扫描设备RFID标签
                    color: "gray"
                    font.pixelSize: 42
                    anchors.top: parent.top
                    anchors.topMargin: 50
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                Image {
                    id: rfidImage
                    source: "qrc:/icon/RFID_54.png"
                    width: 192; height: 139
                    anchors.top: remindingText.bottom
                    anchors.topMargin: 140
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                Text {
                    id: warningText
                    text: os.i18n.ctr(qsTr("In the need to use the RFID function of the page,
    you can use the shortcut key to quickly launch RFID scanning equipment."))//在需要使用RFID功能的页面，您可使用快捷键快速启动RFID扫描设备。
                    color: "#333333"
                    font.pixelSize: 18
                    width: 320
                    wrapMode: Text.Wrap
                    horizontalAlignment: Text.AlignHCenter
                    lineHeightMode: Text.FixedHeight
                    lineHeight: 25.0
                    anchors.top: rfidImage.bottom
                    anchors.topMargin: 25.0
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 220
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }
        }

        function popAndClearStatus(){
            if(scanRFIDPage.pageWillPop == true){
                console.log("popAndClearStatus  scanRFIDPage.pageWillPop == true  return")
                return;
            }
            scanRFIDPage.pageWillPop = true;
            nfcManager.done.disconnect(onRFIDDone);
            nfcManager.error.disconnect(onRFIDError);
            console.log("popAndClearStatus  stopDetection")
            nfcManager.stopDetection();
            console.log("popAndClearStatus  stopDetection ok  pop")
            pageStack.pop();
        }

        /**
         * 扫描失败时，显示"扫描失败，请重新扫描"字符
         */
        function changeTextState() {
            warningText.text = os.i18n.ctr(qsTr("Failed to scan, please try again."));//扫描失败，请重新扫描。
            warningText.color = "#cc181e";
            warningText.font.pixelSize = 16;
            warningText.anchors.bottomMargin = 245;
        }

        function onRFIDError(err) {
            changeTextState();
        }

        function onRFIDDone(uid) {
            console.log("RFID detected --------------------------------------------- ", uid);
            if(scanRFIDPage.pageWillPop == true){
                console.log("onRFIDDone  scanRFIDPage.pageWillPop == true return; ")
                return;
            }
            var rfid = String(uid).toLowerCase();
            scanRFIDPage.strFfid = rfid;
            console.log("onRFIDDone   popAndClearStatus  ")
            scanRFIDPage.popAndClearStatus();
        }

        onStatusChanged:{
            if(status == CPageStatus.WillShow){
                console.log("RFIDScan.qml status == CPageStatus.WillShow ",CPageStatus.WillShow)
            }else if(status == CPageStatus.Show){
                rootWindow.dialogShow = true;
                //rootWindow.canDrag = false;
                rootWindow.closeKeyboard()
                nfcManager.done.connect(onRFIDDone);
                nfcManager.error.connect(onRFIDError);
                console.log("RFIDScan.qml status == CPageStatus.Show ",CPageStatus.Show)
                nfcstartTimer.start()
            }else if(status == CPageStatus.WillHide){
                console.log("RFIDScan.qml status == CPageStatus.WillHide ",CPageStatus.WillHide)
                rootWindow.dialogShow = false;
                if(scanRFIDPage.parentPage && scanRFIDPage.parentPage.refresh)
                    scanRFIDPage.parentPage.refresh = false;
            }else if(status == CPageStatus.Hide){
                console.log("RFIDScan.qml status == CPageStatus.Hide ", CPageStatus.Hide)
            }
        }

        Timer{
            id :nfcstartTimer
            interval: 200
            running: false
            repeat: false
            onTriggered: {
                console.log("RFIDScan.qml nfcstartTimer start  nfcManager.startDetection() ")
                nfcManager.startDetection();
                console.log("RFIDScan.qml nfcstartTimer start  nfcManager.startDetection() ok")
            }
        }

        Component.onCompleted: {
            rootWindow.isScaningRFID = 1;
        }

        Component.onDestruction: {
            console.log("RFID scan  qml  destruction--------------");
            if(scanRFIDPage.strFfid != ""){
                sendRFIDResult(scanRFIDPage.strFfid);
            }
            rootWindow.isScaningRFID = 0;
        }
    }

