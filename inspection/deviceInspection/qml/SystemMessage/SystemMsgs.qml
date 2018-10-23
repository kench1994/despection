/*!
* \file SystemMsgs.qml
* \brief 系统消息页面
*
*系统消息页面: 用于显示系统消息及任务消息列表; 查询消息详情
*
*\author 陈瑞华 chenruihua@syberos.com
*\version 1.0
*\date 2015/9/9
*/
import QtQuick 2.0
import com.syberos.basewidgets 2.0
import com.syberos.generateForm 1.0
import "../Common"
import "../Common/constants.js" as UI

/*!
  \class SystemMsgs
  \inModule 系统消息
  \brief 系统消息，消息列表页
  \ingroup SystemMessage
  系统消息，消息列表页: 显示系统消息及任务消息列表
*/
CommonPage {
    id: messageListPage

    /** type:int   当前未读系统消息个数*/
    property int unReadNotifyItemCount: 0

    /** type:int   当前未读任务消息个数*/
    property int unReadRemindItemCount: 0

    /** type:bool 指示当前消息列表页面是否为系统消息页*/
    property bool isSystemMsgTab: true

    property var messageSequence;
    property var systemMessageSequence;
    property var bussinessMessageSequence;

    property string nameId: ""
    statusBarHoldEnabled: true
    statusBarHoldItemColor: "white"
    orientationPolicy: CPageOrientation.LockPortrait
    clip: true

    Connections {
        target: transmitManager
        onMessageUpdated:{
            console.log("----------onMessageUpdated----------operType:", operType);
            requestMessage(operType)
        }
    }

    Connections {
        target: rootWindow
        onSystemMessageUpdated: {
            console.log("----------onSystemMessageUpdated----------result:", result);
            if(result) {
                CTOAST.requestToast(os.i18n.ctr( qsTr("Update Success") ), "", "");//刷新成功
                requestMessage("Notify")
            } else {
                CTOAST.requestToast(os.i18n.ctr( qsTr("Update Failure") ), "", "");//刷新失败
            }
        }
    }

    contentAreaItem: Rectangle {
        id: content
        Rectangle {
            id: title

            /** type:string 显示系统消息页面未读个数字符串*/
            property string notifyDownTitle: ""

            /** type:string 显示任务消息页面未读个数字符串*/
            property string remindDownTitle: ""

            width: parent.width; height: 80
            color: "white"
            anchors.top: parent.top
            visible: !(msgListView.editing || remindListView.editing)

            Rectangle {
                id: leftItemRect
                width:80; height: parent.height
                color: "transparent"
                anchors.left: parent.left
                anchors.bottom: parent.bottom

                Image {
                    id: backIcon
                    width: 40; height: 40
                    source: "qrc:/icon/back.png"
                    anchors.left: parent.left
                    anchors.leftMargin: 20
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 20
                }

                MouseArea {
                    anchors.fill: parent
                    onPressed: {
                        pageStack.pop();
                    }
                }
            }

            Text {
                id: upTitleText
                text: isSystemMsgTab? "系统通知" : "业务提醒"
                font.pixelSize: 26
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 22
                anchors.horizontalCenter: parent.horizontalCenter
            }
        } //Rectangle{ id: title

        Rectangle {
            id: titleSeparateLine
            width: parent.width; height: 1
            color: "#000000"
            opacity: 0.1
            z: parent.z + 1
            anchors.top: title.bottom
        }

        Rectangle {
            id: backgroundRect
            width: parent.width
            color:"#eff2f2"
            anchors.top: titleSeparateLine.bottom;
            anchors.bottom: bottomButtonSeparateLine.top
        }

        Rectangle {
            id: emptyMsgsBackgroundRect
            width: parent.width;
            color: "transparent"
            anchors.fill: backgroundRect
            visible: isSystemMsgTab?(systemMessageModel.count == 0):(businessMessageModel.count == 0)
            Image {
                id: emptyImage
                width: 108; height: 108
                source: "qrc:/icon/empty.png"
                anchors.top: parent.top
                anchors.topMargin: 166
                anchors.horizontalCenter: parent.horizontalCenter
            }
            Text {
                id: emptyResultText
                text: os.i18n.ctr(qsTr("The result was not found to meet the requirements."))//未找到符合要求的结果
                color: "#bbbbbb"
                font.pixelSize: 22
                width: 320
                wrapMode: Text.Wrap
                horizontalAlignment:Text.AlignHCenter
                lineHeightMode: Text.FixedHeight
                lineHeight: 22.0
                anchors.top: emptyImage.bottom
                anchors.topMargin: 26
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }

        Component{
            id: editDelegate
            CEditListViewDelegate {
                width: msgListView.width; height: 80
                editLabelRightMargin: 0
                visible: true
                Rectangle {
                    id: backGroundRect
                    anchors.fill: parent
                    color: (ReadStatus == 1) ? "#fbfbfb" : "white"
                    z: parent.z - 1
                }

                Image {
                    id: unreadFlagImage
                    source: "qrc:/icon/5icon_14.png"
                    width: 10; height: 10
                    anchors.top: parent.top
                    anchors.topMargin: 20
                    anchors.left: parent.left
                    anchors.leftMargin: 14
                    visible: !(ReadStatus == 1)
                }

                Text {
                    id: firstLeftItemText
                    text: Title
                    width: parent.width*2/3; height: 20
                    color: (ReadStatus == 1)? "#999999" : "#333333"
                    font.pixelSize: 20
                    elide: Text.ElideRight
                    anchors.bottom: unreadFlagImage.bottom
                    anchors.left: unreadFlagImage.right
                    anchors.leftMargin: 10
                }

                Text {
                    id: secondLeftItemText
                    text: PublishDate
                    width: parent.width/2; height: 16
                    color: "#999999"
                    font.pixelSize: 16
                    elide: Text.ElideRight
                    anchors.top: firstLeftItemText.bottom
                    anchors.topMargin: 10
                    anchors.left: unreadFlagImage.right
                    anchors.leftMargin: 10
                }

                Text {
                    id: secondRightItemText
//                    text: PublishDate
                    width: parent.width/2; height: 16
                    color: "#999999"
                    font.pixelSize: 16
                    horizontalAlignment: Text.AlignRight
                    anchors.top: secondLeftItemText.top
                    anchors.right: parent.right
                    anchors.rightMargin: (msgListView.editing || remindListView.editing)? 100 : 30
                }

                Rectangle {
                    width: parent.width; height: 1
                    color: "#dddddd"
                    z: parent.z + 1
                    anchors.bottom: parent.bottom
                }

                onClicked: {
                    pageStack.push("qrc:/qml/SystemMessage/MsgInfo.qml", {"titleText": Title, "bodyText": Content} );
                    if(isSystemMsgTab) {
                        if(ReadStatus == 0) {
                            messageListPage.unReadNotifyItemCount--;
                            updateMessage("Notify", MessageID,"1")
                            systemMessageModel.setData(index, "ReadStatus", "1");
                        }
                    } else {
                        if(ReadStatus == 0) {
                            messageListPage.unReadRemindItemCount--;
                            updateMessage("bussiness", MessageID,"1")
                            businessMessageModel.setData(index, "ReadStatus", "1");
                        }
                    }

                } //end onClicked:
            } //end CEditListViewDelegate {
        } // Component {

        CEditTitleBar {
            id: editStatusTitle
            width: parent.width
            editView: msgListView
            visible: msgListView.editing
            /*add by 消息删除界面“取消”按钮显示不美观（偏剧中显示）*/
            editLeftItemSize: Qt.size(88,40)
            editLeftItemLeftMargin: 20
            editLeftItemPixelSize:gUiConst.getValue("S5")
        }

        CEditListView {
            id: msgListView
            width: parent.width
            anchors.top: title.bottom
            anchors.topMargin: 10
            anchors.bottom: bottomButtonSeparateLine.top
            anchors.bottomMargin: 10
            clip: true
            visible: isSystemMsgTab
            model: systemMessageModel
            delegate: editDelegate
        }

        CEditTitleBar {
            id: editStatusTitle2
            width: parent.width
            editView: remindListView
            visible: remindListView.editing
            /*add by 消息删除界面“取消”按钮显示不美观（偏剧中显示）*/
            editLeftItemSize: Qt.size(88,40)
            editLeftItemLeftMargin: 20
            editLeftItemPixelSize:gUiConst.getValue("S5")
        }

        CEditListView {
            id: remindListView
            width: parent.width
            anchors.top: title.bottom
            anchors.topMargin: 10
            anchors.bottom: bottomButtonSeparateLine.top
            anchors.bottomMargin: 10
            clip: true
            visible: !isSystemMsgTab
            model: businessMessageModel
            delegate: editDelegate
        }

        Rectangle {
            id: bottomButtonSeparateLine
            width: parent.width; height: 1
            color: "#000000"
            opacity: 0.1
            z: parent.z + 1
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 80
            visible: !(msgListView.editing || remindListView.editing)
        }

        CFlowToolBar {
            id: editStatusBottomDeleteButton
            names: [os.i18n.ctr( qsTr("delete") )]//删除
            textColor: ((msgListView.selectedCount > 0)
                        || (remindListView.selectedCount > 0)) ? "#333333" : "gray"
            width: parent.width
            buttonHeight:80
            anchors.bottom: parent.bottom
            buttonColor: ((msgListView.selectedCount > 0)
                          || (remindListView.selectedCount > 0)) ? "#eeeff1" : "lightgray"
            animationVisible: msgListView.editing || remindListView.editing
            enabled: ((msgListView.selectedCount > 0)
                      || (remindListView.selectedCount > 0)) ? true : false

            onClicked: {
                deleteDlg.show();
            }
        }

        Rectangle {
            id: bottomButtonRect
            width: parent.width; height: 80
            color: "transparent"
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            visible: !(msgListView.editing || remindListView.editing)

            Rectangle {
                id: notifyButton
                width: parent.width/2; height: 80
                color: "white"
                anchors.left: bottomButtonRect.left
                anchors.bottom: bottomButtonRect.bottom

                Text {
                    id: sysMsgText
                    text: os.i18n.ctr( qsTr("Notify") )//系统通知
                    color:isSystemMsgTab?  "#999999" : "#333333"
                    font.pixelSize: 22
                    anchors.centerIn: parent
                }

                Rectangle {
                    id: unReadNotifyCountLable
                    width: 30; height: 30
                    anchors.bottom: sysMsgText.top
                    anchors.bottomMargin: -8
                    anchors.left: sysMsgText.right
                    anchors.leftMargin: -8
                    color: "red"
                    radius: 15;
                    visible: !(unReadNotifyItemCount == 0)
                    Text{
                        id: unReadNotifyCountText
                        text: (unReadNotifyItemCount > 99) ? "99+" : unReadNotifyItemCount
                        color: "white"
                        font.pixelSize: 16
                        anchors.centerIn: parent
                    }
                }

                MouseArea {
                    id: notifyMouse
                    anchors.fill: notifyButton
                    onPressed: {
                        isSystemMsgTab = true;
                    }
                }
            } //end  Rectangle { id: notifyButton

            Rectangle {
                id: remindButton
                width: parent.width/2; height: 80
                color: "white"
                anchors.right: bottomButtonRect.right
                anchors.bottom: bottomButtonRect.bottom

                Text {
                    id: remindText
                    text: os.i18n.ctr( qsTr("Remind") )//业务提醒
                    color: !isSystemMsgTab?  "#999999" : "#333333"
                    font.pixelSize: 22
                    anchors.centerIn: parent
                }

                Rectangle {
                    id: unReadRemindCountLable
                    width: 30; height: 30
                    anchors.bottom: remindText.top
                    anchors.bottomMargin: -8
                    anchors.left: remindText.right
                    anchors.leftMargin: -8
                    color: "red"
                    radius: 15;
                    visible: !(unReadRemindItemCount == 0)
                    Text {
                        id: unReadRemindCountText
                        text: (unReadRemindItemCount > 99)? "99+" : unReadRemindItemCount
                        color: "white"
                        font.pixelSize: 16
                        anchors.centerIn: parent
                    }
                }
                MouseArea {
                    id: remindMouse
                    anchors.fill: remindButton
                    onPressed: {
                        isSystemMsgTab = false;
                    }
                }
            } //end Rectangle { id: remindButton

            Rectangle {
                width: 1; height: 50
                x: parent.width/2
                anchors.bottom: bottomButtonRect.bottom
                anchors.bottomMargin: 15
                color: "#dddddd"
            }
        }
    } //end Rectangle { id: bottomButtonRect

    CDialog {
        id:deleteDlg
        titleText: os.i18n.ctr( qsTr("Delete") )//删除
        messageText: os.i18n.ctr( qsTr("Are you sure to delete?") )//确认删除选中的消息吗？
        rejectButtonText: os.i18n.ctr( qsTr("Cancel") )//取消
        acceptedButtonText: os.i18n.ctr( qsTr("Ok") )//确定

        onAccepted: {
            //delete the selected messages need to handled
            var mesageIDs = "";
            var selectedCount = 0;
            if(isSystemMsgTab) {
                selectedCount = msgListView.selectedCount;
                for(var i = 0; i < selectedCount; i++) {
                    mesageIDs += systemMessageModel.getItem(msgListView.selectedIndexs[i]).MessageID;
                    if(i != selectedCount - 1)
                        mesageIDs += ",";
                }
                deleteMessage("Notify", mesageIDs)
                msgListView.editing = false;
            } else {
                selectedCount = remindListView.selectedCount;
                for(var j = 0; j < selectedCount; j++) {
                    mesageIDs += businessMessageModel.getItem(remindListView.selectedIndexs[j]).MessageID;
                    if(j != selectedCount - 1)
                        mesageIDs += ",";
                }
                deleteMessage("bussiness", mesageIDs)
                remindListView.editing = false;
            }
        } //end onAccepted
    } //end  CDialog { id:deleteDlg

    Component.onCompleted: {
        rootWindow.allowToEnterMsgPage = false;
        rootWindow.toHomeFlag = 1;
        requestMessage("Notify")
        requestMessage("bussiness")
    }

    Component.onDestruction: {
        rootWindow.allowToEnterMsgPage = true;
        rootWindow.toHomeFlag = 0;
    }

    /**
     * 遍历系统消息列表，获取未读消息个数
     * @param 无
     */
    function getUnReadMsgModelCount() {
        var item;
        unReadNotifyItemCount = 0;
        for(var i=0; i< systemMessageModel.rowCount(); i++) {
            item = systemMessageModel.getItem(i);
            if(item["ReadStatus"] == 0) {
                unReadNotifyItemCount++;
            }
        }
        console.log("----------unReadNotifyItemCount:", unReadNotifyItemCount);
    }

    /**
     * 遍历任务消息列表，获取未读消息个数
     * @param 无
     */
    function getUnReadRmdModelCount() {
        var item;
        unReadRemindItemCount = 0;
        for(var i=0; i< businessMessageModel.rowCount(); i++) {
            item = businessMessageModel.getItem(i);
            if(item["ReadStatus"] == 0) {
                unReadRemindItemCount++;
            }
        }
        console.log("----------unReadRemindItemCount:", unReadRemindItemCount);
    }

    onUnReadNotifyItemCountChanged: {
        if(unReadNotifyItemCount > 0) {
            if(isSystemMsgTab){
                upTitleText.anchors.bottomMargin = 48;
            }
            title.notifyDownTitle = os.i18n.ctr( qsTr("(%1 item unread)").arg(unReadNotifyItemCount) );//%1条未读
            console.log("----------title.notifyDownTitle:", title.notifyDownTitle);
        } else {
            title.notifyDownTitle = "";
            if(isSystemMsgTab) {
                upTitleText.anchors.bottomMargin = 22
            }
        }
    }

    onUnReadRemindItemCountChanged:  {
        if(unReadRemindItemCount > 0) {
            if(!isSystemMsgTab) {
                upTitleText.anchors.bottomMargin = 48;
            }
            title.remindDownTitle = os.i18n.ctr( qsTr("(%1 item unread)").arg(unReadRemindItemCount) );//%1条未读
            console.log("----------title.remindDownTitle:", title.remindDownTitle);
        } else {
            title.remindDownTitle = "";
            if(!isSystemMsgTab) {
                upTitleText.anchors.bottomMargin = 22
            }
        }
    } //end of  onUnReadRemindItemCountChanged

    function requestMessage(operType){
        var head = {"Objects" : "CMDS_MessageList"};
        var para = {"UserID": inspectionSystemParam.getUserId(), "OperType" : operType};
        if(operType == "Notify"){
            messageListPage.systemMessageSequence = inspectionSystemConfig.getSequence();
            transmitManager.resultOfResponse.disconnect(messageListPage.systemMessageResultResponse);
            transmitManager.resultOfResponse.connect(messageListPage.systemMessageResultResponse);
            transmitManager.requestData(head, para, messageListPage.systemMessageSequence, "", true, false);
        } else {
            messageListPage.bussinessMessageSequence = inspectionSystemConfig.getSequence();
            transmitManager.resultOfResponse.disconnect(messageListPage.bussinessMessageResultResponse);
            transmitManager.resultOfResponse.connect(messageListPage.bussinessMessageResultResponse);
            transmitManager.requestData(head, para, messageListPage.bussinessMessageSequence, "", true, false);
        }
    }

    function systemMessageResultResponse(result, error, action, map){
        if(action != messageListPage.systemMessageSequence){
            console.log("action != messageListPage.systemMessageSequence return");
            return;
        }
        console.debug("----systemMessageResultResponse--onResultOfResponse-------result, error, action ", result, error, action);
        transmitManager.resultOfResponse.disconnect(messageListPage.systemMessageResultResponse);
        if(result && error == 0 && map["tables"].length > 0 && map["tables"][0]["value"] != undefined) {
            var roles = ["MessageID", "OperCode", "Title", "Content", "MessageType",
                         "MessageTopic", "OperType", "Extra", "FromUserAccount",
                         "ToUserAccount", "AttachUsers","PublishDate","ReadStatus"];
            if(map["tables"][0]["value"].length > 0){
                systemMessageModel.setRolesName(roles);
                systemMessageModel.setListData(map["tables"][0]["value"]);
                getUnReadMsgModelCount();
            }
        }
    }

    function bussinessMessageResultResponse(result, error, action, map){
        if(action != messageListPage.bussinessMessageSequence){
            console.log("action != messageListPage.bussinessMessageSequence return");
            return;
        }
        console.debug("----bussinessMessageResultResponse--onResultOfResponse-------result, error, action ", result, error, action);
        transmitManager.resultOfResponse.disconnect(messageListPage.bussinessMessageResultResponse);
        if(result && error == 0 && map["tables"].length > 0 && map["tables"][0]["value"] != undefined) {
            var roles = ["MessageID", "OperCode", "Title", "Content", "MessageType",
                         "MessageTopic", "OperType", "Extra", "FromUserAccount",
                         "ToUserAccount", "AttachUsers","PublishDate","ReadStatus"];
            if(map["tables"][0]["value"].length > 0){
                businessMessageModel.setRolesName(roles);
                businessMessageModel.setListData(map["tables"][0]["value"]);
                getUnReadRmdModelCount();
            }
        }
    }

    function deleteMessage(operType, messageIDs){
        var head = {"Objects" : "CMDS_Message_Delete"};
        var para = {"MessageIDs" : messageIDs};
        console.log("transmitManager.requestData   CMDS_Message_Delete ")
        messageListPage.messageSequence = inspectionSystemConfig.getSequence();
        transmitManager.requestData(head, para, messageListPage.messageSequence, "删除消息失败");
        if(operType == "Notify"){
            console.log("function deleteMessage  removeDataByKeys ", messageIDs);
            systemMessageModel.removeDataByKeys("MessageID", messageIDs);
            getUnReadMsgModelCount();
        } else {
            businessMessageModel.removeDataByKeys("MessageID", messageIDs);
            getUnReadRmdModelCount();
        }
    }

    function updateMessage(messageType, messageID, readStatus){
        var head = {"Objects" : "CMDS_Message_Update"};
        var para = {"MessageIDs" : messageID, "ReadStatuss" : readStatus};
        messageListPage.messageSequence = inspectionSystemConfig.getSequence();
        transmitManager.requestData(head, para, messageListPage.messageSequence, "更新消息失败");
    }
}
