/*!
* \file MsgInfo.qml
* \brief 系统消息-->消息详情页面
*
*消息详情页面: 用于显示系统消息详情：消息标题/消息体
*
*\author 陈瑞华 chenruihua@syberos.com
*\version 1.0
*\date 2015/9/8
*/
import QtQuick 2.0
import com.syberos.basewidgets 2.0
import "../Common"
/*!
  \class MsgInfo
  \inModule 系统消息
  \brief 系统消息，消息详情页
  \ingroup SystemMessage
  系统消息，消息详情页: 显示消息标题及消息内容
*/
CommonPage {
    id: msgInfoPage;

    /** type:alias  消息标题 */
    property alias titleText: title.titleText;

    /** type:alias  消息体 */
    property alias bodyText: textBody.text;

    statusBarHoldEnabled: true
    statusBarHoldItemColor: "white"
    orientationLock: CPageOrientation.LockPortrait
    clip: true

    contentAreaItem:  Item {
        id: content

        CommonTitlebar {
            id: title
            width: parent.width
            leftItemEnabled: true
            rightItemEnabled: false

            leftItemText: ""
            leftItemIcon: "qrc:/icon/back.png"
            leftItemSize:  Qt.size(40,40)

            titleItemEnabled: true
            titleAreaCentered: false
            titlePixelSize: 24
            titleTextColor: "#333333"

            anchors.top: parent.top
            onLeftItemChanged: leftItem.backgroundEnabled = false
            spacingBetweenLeftBorderAndLeftItem: 20            
            spacingBetweenLeftItemAndTitleItem: 10

            onLeftItemTriggered: {
                pageStack.pop();
            }
        }

        Rectangle {
            id: titleSeparateLine
            width: parent.width; height: 1
            color: "#000000"
            opacity: 0.1
            z: parent.z + 1
            anchors.top: title.bottom
        }

        Rectangle {
            id: backGroundRect
            color: "#eff2f2"
            anchors.top: title.bottom
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right

            Flickable {
                id: detailInfo
                anchors.fill: parent
                flickDeceleration: Flickable.VerticalFlick
                contentHeight: textBody.height + 60
                clip:true

                Text {
                    id: textBody
                    anchors.top: parent.top
                    anchors.topMargin: 30
                    anchors.left: parent.left
                    anchors.leftMargin: 30
                    anchors.right: parent.right
                    anchors.rightMargin: 30
                    font.pixelSize: 20
                    color: "#333333"
                    lineHeight: 28.0
                    lineHeightMode: Text.FixedHeight
                    wrapMode: Text.Wrap
                    Component.onCompleted: {
                    }
                }
            }//end  Flickable{ id:detailInfo
        }//end  Rectangle{ id: backGroundRect
    }//end  contentAreaItem:  Item {
}
