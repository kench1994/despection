/*!
* \file DeviceArchives.qml
* \brief 设备档案详情页面
*
*设备档案详情页面
*
*\author 陈瑞华 chenruihua@syberos.com
*\version 1.0
*\date 2015/9/9
*/
import QtQuick 2.0
import com.syberos.basewidgets 2.0
//import com.syberos.gpsmanager 1.0
import "./Common"
import "./Common/constants.js" as UI
import "./Multimedia"
/*!
  \class DeviceArchives
  \inModule qml
  \brief 设备档案详情页面
  \ingroup qml
  设备档案详情页面: 用于显示设备的型号/编码/厂商等基本信息
*/
CommonPage {
    id: deviceArchives

    /** type:alias  设备标题 */
    property alias titleText: title.titleText

    /** type:string   切换功能页面的比对标识符*/
    /** "Associated with RFID tags" "AssociatedWithPhoto"*/
    property string pageActionType

    /** type:real   指示文本距离左端的距离*/
    property real textLeftMargin: 40

    /** type:real   指示文本距离右端的距离*/
    property real textRightMargin: 20

    /** type:real   指示文本距离顶端的距离*/
    property real textTopMargin: 25

    /** type:real   指示文本距离底端的距离*/
    property real textBottomMargin: 25

     /** type:real   指示文本行间距*/
    property real textSpacing: 14

    /** type:real   指示文本字体大小*/
    property real textSize: 18

    /** type:color   指示文本颜色*/
    property color textColor: "#666666"

    /** type:var   指示消息内容*/
    property var info

    /** type:var   指示RFID码*/
    property var rfidValue

    /** type:var   指示窗口对象*/
    property var dialogObj

    /** type:int   flag*/
    property int flag: 0

    property bool gpsPowered: false

    property bool getGpsTimeOut: false

    property bool sendingDataToServer: false

    property string longitude: "0.0"

    property string latitude: "0.0"

    property int usedSatellite: 0

    /** type:var  ScanItem控件对象*/
    property var rfidItemPage:undefined

    property string actionSequence:"-1"

    /**
     * 转发RFID信号
     * @param /
     */
    signal rfidAccepted()

    /**
     * 隐藏RFID窗口信号
     * @param /
     */
    signal rfidDialogHide()

    statusBarHoldEnabled: true
    statusBarHoldItemColor: "white"
    orientationPolicy: CPageOrientation.LockPortrait
    clip: true

    signal resultBarCode(string barCode)
    signal resultRFIDCode(string rfidCode)


    onRfidAccepted: {
        deviceArchives.rfidItemPage.rfidEnabled = true;
        console.debug("----------------onRfidAccepted----------------  ");
//        waitGPSPosition.visible = true;
//        waitGpsIntervalTm.start();
//        gpsManager.slotSyncStart();
        var modifyId = inspectionSystemParam.getUserId();
        deviceArchives.actionSequence = inspectionSystemConfig.getSequence();
        transmitManager.resultOfResponse.disconnect(deviceArchives.resultResponse);
        transmitManager.resultOfResponse.connect(deviceArchives.resultResponse);

        var head = {"Objects" : "EMB_Equipment_QRBCode_Import"};
        var table = {"Name":"EMB_Equipment", "Key":"ID"};
        var dataRow = {
            "EquipmentID" : info["ID"],
            "KeyCode" : rfidValue,
            "CodeType" : "1",
            "ModifiedBy" : inspectionSystemParam.getUserId()
        };
        var dataRowList = [];
        dataRowList.push(dataRow);
        var attachments = [];
        transmitManager.submitData(head, table, dataRowList, attachments, "", deviceArchives.actionSequence, "");
    }

    onRfidDialogHide: {
        deviceArchives.rfidItemPage.rfidEnabled = true;
        console.debug("----------------onFfidDialogHide----------------");
        dialogObj.accepted.disconnect(deviceArchives.rfidAccepted);
        dialogObj.hideFinished.disconnect(deviceArchives.rfidDialogHide);
        dialogObj.destroy();
    }


    onResultRFIDCode:{
        //rfidCode
        deviceArchives.rfidValue = rfidCode;
        console.log("Device RFIDCode : ",info["RFIDCode"]);
        if( info["RFIDCode"] != deviceArchives.rfidValue) {
            // 根据rfid查询设备是否存在。
            deviceArchives.actionSequence = inspectionSystemConfig.getSequence();
            transmitManager.resultOfResponse.disconnect(deviceArchives.resultResponse);
            transmitManager.resultOfResponse.connect(deviceArchives.resultResponse);
            var unitIDs = inspectionSystemParam.getOrganiseUnitIDs();

            var head1 = {};
            head1["Objects"] = "Equipment_QueryOne";
            var para1 = {};
            para1["EquipmentID"] = "";
            para1["KeyCode"] = deviceArchives.rfidValue;
            para1["EquipmentCode"] = "";
            para1["OrganiseUnitID"] = unitIDs;
            console.log("transmitManager.requestData    Equipment_ QueryOne ")
            transmitManager.requestData(head1, para1, deviceArchives.actionSequence, os.i18n.ctr(qsTr("Get associated with RFID tags device list fail") ));
        }else{
            showDialog(4,rfidCode);
        }
    }

    /**
     * 返回应答结果
     * @param result   应答结果
     * @param error    错误类型码
     * @param action  功能切换比对标识符
     * @param map     数据库返回值
     */
    function resultResponse(result, error, action, map) {
        console.debug("----resultResponse-----onResultOfResponse----000----------result, error, action ", result, error, action);
        if(action != deviceArchives.actionSequence){
            console.log("action != deviceArchives.actionSequence  return")
            return;
        }
        transmitManager.resultOfResponse.disconnect(deviceArchives.resultResponse)
        if(result && error == 0) {
            if( map["objects"] != undefined && map["objects"] == "EMB_Equipment_QRBCode_Import"){
                if(flag == 1){
                    CTOAST.requestToast(os.i18n.ctr( qsTr( "Associated success" ) ), "", "" );
                }else if(flag == 2){
                    CTOAST.requestToast(os.i18n.ctr( qsTr( "Replace success" ) ), "", "" );
                }
                pageStack.pop();
                return;
            } else if(map["head"]["objects"] != undefined && map["head"]["objects"] == "Equipment_QueryOne"){
                var values = map["tables"][0]["value"][0];
                if(values == undefined) {
                    if(info["RFIDCode"] == ""){
                        // 该设备未设置rfid
                        showDialog(1,deviceArchives.rfidValue);
                    } else if( info["RFIDCode"] != deviceArchives.rfidValue) {
                        showDialog(2,deviceArchives.rfidValue);
                    }
                } else {
                    showDialog(3,deviceArchives.rfidValue);
                }
            }
        }else {
            //关联失败，此RFID码可能已经被其他单位下的设备使用
            CTOAST.requestToast("Associated fail, RFID code has attached by other device", "", "");
            console.log("DeviceArchives--resultResponse: failed.result:%1 error:%2",result,error);
        }
    }

    function associatedPhotoresultResponse(result, error, action, map) {
        console.debug("associatedPhotoresultResponse----result, error, action ", result, error, action);
        if(action != deviceArchives.actionSequence){
            console.log("action != deviceArchives.actionSequence  return")
            return;
        }
        transmitManager.resultOfResponse.disconnect(deviceArchives.associatedPhotoresultResponse)
        if(result && error == 0) {
            CTOAST.requestToast("关联图片成功", "", "");
            pageStack.pop();
        }
    }

    /**
     * 根据RFID类型,关联RFID码，显示相应窗口
     * @param RFID类型
     * @param RFID码
     */
    function showDialog(rfidType,RFIDCode) {
        deviceArchives.rfidItemPage.rfidEnabled = false;
        switch(rfidType) {
        case 1:
            //是否关联如下RFID标签\n序列号：
            flag = 1;
            dialogObj = Qt.createQmlObject('import QtQuick 2.0;import com.syberos.basewidgets 2.0;CDialog{}', deviceArchives);
            dialogObj.titleText = os.i18n.ctr(qsTr("Associated with RFID tags"));    //关联RFID标签
            dialogObj.messageText = os.i18n.ctr(qsTr("Is associated whith RFID tags\n RFID is:"))+deviceArchives.rfidValue;
            dialogObj.acceptedButtonText = os.i18n.ctr(qsTr("Associate"));           //关联
            dialogObj.rejectButtonText = os.i18n.ctr(qsTr("Cancel"));             //取消
            dialogObj.accepted.connect(deviceArchives.rfidAccepted)
            dialogObj.hideFinished.connect(deviceArchives.rfidDialogHide)
            dialogObj.show();
            break;
        case 2:
             //当前设备已有RFID标签，是否使用如下RFID标签替换？\n序列号：
            flag = 2;
            dialogObj = Qt.createQmlObject('import QtQuick 2.0;import com.syberos.basewidgets 2.0;CDialog{}', deviceArchives);
            dialogObj.titleText = os.i18n.ctr(qsTr("Associated with RFID tags"));    //关联RFID标签
            dialogObj.messageText = os.i18n.ctr(qsTr("The device already had RFID tags, shell we replace it?\n RFID is:")) + deviceArchives.rfidValue;
            dialogObj.acceptedButtonText = os.i18n.ctr(qsTr("Replace"));   //替换
            dialogObj.rejectButtonText = os.i18n.ctr(qsTr("Cancel"));     //取消
            dialogObj.accepted.connect(deviceArchives.rfidAccepted)
            dialogObj.hideFinished.connect(deviceArchives.rfidDialogHide)
            dialogObj.show();
            break;
        case 3:
            //该RFID标签关联过设备，无法再次关联\n序列号：
            dialogObj = Qt.createQmlObject('import QtQuick 2.0;import com.syberos.basewidgets 2.0;CAlertDialog{}', deviceArchives);
            dialogObj.titleText = os.i18n.ctr(qsTr("Associated with RFID tags"));        //关联RFID标签
            dialogObj.messageText = os.i18n.ctr(qsTr("The RFID already has been accocaited with device,\nCan`t accocaite again.RFID is:")) + deviceArchives.rfidValue;
            dialogObj.alertButtonText = os.i18n.ctr(qsTr("I known"));  //知道了
            dialogObj.hideFinished.connect(deviceArchives.rfidDialogHide)
            dialogObj.show();
            break;
        case 4:
            //该RFID标签和设备RFID相同
            dialogObj = Qt.createQmlObject('import QtQuick 2.0;import com.syberos.basewidgets 2.0;CAlertDialog{}', deviceArchives);
            dialogObj.titleText = os.i18n.ctr(qsTr("Associated with RFID tags"));        //关联RFID标签
            dialogObj.messageText = os.i18n.ctr(qsTr("The same RFID and equipment.\nRFID is:")) + deviceArchives.rfidValue;
            dialogObj.alertButtonText = os.i18n.ctr(qsTr("I known"));  //知道了
            dialogObj.hideFinished.connect(deviceArchives.rfidDialogHide)
            dialogObj.show();
            break;
        default:
            console.log("Unsupported rfidType", deviceArchives.rfidValue);
            break;
        }
    }

    contentAreaItem:  Item {
        id: content
        CommonTitlebar{
            id: title
            width: parent.width
            leftItemEnabled: true
            titleItemEnabled: true
            rightItemEnabled: false
            leftItemText:""
            titleText: os.i18n.ctr( qsTr("Equipment archives") )
            onLeftItemTriggered: {
                if(deviceArchives.pageActionType == "AssociatedWithPhoto"){
                    ensureSubmitDlg.type = "backToCancel"
                    ensureSubmitDlg.messageText = "确定放弃图片关联吗？";
                    ensureSubmitDlg.show();
                } else
                    pageStack.pop()
            }
            anchors.top: parent.top
            onLeftItemChanged:leftItem.backgroundEnabled = false
            leftItemBackgroundEnabled: false
        }
        Rectangle {
            width: parent.width
            anchors{
                top: title.bottom
                bottom: parent.bottom
            }
            color:"#eeeff1"
        }
        Flickable{
            id:contentFlick
            anchors.top: title.bottom
            anchors.topMargin: 20
            anchors.bottom:  parent.bottom
            width: parent.width
            clip:true
            contentHeight:contentCol.height + addImageCol.height
            Column{
                id:contentCol
                width: parent.width
                anchors.left: parent.left
                anchors.leftMargin: textLeftMargin
                anchors.right: parent.right
                anchors.rightMargin: textRightMargin
                spacing: 20
                Text {
                    id: equipmentName
                    wrapMode: Text.WrapAnywhere
                    width: parent.width
                    text: info["EquipmentName"]
                    font.pixelSize: textSize
                    font.family: UI.DEFAULT_FONT_FAMILY
                    color: textColor
                }
                Text {
                    id: modelName
                    wrapMode: Text.WrapAnywhere
                    width: parent.width
                    text: os.i18n.ctr(qsTr("ModelName:")) +info["ModelName"]
                    font.pixelSize: textSize
                    font.family: UI.DEFAULT_FONT_FAMILY
                    color: textColor
                }
                Text {
                    id: equipmentCode
                    wrapMode: Text.WrapAnywhere
                    width: parent.width
                    text: os.i18n.ctr(qsTr("EquipmentCode:")) + info["EquipmentCode"]
                    font.pixelSize: textSize
                    font.family: UI.DEFAULT_FONT_FAMILY
                    color: textColor
                }
                Text {
                    id: manufacturerName
                    wrapMode: Text.WrapAnywhere
                    width: parent.width
                    text: os.i18n.ctr(qsTr("ManufacturerName:")) + info["ManufacturerName"]
                    font.pixelSize: textSize
                    font.family: UI.DEFAULT_FONT_FAMILY
                    color: textColor
                }
                Text {
                    id: deviceType
                    wrapMode: Text.WrapAnywhere
                    width: parent.width
                    text: os.i18n.ctr(qsTr("ClassName: ")) + info["ClassName"]//设备分类名称
                    font.pixelSize: textSize
                    font.family: UI.DEFAULT_FONT_FAMILY
                    color: textColor
                }
                Text {
                    id: assetsType
                    wrapMode: Text.WrapAnywhere
                    width: parent.width
                    text: os.i18n.ctr(qsTr("EquipmentSeries: ")) + info["EquipmentSeries"]//设备系列
                    font.pixelSize: textSize
                    font.family: UI.DEFAULT_FONT_FAMILY
                    color: textColor
                }
                Text {
                    id: productionDate
                    wrapMode: Text.WrapAnywhere
                    width: parent.width
                    text: os.i18n.ctr(qsTr("Date of purchase:")) + info["BuyDate"]//购入日期
                    font.pixelSize: textSize
                    font.family: UI.DEFAULT_FONT_FAMILY
                    color: textColor
                }
                Text {
                    id: storagePosition
                    wrapMode: Text.WrapAnywhere
                    width: parent.width
                    text: os.i18n.ctr(qsTr("Positions: ")) + info["OrganiseUnitName"]//设备存放位置
                    font.pixelSize: textSize
                    font.family: UI.DEFAULT_FONT_FAMILY
                    color: textColor
                }
                Text {
                    id: custodian
                    wrapMode: Text.WrapAnywhere
                    width: parent.width
                    text: os.i18n.ctr(qsTr("Cudation:")) + info["Custodian"]//保管人
                    font.pixelSize: textSize
                    font.family: UI.DEFAULT_FONT_FAMILY
                    color: textColor
                }
                Item{
                    width: parent.width
                    height: 20
                }
            }
            Column{
                id:addImageCol
                anchors.top: contentCol.bottom
                width: parent.width
                MultimediaView{
                    id:associatedPhotoItem
                    anchors.bottomMargin: 20
                    parentPageId: deviceArchives
                    actionType: "associatedPhoto"
                    visible:deviceArchives.pageActionType == "AssociatedWithPhoto"
                }
                Rectangle{
                    id:submitRect
                    width: parent.width
                    height: visible? 100:0
                    visible:deviceArchives.pageActionType == "AssociatedWithPhoto"
                    CommonButton{
                        id:submitListBtn
                        enabled:associatedPhotoItem.phmodel.count > 0
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        text:"关联图片"
                        onClicked:{
                            ensureSubmitDlg.type = "submit";
                            ensureSubmitDlg.messageText = "提交后信息不可修改，确定提交吗?"
                            ensureSubmitDlg.show();
                        }
                    }
                }
            }

        }
    }
    Connections{
        target: rootWindow
        onKeyBackTriggered:{
            if(deviceArchives.status == CPageStatus.Show && deviceArchives.pageActionType == "AssociatedWithPhoto"){
                ensureSubmitDlg.type = "backToCancel";
                ensureSubmitDlg.messageText = "确定放弃图片关联吗？";
                ensureSubmitDlg.show();
            }
        }
        onToHome:{
            if(deviceArchives.status == CPageStatus.Show && deviceArchives.pageActionType == "AssociatedWithPhoto"){
                ensureSubmitDlg.type = "homeToCancel";
                ensureSubmitDlg.messageText = "确定放弃图片关联吗？";
                ensureSubmitDlg.show();
            }
        }
    }

    CDialog{
        id:ensureSubmitDlg
        property string type: ""
        titleText : "关联图片"
        acceptedButtonText: "确定"
        rejectButtonText: "取消"
        onAccepted:{
            if(type == "submit"){
                deviceArchives.actionSequence = inspectionSystemConfig.getSequence();
                transmitManager.resultOfResponse.disconnect(deviceArchives.associatedPhotoresultResponse);
                transmitManager.resultOfResponse.connect(deviceArchives.associatedPhotoresultResponse);
                var serialKey = info["SerialKey"];
                if(serialKey == undefined || serialKey == "")
                    serialKey = inspectionSystemConfig.getUUID();
                var head = {"Objects" : "EMB_Equipment_UpdateSerialKey"};
                var table = {"Name":"EMB_Equipment", "Key":"ID"};
                var dataRow = {
                    "EquipmentID" : info["ID"],
                    "SerialKey" : serialKey,
                    "ModifiedBy" : inspectionSystemParam.getUserId()
                };
                var dataRowList = [];
                dataRowList.push(dataRow);
                var attachments = associatedPhotoItem.getMultimediaData();
                transmitManager.submitData(head, table, dataRowList, attachments, serialKey, deviceArchives.actionSequence, "关联图片失败");
                ensureSubmitDlg.hide();
            }else if(type == "backToCancel"){
                    pageStack.pop();
            }else if(type == "homeToCancel"){
                    rootWindow.toHomeBack();
            }
        }
    }

    onStatusChanged: {
        if (status == CPageStatus.Show) {
            if(deviceArchives.pageActionType == "AssociatedWithPhoto")
                rootWindow.dialogShow = true;
            deviceArchives.statusBarHoldEnabled = true
            gScreenInfo.setStatusBarStyle("white");
            gScreenInfo.setStatusBar(true)
        }else if (status == CPageStatus.Hide){
            console.log("DeviceArchives.qml  status == CPageStatus.Hide  ")
            if(deviceArchives.pageActionType == "AssociatedWithPhoto")
                rootWindow.dialogShow = false;
        }else if (status == CPageStatus.WillHide){
            ensureSubmitDlg.hide();
        }
    }
    Component.onCompleted:{
        console.debug("-----------deviceArchives--------Component.onCompleted-----")
        if( deviceArchives.pageActionType == "Associated with RFID tags"){
            var com = Qt.createComponent("qrc:/qml/Common/ScanItem.qml");
            if(com.status == Component.Ready){
                deviceArchives.rfidItemPage = com.createObject(content);
                deviceArchives.rfidItemPage.parentPage = deviceArchives;
                deviceArchives.rfidItemPage.barCodeVisible = false;
                deviceArchives.rfidItemPage.anchors.bottom = content.bottom;
            }
        } else if(deviceArchives.pageActionType == "AssociatedWithPhoto")
            rootWindow.toHomeFlag = 1;
    }

    Component.onDestruction: {
        //console.log("DeviceArchives destruction deviceArchives.rfidItemPage ", deviceArchives.rfidItemPage)
        if(deviceArchives.rfidItemPage){
            console.log("DeviceArchives destruction deviceArchives.rfidItemPage destroy");
            deviceArchives.rfidItemPage.destroy();
            deviceArchives.rfidItemPage = undefined;
        }
        rootWindow.toHomeFlag = 0;
    }
}
