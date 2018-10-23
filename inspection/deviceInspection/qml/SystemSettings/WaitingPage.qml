/*!
* \file CheckVersion.qml
* \brief 版本检查界面
*
*
*
*\author 何昆 hekun@syberos.com
*\version 1.0
*\date 2015/9/9
*/
import QtQuick 2.0
/*!
  \class WatingPage
  \inModule 版本升级模块
  \brief 版本升级提示框控件
  \ingroup SystemSettings
  版本升级提示框控件
*/
Rectangle {
    id: versionNotification
    anchors.fill: parent
    color: "#000000"
    opacity: 0.6
    property string actionType: ""  //checkVersion 检查版本, install 正在安装
    MouseArea {
        anchors.fill: parent;
        onClicked: {
            console.log("push on versionUpdate");
        }
    }
    Rectangle {
        id: versionCheck
        anchors.bottom: parent.bottom;
        width: parent.width;
        height: 100;
        Text {
            id: title;
            anchors.centerIn: parent;
            font.pixelSize: 22;
            color: "#333333";
            text:  actionType == "checkVersion" ? os.i18n.ctr(qsTr("Checking version...")): os.i18n.ctr(qsTr("installing..."))
        }
    }

    onVisibleChanged: {
        if(visible == false && actionType == "checkVersion") {
            rootWindow.dialogShow  = false;
            rootWindow.allowToEnterMsgPage = true;
        } else if(visible == true && actionType == "checkVersion") {
            rootWindow.dialogShow  = true;
            rootWindow.allowToEnterMsgPage = false;
        }
    }
}
