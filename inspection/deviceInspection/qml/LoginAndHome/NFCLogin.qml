import QtQuick 2.0
import com.syberos.basewidgets 2.0
import "../Common"
Rectangle {
    id:nfcLoginRect
    property bool pageWillPop: false
    anchors.fill: parent
    opacity: 0.90
    color:"#000000"
    MouseArea{
        id:allmouse
        anchors.fill: parent
    }

    Text{
        id:nfcText
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 400
        font.pixelSize:22
        color:"white"
        text:"请扫描员工卡"
    }
    CommonButton{
        id:cancelNfc
        property string cancelType: "cancelRfid"
        width:250
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 100
        anchors.horizontalCenter:  parent.horizontalCenter
        pixelSize:22
        text:"取消扫描"
        onClicked:{
            console.log("cancelnfc clicked   set nfcLoginRect visible false")
            if(cancelType == "cancelRfid")
                nfcLoginRect.exitRFID();
            else if(cancelType == "exitButton"){
                cancelNfc.cancelType = "cancelRfid";
                nfcLoginRect.visible = false;
            }
        }
    }

    onVisibleChanged: {
        if(visible){
            nfcManager.done.connect(onRFIDDone);
            nfcManager.error.connect(onRFIDError);
            nfcTm.start();
            nfcWaitTm.start();
        }else{
            nfcTm.stop();
            nfcWaitTm.stop();
            nfcText.text = "请扫描员工卡";
            nfcLoginRect.pageWillPop = false;
            rootWindow.allowToEnterMsgPage = true;
        }
    }

    Timer{
        id:nfcWaitTm
        interval: 20000
        running: false
        repeat: false
        onTriggered: {
            if(nfcLoginRect.pageWillPop == true)
                return;
            nfcLoginRect.pageWillPop = true;
            nfcText.text = "扫描超时"
            cancelNfc.cancelType = "exitButton";
            nfcManager.done.disconnect(onRFIDDone);
            nfcManager.error.disconnect(onRFIDError);
            console.log("nfcWaitTm  stopDetection")
            nfcManager.stopDetection();
            console.log("nfcWaitTm  stopDetection ok")
        }
    }

    Timer{
        id:nfcTm
        interval: 50
        running: false
        repeat: false
        onTriggered: {
            console.log("nfcLoginRect.qml  nfcManager.startDetection() ")
            nfcManager.startDetection();
            console.log("nfcLoginRect.qml  nfcManager.startDetection() ok")
        }
    }

    function onRFIDError(err) {
        console.log("nfcLoginRect.qml onRFIDError   ", err)
        if(nfcLoginRect.pageWillPop == true)
            return;
        nfcLoginRect.pageWillPop = true;
        nfcText.text = "扫描失败";
        cancelNfc.cancelType = "exitButton";
        nfcManager.done.disconnect(onRFIDDone);
        nfcManager.error.disconnect(onRFIDError);
        console.log("nfcWaitTm  stopDetection")
        nfcManager.stopDetection();
        console.log("nfcWaitTm  stopDetection ok")
    }

    function onRFIDDone(uid) {
        console.log("RFID detected --------------------------------------------- ", uid);
        nfcLoginRect.exitRFID();
        console.log("onRFIDDone   popAndClearStatus  ",uid)
        loginTemplate.loginByNfc(uid);
    }
    function exitRFID(){
        if(nfcLoginRect.pageWillPop == true)
            return;
        nfcLoginRect.pageWillPop = true;
        nfcManager.done.disconnect(onRFIDDone);
        nfcManager.error.disconnect(onRFIDError);
        console.log("popAndClearStatus  stopDetection")
        nfcManager.stopDetection();
        console.log("popAndClearStatus  stopDetection ok  pop")
        nfcLoginRect.visible = false;
    }
}
