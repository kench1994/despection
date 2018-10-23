/*!
* \file DeviceInspectionForm.qml
* \brief 设备巡检提交详情页面
*
*此界面是设备巡检提交详情页面，用户在该页面进行操作，将填报数据提交服务器
*
*\author 桓乐乐 huanlele@syberos.com
*\version 1.0
*\date 2015/9/7
*/
import QtQuick 2.0
import com.syberos.basewidgets 2.0
import com.syberos.generateForm 1.0
import "../Common"
import "../Common/constants.js" as UI
CommonPage {
    id: multimediaNotePage
    statusBarHoldEnabled: true
    statusBarHoldItemColor: "white"
    orientationPolicy: CPageOrientation.LockPortrait
    property var parentPage
    property string multimediaType: ""
    property string filePath: ""
    contentAreaItem:  Rectangle {
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
                rightItemEnabled: true
                leftItemPixelSize: 22
                titleAreaCentered:true
                leftItemTextColor: "#666666"
                leftItemText:"取消"
                rightItemText:"确定"
                titleText:"添加标题和备注"
                leftItemIcon:""
                onLeftItemTriggered: {
                    pageStack.pop();
                }
                onRightItemTriggered:{
                    var title, note;
                    if(titleItem.inputText == ""){
                        var pos = String(multimediaNotePage.filePath).lastIndexOf("/");
                        var fileName = String(multimediaNotePage.filePath).substring(pos + 1);
                        title =  fileName;
                    } else
                        title = titleItem.inputText;
                    if(noteTextArea.text == ""){
                        note = "this is " + multimediaNotePage.multimediaType;
                    } else
                        note = noteTextArea.text;
                    multimediaNotePage.parentPage.saveMultimediaFile(multimediaNotePage.filePath, multimediaNotePage.multimediaType, title, note);
                    pageStack.pop(multimediaNotePage.parentPage.parentPageId);
                }
                anchors.top: parent.top
                onLeftItemChanged:leftItem.backgroundEnabled = false
                leftItemBackgroundEnabled: false
                Component.onCompleted: {
                    pageHeadTitle.titleItem.horizontalAlignment = Text.AlignHCenter
                    pageHeadTitle.titleItem.width = 300
                }
            }
        }
        Flickable{
            id:detailInfo
            anchors.top: pageHead.bottom
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            flickDeceleration: Flickable.VerticalFlick
            clip:true
            contentHeight:totalCol.height
            Item{
                id:topToHeaderSeparatorItem
                width: parent.width;height: UI.GROUP_SEPARATOR_HIGHT
            }
            Column{
                id:totalCol
                anchors.top: topToHeaderSeparatorItem.bottom
                width: parent.width
                spacing: 20
                ListInputItem{
                    id:titleItem
                    mainTitle: "标题"
                }
                Rectangle{
                    width: parent.width
                    height: 150
                    Text {
                        id: noteTtitle
                        anchors.left: parent.left
                        anchors.leftMargin: 20
                        anchors.top: parent.top
                        anchors.topMargin: 10
                        font.pixelSize:22
                        text: "备注"
                    }
                    CTextArea{
                        id:noteTextArea
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left:noteTtitle.right
                        anchors.leftMargin: 20
                        anchors.right:parent.right
                        anchors.rightMargin: 20
                        height:parent.height - 20
                        font.pixelSize:22
                        maximumLength:150
                        Text {
                            visible: parent.text == ""
                            color: "#dddddd"
                            font.pixelSize:20
                            text: "请输入相应的备注信息(限100字)"
                        }
                    }
                }
            }
        }
    }
    onStatusChanged: {
        if (status == CPageStatus.WillShow) {
            gScreenInfo.setStatusBarStyle("white");
            gScreenInfo.setStatusBar(true)
        }
    }
}
