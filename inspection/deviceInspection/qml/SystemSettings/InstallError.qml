/*!
* \file InstallError.qml
* \brief 安装新版本失败提示框控件
*
*
*
*\author 何昆 hekun@syberos.com
*\version 1.0
*\date 2015/9/9
*/
import QtQuick 2.0
import com.syberos.basewidgets 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.1
/*!
  \class InstallError
  \inModule 升级模块
  \brief 安装新版本失败提示框控件
  \ingroup SystemSettings
  安装新版本失败提示框控件
*/

Rectangle {
    id: installErrorNotify;
    /** type:string  浮层错误提示信息*/
    property string errMsg: os.i18n.ctr(qsTr("install failed. You should be download and update package again."));
    /** type:int  提示错误类型 0--不提示错误; -1--密码错误; -2--网络错误; -3--被禁用  -4--登陆中*/
    property int errType: 0;
    property string errortitle :os.i18n.ctr(qsTr("install failed"))
    visible: false
    color: "#000000";
    opacity: 0.85;
    anchors.fill: parent;
    MouseArea {
        anchors.fill: parent;
    }
    Text {
        id: title
        text: errortitle
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 300
        font.pixelSize: 28
        color: "#ffffff"
    }
    Text {
        //错误信息
        id: texMsg
        text: errMsg
        font.pixelSize: 24
        color: "#ffffff"
        anchors.top: title.bottom
        anchors.topMargin: 45
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Button {
        id: passwdErrButtons
        width: 392
        height: 64
        anchors.top: texMsg.bottom
        anchors.topMargin: 50
        anchors.horizontalCenter: parent.horizontalCenter
        Text {
            id: texConfirm
            text: os.i18n.ctr(qsTr("OK"))
            font.pointSize: 22
            anchors.centerIn: parent
            color: "#ffffff"
        }
        style: ButtonStyle {
            background: Rectangle {
                color: "#000000"
                radius: height/2
                border.color: "#ffffff"
            }
        }
        onClicked: {
            installErrorNotify.visible = false;
            rootWindow.pageStack.pop();
        }
    }
}





