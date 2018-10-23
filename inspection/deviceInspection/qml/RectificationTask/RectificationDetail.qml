/*!
* \file RectificationDetail.qml
* \brief 检查整改整改项详显页
*
*检查整改整改项详显页
*
*\author 张子健 zhangzijian@syberos.com
*\version 1.0
*\date 2015/9/10
*/
import QtQuick 2.0
import com.syberos.basewidgets 2.0
import "../Common"
import "../Common/constants.js" as UI
/*!
  \class RectificationDetail
  \inModule 检查整改
  \brief 检查整改整改项详显页
  \ingroup RectificationTask
  此界面是检查整改整改项详显页,用于显示检查整改整改项
*/
CommonPage {
    id: deviceTransferDetail

    statusBarHoldEnabled: true
    statusBarHoldItemColor: "white"
    orientationPolicy: CPageOrientation.LockPortrait
    clip: true

    /** type:string 整改内容*/
    property string rectificationContent:""
    /** type:string 显示的整改内容*/
    property string rectificationContents:""
    /** type:string 数据源类型(0整改任务 1整改确认)*/
    property string rectificationType:""
    /** type:string 整改状态*/
    property string rectificationStatus:""

    Component.onCompleted: {
        var oneworld = rectificationContent.split("；")
        var i = 0;
        for(i = 0;i<oneworld.length;i++){
            rectificationContents = rectificationContents+oneworld[i]+"\n"
        }
        type2.text = rectificationContents
    }

    contentAreaItem: Rectangle {
        id: rect
        anchors.fill: parent
        color:"#eeeff1"
        Rectangle{
            id:pageHead
            width: parent.width
            height: pageHeadTitle.height
            CommonTitlebar{
                id: pageHeadTitle
                width: parent.width
                clip:true
                leftItemEnabled: true
                titleItemEnabled: true
                rightItemEnabled: false
                leftItemText:""
                titleText:os.i18n.ctr(qsTr("Rectification Content"))
                onLeftItemTriggered: {
                    pageStack.pop()
                }
                anchors.top: parent.top
                onLeftItemChanged:leftItem.backgroundEnabled = false
                leftItemBackgroundEnabled: false
            }
        }
        Item{
            id:topToHeaderSeparatorItem
            anchors.top: pageHead.bottom
            width: parent.width;height: UI.GROUP_SEPARATOR_HIGHT
        }

        Flickable{
            id:detailInfoFlick
            anchors.top: topToHeaderSeparatorItem.bottom
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            flickDeceleration: Flickable.VerticalFlick
            boundsBehavior: Flickable.StopAtBounds
            maximumFlickVelocity: 70
            contentWidth: parent.width
            contentHeight:rectangleType1.visible ?type1.contentHeight :type2.contentHeight
            clip:true

            Rectangle{
                id:rectangleType1
                visible:String(rectificationType) == "2"
                height: type1.height + 30
                anchors.top: topToHeaderSeparatorItem.bottom
                anchors.right: parent.right
                anchors.left: parent.left
                Text {
                    id: type1
                    text: rectificationContent
                    color: "#666666"
                    font.pixelSize: 16
                    anchors {
                        top:parent.top
                        topMargin: 10
                        left: parent.left
                        leftMargin: 20
                        right: parent.right
                        rightMargin: 20
                    }
                    wrapMode:Text.WrapAnywhere
                    lineHeight:1.5
                }
            }
            Rectangle{
                id:rectangleType2
                visible:String(rectificationType) == "1"
                height: type2.height+ 30
                anchors.top: topToHeaderSeparatorItem.bottom
                anchors.right: parent.right
                anchors.left: parent.left
                Text {
                    id: type2
                    text: rectificationContents
                    color: "#666666"
                    font.pixelSize: 16
                    anchors {
                        top:parent.top
                        topMargin: 10
                        left: parent.left
                        leftMargin: 20
                        right: parent.right
                        rightMargin: 20
                    }
                    wrapMode:Text.WrapAnywhere
                    lineHeight:1.5
                }
            }
        }
    }
}
