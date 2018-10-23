/*!
* \file SearchPage.qml
* \brief 设备查询界面. 包括标题控件，设备扫描控件，设备查询控件
*\author 何昆 hekun@syberos.com
*\version 1.0
*\date 2015/9/8
*/
import QtQuick 2.0
import com.syberos.basewidgets 2.0
/*!
  \class SearchPage
  \inModule 公共控件
  \brief  设备查询界面
  \ingroup common
  设备查询界面. 包括标题控件，设备扫描控件，设备查询控件
*/
CommonPage {
    id: searchPage
    /** type:alias  设置页面标题*/
    property alias titleText: title.titleText
    /** type:string  頁面标识符;
        "Failure to declare"--故障申报界面;
        "device search"--设备查询界面;
        "Ops query"--运维查询;
        "Associated with RFID tags"--关联RFID
        "AssociatedWithPhoto"  --关联图片
    */
    property string pageActionType
    /**type: bool 是否支持扫描控件*/
    property bool enabledScanItem: false

    /** type:var  ScanItem控件对象*/
    property var scanItemPage:undefined
    property string actionSequence: "-1"
    property string isBarCodeScan: "0"
    property string isRFIDScan: "0"
    property string eRBQCode: ""
    property string codeType: ""
    statusBarHoldEnabled: true
    statusBarHoldItemColor: "white"
    orientationPolicy: CPageOrientation.LockPortrait
    clip: true

    signal resultBarCode(string barCode)
    signal resultRFIDCode(string rfidCode)

    onStatusChanged:{
        if(status == CPageStatus.Show){
            console.log("SearchPage.qml status == CPageStatus.Show ")
        }else if(status == CPageStatus.WillShow){
            console.log("SearchPage.qml status == CPageStatus.WillShow ")
        }else if(status == CPageStatus.WillHide){
            console.log("SearchPage.qml status == CPageStatus.WillHide ")
        }else if(status == CPageStatus.Hide){
             console.log("SearchPage.qml status == CPageStatus.Hide ")
        }
    }

    contentAreaItem:  Item {
        CommonTitlebar{
            id: title
            width: parent.width
            leftItemEnabled: true
            titleItemEnabled: true
            rightItemEnabled: false
            leftItemText:""
            onLeftItemTriggered: pageStack.pop()
            anchors.top: parent.top
            onLeftItemChanged:leftItem.backgroundEnabled = false
            leftItemBackgroundEnabled: false
        }
        SearchItem {
            id: saearchItem
            anchors.top: title.bottom
            nameRectVisible: searchPage.pageActionType !== "Associated with RFID tags" &&
                             searchPage.pageActionType !== "AssociatedWithPhoto"
            exRectVisible: searchPage.pageActionType !== "Associated with RFID tags" &&
                           searchPage.pageActionType !== "AssociatedWithPhoto"
            modelRectVisible: searchPage.pageActionType !== "Associated with RFID tags" &&
                              searchPage.pageActionType !== "AssociatedWithPhoto"
            onClickSearchButton: {
                rootWindow.closeKeyboard();
                getSearchResultList(name, code, model);
            }
        }
    }

    onResultBarCode:{
        //barCode
        console.log("searchPage  onResultBarCode  ", barCode, "  pageActionType  ", searchPage.pageActionType)
        searchPage.isBarCodeScan = "1";
        searchPage.isRFIDScan = "0";
        getDeviceDetail(barCode, "BarCode");
    }

    onResultRFIDCode: {
        //rfidCode
        console.log("searchPage  onResultRFIDCode  ", rfidCode, "  pageActionType  ", searchPage.pageActionType)
        searchPage.isBarCodeScan = "0";
        searchPage.isRFIDScan = "1";
        getDeviceDetail(rfidCode, "RFIDCode");
    }

    //用于点击查询按钮，查询出列表
    function getSearchResultList(name, code, model) {
        console.debug("------get SearchResultList-------------action, name, code, model",
                      pageActionType, name, code, model)

        searchPage.actionSequence = inspectionSystemConfig.getSequence();
        transmitManager.resultOfResponse.disconnect(resultResponse);
        transmitManager.resultOfResponse.connect(resultResponse);
        var  unitIDs = inspectionSystemParam.getOrganiseUnitIDs();

        var head = {};
        var para = {};
        para["EquipmentName"] = name;
        para["EquipmentCode"] = code;
        para["ModelName"] = model;
        para["OrganiseUnitIDs"] = unitIDs;

        if(searchPage.pageActionType == "Ops query") {
            head["Objects"] = "EMF_FaultRepair_Query_List";
            console.log("transmitManager.requestData   EMF_FaultRepair_Query_List ")
            transmitManager.requestData(head, para, searchPage.actionSequence, os.i18n.ctr(qsTr("Get ops device list fail")));

        } else if(searchPage.pageActionType == "device search") {
            head["Objects"] = "Equipment_QueryList_ByName";
            console.log("transmitManager.requestData   Equipment_ QueryList_ByName ")
            transmitManager.requestData(head, para, searchPage.actionSequence, os.i18n.ctr(qsTr("Get device list fail")));
        } else if(pageActionType == "Failure to declare") {
            head["Objects"] = "Equipment_QueryList_ByName";
             transmitManager.requestData(head, para, searchPage.actionSequence, os.i18n.ctr(qsTr("Get failure device list fail")));
        } else if(searchPage.pageActionType == "Associated with RFID tags" ) {
            var head1 = {"Objects" : "Equipment_ConRFID_Query_ByRFIDCode"};
            var para1 = {"EquipmentID" : "","KeyCode" : "", "EquipmentCode" : code, };
            transmitManager.requestData(head1, para1, searchPage.actionSequence, os.i18n.ctr(qsTr("Get associated with RFID tags device list fail")));
        } else if(searchPage.pageActionType == "AssociatedWithPhoto"){
            var head2 = {"Objects" : "Equipment_QueryOne"};
            var para2 = {"EquipmentID" : "", "KeyCode" : "", "EquipmentCode" : code, "OrganiseUnitID" : unitIDs};
            transmitManager.requestData(head2, para2, searchPage.actionSequence, "获取设备信息失败");
        }
    }

    /**
    *接收并处理RFID值。并根据action值，发送不同的请求命令到服务器
    * @param RFIDCode:string RFID扫描结果
    */
    function getDeviceDetail(code, codeType) {
        searchPage.actionSequence = inspectionSystemConfig.getSequence();
        var  unitIDs = inspectionSystemParam.getOrganiseUnitIDs();
        transmitManager.resultOfResponse.disconnect(resultResponse)
        transmitManager.resultOfResponse.connect(resultResponse)
        if(searchPage.pageActionType == "device search" || searchPage.pageActionType == "AssociatedWithPhoto") {
            var head1 = {"Objects" : "Equipment_QueryOne"};
            var para1 = {"EquipmentID" : "", "KeyCode" : code, "EquipmentCode" : "", "OrganiseUnitID" : unitIDs};
            console.log("transmitManager.requestData    Equipment_ QueryOne ")
            transmitManager.requestData(head1, para1, searchPage.actionSequence, os.i18n.ctr(qsTr("Get device fail")));
        } else if(searchPage.pageActionType == "Failure to declare") {
            var head2 = {"Objects" : "EMF_FaultRepair_Para"};
            var para2 = {"EquipmentID" : "", "KeyCode" : code, "OrganiseUnitID" : unitIDs};
            searchPage.eRBQCode = code;
            if(codeType == "BarCode"){
                searchPage.codeType = "3";
            } else {
                searchPage.codeType = "2";
            }
            console.log("transmitManager.requestData    EMF_FaultRepair_ Para ")
            transmitManager.requestData(head2, para2, searchPage.actionSequence, os.i18n.ctr(qsTr("Get fault repair para fail")));
        }
    }

    function resultResponse(result, error, action, map) {
        console.debug("searchapage -onResultOfResponse--------------result, error, action  actionSequence",
                      result, error, action, searchPage.actionSequence);

        if(action != searchPage.actionSequence){
            console.log("action != searchPage.actionSequence  return")
            return;
        }
        transmitManager.resultOfResponse.disconnect(resultResponse);
        if(map["head"]["objects"] == "Equipment_QueryOne" || map["head"]["objects"] == "Equipment_ConRFID_Query_ByRFIDCode") { //扫码或者RFID取单个设备详情
            //设备明细
            if(map["tables"].length <= 0 || map["tables"][0]["value"].length <= 0) {
                CTOAST.requestToast(os.i18n.ctr(qsTr("No device")),"","");
                return;
            }
            var titleText0 =  ""
            if(searchPage.pageActionType == "device search") {
                titleText0 = os.i18n.ctr(qsTr("Equipment archives"))
            } else if(searchPage.pageActionType == "Associated with RFID tags") {
                titleText0 = os.i18n.ctr(qsTr("Scanning to associate RFID tags"))
            }else if(searchPage.pageActionType == "AssociatedWithPhoto") {
                titleText0 = "关联图片"
            }
            pageStack.push("qrc:/qml/DeviceArchives.qml",
                           {"titleText":titleText0,
                               "pageActionType": searchPage.pageActionType,
                               "info":map["tables"][0]["value"][0]});
        }else if(map["head"]["objects"] == "EMF_FaultRepair_Para"){
            ////扫码或者RFID取单个设备的故障申报详情
            //故障申报
            if(map["tables"].length <= 0 || map["tables"][0]["value"].length <= 0) {
                CTOAST.requestToast(os.i18n.ctr(qsTr("No device")),"","");
                return;
            }
            var paramValue =  {
                "IsRFIDScan": searchPage.isRFIDScan,
                "IsBarCodeScan": searchPage.isBarCodeScan,
                "ERBQCode": searchPage.eRBQCode,
                "CodeType": searchPage.codeType}
            pageStack.push("qrc:/qml/DeviceOperationTask/FaultReport.qml", {"dataPackageValue": map,
                               "paramValue": paramValue});
        }else if(map["head"]["objects"] == "Equipment_QueryList_ByName"){
             //通过名字查询出设备列表 device search or Failure to declare
            //设备查询 or 故障申报查询列表
            console.log("objects  is  Equipment_QueryList_ByName")
            var titleText = "";
            if(searchPage.pageActionType == "device search") {
                titleText = os.i18n.ctr(os.i18n.ctr(qsTr("Device search")));
            }else if(searchPage.pageActionType == "Failure to declare") {
                titleText = os.i18n.ctr(os.i18n.ctr(qsTr("Failure to declare")));
            }
            var searchParam = {
                "titleText": titleText,
                "pageActionType": searchPage.pageActionType,
            }
            pageStack.push("qrc:/qml/Common/SearchBaseList.qml",{
                               "originalData":map,
                               "otherParam":searchParam
                           });
        }else if(map["head"]["objects"] == "EMF_FaultRepair_Query_List") {//通过名字查询出故障设备列表  Ops query
            //运维查询
            console.log("objects  is  EMF_FaultRepair_Query_List")
            var repairSearchParam = {
                "titleText": os.i18n.ctr(qsTr("Search Result")),
                "pageActionType": searchPage.pageActionType
            }
            pageStack.push("qrc:/qml/Common/SearchBaseList.qml",{
                               "originalData":map,
                               "otherParam":repairSearchParam
                           });
        }
    }


    Component.onCompleted: {
        console.debug("--------searchPage-----------Component.onCompleted-------action is ",
                      searchPage.pageActionType);
        if(enabledScanItem) {
            var com = Qt.createComponent("qrc:/qml/Common/ScanItem.qml");
            if(com.status == Component.Ready) {
                searchPage.scanItemPage = com.createObject(title);
                searchPage.scanItemPage.parentPage = searchPage;
                searchPage.scanItemPage.anchors.topMargin = title.height+5;
                searchPage.scanItemPage.anchors.top =  title.top;
                saearchItem.anchors.topMargin = searchPage.scanItemPage.height;
            }
        }
    }

    Component.onDestruction: {
        console.log("--------searchPage-----------Component.onDestruction-------")
        if(enabledScanItem && searchPage.scanItemPage){
            searchPage.scanItemPage.destroy();
            searchPage.scanItemPage = undefined;
        }
    }
}
