/*!
* \file FloatingLayer.qml
* \brief 提示登录错误浮层
*
*
*
*\author 何昆 hekun@syberos.com
*\version 1.0
*\date 2015/9/9
*/
import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.1
import "../Common"
/*!
  \class FloatingLayer
  \inModule 登录模块
  \brief 登录错误浮层控件
  \ingroup LoginAndHome
   登录错误浮层控件
*/

Rectangle {
    id: notification;
    /** type:string  浮层错误提示信息*/
    property string errMsg: ""
    /** type:int  提示错误类型 0--不提示错误; -1--密码错误; -2--网络错误; -3--被禁用  -4--登陆中  -5--密码修改成功标志 -6--密码修改失败标志*/
    property int errType: 0;
    property string errortitle :os.i18n.ctr(qsTr("Login failure"))
    visible: false
    color: errType == -4? "grey" : "#000000";
    opacity: errType == -4? 0.6 : 0.85;
    anchors.fill: parent;
    MouseArea {
        anchors.fill: parent;
    }
    Text {
        id: title
        text: errortitle
        visible: errType != -4
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
        visible: errType != -4
        font.pixelSize: 24
        color: "#ffffff"
        anchors.top: title.bottom
        anchors.topMargin: 45
        anchors.horizontalCenter: parent.horizontalCenter
    }
    CommonButton{
        id:passwdErrButtons
        width:392
        anchors.top: texMsg.bottom
        anchors.topMargin: 50
        anchors.horizontalCenter: parent.horizontalCenter
        pixelSize:22
        visible: errType == -1 || errType == -3 || errType == -5 || errType == -6 ? true : false
        text:"确定"
        onClicked:{
            notification.visible = false;
            if(errType == -5){
                rootWindow.popToFirstPage()
            }
        }
    }
//    Button {
//        id: passwdErrButtons
//        width: 392
//        height: 64
//        anchors.top: texMsg.bottom
//        anchors.topMargin: 50
//        anchors.horizontalCenter: parent.horizontalCenter
//        visible: errType == -1 || errType == -3 || errType == -5 || errType == -6 ? true : false
//        Text {
//            id: texConfirm
//            text: os.i18n.ctr(qsTr("OK"))
//            font.pixelSize: 28
//            anchors.centerIn: parent
//            color: "#ffffff"
//        }
//        style: ButtonStyle {
//            background: Rectangle {
//                color: "#000000"
//                radius: height/2
//                border.color: "#ffffff"
//            }
//        }
//        onClicked: {
//            console.log("Reset userName or Password.");
//            notification.visible = false;
//            if(errType == -5){
//                 rootWindow.popToFirstPage()
//            }
//        }
//    }

    Item {
        //网络错误
        id: netErrButtons;
        anchors.top: texMsg.bottom;
        anchors.topMargin: 50;
        width: 392;
        height: 64;
        anchors.horizontalCenter: parent.horizontalCenter;
        visible: errType == -2 ? true : false;
        Rectangle {
            id: rectKnowned;
            anchors.left: parent.left;
            anchors.top: parent.top;
            width: 182;
            height: 64;
            radius:20;
            border.color: "#ffffff";
            opacity: 0.85;
            color: "transparent";
            Text {
                id:texKnowned;
                font.pixelSize:22
                text: os.i18n.ctr(qsTr("I known"));
                anchors.centerIn: parent;
                color: "#ffffff"
            }
            MouseArea {
                anchors.fill: parent;
                onClicked: {
                    console.log("exit notification");
                    notification.visible = false;
                }
            }
        }
        Rectangle {
            id: rectGoSetting;
            anchors.top: parent.top;
            anchors.right: parent.right;
            width: 182;
            height: 64;
            radius: 20;
            color: "red";
            Text {
                id: texGoSetting;
                font.pixelSize:22
                text: os.i18n.ctr(qsTr("Setting"))
                anchors.centerIn: parent;
            }
            MouseArea {
                anchors.fill: parent;
                onClicked: {
                    console.log("Go to settings.");
                    notification.visible = false;
                    rootWindow.pageStack.push("qrc:/qml/SystemSettings/SystemSetting.qml")
                }
            }
        }
    }
}





