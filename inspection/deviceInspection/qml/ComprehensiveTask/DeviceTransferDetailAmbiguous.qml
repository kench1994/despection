/*!
* \file DeviceTransferDetail.qml
* \brief 设备调拨详情页面
*
*此界面设备调拨详情页面，用户对设备的调出和调入进行确认
*
*\author 桓乐乐 huanlele@syberos.com
*\version 1.0
*\date 2015/9/7
*/
import QtQuick 2.0
import com.syberos.basewidgets 2.0
import "../Common"
import "../Common/constants.js" as UI
/*!
  \class DeviceTransferDetail
  \inModule 设备调拨
  \brief 设备调拨详情页面
  \ingroup ComprehensiveTask
  设备调拨详情页面，用户对设备的调出和调入进行确认
*/
CommonPage {
    id: deviceTransferDetail

    property var allocationModel:["调出" , "调入"]
    property int allocationType: 0
    property string actionSequence : "-1"

    statusBarHoldEnabled: true
    statusBarHoldItemColor: "white"
    orientationPolicy: CPageOrientation.LockPortrait
    clip: true

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
                leftItemPixelSize: 22
                titleAreaCentered:true
                leftItemTextColor: "#666666"
                leftItemText:"取消"
                leftItemIcon:""
                titleText:"设备调拨确认信息"
                onLeftItemTriggered: {
                    ensureSubmitDlg.type = "backToCancel";
                    ensureSubmitDlg.messageText = "确认放弃设备调拨?"
                    ensureSubmitDlg.show();
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
            id:detailInfoFlick
            visible: false
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
                //spacing: 20
                ListInputItem{
                    id:orderCode
                    maximumLength:100
                    mainTitle : "调拨单号"
                    editPlaceHoldeText:"必填"
                }
                ListItem {
                    id: allocationTypeItem
                    mainTitle: "调拨类型"
                    type:"category"
                    endTitle: allocationModel[allocationType]
                    onClicked: {
                        allocationTypeDialog.select(allocationType, true);
                        allocationTypeDialog.show()
                    }
                }
                Rectangle {
                    width: parent.width
                    height: 40
                    color: "#eff1f3"
                    Text {
                        id: sectionTitle
                        font.pixelSize: 16
                        color: "#999999"
                        anchors.left: parent.left
                        anchors.top: parent.top
                        anchors.topMargin: 12
                        anchors.leftMargin: 20
                        text: "调拨说明"
                    }
                }
                Rectangle{
                    id:transferInstrctContent
                    width: parent.width
                    height: content.height + 20

                    Text {
                        id: content
                        anchors.left: parent.left
                        anchors.leftMargin: 20
                        font.pixelSize: 24
                        anchors.verticalCenter: parent.verticalCenter
                        text: "同意调拨"
                    }
                }
                Item{
                    width: parent.width;height: UI.GROUP_SEPARATOR_HIGHT
                }
                Rectangle{
                    id:imgNoteText
                    width: parent.width
                    height: visible? 200:0
                    CommonTextArea{
                        id:deviceTransferRemark
                        height:parent.height - 20
                        font.pixelSize:20
                        anchors.bottom:parent.bottom
                        width:parent.width - 40
                        maximumLength:300
                        Text {
                            id:placeHoldText
                            visible: parent.text == ""
                            color: "#dddddd"
                            font.pixelSize:parent.font.pixelSize
                            text: "请填写情况说明"
                        }
                        anchors.horizontalCenter: parent.horizontalCenter
                        objectName:"deviceFaultNote"
                    }
                }
                Item{
                    width: parent.width;height: UI.GROUP_SEPARATOR_HIGHT
                }
                CommonButton{
                    id:submmitBtn
                    text:{
                        if(allocationType == 0)
                            return   "确认调出"
                        else if(allocationType == 1)
                            return   "确认调入"
                    }
                    onClicked:{
                        console.log("aaaaaaaaaaaaaaaaa")
                        ensureSubmitDlg.show();
                        if(allocationType == 0)
                            ensureSubmitDlg.messageText = "确认后信息不可更改，是否确认调拨?"
                        else if(allocationType == 1)
                            ensureSubmitDlg.messageText = "确认后信息不可更改，是否确认调拨?"
                        ensureSubmitDlg.type = "submit";
                    }
                }
                Item{
                    width: parent.width
                    height: UI.GROUP_SEPARATOR_HIGHT*3
                }
            }
        }
    }

    CDialog{
        id:ensureSubmitDlg
        property string type: ""
        titleText : "设备调拨"
        acceptedButtonText: "确定"
        rejectButtonText: "取消"
        onAccepted:{
            if(type == "submit"){
                sendRequest();
            }else if(type == "backToCancel"){
                pageStack.pop();
            }else if(type == "homeToCancel"){
                rootWindow.toHomeBack();
            }
        }
    }

    CListDialog{
        id: allocationTypeDialog
        maxVisibleItems: 2
        titleAreaEnabled: false
        buttonAreaEnabled: false
        model: allocationModel
        onDelegateItemTriggered:{
            allocationType = index;
            allocationTypeDialog.hide();
        }
    }

    Connections{
        target: rootWindow
        onKeyBackTriggered:{
            ensureSubmitDlg.type = "backToCancel";
            ensureSubmitDlg.messageText = "确认放弃设备调拨?"
            ensureSubmitDlg.show();
        }
        onToHome:{
            allocationTypeDialog.hide();
            ensureSubmitDlg.type = "homeToCancel";
            ensureSubmitDlg.messageText = "确认放弃设备调拨?"
            ensureSubmitDlg.show();
        }
    }

    /**
     * 提交设备调拨数据
     */
    function sendRequest(){
        var type;
        var status = "0";
        if(allocationType == 0){
            type = "transferOut";
            status = "1";
        } else if(allocationType == 1){
            type = "transferIn";
            status = "2";
        } else{
            console.log("status error")
            return;
        }
        if(orderCode.inputText == ""){
            CTOAST.requestToast("请输入单号","","");
            return;
        }
        for(var i = 0; i < inspectionTaskModel.rowCount(); i++) {
            var item = inspectionTaskModel.getItem(i);
            console.debug("-------------------OrderCode, Status", item["OrderCode"], item["Status"])
            var orderCode1 = item["OrderCode"].toLocaleUpperCase();
            var orderCode2 = orderCode.inputText.toLocaleUpperCase();
            if(orderCode1 == orderCode2){
                if(item["Status"] != status ) {
                    CTOAST.requestToast("调拨类型选择错误","","");
                    return;
                }
                break;
            }
        }
        var valueMap = {};
        valueMap["OrderCode"] = orderCode.inputText;
        valueMap["ModifiedBy"] = inspectionSystemParam.getUserId();
        if(type == "transferOut"){
            valueMap["Status"] = "1"
            valueMap["OutUser"] = inspectionSystemParam.getUserId();
            valueMap["InUser"] = " ";
            valueMap["OutReMark"] = deviceTransferRemark.text == "" ? " " : deviceTransferRemark.text;
            valueMap["InReMark"] = " ";
            valueMap["InOrganiseUnit"] = " ";
            valueMap["InDepartment"] = " ";
        }else if(type == "transferIn"){
            valueMap["Status"] = "2";
            valueMap["OutUser"] = " ";
            valueMap["InUser"] = inspectionSystemParam.getUserId();
            valueMap["OutReMark"] = " ";
            valueMap["InReMark"] = deviceTransferRemark.text  == "" ? " " : deviceTransferRemark.text;
            valueMap["InOrganiseUnit"] = inspectionSystemParam.getOrganiseUnitIDs();
            valueMap["InDepartment"] = inspectionSystemParam.getUserPropertyByKey("DepartmentIDs");
        }
        var valueMapList = [];
        valueMapList.push(valueMap);
        console.log("type is  ", type, " valueMap is  ", valueMap["OrderCode"]);
        deviceTransferDetail.actionSequence = inspectionSystemConfig.getSequence()
        transmitManager.resultOfResponse.disconnect(deviceTransferDetail.resultResponse)
        transmitManager.resultOfResponse.connect(deviceTransferDetail.resultResponse)

        var head = {"Objects" : "EMA_Allocation_Import"};
        var table = {"Name":"EMA_Allocation", "Key":"ID"};
        var attachments = [];
        transmitManager.submitData(head, table, valueMapList, attachments, "", deviceTransferDetail.actionSequence, "调拨提交失败");
    }


    /**
     * 接受服务器返回数据的槽函数, 调拨提交返回数据
     * @param result:bool 获取数据是否成功
     * @param error:int 错误代码
     * @param action:string 当前发送数据动作类型
     * @param map:var    服务器返回值
     */
    function resultResponse(result, error, action, map){
        console.debug("-Device--ListDelegate-onResultOfResponse---result, error, action: ", result, error, action);
        if(action != deviceTransferDetail.actionSequence){
            console.log("action != deviceTransferDetail.actionSequence  return")
            return;
        }

        transmitManager.resultOfResponse.disconnect(deviceTransferDetail.resultResponse);
        if(result){
            CTOAST.requestToast("调拨成功", "", "");
            pageStack.pop();
        }
    }
    onStatusChanged: {
        if (status == CPageStatus.Show) {
            rootWindow.dialogShow = true;
        }else if (status == CPageStatus.WillHide){
            rootWindow.dialogShow = false;
        }
    }

//    var taskValues = [ID, OrderCode, OrderName, OutOrganiseUnit, OutDepartMent,
//                      InOrganiseUnit, InDepartMent, OperatorDate,Status,
//                      OutOrganiseUnitName, InOrganiseUnitName];

    Component.onCompleted: {
        rootWindow.toHomeFlag = 1;
        detailInfoFlick.visible = true;
    }

    Component.onDestruction: {
        console.log("DeviceTransferDetail.qml destruction")
        rootWindow.toHomeFlag = 0;
        transmitManager.resultOfResponse.disconnect(deviceTransferDetail.resultResponse);
    }
}
