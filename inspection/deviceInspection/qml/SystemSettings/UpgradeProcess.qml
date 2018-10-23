/*!
* \file UpgradeProcess.qml
* \brief 下载安装升级包界面
*
*
*
*\author 何昆 hekun@syberos.com
*\version 1.0
*\date 2015/9/9
*/
import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1

import QtGraphicalEffects 1.0
import com.syberos.basewidgets 2.0
import "../Common"
import "../Common/constants.js" as UI
/*!
  \class UpgradeProcess
  \inModule 版本升级模块
  \brief 下载安装升级包界面
  \ingroup common
  下载安装升级包界面，支持进度条显示
*/
CommonPage {
    id:processId
    /** type:string 压缩包下载路径*/
    property string packageFullPath: "";
    /** type:string 压缩包类型*/
    property string packageType: "";

    property string actionSequence : "-1"
    Text {
        id: titleTex
        text:  "版本更新"
        anchors.top: processId.top
        anchors.topMargin: 70
        anchors.horizontalCenter: processId.horizontalCenter
    }

    Rectangle {
        id: outCircle
        anchors.top: titleTex.bottom
        anchors.topMargin: 100
        anchors.horizontalCenter: parent.horizontalCenter
        width: 290
        height: 290
        radius: height/2
        border.color: "#333333"
    }
    Rectangle {
        id: interCircleId
        anchors.centerIn: outCircle
        z:10
        width: outCircle.width -10
        height: outCircle.height -10
        radius:  height/2
        border.color: "#333333"
    }

    Text {
        id: statusId
        z:10
        text:  "0%";
        font.pixelSize: 48
        color: "#000000"
        anchors.centerIn: outCircle
    }
    Text {
        id: errInfoId
        text:  "下载失败"
        color: "red"
        visible: false
        z:10;
        anchors.top: statusId.bottom
        anchors.topMargin: 10
        font.pixelSize: 20
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Text {
        id: infoId
        wrapMode: Text.WordWrap
        text:  os.i18n.ctr(qsTr("We are downloading image, please Maintain full charge, and don`t poweroff or ohter operations. ")) //正在下载更新文件，请保持手机电量充足，不要关机或进行其他操作.
        font.pixelSize: 18
        color: "#666666"
        anchors.top: outCircle.bottom
        anchors.topMargin: 70
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width-100
    }
    Button {
        id: cancelId
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 40
        anchors.left: parent.left
        anchors.leftMargin: 50
        width: parent.width/2-60
        height: 40
        visible: false
        style: ButtonStyle {
            background: Rectangle {
                radius: cancelId.height/2
                border.color: "#333333"
                Text {
                    id: cancalTextId
                    text: os.i18n.ctr(qsTr("cancel"))
                    anchors.centerIn: parent
                }

            }
        }
        onClicked: {
            rootWindow.toHomeFlag = 0
            pageStack.pop()
        }
    }

    Button {
        id: retryId
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 40
        anchors.left: cancelId.right
        anchors.leftMargin: 10
        width: parent.width/2-60
        height: 40
        visible: false
        style: ButtonStyle {
            background: Rectangle {
                radius: cancelId.height/2
                border.color: "#333333"
                Text {
                    id: retryTextId
                    text:os.i18n.ctr(qsTr("Retry"))
                    anchors.centerIn: parent
                }

            }
        }
        onClicked: {
            errInfoId.visible = false;
            retryId.visible = false;
            cancelId.visible = false;
            startDownLoadPackage();
        }
    }

    ConicalGradient {
        anchors.fill: outCircle
        id: progressCircle
        angle: 0.0
        gradient: Gradient {
            GradientStop { position: 0.0; color: "white" }
            GradientStop { position: 1.0; color: "red" }
        }
        source: outCircle
    }

    WaitingPage{
        id:installPage
        actionType: "install"
        z:100
        visible: false
    }

    InstallError {
        id: installErrNotification
        z:1000
        visible: false
    }

    RebootItem{
        id:rebootDlg
        z:1001
        visible: false
    }

    function startDownLoadPackage(){
        var head = {"Objects" : "DownloadFileStream", "Type" : "0"};
        var para = {"FileUrl" : rootWindow.appFilePath, "FilePath" : "/data/home/user/appUpgrade/deviceInspection.sop"};
        console.log("transmitManager.requestData   DownloadFileStream ")
        processId.actionSequence = inspectionSystemConfig.getSequence();
        transmitManager.requestData(head, para, processId.actionSequence, "", true, false);
    }

    /**
     * 接收下载失败槽函数，显示下载失败界面
     */
    function downloadErrOccur()
    {
        errInfoId.visible = true;
        retryId.visible = true;
        cancelId.visible = true;
    }

    function downloadProgress(map){
        if(map["status"] == "5")
            return;
        var percent_val = (Number(map["progress"])/Number(map["length"])).toPrecision(3);
        console.log("/n/n/ndownloadProgress    progress is ", map["progress"], "   length is ", map["length"], "  percent_val is  ", percent_val,
                    "  Number(percent_val) * 100 ", Math.floor(Number(percent_val) * 100));
        statusId.text = String(Math.floor(Number(percent_val) * 100)) + "%"
        var angle_val = Number(percent_val * 360).toPrecision(3);
        progressCircle.angle = angle_val;
    }
    /**
     * 处理升级包下载失败
     */
    function downloadPackageError(map) {
        console.debug("----main.qml-----downloadPackageError--------------action, error, serverCode,description ", map["description"]);
        downloadCount++;
        if(downloadCount > UI.MAX_DOWNLOAD_COUNT) {
            downloadCount = 0;
            downloadErrOccur();
        }else {
            console.log("restart download package.");
            startDownLoadPackage();
        }
    }
    /**
     * 处理升级包下载成功，执行升级
     */
    function downloadPackageResult(map) {
        console.debug("----UpgradeProcess.qml-----downloadPackageResult-------------- ", map["target"]);
        //升级包下载成功
        //upgradeThread.addTask(map["target"], "zip");
        upgradeThread.addTask(map["target"], rootWindow.appMD5, rootWindow.appSHA1);
    }
    /**
     *显示正在安装界面
    */
    function installProgress(fullPath, status, result) {
        if(String(status) == UI.DOWNLOAD_STATUS_SUCCESS) {
            console.log("Start install ",fullPath);
            /// 判断时间
            var time = versioninfo["head"]["time"];
            inspectionSystemConfig.checkSysTime(time);
            installPage.visible = true;
        } else if(String(status) == UI.INSTALL_STATUS_SUCCESS){
            installPage.visible = false;
            rebootDlg.visible = true;
        }
    }

    /**
     *接收安装结果信号
    */
    function installResult(fullPath) {
        var logMsg = "install " + fullPath + "SUCCESS!";
        console.log(logMsg);
    }

    /**
     *接收升级包安装失败信号.如果失败次数不足，则重复安装。否则，提示安装失败
     * @param   reason:string 失败原因
     * @param   errCode：失败错误码
    */
    function recvInstallError(reason, errCode) {
        console.log("install Package failed, reason:", reason,"   error conde:", errCode);
            installErrNotification.visible = true;
    }

    function resultOfResponse(result, error, action, map){
        console.log("----UpgradeProcess----result, error,action, actionSequence ",
                      result,  error, action, processId.actionSequence);
        if(action != processId.actionSequence){
            console.log("action != processId.actionSequence  return");
            return;
        }
        if(map["binaryType"] == "binaryError"){
            downloadPackageError(map);
        } else if(map["binaryType"] == "binaryResponse"){
            downloadPackageResult(map);
        } else if(map["binaryType"] == "binaryProgress"){
            downloadProgress(map);
        }
    }

    onStatusChanged:{
        if(status == CPageStatus.WillShow){
            rootWindow.dialogShow = true;
            rootWindow.isUpdating = true;
            startDownLoadPackage();
        }else if(status == CPageStatus.WillHide){
            rootWindow.dialogShow  = false;
            rootWindow.isUpdating = false;
        }
    }

    Component.onCompleted: {
       transmitManager.resultOfResponse.connect(processId.resultOfResponse)
        upgradeThread.process.connect(installProgress);
        upgradeThread.response.connect(installResult);
        upgradeThread.sendInstallError.connect(recvInstallError);
    }
    Component.onDestruction: {
        transmitManager.resultOfResponse.disconnect(processId.resultOfResponse)
        upgradeThread.process.disconnect(installProgress);
        upgradeThread.response.disconnect(installResult);
        upgradeThread.sendInstallError.disconnect(recvInstallError);
    }
}
