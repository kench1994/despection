import QtQuick 2.0
import com.syberos.basewidgets 2.0
import "../Common"
BaseList{
    id:inventoryTaskList
    property string nameId: ""
    property string taskActionSequence: ""
    //任务列表代理
    Component{
        id:taskListDelegate
        InventoryTaskDelegate{}
    }

    //设备列表代理
    Component{
        id:deviceListDelegate
        InventoryDeviceDelegate{}
    }

    onResultBarCode: {
        console.log("inventoryTaskList  onResultBarCode  ", barCode)
        getObjectByCode(barCode, "BarCode");
    }
    onResultRFIDCode: {
        console.log("inventoryTaskList  onResultRFIDCode  ", rfidCode)
        getObjectByCode(rfidCode, "RFIDCode");
    }
    function getObjectByCode(code, codeType){
        console.log("inventoryTaskList   ", code, codeType);
        var data = inventoryTaskList.originalData["tables"][0]["value"];
        if(data.length == 0){
            CTOAST.requestToast("没有匹配的盘点项","","");
            return;
        }
        var headObj = inventoryTaskList.originalData["head"]["objects"];
        if(headObj === "EMI_InventoryEquipmentList"){
            var i = 0;
            for( ; i < data.length; i++){
                var barCode = String(data[i]["BarCode"]).toLowerCase();
                var qrCode = String(data[i]["QRCode"]).toLowerCase();
                var rfidCode = String(data[i]["RFIDCode"]).toLowerCase();
                console.log("barCode is ", barCode, " qrCode is ", qrCode, "  rfidCOde is ", rfidCode)
                if(code == barCode || code == rfidCode || code == qrCode)
                    break;
            }

            if(i >= data.length){
                CTOAST.requestToast("没有匹配的盘点项","","");
                return;
            }
            var head = {"Objects" : "EMI_InventoryEquipment_Para"};
            var para = {"TaskID" : inventoryTaskList.otherParam["TaskID"],
                "ID" : data[i]["ID"],
                "RFIDCode" : "",
                "BarCode" : ""};
            if(codeType == "BarCode"){
                inventoryTaskList.isBarCodeScan = "1";
                inventoryTaskList.isRFIDScan = "0";
            } else if(codeType == "RFIDCode"){
                inventoryTaskList.isBarCodeScan = "0";
                inventoryTaskList.isRFIDScan = "1";
            }
            transmitManager.resultOfResponse.disconnect(inventoryTaskList.inventoryResultResponse)
            transmitManager.resultOfResponse.connect(inventoryTaskList.inventoryResultResponse)
            inventoryTaskList.actionSequence = inspectionSystemConfig.getSequence();
            transmitManager.requestData(head, para, inventoryTaskList.actionSequence, "获取盘点项详情失败");
        }
    }

    onStatusChanged:{
        if(status == CPageStatus.WillShow){
            console.log("inventoryTaskList.qml  refresh ===", refresh);
            if(refresh === true){
                var headObj = inventoryTaskList.originalData["head"]["objects"];
                console.log("InspectionBaseList.qml  refresh === true action  ", headObj);
                if(headObj == "EMI_InventoryTask_RuningList"){
                    var head = {"Objects" : "EMI_InventoryTask_RuningList"};
                    var para = {"UserID" : inspectionSystemParam.getUserId()};
                    console.log("transmitManager.requestData   EMI_InventoryTask_RuningList ")
                    inventoryTaskList.taskActionSequence = inspectionSystemConfig.getSequence();
                    transmitManager.requestData(head, para, inventoryTaskList.taskActionSequence, os.i18n.ctr(qsTr("Get inventory task fail")));
                } else if(headObj == "EMI_InventoryEquipmentList"){
                    inventoryTaskList.actionSequence = inspectionSystemConfig.getSequence();
                    transmitManager.resultOfResponse.disconnect(inventoryTaskList.inventoryResultResponse);
                    transmitManager.resultOfResponse.connect(inventoryTaskList.inventoryResultResponse);
                    var head1 = {"Objects" : "EMI_InventoryEquipmentList"};
                    var para1 = {};
                    para1["TaskID"] = inventoryTaskList.otherParam["TaskID"];
                    para1["UserID"] = inspectionSystemParam.getUserId();
                    para1["EquipmentName"] = "";
                    console.log("transmitManager.requestData   EMI_ InventoryEquipmentList ")
                    transmitManager.requestData(head1, para1, inventoryTaskList.actionSequence, os.i18n.ctr(qsTr("Get inventory equipment list fail")));
                }
            }
        }else if(status == CPageStatus.Show){
            console.log("inventoryTaskList.qml  status == CPageStatus.Show")
        }else if(status == CPageStatus.WillHide){
            console.log("inventoryTaskList.qml  status == CPageStatus.WillHide  ")
            //默认下次进入此页面刷新
            inventoryTaskList.refresh = true;
        }else if(status == CPageStatus.Hide){
            console.log("inventoryTaskList.qml  status == CPageStatus.Hide")
        }
    }

    /**
     * 接受服务器返回数据的槽函数
     * @param result:bool 获取数据是否成功
     * @param error:int 错误代码
     * @param action:string 当前发送数据动作类型
     * @param map:var    服务器返回值
     */
    function inventoryResultResponse(result, error, action, map){
        console.debug("----inventoryTaskList-----Response--------------result, error,action, actionSequence ",
                      result,  error, action, inventoryTaskList.actionSequence);
        if(action != inventoryTaskList.actionSequence && action != inventoryTaskList.taskActionSequence){
            console.log("action != inventoryTaskList.actionSequence && action != inventoryTaskList.taskActionSequence return");
            return;
        }
        inventoryTaskList.refresh = true;
        if(action == inventoryTaskList.actionSequence)
            transmitManager.resultOfResponse.disconnect(inventoryTaskList.inventoryResultResponse);
        if(result && error == 0){
            var headObj = map["head"]["objects"];
            if(headObj == "EMI_InventoryEquipment_Para"){
                if(map["tables"].length <= 0 || map["tables"][0]["value"].length <= 0){
                    CTOAST.requestToast(os.i18n.ctr(qsTr("No device")),"","");
                    return;
                }
                var paramValue=  {
                    "IsRFIDScan": inventoryTaskList.isRFIDScan,
                    "IsBarCodeScan": inventoryTaskList.isBarCodeScan,
                    "limitMode": inventoryTaskList.otherParam["LimitMode"],
                    "enableImage": inventoryTaskList.otherParam["EnableImage"],
                    "TaskID": inventoryTaskList.otherParam["TaskID"],
                    "parentPage":inventoryTaskList
                };

                pageStack.push("qrc:/qml/InventoryTask/DeviceInventoryForm.qml", {"dataPackageValue": map,
                                                                    "paramValue": paramValue});
            }else{
                console.log("inventoryTaskList.model.setListData 00000000000000000000")
                inventoryTaskList.model.setListData(map["tables"][0]["value"]);
                inventoryTaskList.originalData = map;
            }
        }
    }
    Component.onCompleted: {
        //页面第一次加载，不刷新
        inventoryTaskList.refresh = false;
        var headObj = inventoryTaskList.originalData["head"]["objects"];

        var roles;
        if(headObj === "EMI_InventoryTask_RuningList"){ //任务列表
            //接受二次信号
            console.log("transmitManager.resultOfResponse.connect(inventoryTaskList.inventoryResultResponse)")
            transmitManager.resultOfResponse.connect(inventoryTaskList.inventoryResultResponse);
            roles = ["ID", "TaskCode", "TaskName", "LimitMode", "EnableImage", "StartDate", "EndDate", "Circle", "SyncStatus", "Rate"];
            var name;
            var clientPriority = inspectionSystemConfig.getClientPriority();
            if(clientPriority == false){
                name = inspectionSystemParam.getUserPermissionData(inventoryTaskList.nameId, "ModuleName");
            } else
                name = inspectionFuncsItemModel.getNodeParas(inventoryTaskList.nameId, "")["NameCN"];
            inventoryTaskList.titleText = name;
            inventoryTaskList.emptyText = os.i18n.ctr(qsTr("No inventory task"));
            inventoryTaskList.delegateCom = taskListDelegate;
            inspectionTaskModel.setRolesName(roles);
            inspectionTaskModel.setListData(inventoryTaskList.originalData["tables"][0]["value"]);
            inventoryTaskList.model = inspectionTaskModel;
            inventoryTaskList.titleRightItemIcon = "qrc:/icon/action_bar_menu.png"
            inventoryTaskList.titleRightItemEnable = true;
            inventoryTaskList.offlineEnable = true
        } else if (headObj === "EMI_InventoryEquipmentList"){//设备列表
            roles = ["ID", "EquipmentID", "EquipmentName", "EquipmentCode", "ModelName", "Custodian","RFIDCode","BarCode"];
            inventoryTaskList.enabledScanItem = inventoryTaskList.otherParam["EnabledScanItem"];
            inventoryTaskList.titleText = inventoryTaskList.otherParam["TaskName"];
            inventoryTaskList.emptyText = os.i18n.ctr(qsTr("No inventory device"));
            inventoryTaskList.delegateCom = deviceListDelegate;
            inspectionTaskItemModel.setRolesName(roles);
            inspectionTaskItemModel.setListData(inventoryTaskList.originalData["tables"][0]["value"]);
            inventoryTaskList.model = inspectionTaskItemModel;
        }
    }
    Component.onDestruction: {
        console.log("InventoryBaseList.qml onDestruction ")
        transmitManager.resultOfResponse.disconnect(inventoryTaskList.inventoryResultResponse);
    }

    onSyncTask: {
        console.debug("---------------onSyncTask-----------EISM_GetDailyRunningTask-----")
        inventoryTaskList.taskActionSequence = inspectionSystemConfig.getSequence();
        var head = {"Objects" : "EMI_InventoryTask_RuningList"};
        var para = {"UserID" : inspectionSystemParam.getUserId()};
        transmitManager.requestTaskData(head, para, inventoryTaskList.taskActionSequence, os.i18n.ctr(qsTr("Get inventory task fail")));
    }
}
