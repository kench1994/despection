/*!
* \file FaultReport.qml
* \brief 故障申报详情页面
*
*此界面故障申报详情页面，用户对故障申报进行确认
*
*\author 桓乐乐 huanlele@syberos.com
*\version 1.0
*\date 2015/9/7
*/
import QtQuick 2.0
import com.syberos.basewidgets 2.0
import "../Common"
import "../Common/constants.js" as UI
import "../Multimedia"
/*!
  \class FaultReport
  \inModule 故障申报详情页面
  \brief 故障申报详情页面
  \ingroup ComprehensiveTask
  故障申报详情页面，用户对故障申报进行确认
*/
CommonPage {
    id: deviceFaultReportPage

    objectName:"deviceFaultReportPage"
    statusBarHoldEnabled: true
    statusBarHoldItemColor: "white"
    orientationPolicy: CPageOrientation.LockPortrait
    clip: true

    /** type:bool 从服务器获取的数据解析是否正确*/
    property bool   dataError: false

    /** type:string 设备型号*/
    property string equipmentID: ""

    /** type:var 页面展示需要，但是没有包含在dataPackageValue数据中的数据*/
    property var    paramValue

    /** type:var 从服务器获取的调拨详情数据*/
    property var    dataPackageValue

    /** type:var 故障类型列表*/
    property var    faultTypeList: []

    /** type:var 故障类型字典列表*/
    property var    faultTypeDicItemList: []

    /** type:var 故障原因列表*/
    property var    faultReasonList: []

    /** type:var 故障原因字典列表*/
    property var    faultReasonDicItemCodeList: []

    property string actionSequence:"-1"

    Component.onCompleted: {
        console.log("fault  dataPackageValue[tables].length", dataPackageValue["tables"].length)
        rootWindow.toHomeFlag = 1;
        deviceFaultReportPage.dataError = true;
        if(dataPackageValue["tables"].length <= 0){
            CTOAST.requestToast(os.i18n.ctr(qsTr("error: tables is empty")),"","");
            return;
        } else if(dataPackageValue["tables"][0]["value"].length <= 0){
            CTOAST.requestToast(os.i18n.ctr(qsTr("error: tables is empty")),"","");
            return;
        }
        var repairStatus = dataPackageValue["tables"][0]["value"][0]["RepairStatus"];
        if(repairStatus != undefined && repairStatus == "1"){
            CTOAST.requestToast("设备存在未完成的运维单,无法进行上报","","");
            popTm.start();
            return;
        }

        detailInfoFlick.visible = true;
        deviceFaultReportPage.dataError = false;
        deviceFaultReportPage.equipmentID = dataPackageValue["tables"][0]["value"][0]["ID"];
        deviceFaultReportTitle.deviceName1 = dataPackageValue["tables"][0]["value"][0]["EquipmentName"];
        deviceFaultReportTitle.deviceLabel2 = "编号:";
        deviceFaultReportTitle.deviceName2 = dataPackageValue["tables"][0]["value"][0]["EquipmentCode"];
        if(deviceFaultReportPage.equipmentID == ""){
            deviceFaultReportTitle.deviceLabel3 = "RFID码:";
            deviceFaultReportTitle.deviceLabel4 = "二维码:";
            deviceFaultReportTitle.deviceName3 = ""
            deviceFaultReportTitle.deviceName4 = "";
            if(deviceFaultReportPage.paramValue["CodeType"] == "2"){
                deviceFaultReportTitle.deviceName3 = dataPackageValue["tables"][0]["value"][0]["RFIDCode"];
            } else if(deviceFaultReportPage.paramValue["CodeType"] == "3"){
                deviceFaultReportTitle.deviceName4 = dataPackageValue["tables"][0]["value"][0]["BarCode"];
            }
        } else {
            deviceFaultReportTitle.deviceLabel3 = "厂商:";
            deviceFaultReportTitle.deviceName3 = dataPackageValue["tables"][0]["value"][0]["EnterpriseName"];
            deviceFaultReportTitle.deviceLabel4 = "型号:";
            deviceFaultReportTitle.deviceName4 = dataPackageValue["tables"][0]["value"][0]["ModelName"];
        }

        if(dataPackageValue["tables"].length >= 2 && dataPackageValue["tables"][1]["value"].length > 0){
            var tmpFaultType = dataPackageValue["tables"][1]["value"]
            for(var i = 0; i < tmpFaultType.length; i++){
                faultTypeList.push(tmpFaultType[i]["Name"])
                faultTypeDicItemList.push(tmpFaultType[i]["DicItemCode"])
            }
            deviceFaultType.endTitle = faultTypeList[0];
            deviceFaultType.itemId = faultTypeDicItemList[0];
            console.log("page completed  faultTypeDicItemList ", faultTypeDicItemList.length)
        }
        if(dataPackageValue["tables"].length >= 3 && dataPackageValue["tables"][2]["value"].length > 0){
            var tmpFaultReason = dataPackageValue["tables"][2]["value"]
            for(var j= 0; j < tmpFaultReason.length; j++){
                faultReasonList.push(tmpFaultReason[j]["Name"])
                faultReasonDicItemCodeList.push(tmpFaultReason[j]["DicItemCode"])
            }
            deviceFaultReason.endTitle = faultReasonList[0];
            deviceFaultReason.itemId = faultReasonDicItemCodeList[0];
        }

    }

    Component.onDestruction: {
        rootWindow.toHomeFlag = 0;
    }
    Timer{
        id:popTm
        interval: 500
        running: false
        repeat: false
        onTriggered: {
            pageStack.pop();
        }
    }

    Connections{
        target: rootWindow
        onKeyBackTriggered:{
            if(deviceFaultReportPage.status == CPageStatus.Show){
                if(deviceFaultReportPage.dataError == true){
                    pageStack.pop();
                }else{
                    console.log("ensureSubmitDlg.show(); 1111111111111    ")
                    ensureSubmitDlg.type = "backToCancel";
                    ensureSubmitDlg.messageText = os.i18n.ctr(qsTr("Abandon fault report"));
                    ensureSubmitDlg.show();
                }
            }
        }
        onToHome:{
                faultPageChiDlg.hide();
                ensureSubmitDlg.type = "homeToCancel";
                ensureSubmitDlg.messageText = os.i18n.ctr(qsTr("Abandon fault report"));
                ensureSubmitDlg.show();
        }
    }

    onStatusChanged: {
        if (status == CPageStatus.Show) {
            rootWindow.dialogShow = true;
            deviceFaultReportPage.statusBarHoldEnabled = true
            gScreenInfo.setStatusBarStyle("white");
            gScreenInfo.setStatusBar(true)
        }else if (status == CPageStatus.Hide){
            console.log("FaultReport.qml  status == CPageStatus.Hide  ")
            rootWindow.dialogShow = false;
        }else if (status == CPageStatus.WillHide){
            console.log("status == CPageStatus.WillHide  ensureSubmitDlg.hide();  ");
            faultPageChiDlg.hide();
            ensureSubmitDlg.hide();
        }
    }

    contentAreaItem:  Rectangle {
        id: rect
        anchors.fill: parent
        color:"#eeeff1"

        Rectangle{
            id:pageHead
            width: parent.width
            height: pageHeadTitle.height
            //color:"red"

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
                leftItemText:os.i18n.ctr(qsTr("Cancel"))
                leftItemIcon:""
                titleText:os.i18n.ctr(qsTr("Fault report"))
                onLeftItemTriggered: {
                    if(deviceFaultReportPage.dataError == true)
                        pageStack.pop();
                    else {
                        ensureSubmitDlg.type = "backToCancel";
                        ensureSubmitDlg.messageText = os.i18n.ctr(qsTr("Abandon fault report"));
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
            anchors.top: pageHead.bottom
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            flickDeceleration: Flickable.VerticalFlick
            clip:true
            visible: false
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
                    }

                    ListItem{
                        id:deviceFaultType
                        objectName: "deviceFaultType"
                        mainTitle: os.i18n.ctr(qsTr("Fault type"))//"故障类型"
                        type:"category"
                        endImage: "qrc:/icon/icon_08.png"
                        onClicked: {
                            faultPageChiDlg.showType = "faultType"
                            faultPageChiDlg.model = faultTypeList;
                            faultPageChiDlg.show();
                            var selectIndex = -1;
                            console.log("falutType list  is  ", faultTypeList);
                            for(var i = 0; i < faultTypeList.length; i++){
                                if(faultTypeList[i] == deviceFaultType.endTitle){
                                    selectIndex = i;
                                    break;
                                }
                            }
                            for(var j = 0; j < faultTypeList.length; j++){
                                faultPageChiDlg.select([j],false);
                            }
                                faultPageChiDlg.select([selectIndex],true);
                        }
                    }

                    ListItem{
                        id:deviceFaultReason
                        objectName: "deviceFaultReason"
                        mainTitle: os.i18n.ctr(qsTr("Fault reason"))//"故障原因"
                        type:"category"
                        endImage: "qrc:/icon/icon_08.png"
                        onClicked: {
                            faultPageChiDlg.showType = "faultReason"
                            faultPageChiDlg.model = faultReasonList;
                            faultPageChiDlg.show();
                            var selectIndex = -1;
                            for(var i = 0; i < faultReasonList.length; i++){
                                if(faultReasonList[i] == deviceFaultReason.endTitle){
                                    selectIndex = i;
                                    break;
                                }
                            }
                            for(var j = 0; j < faultReasonList.length; j++){
                                faultPageChiDlg.select([j],false);
                            }
                            faultPageChiDlg.select([selectIndex],true);
                        }
                    }

                    MultimediaView{
                        id:faultImageList
                        parentPageId: deviceFaultReportPage
                    }
                    Rectangle{
                        width: parent.width
                        height: 100
                        CTextArea{
                            id:deviceFaultLevel
                            height:parent.height - 20
                            font.pixelSize:20
                            anchors.bottom:parent.bottom
                            width:parent.width - 40
                            maximumLength:150
                            Text {
                                visible: parent.text == ""
                                color: "#dddddd"
                                font.pixelSize:parent.font.pixelSize
                                text: "情况说明:"
                            }
                            anchors.horizontalCenter: parent.horizontalCenter
                            objectName:"deviceFaultNote"
                        }
                    }
                }

                CommonButton{
                    id:submmitBtn
                    text:os.i18n.ctr(qsTr("Submit"))
                    objectName:"deviceFaultReportSubmitBtn"
                    signal submitClicked()
                    onClicked:{
                        console.log("aaaaaaaaaaaaaaaaa  type  ", deviceFaultType.itemId, "   reason  ", deviceFaultReason.itemId)
                        ensureSubmitDlg.type = "submit";
                        ensureSubmitDlg.messageText = os.i18n.ctr(qsTr("No change after the submit information, sure to submit?"))
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
        titleText : os.i18n.ctr(qsTr("Fault report"))
        acceptedButtonText: os.i18n.ctr(qsTr("Ok"))//确定
        rejectButtonText: os.i18n.ctr(qsTr("Cancel"))//取消
        onAccepted:{
            if(type == "submit"){
                var serialKey = inspectionSystemConfig.getUUID();
                var multimediaFileList = faultImageList.getMultimediaData();
                var faultNote = deviceFaultLevel.text;
                var head = {"Objects" : "EMF_FaultRepair_Import_Sync"};
                var table = {"Name":"EMF_FaultRepair", "Key":"ID"};
                var dataRow = {
                    "ID" : inspectionSystemConfig.getUUID(),
                    "ReportOrganiseUnit" : inspectionSystemParam.getOrganiseUnitIDs(),
                    "EquipmentID" : deviceFaultReportPage.equipmentID,
                    "IsRFIDScan" : deviceFaultReportPage.paramValue["IsRFIDScan"],
                    "IsBarCodeScan" : deviceFaultReportPage.paramValue["IsBarCodeScan"],
                    "PDAID" : inspectionSystemParam.getPadId(),
                    "FaultType" : deviceFaultType.itemId,
                    "FaultReasonID" : deviceFaultReason.itemId,
                    "ReportUser" : inspectionSystemParam.getUserId(),
                    "Explain" : faultNote,
                    "CreatedBy" : inspectionSystemParam.getUserId(),
                    "ModifiedBy" : inspectionSystemParam.getUserId(),
                    "ERBQCode": deviceFaultReportPage.paramValue["ERBQCode"],
                    "CodeType": deviceFaultReportPage.paramValue["CodeType"],
                    "SerialKey" : serialKey
                };
                var dataRowList = [];
                dataRowList.push(dataRow);
                deviceFaultReportPage.actionSequence = inspectionSystemConfig.getSequence();
                transmitManager.resultOfResponse.disconnect(deviceFaultReportPage.resultResponse);
                transmitManager.resultOfResponse.connect(deviceFaultReportPage.resultResponse);
                transmitManager.submitData(head, table, dataRowList, multimediaFileList, serialKey, deviceFaultReportPage.actionSequence, "故障申报失败");
                ensureSubmitDlg.hide();
            } else if(type == "backToCancel"){
                    pageStack.pop();
            }else if(type == "homeToCancel"){
                    rootWindow.toHomeBack();
            }
        }
    }

    CListDialog{
        id: faultPageChiDlg
        property string showType:""
        maxVisibleItems:4
        titleAreaEnabled:false
        buttonAreaEnabled: false
        objectName:"deviceFaultReportchDlg"
        //model: // model

        onDelegateItemTriggered:{
            if(showType == "faultType"){
                console.log(" faultTypeList[index]  ",  faultTypeList[index])
                deviceFaultType.endTitle = faultTypeList[index];
                deviceFaultType.itemId = faultTypeDicItemList[index];
            }else if(showType == "faultReason"){
                deviceFaultReason.endTitle = faultReasonList[index];
                deviceFaultReason.itemId = faultReasonDicItemCodeList[index];
            }
            faultPageChiDlg.hide();
        }
    }
    function resultResponse(result, error, action, map){
        if(action != deviceFaultReportPage.actionSequence){
            console.log("FalutReport.qml  action != deviceFaultReportPage.actionSequence  return")
            return;
        }
        transmitManager.resultOfResponse.disconnect(resultResponse)
        if(Number(error) != -8){
            if(Number(error) == 0)
                CTOAST.requestToast("故障申报成功", "", "");
            pageStack.pop()
        }
    }
}
