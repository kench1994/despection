import QtQuick 2.0
import com.syberos.basewidgets 2.0

import "../Common"

CommonPage {
    id: scanTask
    property string isRFIDCodeScan: "0"
    property string isBarCodeScan: "0"
    property string scanCode: ""
    property string actionSequence: "-1"
    property bool refresh: true
    signal resultRFIDCode(string rfidCode)
    signal resultBarCode(string barCode)
    Component.onCompleted: {
        scanTask.refresh = false;
        var empty = [];
        scanInspectionModel.setListData(empty)
        scanOprationModel.setListData(empty)
        scanRectificationModel.setListData(empty)
        scanMaintainModel.setListData(empty)
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
            titleText: "扫码任务"
            onLeftItemTriggered: pageStack.pop()
            anchors.top: parent.top
            onLeftItemChanged:leftItem.backgroundEnabled = false
            leftItemBackgroundEnabled: false
        }
        Rectangle {
            id: backGroundRect
            color: "#eff1f3"
            anchors.top: title.bottom
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            visible: false
            Flickable {
                id: taskInfo
                width: parent.width
                anchors.fill: parent
                flickDeceleration: Flickable.VerticalFlick
                clip: true
                contentHeight: totalCol.height
                Column{
                    id: totalCol
                    anchors.top:parent.top
                    width: parent.width
                    spacing: 10
                    Item{
                        id: inspectionItem
                        width: parent.width
                        height: inspectionText.height + inspectionList.height
                        visible: inspectionSystemParam.checkUserPermission("Inspection")
                        ListItem {
                            id: inspectionText
                            anchors.top: parent.top
                            mainTitle: "设备巡检"
                            type: "category"
                            endImage: ""
                            headImage: "qrc:/icon/2icon_17.png"
                            onClicked: {
                                inspectionList.visible = !inspectionList.visible
                            }
                        }
                        ListView {
                            id: inspectionList
                            anchors.top: inspectionText.bottom
                            anchors.left: parent.left
                            anchors.leftMargin: 20
                            anchors.right: parent.right
                            height: visible?contentHeight:0
                            model: scanInspectionModel
                            delegate: inspectionTaskDelegateCom
                            interactive: false
                        }
                    }
                    Item{
                        id: rectificationItem
                        clip: true
                        width: parent.width
                        height: rectificationText.height + rectificationList.height
                        visible: inspectionSystemParam.checkUserPermission("Standardization")
                        ListItem {
                            id: rectificationText
                            anchors.top: parent.top
                            mainTitle: "规范检查"
                            type: "category"
                            endImage: ""
                            headImage: "qrc:/icon/2icon_27.png"
                            onClicked: {
                                rectificationList.visible = !rectificationList.visible
                            }
                        }
                        ListView {
                            id: rectificationList
                            anchors.top: rectificationText.bottom
                            anchors.left: parent.left
                            anchors.leftMargin: 20
                            anchors.right: parent.right
                            height: visible?contentHeight:0
                            model: scanRectificationModel
                            delegate: rectificationDelegateCom
                            interactive: false
                        }
                    }
                    Item{
                        id: oprationItem
                        clip: true
                        width: parent.width
                        height: oprationText.height + oprationList.height
                        visible: inspectionSystemParam.checkUserPermission("Operation")
                        ListItem {
                            id: oprationText
                            anchors.top: parent.top
                            mainTitle: "设备维修"
                            type: "category"
                            endImage: ""
                            headImage: "qrc:/icon/2icon_05.png"
                            onClicked: {
                                oprationList.visible = !oprationList.visible
                            }
                        }
                        ListView {
                            id: oprationList
                            anchors.top: oprationText.bottom
                            anchors.left: parent.left
                            anchors.leftMargin: 20
                            anchors.right: parent.right
                            height: visible?contentHeight:0
                            model: scanOprationModel
                            delegate: oprationDelegateCom
                            interactive: false
                        }
                    }
                    Item{
                        id: maintainItem
                        clip: true
                        width: parent.width
                        height: maintainText.height + maintainList.height
                        visible: inspectionSystemParam.checkUserPermission("Maintenance")
                        ListItem {
                            id: maintainText
                            anchors.top: parent.top
                            mainTitle: "设备保养"
                            type: "category"
                            endImage: ""
                            headImage: "qrc:/icon/2icon_19.png"
                            onClicked: {
                                maintainList.visible = !maintainList.visible
                            }
                        }
                        ListView {
                            id: maintainList
                            anchors.top: maintainText.bottom
                            anchors.left: parent.left
                            anchors.leftMargin: 20
                            anchors.right: parent.right
                            height: visible?contentHeight:0
                            model: scanMaintainModel
                            delegate: maintainDelegateCom
                            interactive: false
                        }
                    }
                }
            }
        }
        ScanItem {
            id: scanItem
            showLine: false
            anchors.bottom: parent.bottom
            parentPage: scanTask
        }
        Text {
            id: promptText
            anchors.top: title.bottom
            anchors.bottom: scanItem.top
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            width: parent.width
            font.pixelSize: 30
            color: "grey"
            text: "......请选择扫码方式......"
        }
        Component {
            id: inspectionTaskDelegateCom
            TwoLineItem {
                id: inspectionDelegate
                property string actionSequence: "-1"
                /*
                InspectionMode: 1:普通设备；2：复合对象巡检；3：路线巡检; 4：巡检点巡检;5：组对象巡检;
                为1时，直接进入设备列表
                */
                opacity: mouseEnable? 1: 0.5
                firstLeftText: ObjectName
                secondLeftText:  "位置:" + Positions
                secondRightText: "型号:" + ModelName
                rFIDCode:RFIDCode
                barCode:BarCode
                onTriggered: {
                    console.debug("---TaskObjectDataKey: ",TaskObjectDataKey, "  twolimitMode ", twolimitMode, " rFIDCode  ", rFIDCode, " barCode ", barCode,
                                  " judge is ", (twolimitMode === "1" && rFIDCode === "" && barCode === "") || twolimitMode === "0");

                    if(InspectionMode == "3" && IsClick == "0") {
                        CTOAST.requestToast("请按照路线进行巡检","","");
                        return;
                    } else if(InspectionMode == "5") {
                        //组对象巡检模式扫码后返回的是组代表，点击进入整个组设备列表页面
                        inspectionDelegate.actionSequence = inspectionSystemConfig.getSequence();
                        transmitManager.resultOfResponse.disconnect(inspectionDelegate.resultResponse)
                        transmitManager.resultOfResponse.connect(inspectionDelegate.resultResponse)
                        var head = {"Objects" : "EISM_GetTaskObjectListByGroup_RFID"};
                        var para = {"TaskGroupDataKey" : TaskGroupDataKey};
                        console.log("transmitManager.requestData   EISM_GetTaskObjectListByGroup_RFID ")
                        transmitManager.requestData(head, para, inspectionDelegate.actionSequence, "获取巡检组对象任务项失败");
                    } else {
                        inspectionDelegate.actionSequence = inspectionSystemConfig.getSequence();
                        transmitManager.resultOfResponse.disconnect(inspectionDelegate.resultResponse)
                        transmitManager.resultOfResponse.connect(inspectionDelegate.resultResponse)
                        var head1 = {"Objects" : "EISM_GetTaskObjectDetail"};
                        var para1 = {"TaskObjectDataKey" : TaskObjectDataKey};
                        console.log("transmitManager.requestData   EISM_GetTaskObjectDetail ")
                        transmitManager.requestData(head1, para1, inspectionDelegate.actionSequence, "获取巡检任务项明细失败");
                    }
                }
                function resultResponse(result, error, action, map){
                    console.debug("---inspectionDelegate---onResultOfResponse---result, error: action  actionSequece",
                                  result, error, action, inspectionDelegate.actionSequence);
                    if(action != inspectionDelegate.actionSequence){
                        console.log("action != inspectionDelegate  return")
                        return;
                    }

                    transmitManager.resultOfResponse.disconnect(inspectionDelegate.resultResponse)
                    if(result && error == 0){
                        if(InspectionMode == "5"){
                            var otherParam = {
                                "TaskGroupName": "组对象巡检",
                                "TaskGroupDataKey": TaskGroupDataKey,
                                "TaskDataKey" : TaskDataKey,
                                "LimitMode": "0",
                                "EnableUploadAttach": EnableUploadAttach,
                                "EnabledScanItem": true,
                                "InspectionMode": InspectionMode
                            }
                            pageStack.push("qrc:/qml/InspectionTask/InspectionBaseList.qml",
                                           {
                                               "originalData": map,
                                               "otherParam":otherParam,
                                           });
                        } else {
                            var paramValue =  {
                                "IsRFIDScan": isRFIDCodeScan,
                                "IsBarCodeScan": isBarCodeScan,
                                "EnableUploadAttach": EnableUploadAttach,
                                "PageFormType":"inspection",
                                "InspectionMode": InspectionMode,
                                "isEditGroupData" : "0",
                                "IsDelegate": "0",
                                "SelectedIndex" : 0,
                                "ParentPage":scanTask
                            };
                            pageStack.push("qrc:/qml/InspectionTask/DeviceInspectionForm.qml",
                                           {
                                               "dataPackageValue": map,
                                               "paramValue": paramValue
                                           });
                        }
                    }
                }
            }
        }
        Component {
            id: oprationDelegateCom
            TwoLineItem {
                id: oprationDelegate
                property var  statusMap: {"1":"维修受理" ,"2":"维修审批", "3":"维修派工", "4":"维修登记", "5":"维修费用","6":"维修评价"}
                property var  faultTypeMap: {"1":"运行损坏" ,"2":"意外损坏", "3":"事故损坏", "4":"其他原因"}
                property string actionSequence: "-1"
                firstLeftText: EquitmentName
                firstRightText: "型号:" + ModelName
                secondLeftText: "编号:" + EquipmentCode
                secondRightText: "厂商:" + EnterpriseName
                onTriggered: {
                    oprationDelegate.actionSequence = inspectionSystemConfig.getSequence();
                    transmitManager.resultOfResponse.disconnect(oprationDelegate.resultResponse)
                    transmitManager.resultOfResponse.connect(oprationDelegate.resultResponse)
                    var head1 = {"Objects" : "EMF_FaultRepair_TaskAllInfo"};
                    var para1 = {"ID" : ID, "UserCode" : inspectionSystemParam.getUserPropertyByKey("UserCode"), "Status":Status};
                    console.log("transmitManager.requestData   EMF_FaultRepair_TaskAllInfo ")
                    transmitManager.requestData(head1, para1, oprationDelegate.actionSequence, "获取维修任务项明细失败");
                }
                function resultResponse(result, error, action, map){
                    console.debug("---oprationDelegate---onResultOfResponse---result, error: ", result, error);
                    if(action != oprationDelegate.actionSequence){
                        console.log("action != oprationDelegate.actionSequence return")
                        return;
                    }
                    transmitManager.resultOfResponse.disconnect(oprationDelegate.resultResponse)
                    if(result && error == 0){
                        var otherParam = {
                            "EquipmentCode" : EquipmentCode,
                            "Taskstatus": Status,
                            "TitleName": statusMap[Status],
                            "TaskID" : ID,
                            "ParentPage":scanTask};
                        pageStack.push("qrc:/qml/DeviceOperationTask/FaultRepairMainPage.qml",
                                       {"originalData": map,
                                           "otherParam": otherParam
                                       });
                    }
                }
            }
        }
        Component {
            id: rectificationDelegateCom
            TwoLineItem {
                id: rectificationDelegate
                property string actionSequence: "-1"
                firstLeftText: ItemName
                secondLeftText: ItemFullName
                rFIDCode: RFIDCode
                barCode:  BarCode
                onTriggered: {
                    rectificationDelegate.actionSequence = inspectionSystemConfig.getSequence();
                    transmitManager.resultOfResponse.disconnect(rectificationDelegate.resultResponse)
                    transmitManager.resultOfResponse.connect(rectificationDelegate.resultResponse)
                    var head = {"Objects" : "EXAM_ExamineTaskItemDetail"};
                    var para = {"TaskItemDataKey" : TaskItemDataKey};
                    console.log("transmitManager.requestData   EXAM_ExamineTaskItemDetail ")
                    transmitManager.requestData(head, para, rectificationDelegate.actionSequence, "获取规范检查任务项明细失败");
                }
                function resultResponse(result, error, action, map){
                    console.debug("---rectificationDelegate ---Response---result, error: ", result, error);
                    if(action != rectificationDelegate.actionSequence){
                        console.log("action != rectificationDelegate.actionSequence  return");
                        return;
                    }
                    transmitManager.resultOfResponse.disconnect(rectificationDelegate.resultResponse)
                    if(result && error == 0){
                        var paramValue =  {
                            "IsRFIDScan": isRFIDCodeScan,
                            "IsBarCodeScan": isBarCodeScan,
                            "TaskObjectDataKey": TaskObjectDataKey,
                            "EnableUploadAttach": EnableUploadAttach,
                            "parentPage":scanTask
                        }
                        pageStack.push("qrc:/qml/StandardizedTask/DeviceStdCheckForm.qml", {"dataPackageValue": map,
                                           "paramValue": paramValue});
                    }
                }

            }
        }
        Component {
            id: maintainDelegateCom
            TwoLineItem {
                id: maintainDelegate
                property string actionSequence: "-1"
                firstLeftText: ObjectName
                secondLeftText: "型号:" + ModelName
                secondRightText: "位置:" + Positions
                rFIDCode: RFIDCode
                barCode: BarCode
                onTriggered: {
                    maintainDelegate.actionSequence = inspectionSystemConfig.getSequence();
                    transmitManager.resultOfResponse.disconnect(maintainDelegate.resultResponse)
                    transmitManager.resultOfResponse.connect(maintainDelegate.resultResponse)
                    var head = {"Objects" : "EMM_GetTaskObjectDetail"};
                    var para = {"TaskObjectDataKey" : TaskObjectDataKey};
                    console.log("transmitManager.requestData   EMM_GetTaskObjectDetail")
                    transmitManager.requestData(head, para, maintainDelegate.actionSequence, "获取保养任务项明细失败");
                }
                function resultResponse(result, error, action, map){
                    console.debug("--ItemListDelegate--onResultOfResponse--result, error: ", result, error);
                    if(action != maintainDelegate.actionSequence){
                        console.log("action != maintainDelegate.actionSequence  return")
                        return;
                    }
                    transmitManager.resultOfResponse.disconnect(maintainDelegate.resultResponse)
                    if(result && error == 0){
                        console.debug("---map:", map)
                        var paramValue =  {
                            "IsRFIDScan": isRFIDCodeScan,
                            "IsBarCodeScan": isBarCodeScan,
                            "EnableUploadAttach": img,
                            "pageFormType":"maintain",
                            "parentPage":scanTask,
                            "TaskObjectDataKey":TaskObjectDataKey
                        }
                        pageStack.push("qrc:/qml/DeviceMaintainTask/DeviceMaintainForm.qml", {"dataPackageValue": map,
                                           "paramValue": paramValue});
                    }
                }
            }
        }
    }
    onResultRFIDCode : {
        console.debug("--------------RFIDCode:", rfidCode)
        isRFIDCodeScan = "1";
        isBarCodeScan = "0";
        scanCode = rfidCode;
        var head = {"Objects": "Equipment_ScanTaskListByRFID"};
        var para = {"RFIDCode" : rfidCode, "UserId": inspectionSystemParam.getUserId()};
        console.log("transmitManager.requestData   Equipment_ScanTaskListByRFID ")
        scanTask.actionSequence = inspectionSystemConfig.getSequence();
        transmitManager.resultOfResponse.disconnect(scanTask.resultResponse)
        transmitManager.resultOfResponse.connect(scanTask.resultResponse)
        transmitManager.requestData(head, para, scanTask.actionSequence, "");
    }
    onResultBarCode: {
        console.debug("--------------BarCode: ", barCode)
        isRFIDCodeScan = "0";
        isBarCodeScan = "1";
        scanCode = barCode;
        var head = {"Objects": "Equipment_ScanTaskListByRFID"};
        var para = {"RFIDCode" : barCode, "UserId": inspectionSystemParam.getUserId()};
        console.log("transmitManager.requestData   Equipment_ScanTaskListByRFID ")
        scanTask.actionSequence = inspectionSystemConfig.getSequence();
        transmitManager.resultOfResponse.disconnect(scanTask.resultResponse)
        transmitManager.resultOfResponse.connect(scanTask.resultResponse)
        transmitManager.requestData(head, para, scanTask.actionSequence, "");
    }
    function resultResponse(result, error, action, map){
        console.debug("---scanTask---onResultOfResponse---result, error: action  actionSeqence ",
                      result, error, action, scanTask.actionSequence);
        if(action != scanTask.actionSequence){
            console.log("action != scanTask.actionSequence  return")
            return;
        }
        scanTask.refresh = true;
        scanItem.visible = false;
        promptText.visible = false;
        backGroundRect.visible = true;
        transmitManager.resultOfResponse.disconnect(scanTask.resultResponse)
        var tables = map["tables"];
        var roles;
        for(var i = 0; i < tables.length; i++){
            var table = tables[i];
            var name = table["name"];
            if(name == "ism_inspectiontaskobject"){
                //巡检
                roles = ["TaskObjectDataKey","RealObjectID","TaskGroupDataKey","TaskDataKey",
                         "ObjectName","ObjectFullName","RealObjectType","RFIDCode","BarCode","QRCode",
                         "OrderIndex","SerialKey","Positions","ModelName","IsClick","IsDelegate",
                         "DeviceLastModifiedDate","InspectionMode","EnableUploadAttach"];
                scanInspectionModel.setRolesName(roles);
                scanInspectionModel.setListData(table["value"]);
            }else if(name == "EXAM_ExamineTaskItem"){
                //规范检查
                roles = ["TaskItemID","TaskItemDataKey","ItemName","ItemFullName","TaskObjectDataKey",
                         "TaskID","SerialKey","RFIDCode","BarCode","QRCode","EnableUploadAttach"];
                scanRectificationModel.setRolesName(roles);
                scanRectificationModel.setListData(table["value"]);
            }else if(name == "emf_faultrepair"){
                //运维
                roles = ["ID","FaultCode","Status","EquitmentName","ReporterOrganiseName","FaultType",
                         "ReporterName","ReportDate","Positions","EnterpriseName","EquipmentCode",
                         "ModelName","SerialKey"];
                scanOprationModel.setRolesName(roles);
                scanOprationModel.setListData(table["value"]);
            }else if(name == "emm_maintenancetaskobject"){
                //保养
                roles = ["TaskObjectDataKey","ObjectName","ObjectFullName","RealObjectType","RealObjectID",
                         "SerialKey","TaskDataKey","TaskGroupDataKey","RFIDCode","BarCode","QRCode",
                         "RealObjectModifiedDate","Positions","ModelName","img"];
                scanMaintainModel.setRolesName(roles);
                scanMaintainModel.setListData(table["value"]);
            }
        }
    }
    onStatusChanged:{
        if(status == CPageStatus.WillShow){
            console.log("ScanTask.qml  refresh ===", refresh);
            if(refresh == true){
                if(scanCode != "" && (isRFIDCodeScan == "1" || isBarCodeScan == "1")){
                    var head = {"Objects": "Equipment_ScanTaskListByRFID"};
                    var para = {"RFIDCode" : scanCode, "UserId": inspectionSystemParam.getUserId()};
                    console.log("transmitManager.requestData   Equipment_ScanTaskListByRFID ")
                    scanTask.actionSequence = inspectionSystemConfig.getSequence();
                    transmitManager.resultOfResponse.disconnect(scanTask.resultResponse)
                    transmitManager.resultOfResponse.connect(scanTask.resultResponse)
                    transmitManager.requestData(head, para, scanTask.actionSequence, "");
                }
            }
        }else if(status == CPageStatus.WillHide){
            scanTask.refresh = true;
        }
    }
}
