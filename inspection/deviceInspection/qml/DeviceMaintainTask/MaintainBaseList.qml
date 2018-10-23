import QtQuick 2.0
import com.syberos.basewidgets 2.0
import "../Common"
BaseList{
    id:maintainTaskList
    //获取任务的action
    property string taskActionSequence: ""
    property string nameId: ""
    property int selectedIndex : -1
    //任务列表代理
    Component{
        id:taskListDelegate
        MaintainTaskDelegate{}
    }
    //套件列表代理
    Component{
        id:suitListDelegate
        MaintainSuiteDelegate{}
    }
    //设备列表代理
    Component{
        id:deviceListDelegate
        MaintainDeviceDelegate{}
    }

    onResultBarCode: {
        getObjectByCode(barCode, "BarCode");
    }
    onResultRFIDCode: {
        getObjectByCode(rfidCode, "RFIDCode");
    }
    function getObjectByCode(code, codeType){
        console.log("maintain   ", code, codeType);
        var data = maintainTaskList.originalData["tables"][0]["value"];
        if(data.length == 0){
            CTOAST.requestToast("没有匹配的保养任务项","","");
            return;
        }
        var headObj = maintainTaskList.originalData["head"]["objects"];
        if(headObj === "EMM_GetTaskObjectListByTask" || headObj == "EMM_GetTaskObjectListByGroup"){
            var i = 0;
            for( ; i < data.length; i++){
                var barCode = data[i]["BarCode"].toLowerCase();
                var qrCode = data[i]["QRCode"].toLowerCase();
                var rfidCode = String(data[i]["RFIDCode"]).toLowerCase();
                if(code == barCode || code == rfidCode || code == qrCode)
                    break;
            }
            if(i >= data.length){
                CTOAST.requestToast("没有匹配的保养任务项","","");
                return;
            }
            maintainTaskList.selectedIndex = i;
            if(codeType == "BarCode"){
                maintainTaskList.isBarCodeScan = "1";
                maintainTaskList.isRFIDScan = "0";
            } else if(codeType == "RFIDCode"){
                maintainTaskList.isBarCodeScan = "0";
                maintainTaskList.isRFIDScan = "1";
            }
            transmitManager.resultOfResponse.disconnect(maintainTaskList.resultResponse)
            transmitManager.resultOfResponse.connect(maintainTaskList.resultResponse)
            maintainTaskList.actionSequence = inspectionSystemConfig.getSequence();
            var head = {"Objects" : "EMM_GetTaskObjectDetail"};
            var para = {"TaskObjectDataKey" : data[i]["TaskObjectDataKey"]};
            transmitManager.requestData(head, para, maintainTaskList.actionSequence, "获取保养任务项详情失败");
        }
    }

    onSearchTextChanged: {
        if(!searchInputItem.visible)
            return;
        console.log("onSearchTextChanged :",text, "  inspectionTaskItemModel.count ", inspectionTaskItemModel.rowCount());
        commonProxyModel.setModelFilter(".*" + text.trim() + ".*");
        console.log("commonProxyModel.setModelFilter  ", ".*" + text.trim() + ".*", "   proxyModel count is ", commonProxyModel.itemCount());
    }

    onStatusChanged:{
        if(status == CPageStatus.WillShow){
            console.log("MaintainBaseList.qml  refresh ===", refresh);
            if(refresh === true){
                var headObj = maintainTaskList.originalData["head"]["objects"];
                if(headObj == "EMM_GetUserRunningTask"){
                    transmitManager.resultOfResponse.disconnect(maintainTaskList.resultResponse)
                    transmitManager.resultOfResponse.connect(maintainTaskList.resultResponse)

                    var head = {"Objects" : "EMM_GetUserRunningTask"};
                    var para = {"UserID" : inspectionSystemParam.getUserId()};
                    console.log("transmitManager.requestData   EMM_GetUserRunningTask ")
                    maintainTaskList.taskActionSequence = inspectionSystemConfig.getSequence();
                    transmitManager.requestData(head, para, maintainTaskList.taskActionSequence, "获取保养任务列表失败");
                }else if(headObj == "EMM_GetTaskGroupList"){
                    transmitManager.resultOfResponse.disconnect(maintainTaskList.resultResponse);
                    transmitManager.resultOfResponse.connect(maintainTaskList.resultResponse);

                    var head1 = {"Objects" : "EMM_GetTaskGroupList"};
                    var para1 = {"UserID" : inspectionSystemParam.getUserId(), "TaskDataKey" : maintainTaskList.otherParam["TaskDataKey"]};
                    console.log("transmitManager.requestData   EMM_ GetTaskGroupList ")
                    maintainTaskList.actionSequence = inspectionSystemConfig.getSequence();
                    transmitManager.requestData(head1, para1, maintainTaskList.actionSequence, "获取保养任务组列表失败");
                }else if(headObj == "EMM_GetTaskObjectListByGroup" || headObj == "EMM_GetTaskObjectListByTask"){
                    transmitManager.resultOfResponse.disconnect(maintainTaskList.resultResponse)
                    transmitManager.resultOfResponse.connect(maintainTaskList.resultResponse)

                    var head2 = {"Objects" : headObj};
                    var para2 = {};
                    if(headObj == "EMM_GetTaskObjectListByTask")
                        para2 = {"UserID" : inspectionSystemParam.getUserId(), "TaskDataKey" : maintainTaskList.otherParam["TaskDataKey"]};
                    else if(headObj == "EMM_GetTaskObjectListByGroup")
                        para2 = {"TaskGroupDataKey" : maintainTaskList.otherParam["TaskGroupDataKey"]};
                    console.log("transmitManager.requestData   ", headObj);
                    maintainTaskList.actionSequence = inspectionSystemConfig.getSequence();
                    transmitManager.requestData(head2, para2, maintainTaskList.actionSequence, "获取保养任务项列表失败");
                }
            }

        }else if(status == CPageStatus.Show){
            console.log("MaintainBaseList.qml  status == CPageStatus.Show")
        }else if(status == CPageStatus.WillHide){
            console.log("MaintainBaseList.qml  status == CPageStatus.WillHide")
            maintainTaskList.refresh = true;
            searchInputItem.visible = false;
            searchInputItem.inputText = "";
            rootWindow.closeKeyboard();
            var headerObj1 = maintainTaskList.originalData["head"]["objects"];
            if(headerObj1 == "EMM_GetTaskObjectListByGroup" || headerObj1 == "EMM_GetTaskObjectListByTask"){
                commonProxyModel.setModelFilter(".*.*");
            }
        }else if(status == CPageStatus.Hide){
            console.log("MaintainBaseList.qml  status == CPageStatus.Hide")
        }
    }

    /**
     * 接受服务器返回数据的槽函数
     * @param result:bool 获取数据是否成功
     * @param error:int 错误代码
     * @param action:string 当前发送数据动作类型
     * @param map:var    服务器返回值
     */
    function resultResponse(result, error, action, map){
        console.log("maintainTaskList---Response--result, error,action, actionSequence ",  result,  error, action, maintainTaskList.actionSequence);

        if(action != maintainTaskList.actionSequence && action != maintainTaskList.taskActionSequence){
            console.log("action != maintainTaskList.actionSequence && action != maintainTaskList.taskActionSequence  return");
            return;
        }
        maintainTaskList.refresh = true;
        if(action == maintainTaskList.actionSequence){
            transmitManager.resultOfResponse.disconnect(maintainTaskList.resultResponse)
        }
        if(result && error == 0){
            var headObj = map["head"]["objects"];
            if(headObj == "EMM_GetTaskObjectDetail"){
                if(map["tables"].length <= 0 || map["tables"][0]["value"].length <= 0){
                    CTOAST.requestToast(os.i18n.ctr(qsTr("No device")),"","");
                    return;
                }
                var dataMap = maintainTaskList.originalData["tables"][0]["value"];
                var paramValue=  {
                    "IsRFIDScan": maintainTaskList.isRFIDScan,
                    "IsBarCodeScan": maintainTaskList.isBarCodeScan,
                    "EnableUploadAttach": maintainTaskList.otherParam["EnableUploadAttach"],
                    "pageFormType":"maintain",
                    "parentPage":maintainTaskList,
                    "TaskObjectDataKey":dataMap[maintainTaskList.selectedIndex]["TaskObjectDataKey"]
                };
                pageStack.push("qrc:/qml/DeviceMaintainTask/DeviceMaintainForm.qml",
                               {"dataPackageValue": map,
                                   "paramValue": paramValue});
            }else if(headObj == "EMM_Off_GetPrePlanIDsByUserID"){
                CTOAST.requestToast("同步未来任务完成","","");
            }else{
                console.log("maintainTaskList.model.setListData 00000000000000000000")
                maintainTaskList.model.setListData(map["tables"][0]["value"]);
                maintainTaskList.originalData = map;
                if((headObj == "EMM_GetTaskObjectListByGroup" || headObj == "EMM_GetTaskObjectListByTask")
                        && inspectionTaskItemModel.rowCount() == 0){
                    maintainTaskList.titleRightItemEnable = false;
                }
            }
        }
    }

    Component.onCompleted: {
        //页面第一次加载，不刷新
        maintainTaskList.refresh = false;
        var headObj = maintainTaskList.originalData["head"]["objects"];
        var roles;
        if(headObj === "EMM_GetUserRunningTask"){ //任务列表
            //接受二次信号
            console.log("transmitManager.resultOfResponse.connect(maintainTaskList.resultResponse)")
            transmitManager.resultOfResponse.connect(maintainTaskList.resultResponse);
            roles = ["TaskDataKey", "TaskCode", "TaskName", "LimitMode", "EnableUploadAttach", "Circle", "Rate","ModelType", "SyncStatus"];
            var name = "";
            var clientPriority = inspectionSystemConfig.getClientPriority();
            if(clientPriority == false){
                name = inspectionSystemParam.getUserPermissionData(maintainTaskList.nameId, "ModuleName");
            } else
                name = inspectionFuncsItemModel.getNodeParas(maintainTaskList.nameId, "")["NameCN"];
            maintainTaskList.titleText = name;
            maintainTaskList.emptyText = "无保养任务";
            maintainTaskList.delegateCom = taskListDelegate;
            inspectionTaskModel.setRolesName(roles);
            inspectionTaskModel.setListData(maintainTaskList.originalData["tables"][0]["value"]);
            maintainTaskList.model = inspectionTaskModel;
            maintainTaskList.titleRightItemIcon = "qrc:/icon/action_bar_menu.png"
            maintainTaskList.titleRightItemEnable = true;
            maintainTaskList.futureEnable = true
            maintainTaskList.offlineEnable = true
        } else if (headObj === "EMM_GetTaskGroupList"){//套件列表
            roles = ["TaskGroupDataKey", "TaskDataKey", "TaskGroupName", "PostID", "TotalObjectCount", "ExamObjectCount", "OrderIndex"];
            maintainTaskList.titleText = maintainTaskList.otherParam["TaskName"];
            maintainTaskList.emptyText = "无保养任务组";
            maintainTaskList.delegateCom = suitListDelegate;
            inspectionTaskSuiteModel.setRolesName(roles);
            inspectionTaskSuiteModel.setListData(maintainTaskList.originalData["tables"][0]["value"]);
            maintainTaskList.model = inspectionTaskSuiteModel;
        } else if (headObj === "EMM_GetTaskObjectListByGroup" || headObj == "EMM_GetTaskObjectListByTask") {//设备列表
            console.log("GetTaskObjectListByGroup------")
            roles = ["TaskObjectDataKey", "ObjectName", "ObjectFullName", "RFIDCode", "BarCode", "Positions", "ModelName"];
            if(headObj == "EMM_GetTaskObjectListByTask")
                maintainTaskList.titleText = maintainTaskList.otherParam["TaskName"];
            else if(headObj == "EMM_GetTaskObjectListByGroup")
                maintainTaskList.titleText = maintainTaskList.otherParam["TaskGroupName"];

            maintainTaskList.emptyText = "无保养任务项";
            maintainTaskList.delegateCom = deviceListDelegate;
            inspectionTaskItemModel.setRolesName(roles);
            inspectionTaskItemModel.setListData(maintainTaskList.originalData["tables"][0]["value"]);
            commonProxyModel.filterKeyColumn = 0;
            commonProxyModel.filterRole = inspectionTaskItemModel.getRoleByRoleName("ObjectName");
            commonProxyModel.setModel(inspectionTaskItemModel);
            maintainTaskList.model = commonProxyModel;
            if(inspectionTaskItemModel.rowCount() > 0){
                maintainTaskList.titleRightItemText = "搜索"
                maintainTaskList.titleRightItemEnable = true;
            }
        }
    }
    Component.onDestruction: {
        console.log("MaintainBaseList.qml  destruction")
        transmitManager.resultOfResponse.disconnect(maintainTaskList.resultResponse);
    }

    onSyncPreTask:{
        console.debug("---------------onSyncPreTask-----------dateTime: ", dateTime)
        var head = {"Objects" : "EMM_Off_GetPrePlanIDsByUserID"};
        var para = {"UserID" : inspectionSystemParam.getUserId(), "PreBuildEndTimeStr" : dateTime};
        console.log("transmitManager.syncPreTask   ", "EMM_Off_GetPrePlanIDsByUserID");
        maintainTaskList.actionSequence = inspectionSystemConfig.getSequence();
        transmitManager.resultOfResponse.disconnect(maintainTaskList.resultResponse)
        transmitManager.resultOfResponse.connect(maintainTaskList.resultResponse)
        transmitManager.syncPreTask(head, para, maintainTaskList.actionSequence, "获取保养未来任务失败");
    }

    onSyncTask: {
        console.debug("---------------onSyncTask-----------EMM_GetUserRunningTask------")
        var head = {"Objects" : "EMM_GetUserRunningTask"};
        var para = {"UserID" : inspectionSystemParam.getUserId()};
        maintainTaskList.taskActionSequence = inspectionSystemConfig.getSequence();
        transmitManager.requestTaskData(head, para, maintainTaskList.taskActionSequence, "获取保养任务列表失败");
    }
}
