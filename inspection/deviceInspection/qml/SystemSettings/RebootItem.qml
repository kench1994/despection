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
import QtQuick 2.0
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
    property string errortitle :"升级完成,请重启手机"
    visible: false
    color: "black"
    opacity:  0.85
    anchors.fill: parent;
    MouseArea {
        anchors.fill: parent;
    }
    Text {
        id: title
        text: errortitle
        anchors.centerIn: parent
        color:"white"
        font.pixelSize: 28
    }
    CommonButton{
        id:passwdErrButtons
        width:392
        anchors.top: title.bottom
        anchors.topMargin: 50
        anchors.horizontalCenter: parent.horizontalCenter
        pixelSize:22
        text:"确定"
        onClicked:{
            console.log("***************确定重启*******************");
            myInspectSysSetting.reBootDevice();
        }
    }
}
