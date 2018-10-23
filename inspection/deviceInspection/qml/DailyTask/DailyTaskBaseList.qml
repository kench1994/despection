import QtQuick 2.0
import com.syberos.basewidgets 2.0
import "../Common"
BaseList{
    id:inspectionTaskList
    property string nameId: ""
    property int selectedIndex: -1
    property string taskActionSequence: ""
    //任务列表代理
    Component{
        id:taskListDelegate
        DailyTaskListDelegate{}
    }
    //套件列表代理
    Component{
        id:suitListDelegate
        DailyTaskSuiteDelegate{}
    }
    //设备列表代理
    Component{
        id:deviceListDelegate
        DailyTaskDeviceListDelegate{
            id:delegate
        }
    }

    onResultBarCode: {
        var headObj = inspectionTaskList.originalData["head"]["objects"];
        if(headObj == "EISM_GetDailyTaskGroupList")
            getGroupListByCode(barCode, "BarCode");
        else
            getObjectByCode(barCode, "BarCode");
    }
    onResultRFIDCode: {
        var headObj = inspectionTaskList.originalData["head"]["objects"];
        if(headObj == "EISM_GetDailyTaskGroupList")
            getGroupListByCode(rfidCode, "RFIDCode");
        else
            getObjectByCode(rfidCode, "RFIDCode");
    }

    function getGroupListByCode(code, codeType){
        var data = inspectionTaskList.originalData["tables"][0]["value"];
        if(data.length == 0){
            CTOAST.requestToast("没有匹配的日常任务组","","");
            return;
        }
        var i = 0;
        for( ; i < data.length; i++){
            var barCode = data[i]["BarCode"].toLowerCase();
            var qrCode = data[i]["QRCode"].toLowerCase();
            var rfidCode = String(data[i]["RFIDCode"]).toLowerCase();
            if(code == barCode || code == rfidCode || code == qrCode)
                break;
        }
        if(i >= data.length){
            CTOAST.requestToast("没有匹配的日常任务组","","");
            return;
        }
        inspectionTaskList.selectedIndex = i;
        transmitManager.resultOfResponse.disconnect(inspectionTaskList.inspectionResultResponse)
        transmitManager.resultOfResponse.connect(inspectionTaskList.inspectionResultResponse)
        inspectionTaskList.actionSequence = inspectionSystemConfig.getSequence();
        var head = {"Objects" : "EISM_GetDailyObjectListByGroup"};
        var para = {"TaskDataKey" : data[i]["TaskDataKey"],"TaskGroupDataKey" : data[i]["TaskGroupDataKey"] };
        transmitManager.requestData(head, para, inspectionTaskList.actionSequence, "获取日常任务对象列表失败");
    }

    function getObjectByCode(code, codeType){
        console.log("DailyTaskBaseList.qml   ", code, codeType);
        var data = inspectionTaskList.originalData["tables"][0]["value"];
        var headObj = inspectionTaskList.originalData["head"]["objects"];
        if(headObj == "EISM_GetDailyObjectListByTask" || headObj == "EISM_GetDailyObjectListByGroup"){
            var i = 0;
            for( ; i < data.length; i++){
                var barCode = data[i]["BarCode"].toLowerCase();
                var qrCode = data[i]["QRCode"].toLowerCase();
                var rfidCode = String(data[i]["RFIDCode"]).toLowerCase();
                if(code == barCode || code == rfidCode || code == qrCode)
                    break;
            }
            if(i >= data.length){
                CTOAST.requestToast("没有匹配的日常任务项","","");
                return;
            }
            if(codeType == "BarCode"){
                inspectionTaskList.isBarCodeScan = "1";
                inspectionTaskList.isRFIDScan = "0";
            } else if(codeType == "RFIDCode"){
                inspectionTaskList.isBarCodeScan = "0";
                inspectionTaskList.isRFIDScan = "1";
            }

            transmitManager.resultOfResponse.disconnect(inspectionTaskList.inspectionResultResponse)
            transmitManager.resultOfResponse.connect(inspectionTaskList.inspectionResultResponse)
            inspectionTaskList.actionSequence = inspectionSystemConfig.getSequence();

            var head = {"Objects" : "EISM_GetDailyObjectDetail"};
            var para = {"TaskGroupDataKey" : data[i]["TaskGroupDataKey"], "PlanObjectID" : data[i]["PlanObjectID"]};
            console.log("transmitManager.requestData    EISM_GetDailyObjectDetail ")
            transmitManager.requestData(head, para, inspectionTaskList.actionSequence, "获取日常任务对象详情失败");
        }
    }

    onSearchTextChanged: {
        if(!searchInputItem.visible)
            return;
        console.log("onSearchTextChanged :",text, "  inspectionTaskItemModel.count ", inspectionTaskItemModel.rowCount());
        commonProxyModel.setModelFilter( ".*" + text.trim() + ".*");
        console.log("commonProxyModel.filterRegExp  ", ".*" + text + ".*", "   proxyModel is  ", commonProxyModel.itemCount());
    }

    onStatusChanged:{
        if(status == CPageStatus.WillShow){
            console.log("DailyTaskBaseList.qml  refresh ===", refresh);
        }else if(status == CPageStatus.Show){
            console.log("DailyTaskBaseList.qml  status == CPageStatus.Show")
        }else if(status == CPageStatus.WillHide){
            console.log("DailyTaskBaseList.qml  status == CPageStatus.WillHide")
            inspectionTaskList.refresh = true;
            searchInputItem.visible = false;
            searchInputItem.inputText = "";
            rootWindow.closeKeyboard();
            var headerObj1 = inspectionTaskList.originalData["head"]["objects"] ;
            if(headerObj1 === "EISM_GetDailyObjectListByGroup" || headerObj1 == "EISM_GetDailyObjectListByTask"){
                commonProxyModel.setModelFilter(".*.*");
            }
        }else if(status == CPageStatus.Hide){
            console.log("DailyTaskBaseList.qml  status == CPageStatus.Hide")
        }
    }

    /**
     * 接受服务器返回数据的槽函数
     * @param result:bool 获取数据是否成功
     * @param error:int 错误代码
     * @param action:string 当前发送数据动作类型
     * @param map:var    服务器返回值
     */
    function inspectionResultResponse(result, error, action, map){
        console.log("----DailyTaskBaseList-----Response--result, error,action, actionSequence ",
                      result,  error, action, inspectionTaskList.actionSequence);
        if(action != inspectionTaskList.actionSequence  && action != inspectionTaskList.taskActionSequence){
            console.log("action != inspectionTaskList.actionSequence  return");
            return;
        }
        inspectionTaskList.refresh = true;
        if(action == inspectionTaskList.actionSequence)
            transmitManager.resultOfResponse.disconnect(inspectionTaskList.inspectionResultResponse)
        if(result && error == 0){
            var headerObj = map["head"]["objects"];
            if(headerObj == "EISM_GetDailyObjectDetail"){
                if(map["tables"].length <= 0 || map["tables"][0]["value"].length <= 0){
                    CTOAST.requestToast("没有找到日常任务对象","","");
                    return;
                }
                var paramValue=  {
                    "IsRFIDScan": inspectionTaskList.isRFIDScan,
                    "IsBarCodeScan": inspectionTaskList.isBarCodeScan,
                    "TaskDataKey" : inspectionTaskList.otherParam["TaskDataKey"],
                    "EnableUploadAttach": inspectionTaskList.otherParam["EnableUploadAttach"],
                    "pageFormType":"dailyTask",
                    "parentPage":inspectionTaskList
                };
                pageStack.push("qrc:/qml/DailyTask/DailyTaskForm.qml",
                               {"dataPackageValue": map,
                                   "paramValue": paramValue});
            } else if(inspectionTaskList.originalData["head"]["objects"] == "EISM_GetDailyTaskGroupList" && headerObj == "EISM_GetDailyObjectListByGroup"){
                var dataMap = inspectionTaskList.originalData["tables"][0]["value"][inspectionTaskList.selectedIndex];
                var otherParam = {
                    "TaskGroupName": dataMap["TaskGroupName"],
                    "TaskGroupDataKey": dataMap["TaskGroupDataKey"],
                    "TaskDataKey" : dataMap["TaskDataKey"],
                    "LimitMode": inspectionTaskList.otherParam["LimitMode"],
                    "EnableUploadAttach": inspectionTaskList.otherParam["EnableUploadAttach"],
                    "EnabledScanItem": true,
                    "InspectionMode":inspectionTaskList.otherParam["InspectionMode"]
                }
                pageStack.push("qrc:/qml/DailyTask/DailyTaskBaseList.qml",
                               {
                                   "originalData": map,
                                   "otherParam":otherParam,
                               });
            } else {
                console.log("DailyTaskBaseList.model.set ListData 00000000000000000000")
                inspectionTaskList.model.setListData(map["tables"][0]["value"]);
                inspectionTaskList.originalData = map;
                if ((headerObj === "EISM_GetDailyObjectListByGroup" || headerObj == "EISM_GetDailyObjectListByTask")
                        && inspectionTaskItemModel.rowCount() == 0){
                    inspectionTaskList.titleRightItemEnable = false;
                }
            }
        }
    }

    Component.onCompleted: {
        //页面第一次加载，不刷新
        inspectionTaskList.refresh = false;
        var headObj = inspectionTaskList.originalData["head"]["objects"];
        var roles;
        if(headObj === "EISM_GetDailyRunningTask"){ //任务列表
            roles = ["TaskDataKey", "TaskName", "LimitMode", "EnableUploadAttach", "InspectionMode", "TaskStartTime", "SyncStatus"];
            var name = "";
            var clientPriority = inspectionSystemConfig.getClientPriority();
            if(clientPriority == false){
                name = inspectionSystemParam.getUserPermissionData(inspectionTaskList.nameId, "ModuleName");
                 console.log("completed11111111  name is  ", name)
            } else {
                name = inspectionFuncsItemModel.getNodeParas(inspectionTaskList.nameId, "")["NameCN"];
                console.log("completed222222222  name is  ", name, " inspectionTaskList.nameId ", inspectionTaskList.nameId)
            }

            if(name == ""){
                name = "日常任务";
            }
            transmitManager.resultOfResponse.connect(inspectionTaskList.inspectionResultResponse);
            inspectionTaskList.titleText = name;
            inspectionTaskList.emptyText = "无日常任务";
            inspectionTaskList.delegateCom = taskListDelegate;
            inspectionTaskModel.setRolesName(roles);
            inspectionTaskModel.setListData(inspectionTaskList.originalData["tables"][0]["value"]);
            inspectionTaskList.model = inspectionTaskModel;
            inspectionTaskList.titleRightItemIcon = "qrc:/icon/action_bar_menu.png"
            inspectionTaskList.titleRightItemEnable = true;
            inspectionTaskList.offlineEnable = true
        }else if (headObj === "EISM_GetDailyTaskGroupList"){//套件列表
           // <Field Value="TaskGroupDataKeyやTaskDataKeyやTaskGroupNameやPostIDやTotalObjectCountやExamObjectCountやRFIDCodeやBarCodeやQRCodeやOrderIndex"/>
            roles = ["TaskGroupDataKey", "TaskDataKey", "TaskGroupName", "TotalObjectCount", "RFIDCode", "BarCode", "QRCode"];
            inspectionTaskList.titleText = inspectionTaskList.otherParam["TaskName"];
            inspectionTaskList.emptyText = "无日常任务组列表";
            inspectionTaskList.delegateCom = suitListDelegate;
            inspectionTaskSuiteModel.setRolesName(roles);
            inspectionTaskSuiteModel.setListData(inspectionTaskList.originalData["tables"][0]["value"]);
            inspectionTaskList.model = inspectionTaskSuiteModel;
        }else if (headObj === "EISM_GetDailyObjectListByGroup" || headObj == "EISM_GetDailyObjectListByTask") {//设备列表
            roles = ["PlanObjectID", "ObjectName", "ObjectFullName", "TaskGroupDataKey", "PlanGroupID", "RFIDCode", "BarCode" ,
                    "QRCode",  "RealObjectID", "Positions", "ModelName"];
            inspectionTaskList.enabledScanItem = inspectionTaskList.otherParam["EnabledScanItem"];
            if(headObj == "EISM_GetDailyObjectListByTask")
                inspectionTaskList.titleText = inspectionTaskList.otherParam["TaskName"];
            else if(headObj == "EISM_GetDailyObjectListByGroup")
                inspectionTaskList.titleText = inspectionTaskList.otherParam["TaskGroupName"];
            inspectionTaskList.emptyText = "无日常任务对象列表";
            inspectionTaskList.delegateCom = deviceListDelegate;
            inspectionTaskItemModel.setRolesName(roles);
            inspectionTaskItemModel.setListData(inspectionTaskList.originalData["tables"][0]["value"]);
            commonProxyModel.filterKeyColumn = 0;
            commonProxyModel.filterRole = inspectionTaskItemModel.getRoleByRoleName("ObjectName");
            commonProxyModel.setModel(inspectionTaskItemModel);
            inspectionTaskList.model = commonProxyModel;
            if(inspectionTaskItemModel.rowCount() > 0){
                inspectionTaskList.titleRightItemText = "搜索"
                inspectionTaskList.titleRightItemEnable = true;
            }
        }
    }

    Component.onDestruction: {
        console.log("InspectionBaselist.qml onDestruction ")
        transmitManager.resultOfResponse.disconnect(inspectionTaskList.inspectionResultResponse);
    }

    onSyncTask: {
        console.debug("---------------onSyncTask-----------EISM_GetDailyRunningTask------")
        var head = {"Objects" : "EISM_GetDailyRunningTask"};
        var para = {"UserID" : inspectionSystemParam.getUserId()};
        inspectionTaskList.taskActionSequence = inspectionSystemConfig.getSequence();
        transmitManager.requestTaskData(head, para, inspectionTaskList.taskActionSequence, "获取日常任务列表失败");
    }
}
