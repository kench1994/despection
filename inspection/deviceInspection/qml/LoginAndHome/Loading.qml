/*!
* \file Loading.qml
* \brief 数据加载提示控件
*
*
*
*\author 何昆 hekun@syberos.com
*\version 1.0
*\date 2015/9/9
*/

import QtQuick 2.0
import com.syberos.basewidgets 2.0

/*!
  \class Loading
  \inModule 数据加载控件
  \brief 数据加载提示框
  \ingroup LoginAndHome
  数据加载提示框，防止界面在处理请求期间无响应的效果
*/
MouseArea{
    id: rect
    /** type:bool  后台处理是否尚在进行*/
    property bool running: false
    visible: false
    anchors.fill: parent

    Rectangle{
        width: parent.width
        height: parent.height
        color: "#000000"
        opacity: 0.73
    }
    Rectangle{
        anchors.bottom: parent.bottom
        height: 100
        width:parent.width
        color: "#ffffff"
        radius: 50
    }
    Rectangle{
        height:60
        width:parent.width
        anchors.bottom: parent.bottom
        color: "#ffffff"
            Text {
                id: loadingText
                anchors {
                    top: parent.top
                    horizontalCenter: parent.horizontalCenter
                }
                text: os.i18n.ctr(qsTr("data loding..."))
                font.pixelSize: 32
                color: "#000000"
            }

            Image {
                id: loadingIcon
                source: "qrc:/icon/5icon_09.png"
                width: 30; height: 30
                anchors{
                    right: loadingText.left
                    rightMargin: 20
                    verticalCenter: loadingText.verticalCenter
                }
            }

            NumberAnimation {
                running: rect.visible
                loops: Animation.Infinite
                target: loadingIcon
                from: 0; to: 360
                property: "rotation"
                duration: 500
            }
    }
    onRunningChanged: {
        if(rect.running) {
            console.log("-----------running is",rect.running, "  parent is ", rect.parent)
            rect.visible = true
            rect.parent.dataLoading = 1;
        }else {
            console.log("-----------running is",rect.running)
            rect.visible = false
            rect.parent.dataLoading = 0;
        }
    }
}
