/*!
* \file DeviceInventoryForm.qml
* \brief 设备盘点提交详情页面
*
*此界面是设备盘点提交详情页面，用户在该页面进行操作，将填报数据提交服务器
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
  \class DeviceInventoryForm
  \inModule 设备盘点
  \brief 设备盘点提交详情页面
  \ingroup InventoryTask
  此界面是设备盘点提交详情页面，用户在该页面进行操作，将填报数据提交服务器
*/
CommonPage {
    id: deviceInventoryPage

    /** type:string 设备名称*/
    property string equipmentNameStr: ""

    /** type:string 设备编码*/
    property string equipmentCodeStr: ""

    /** type:string 设备型号*/
    property string equipmentModelNameStr: ""

    /** type:string 设备厂商*/
    property string equipmentEnterpriseNameStr: ""

    /** type:string 设备分类*/
    property string equipmentClassNameStr: ""

    /** type:string 设备单位名称*/
    property string equipmentOrganiseUnitNameStr: ""

    /** type:string 设备保管人*/
    property string equipmentCustodianStr: ""

    /** type:bool 页面是否正在显示，主要用来出来back键*/
    property bool   pageShow: false

    /** type:bool 从服务器获取的数据解析是否正确*/
    property bool   dataError: false

    /** type:var 盘点项列表*/
    property var    modeList:[]

    /** type:var 盘点项索引列表*/
    property var    modeListIndex: []

    /** type:var 页面展示需要，但是没有包含在dataPackageValue数据中的数据*/
    property var    paramValue

    /** type:var 从服务器获取的调拨详情数据*/
    property var    dataPackageValue

    property string actionSequence: "-1"

    objectName:"deviceInventoryFormPage"
    statusBarHoldEnabled: true
    statusBarHoldItemColor: "white"
    orientationPolicy: CPageOrientation.LockPortrait
    clip: true
    Connections{
        target: rootWindow
        onKeyBackTriggered:{
            if(deviceInventoryPage.pageShow == true){
                if(deviceInventoryPage.dataError == true) {
                    pageStack.pop();
                } else {
                    ensureSubmitDlg.type = "backToCancel";
                    ensureSubmitDlg.messageText = os.i18n.ctr(qsTr("Abandon equipment inventory information"));
                    ensureSubmitDlg.show();
                }
            }
        }
        onToHome:{
            inventorysDialog.hide();
            ensureSubmitDlg.type = "homeToCancel";
            ensureSubmitDlg.messageText = os.i18n.ctr(qsTr("Abandon equipment inventory information"));
            ensureSubmitDlg.show();
        }
    }

    onStatusChanged: {
        if (status == CPageStatus.Show) {
            deviceInventoryPage.pageShow = true;
            rootWindow.dialogShow = true;
            deviceInventoryPage.statusBarHoldEnabled = true
            gScreenInfo.setStatusBarStyle("white");
            gScreenInfo.setStatusBar(true)
        }else if (status == CPageStatus.WillHide) {
            deviceInventoryPage.pageShow = false;
            rootWindow.dialogShow = false;
            ensureSubmitDlg.hide();
            inventorysDialog.hide();
        }
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
                //visible:false
                leftItemEnabled: true
                titleItemEnabled: true
                rightItemEnabled: false
                leftItemPixelSize: 22
                titleAreaCentered:true
                leftItemTextColor: "#666666"
                leftItemText:os.i18n.ctr(qsTr("Cancel"))
                leftItemIcon:""
                titleText:os.i18n.ctr(qsTr("Equipment inventory information"))
                onLeftItemTriggered: {
                    if(deviceInventoryPage.dataError == true)
                        pageStack.pop()
                    else {
                        ensureSubmitDlg.type = "backToCancel";
                        ensureSubmitDlg.messageText = os.i18n.ctr(qsTr("Abandon equipment inventory information"));
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
            visible: false
            clip:true
            contentHeight:totalCol.height + topToHeaderSeparatorItem.height
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
                    objectName: "deviceInventoryFormCol"
                    DeviceTitle{
                        deviceName1: deviceInventoryPage.equipmentNameStr
                        deviceLabel2: os.i18n.ctr(qsTr("Equipment code :"))//"设备编码:"
                        deviceName2: deviceInventoryPage.equipmentCodeStr
                        deviceLabel3: os.i18n.ctr(qsTr("Equipment model mame :"))//"设备型号:"
                        deviceName3: deviceInventoryPage.equipmentModelNameStr
                        deviceLabel4: os.i18n.ctr(qsTr("Equipment enterprise name :"))//"生产厂商:"
                        deviceName4: deviceInventoryPage.equipmentEnterpriseNameStr
                        deviceLabel5: os.i18n.ctr(qsTr("Equipment class name :"))//"设备分类:"
                        deviceName5: deviceInventoryPage.equipmentClassNameStr
                        deviceLabel6: os.i18n.ctr(qsTr("Equipment organiseUnit name :"))//"所属单位:"
                        deviceName6: deviceInventoryPage.equipmentOrganiseUnitNameStr
                        deviceLabel7: os.i18n.ctr(qsTr("Equipment custodian :"))//"保管人:"
                        deviceName7: deviceInventoryPage.equipmentCustodianStr

                    }
                    ListItem{
                        id:inventoryStatus
                        mainTitle: os.i18n.ctr(qsTr("Inventory status :"))
                        type:"category"
                        onClicked: {
                            //modeListIndex
                            var selectIndex = -1;
                            for(var i = 0; i < modeListIndex.length; i++) {
                                if(inventoryStatus.itemId == modeListIndex[i]) {
                                    selectIndex = i;
                                    break;
                                }
                            }
                            inventorysDialog.select([selectIndex], true);
                            inventorysDialog.show();
                        }
                    }

                    Rectangle{
                        id: imgNoteText
                        width: parent.width
                        height: visible? 200:0
                        //CTextArea{
                        CommonTextArea{
                            id:deviceInventoryNote
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
                                text: os.i18n.ctr(qsTr("Fill in information"))
                            }
                            anchors.horizontalCenter: parent.horizontalCenter
                            objectName:"deviceFaultNote"
                        }
                    }
                }

                CommonButton{
                    id: submmitBtn
                    signal submitClicked()
                    text: os.i18n.ctr(qsTr("Submit"))
                    objectName: "deviceInventoryFormSbmBtn"
                    onClicked:{
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
        id: ensureSubmitDlg
        property string type: ""
        titleText : os.i18n.ctr(qsTr("Equipment inventory information"))
        acceptedButtonText: os.i18n.ctr(qsTr("Ok"))
        rejectButtonText: os.i18n.ctr(qsTr("Cancel"))
        onAccepted:{
            if(type == "submit"){
                sendRequest();
            }else if(type == "backToCancel"){
               pageStack.pop();
            }else if (type == "homeToCancel")
                rootWindow.toHomeBack();
        }
    }

    CListDialog{
        id: inventorysDialog
        maxVisibleItems: 4
        titleAreaEnabled: false
        buttonAreaEnabled: false
        objectName:"deviceInventoryFormChDlg"
        onDelegateItemTriggered:{
            inventoryStatus.endTitle = modeList[index];
            inventoryStatus.itemId = modeListIndex[index];
            inventorysDialog.hide();
        }
    }

    /**
     * 提交盘点数据
     */
    function sendRequest() {
        var valueMap = {};
        valueMap["ID"] = dataPackageValue["tables"][0]["value"][0]["ID"];
        valueMap["TaskID"] = paramValue["TaskID"];
        valueMap["InventoryUser"] = inspectionSystemParam.getUserId();
        valueMap["InventoryResult"] = inventoryStatus.itemId;
        valueMap["IsRFIDScan"] = paramValue["IsRFIDScan"];
        valueMap["IsBarCodeScan"] = paramValue["IsBarCodeScan"];
        valueMap["PDAID"] = inspectionSystemParam.getPadId();
        valueMap["ReMark"] = deviceInventoryNote.text;
        valueMap["ModifiedBy"] = inspectionSystemParam.getUserId();

        deviceInventoryPage.actionSequence = inspectionSystemConfig.getSequence();
        transmitManager.resultOfResponse.disconnect(deviceInventoryPage.resultResponse)
        transmitManager.resultOfResponse.connect(deviceInventoryPage.resultResponse)
        var head = {"Objects" : "EMI_InventoryEquipment_Import"};
        var table = {"Name":"EMI_InventoryEquipment", "Key":"ID"};
        var attachments = [];
        var valueMapList = [];
        valueMapList.push(valueMap);
        transmitManager.submitData(head, table, valueMapList, attachments, "", deviceInventoryPage.actionSequence, os.i18n.ctr(qsTr("Inventory submit fail")));
    }

    /**
     * 接受服务器返回数据的槽函数
     * @param result:bool 获取数据是否成功
     * @param error:int 错误代码
     * @param action:string 当前发送数据动作类型
     * @param map:var    服务器返回值
     */
    function resultResponse(result, error, action, map) {
        console.debug("---deviceInventoryForm--onResultOfResponse--------------result, error, action: "
                      , result, error, action);
        if(action != deviceInventoryPage.actionSequence){
            console.log("action != deviceInventoryPage.actionSequence return")
            return;
        }
        transmitManager.resultOfResponse.disconnect(deviceInventoryPage.resultResponse)
        if(result == true && Number(error) == 0) {
            CTOAST.requestToast(os.i18n.ctr(qsTr("Inventory success")), "", "");
            pageStack.pop();
        }
    }
    Component.onCompleted: {
        rootWindow.toHomeFlag = 1;
        console.log("inventory  dataPackageValue[tables].length", dataPackageValue["tables"].length)
        deviceInventoryPage.dataError = true;
        if(dataPackageValue["tables"].length < 2){
            CTOAST.requestToast(os.i18n.ctr(qsTr("error: tables is empty")),"","");
            return;
        } else {
            if(dataPackageValue["tables"][0]["value"].length <= 0){
                CTOAST.requestToast(os.i18n.ctr(qsTr("error: tables[0].value is empty")),"","");
                return;
            }
            if(dataPackageValue["tables"][1]["value"].length <= 0){
                CTOAST.requestToast(os.i18n.ctr(qsTr("error: tables[1].value is empty")),"","");
                return;
            }
        }
        deviceInventoryPage.dataError = false;
        detailInfoFlick.visible = true;
        deviceInventoryPage.equipmentNameStr = dataPackageValue["tables"][0]["value"][0]["EquipmentName"];
        deviceInventoryPage.equipmentCodeStr = dataPackageValue["tables"][0]["value"][0]["EquipmentCode"];
        deviceInventoryPage.equipmentModelNameStr = dataPackageValue["tables"][0]["value"][0]["ModelName"];
        deviceInventoryPage.equipmentEnterpriseNameStr = dataPackageValue["tables"][0]["value"][0]["EnterpriseName"];
        deviceInventoryPage.equipmentClassNameStr = dataPackageValue["tables"][0]["value"][0]["ClassName"];
        deviceInventoryPage.equipmentOrganiseUnitNameStr = dataPackageValue["tables"][0]["value"][0]["OrganiseUnitName"];
        deviceInventoryPage.equipmentCustodianStr = dataPackageValue["tables"][0]["value"][0]["Custodian"];

        var initialData = dataPackageValue["tables"][1]["value"];
        inventoryStatus.endTitle = initialData[0]["Name"];
        inventoryStatus.itemId = initialData[0]["DicItemCode"];
        console.log("initialData is length is ", initialData.length)
        for(var i = 0; i < initialData.length; i++) {
            modeList.push(initialData[i]["Name"])
            modeListIndex.push(initialData[i]["DicItemCode"])
        }
        console.log("modeList is  ", modeList)
        inventorysDialog.model = modeList;
    }

    Component.onDestruction: {
        rootWindow.toHomeFlag = 0;
        transmitManager.resultOfResponse.disconnect(deviceInventoryPage.resultResponse);
    }

}
