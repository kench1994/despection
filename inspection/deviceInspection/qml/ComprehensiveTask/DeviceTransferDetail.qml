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
    /** type:bool 从服务器获取的数据解析是否正确*/
    property bool dataError: false

    /** type:var 页面展示需要，但是没有包含在dataPackageValue数据中的数据*/
    property var  paramValue

    /** type:var 从服务器获取的调拨详情数据*/
    property var  dataPackageValue

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
                //visible:false
                leftItemEnabled: true
                titleItemEnabled: true
                rightItemEnabled: false
                leftItemPixelSize: 22
                titleAreaCentered:true
                leftItemTextColor: "#666666"
                leftItemText:"取消"
                leftItemIcon:""
                titleText:paramValue[2]
                onLeftItemTriggered: {
                    if(deviceTransferDetail.dataError == true)
                        pageStack.pop();
                    else{
                        ensureSubmitDlg.type = "backToCancel";
                        ensureSubmitDlg.messageText = "确认放弃设备调拨?"
                        ensureSubmitDlg.show();
                    }
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
                DeviceTitle{
                    deviceLabel1: "调拨单号:"
                    deviceName1: paramValue[1]
                    deviceLabel2: "经办时间:"
                    deviceName2: paramValue[7]
                    deviceLabel3: "调出单位:"
                    deviceName3: paramValue[9]
                    deviceLabel4: "调入单位:"
                    deviceName4: paramValue[10]
                    deviceLabel5: "计划状态:"
                    deviceName5: {
                        var tmpStatus;
                        console.log("switch(paramValue[8])----------------------------------------    ", paramValue[8])
                        switch(paramValue[8]){
                        case "-1":
                            tmpStatus = "删除"
                            break;
                        case "0":
                            tmpStatus = "审核未通过"
                            break;
                        case "1":
                            tmpStatus = "审核通过"
                            break;
                        case "2":
                            tmpStatus = "调出"
                            break;
                        case "3":
                            tmpStatus = "调入"
                            break;
                        case "4":
                            tmpStatus = "完成"
                            break;
                        default:
                            tmpStatus = "未知"
                        }
                        return tmpStatus;
                    }
                }
                Item{
                    width: parent.width
                    height: 30
                }
                ListItem{
                    id:transferDeviceList
                    mainTitle: "调拨设备"
                    type: "category"
                    endImage: "qrc:/icon/xiangyou.png"
                    onClicked: {
                        console.log("get transfer device list")
                        deviceTransferDetail.actionSequence = inspectionSystemConfig.getSequence();
                        transmitManager.resultOfResponse.disconnect(deviceTransferDetail.resultDeviceListResponse)
                        transmitManager.resultOfResponse.connect(deviceTransferDetail.resultDeviceListResponse)
                        var head = {"Objects" : "EMA_AllocationEquipmentList"};
                        var para = {"OrderCode" : paramValue[1]};
                        console.log("transmitManager.requestData   EMA_AllocationEquipmentList ")
                        transmitManager.requestData(head, para, deviceTransferDetail.actionSequence, "获取调拨设备列表失败");
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
                        if(paramValue[8] == "1")
                            return   "确认调出"
                        else if(paramValue[8] == "2")
                            return   "确认调入"
                    }
                    onClicked:{
                        console.log("aaaaaaaaaaaaaaaaa")
                        ensureSubmitDlg.show();
                        if(paramValue[8] == "1")
                            ensureSubmitDlg.messageText = "确认后信息不可更改，是否确认调拨?"
                        else if(paramValue[8] == "2")
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
        id: ipModelDialog
        maxVisibleItems:4
        buttonAreaEnabled: false
        onDelegateItemTriggered:{
            ipModelDialog.hide();
        }
    }

    Connections{
        target: rootWindow
        onKeyBackTriggered:{
            if(deviceTransferDetail.dataError == true)
                pageStack.pop();
            else{
                ensureSubmitDlg.type = "backToCancel";
                ensureSubmitDlg.messageText = "确认放弃设备调拨?"
                ensureSubmitDlg.show();
            }
        }
        onToHome:{
            ipModelDialog.hide();
            ensureSubmitDlg.type = "homeToCancel";
            ensureSubmitDlg.messageText = "确认放弃设备调拨?"
            ensureSubmitDlg.show();
        }
    }

    /**
     * 提交设备调拨数据
     */
    function sendRequest(){
        console.log("request  OutOrganiseUnit   ", paramValue[3], "  OutDepartMent  ",
                    paramValue[4], " InOrganiseUnit  ", paramValue[5], "  InDepartMent ", paramValue[6]);
        var type;
        if(paramValue[8] == "1")
            type = "transferOut";
        else if(paramValue[8] == "2")
            type = "transferIn";
        else{
            console.log("status error")
            return;
        }
        var valueMap = {};
        valueMap["OrderCode"] = paramValue[1];
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
     * 接受服务器返回数据的槽函数, 获取设备列表
     * @param result:bool 获取数据是否成功
     * @param error:int 错误代码
     * @param action:string 当前发送数据动作类型
     * @param map:var    服务器返回值
     */
    function resultDeviceListResponse(result, error, action, map){
        console.debug("----ListDelegate-----onResultOfResponse--------------result, error: ", result, error);
        console.log("===DeviceTransferDetail====OutOrganiseUnit======00======"+paramValue[9]);
        if(action != deviceTransferDetail.actionSequence){
            console.log("action != deviceTransferDetail.actionSequence  return")
            return;
        }

        transmitManager.resultOfResponse.disconnect(deviceTransferDetail.resultDeviceListResponse)
        if(result && error == 0){
            var recipientsOtherParam = {
                "outOrganiseUnit" : paramValue[3],
                "EnabledScanItem": false
            }
            pageStack.push("qrc:/qml/ComprehensiveTask/ComprehensiveBaseList.qml",
                           {
                               "originalData": map,
                               "otherParam":recipientsOtherParam,
                           });
        }
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
        if(deviceTransferDetail.paramValue.length <= 0){
            deviceTransferDetail.dataError = true;
            CTOAST.requestToast("error: tables is empty", "", "");
            return;
        }
        detailInfoFlick.visible = true;
        console.log("paramValue   ", paramValue)
    }

    Component.onDestruction: {
        console.log("DeviceTransferDetail.qml destruction")
        rootWindow.toHomeFlag = 0;
        transmitManager.resultOfResponse.disconnect(deviceTransferDetail.resultResponse);
        transmitManager.resultOfResponse.disconnect(deviceTransferDetail.resultDeviceListResponse)
    }
}
