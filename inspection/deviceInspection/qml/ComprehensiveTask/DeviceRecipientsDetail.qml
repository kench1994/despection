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

    /** type:bool 从服务器获取的数据解析是否正确*/
    property bool dataError: false

    /** type:var 页面展示需要，但是没有包含在dataPackageValue数据中的数据*/
    property var paramValue

    /** type:var 从服务器获取的调拨详情数据*/
    property var dataPackageValue

    property string actionSequence:"-1"
    objectName:"deviceFaultReportPage"
    statusBarHoldEnabled: true
    statusBarHoldItemColor: "white"
    orientationPolicy: CPageOrientation.LockPortrait
    clip: true

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
                    if(deviceRecipientsDetail.dataError == true)
                        pageStack.pop();
                    else {
                        ensureSubmitDlg.type = "backToCancel";
                        ensureSubmitDlg.messageText = "确认放弃领用信息填报吗?";
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
                spacing: 20
                Column{
                id:contentCol
                    width: parent.width
                    spacing: 5
                    DeviceTitle{
                        id:deviceFaultReportTitle
                        deviceName1:dataPackageValue["tables"][0]["value"][0]["EquipmentName"]
                        deviceLabel2:"编号:"
                        deviceName2: dataPackageValue["tables"][0]["value"][0]["EquipmentCode"]
                        deviceLabel3:"型号:"
                        deviceName3: dataPackageValue["tables"][0]["value"][0]["ModelName"]
                        deviceLabel4: "厂商:"
                        deviceName4: dataPackageValue["tables"][0]["value"][0]["ManufacturerName"]
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
            if(deviceRecipientsDetail.dataError == true)
                pageStack.pop();
            else {
                ensureSubmitDlg.type = "backToCancel";
                ensureSubmitDlg.messageText = "确认放弃领用信息填报吗?";
                ensureSubmitDlg.show();
            }
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
        var valueMapList = [];
        var valueMap = {};
        //IDやOrderCodeやDistributeIDやEquipmentIDやReceiveUserIDやReceiveDateやIsRFIDScanやEquipmentNameや
        //EquipmentCodeやRFIDCodeやEnterpriseNameやModelNameやManufacturerNameやLongitudeやLatitudeやPositionsやCustodianやRecipient

       //?OrderCode,?ERBQCode,?CodeType,?Positions,?Custodian,?Recipient,?Longitude,?Latitude,?isRFIDScan,?IsBarCodeScan,?PDAID,?ModifiedBy
        valueMap["OrderCode"] = dataPackageValue["tables"][0]["value"][0]["OrderCode"];
        valueMap["ERBQCode"] = paramValue["ERBQCode"];
        valueMap["CodeType"] = paramValue["CodeType"];
        valueMap["Positions"] = equipmentPosition.inputText == "" ? " " : equipmentPosition.inputText;
        valueMap["Custodian"] = equipmentCustodian.inputText == "" ? " " : equipmentCustodian.inputText;
        valueMap["Recipient"] = equipmentRecipient.inputText == "" ? " " : equipmentRecipient.inputText;
        valueMap["Longitude"] = "1.01";
        valueMap["Latitude"] = "1.01";
        valueMap["IsRFIDScan"] = paramValue["IsRFIDScan"];
        valueMap["IsBarCodeScan"] = paramValue["IsBarCodeScan"];
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
        deviceRecipientsDetail.dataError = true;
        if(dataPackageValue["tables"].length <= 0) {
            CTOAST.requestToast("error: tables is empty","","");
            return;
        } else if(dataPackageValue["tables"][0]["value"].length <= 0) {
            CTOAST.requestToast("error: tables.value is empty", "", "");
            return;
        }
        deviceRecipientsDetail.dataError = false;
        detailInfoFlick.visible = true;
    }
    Component.onDestruction: {
        console.log("DeviceRecipientsDetail.qml destruction")
        rootWindow.toHomeFlag = 0;
        transmitManager.resultOfResponse.disconnect(deviceRecipientsDetail.resultResponse);
    }
}
