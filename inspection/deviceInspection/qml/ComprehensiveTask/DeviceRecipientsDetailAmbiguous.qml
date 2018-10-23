/*!
* \file deviceRecipientsDetail.qml
* \brief 设备领用详情页面
*
*此界面设备领用详情页面，用户对设备领用进行确认
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
  \class deviceRecipientsDetail
  \inModule 设备领用详情
  \brief 设备领用详情页面
  \ingroup ComprehensiveTask
  设备领用详情页面，用户对设备领用进行确认
*/
CommonPage {
    id: deviceRecipientsDetail

    property string actionSequence:"-1"
    property string codeType : "1"
    property string codeERBQ : ""
    property bool isRFIDScan: false
    property bool isBarCodeScan: false
    statusBarHoldEnabled: true
    statusBarHoldItemColor: "white"
    orientationPolicy: CPageOrientation.LockPortrait
    clip: true
    signal resultRFIDCode(string rfidCode)
    signal resultBarCode(string barCode)

    onResultRFIDCode: {
        equipmentCode.inputText = rfidCode;
        codeType = "2";
        codeERBQ = rfidCode;
        isRFIDScan = true;
        isBarCodeScan = false;
    }
    onResultBarCode: {
        equipmentCode.inputText = barCode;
        codeType = "3";
        codeERBQ = barCode;
        isRFIDScan = false;
        isBarCodeScan = true;
    }

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
                rightItemEnabled: false
                leftItemPixelSize: 22
                titleAreaCentered:true
                leftItemTextColor: "#666666"
                leftItemText:"取消"
                leftItemIcon:""
                titleText:"设备领用确认信息"
                onLeftItemTriggered: {
                    ensureSubmitDlg.type = "backToCancel";
                    ensureSubmitDlg.messageText = "确认放弃领用信息填报吗?";
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
        ScanItem {
            id: scan
            anchors.top: pageHead.bottom
            parentPage: deviceRecipientsDetail
        }

        Flickable{
            id:detailInfoFlick
            visible: false
            anchors.top: scan.bottom
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
                Column{
                id:contentCol
                    width: parent.width
                    spacing: 5
                    ListInputItem{
                        id:orderCode
                        maximumLength:100
                        mainTitle : "领用单号"
                        editPlaceHoldeText:"必填"
                    }
                    ListInputItem{
                        id:equipmentCode
                        maximumLength:100
                        mainTitle : "设备编码"
                        editPlaceHoldeText:"必填"
                        onTextChanged: {
                            if(text != ""){
                                codeType = "1";
                            }
                        }
                    }
                    ListInputItem{
                        id:equipmentPosition
                        maximumLength:100
                        mainTitle : "设备位置"
                        editPlaceHoldeText:"非必填"
                    }
                    ListInputItem{
                        id:equipmentCustodian
                        maximumLength:15
                        mainTitle: "设备保管人"
                        editPlaceHoldeText:"非必填"
                    }
                    ListInputItem{
                        id:equipmentRecipient
                        maximumLength:15
                        mainTitle: "设备领用人"
                        editPlaceHoldeText:"非必填"
                    }
                }

                CommonButton{
                    id:submmitBtn
                    signal submitClicked()

                    text: "确认领用"
                    onClicked:{
                        ensureSubmitDlg.type = "submit";
                        ensureSubmitDlg.messageText = "提交后信息不可更改,确认提交吗?";
                        ensureSubmitDlg.show();

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
        titleText : "设备领用确认信息"
        acceptedButtonText: "确定"
        rejectButtonText: "取消"
        onAccepted:{
            if(type == "submit") {
                sendRequest();
            }else if(type == "backToCancel"){
                pageStack.pop();
            }else if(type == "homeToCancel"){
                rootWindow.toHomeBack();
            }
        }
    }

    Connections{
        target: rootWindow
        onKeyBackTriggered:{
            ensureSubmitDlg.type = "backToCancel";
            ensureSubmitDlg.messageText = "确认放弃领用信息填报吗?";
            ensureSubmitDlg.show();
        }
        onToHome:{
            ensureSubmitDlg.type = "homeToCancel";
            ensureSubmitDlg.messageText = "确认放弃领用信息填报吗?";
            ensureSubmitDlg.show();
        }
    }

    /**
     * 提交设备领用数据
     */
    function sendRequest() {
        console.log("-------------sendRequest-------------codeType: ", codeType)
        if(codeType == "1" && equipmentCode.inputText == "") {
            CTOAST.requestToast("请输入设备编号","","");
            return;
        }else if(codeERBQ == "" && codeType != "1") {
            CTOAST.requestToast("未知扫码错误","","");
            return;
        }
        if(orderCode.inputText == "") {
            CTOAST.requestToast("请输入单号","","");
            return;
        }
        var valueMapList = [];
        var valueMap = {};
        //IDやOrderCodeやDistributeIDやEquipmentIDやReceiveUserIDやReceiveDateやIsRFIDScanやEquipmentNameや
        //EquipmentCodeやRFIDCodeやEnterpriseNameやModelNameやManufacturerNameやLongitudeやLatitudeやPositionsやCustodianやRecipient
        //?OrderCode,?ERBQCode,?CodeType,?Positions,?Custodian,?Recipient,?Longitude,?Latitude,?isRFIDScan,?IsBarCodeScan,?PDAID,?ModifiedBy
        valueMap["OrderCode"] = orderCode.inputText;
        valueMap["Positions"] = equipmentPosition.inputText == "" ? " " : equipmentPosition.inputText;
        valueMap["Custodian"] = equipmentCustodian.inputText == "" ? " " : equipmentCustodian.inputText;
        valueMap["Recipient"] = equipmentRecipient.inputText == "" ? " " : equipmentRecipient.inputText;
        valueMap["Longitude"] = "1.01";
        valueMap["Latitude"] = "1.01";
        if(codeType == "1"){
            valueMap["ERBQCode"] = equipmentCode.inputText;
            valueMap["CodeType"] = "1";
            valueMap["IsRFIDScan"] = "0";
            valueMap["IsBarCodeScan"] = "0";
        }else if(codeType == "2"){
            valueMap["ERBQCode"] = codeERBQ;
            valueMap["CodeType"] = "2";
            valueMap["IsRFIDScan"] = "1";
            valueMap["IsBarCodeScan"] = "0";
        }else if(codeType == "3"){
            valueMap["ERBQCode"] = codeERBQ;
            valueMap["CodeType"] = "3";
            valueMap["IsRFIDScan"] = "0";
            valueMap["IsBarCodeScan"] = "1";
        }
        valueMap["PDAID"] = inspectionSystemParam.getPadId();
        valueMap["ModifiedBy"] = inspectionSystemParam.getUserId();
        valueMapList.push(valueMap);
        deviceRecipientsDetail.actionSequence = inspectionSystemConfig.getSequence();
        transmitManager.resultOfResponse.disconnect(deviceRecipientsDetail.resultResponse)
        transmitManager.resultOfResponse.connect(deviceRecipientsDetail.resultResponse)

        var head = {"Objects" : "EMD_DistributeEquipment_Import"};
        var table = {"Name":"EMD_DistributeEquipment", "Key":"ID"};
        var attachments = [];
        transmitManager.submitData(head, table, valueMapList, attachments, "", deviceRecipientsDetail.actionSequence, "领用提交失败");
    }

    /**
     * 接受服务器返回数据的槽函数
     * @param result:bool 获取数据是否成功
     * @param error:int 错误代码
     * @param action:string 当前发送数据动作类型
     * @param map:var    服务器返回值
     */
    function resultResponse(result, error, action, map) {
        console.debug("---Device----------------ListDelegate-----onResultOfResponse--------------result, error, action: ",
                      result, error, action);
        if(action != deviceRecipientsDetail.actionSequence){
            console.log("action != deviceRecipientsDetail.actionSequence return")
            return;
        }
        transmitManager.resultOfResponse.disconnect(deviceRecipientsDetail.resultResponse);
        if(result == true){
            CTOAST.requestToast("领用成功", "", "");
            pageStack.pop();
        }
    }

    onStatusChanged: {
        if (status == CPageStatus.Show) {
            rootWindow.dialogShow = true;
        }else if (status == CPageStatus.Hide){
            rootWindow.dialogShow = false;
        }
    }

    Component.onCompleted: {
        rootWindow.toHomeFlag = 1;
        detailInfoFlick.visible = true;
    }
    Component.onDestruction: {
        console.log("DeviceRecipientsDetail.qml destruction")
        rootWindow.toHomeFlag = 0;
        transmitManager.resultOfResponse.disconnect(deviceRecipientsDetail.resultResponse);
    }
}
