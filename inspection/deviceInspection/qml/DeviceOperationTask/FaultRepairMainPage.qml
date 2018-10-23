import QtQuick 2.0
import com.syberos.basewidgets 2.0
import "../Common"
import "../Common/constants.js" as UI
import "../DateTime"
CommonPage {
    id: deviceOperationDetail
    statusBarHoldEnabled: true
    statusBarHoldItemColor: "white"
    orientationPolicy: CPageOrientation.LockPortrait
    clip: true
    color: "white"
    /*当前流程到了第几步
      流程分为如下：
      0：申报;1：受理(accept);2：审批(Approve);3：派工(dispatch);4：维修(maintain);5：费用等级(cost);6：评价(appraise)
      0,1必须有，2-6可以配置
    */
    property var originalData
    property var otherParam
    property bool  showDetail: false
    property alias dateTimeItem: selectDateTimeDlg
    signal accepted()
    CDialog{
        id:ensureSubmitDlg
        property string type: ""
        titleText : deviceOperationDetail.otherParam["TitleName"]
        messageText:type == "submit" ?   "提交后信息不可更改，确认提交吗?" : "确定放弃" + titleText + "信息填报吗？"
        acceptedButtonText: "确定"
        rejectButtonText: "取消"
        onAccepted:{
            if(ensureSubmitDlg.type == "submit"){
                deviceOperationDetail.accepted();
            } else if(ensureSubmitDlg.type == "backToCancel"){
                pageStack.pop();
            } else if(ensureSubmitDlg.type == "homeToCancel"){
                rootWindow.toHomeBack();
            }
        }
    }

    Connections{
        target: rootWindow
        onKeyBackTriggered:{
            if(deviceOperationDetail.status == CPageStatus.Show){
                rootWindow.closeKeyboard()
                ensureSubmitDlg.type = "backToCancel";
                ensureSubmitDlg.show();
            }
        }
        onToHome:{
            rootWindow.closeKeyboard()
            ensureSubmitDlg.type = "homeToCancel"
            ensureSubmitDlg.show();
        }
    }

    contentAreaItem: Rectangle {
        id: title
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
                leftItemPixelSize: 22
                titleAreaCentered:true
                leftItemTextColor: "#666666"
                titleText:deviceOperationDetail.otherParam["TitleName"]
                onLeftItemTriggered: {
                    rootWindow.closeKeyboard();
                    ensureSubmitDlg.type = "backToCancel";
                    ensureSubmitDlg.show();
                }
                anchors.top: parent.top
                leftItemBackgroundEnabled: false
            }
        }

        Flickable{
            id:detailInfoFlick
            flickDeceleration: Flickable.VerticalFlick
            width:parent.width
            anchors.top: pageHead.bottom
            anchors.bottom: parent.bottom
            clip:true
            contentHeight:details.height + topToHeaderSeparatorItem.height
            Item{
                id:topToHeaderSeparatorItem
                width: parent.width;height: UI.GROUP_SEPARATOR_HIGHT
            }
            Column{
                id :details
                width: parent.width
                anchors.top: topToHeaderSeparatorItem.bottom
                ListItem{
                    id:deviceNameItem
                    mainTitle: deviceOperationDetail.originalData["tables"][0]["value"][0]["EquitmentName"]
                    subTitle: "位置:" +deviceOperationDetail.originalData["tables"][0]["value"][0]["EquitmentLocation"]
                    + "\n编码:" +deviceOperationDetail.otherParam["EquipmentCode"]
                    onClicked: {
                        deviceOperationDetail.showDetail = !deviceOperationDetail.showDetail;
                    }
                }
                FaultRepairDetail{
                    id:repairDatail
                }
            }
        }
    }

    SelectDateTime {
        id: selectDateTimeDlg
    }

    onStatusChanged:{
        if(status == CPageStatus.Show){
            rootWindow.dialogShow = true;
            rootWindow.toHomeFlag = 1;
            deviceOperationDetail.statusBarHoldEnabled = true
            gScreenInfo.setStatusBarStyle("white");
            gScreenInfo.setStatusBar(true)
        } else if(status == CPageStatus.WillHide){
            rootWindow.dialogShow = false;
            rootWindow.toHomeFlag = 0;
        }
    }
    Component.onCompleted: {
        var taskStatus = deviceOperationDetail.otherParam["Taskstatus"];
        var qmlpath;
        switch(taskStatus){
        case "1":
            qmlpath = "qrc:/qml/DeviceOperationTask/FaultAcceptForm.qml";
            break;
        case "2":
            qmlpath = "qrc:/qml/DeviceOperationTask/FaultApproveForm.qml";
            break;
        case "3":
            qmlpath = "qrc:/qml/DeviceOperationTask/FaultDispatchForm.qml";
            break;
        case "4":
            qmlpath = "qrc:/qml/DeviceOperationTask/FaultMaintainForm.qml";
            break;
        case "5":
            qmlpath = "qrc:/qml/DeviceOperationTask/FaultCostForm.qml";
            break;
        case "6":
            qmlpath = "qrc:/qml/DeviceOperationTask/FaultAppraiseForm.qml";
            break;
        default:
            qmlpath = "";
        }
        if(qmlpath != ""){
            console.log("FaultRepairMainPage createComponent is ", qmlpath);
            var com = Qt.createComponent(qmlpath);
            if(com.status == Component.Ready) {
                com.createObject(details);
            } else
                console.log("FaultRepairMainPage createComponent fail  ", com.errorString())
        } else
            console.log("qmlpath is empty  error , status is ", taskStatus);
    }
}

