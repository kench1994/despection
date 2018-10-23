import QtQuick 2.0
import com.syberos.basewidgets 2.0
import "../Common"
BaseList{
    id:stdTaskList
    //获取任务的action
    property string taskActionSequence: ""
    property string nameId: ""
    property int selectedIndex: -1
    //任务列表代理
    Component{
        id:taskListDelegate
        StandardizedListDelegate{}
    }
    //套件列表代理
    Component{
        id:suitListDelegate
        StandardizedSuiteDelegate{}
    }
    //设备列表代理
    Component{
        id:deviceListDelegate
        StandardizedItemListDelegate{}
    }

    onResultBarCode: {
        console.log("stdTaskList  onResultBarCode  ", barCode)
        var headObj = stdTaskList.originalData["head"]["objects"];
        if(headObj == "EXAM_ExamineTaskObjectList")
            getObjectListByCode(barCode, "BarCode");
        else if(headObj == "EXAM_ExamineTaskItemList")
            getObjectByCode(barCode, "BarCode");
    }
    onResultRFIDCode: {
        console.log("stdTaskList  onResultRFIDCode  ", rfidCode)
        var headObj = stdTaskList.originalData["head"]["objects"];
        if(headObj == "EXAM_ExamineTaskObjectList")
            getObjectListByCode(rfidCode, "RFIDCode");
        else if(headObj == "EXAM_ExamineTaskItemList")
            getObjectByCode(rfidCode, "RFIDCode");
    }

    function getObjectListByCode(code, codeType){
        var data = stdTaskList.originalData["tables"][0]["value"];
        if(data.length == 0){
            CTOAST.requestToast("没有匹配的规范检查组","","");
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
            CTOAST.requestToast("没有匹配的规范检查组","","");
            return;
        }
        stdTaskList.selectedIndex = i;
        var head2 = {"Objects" : "EXAM_ExamineTaskItemList"};
        var para2 = {"TaskObjectDataKey" : data[i]["TaskObjectDataKey"]};
        console.log("transmitManager.requestData   EXAM_ExamineTaskItemList")
        stdTaskList.actionSequence = inspectionSystemConfig.getSequence();
        transmitManager.resultOfResponse.disconnect(stdTaskList.inspectionResultResponse)
        transmitManager.resultOfResponse.connect(stdTaskList.inspectionResultResponse)
        transmitManager.requestData(head2, para2, stdTaskList.actionSequence, "获取规范检查项列表失败");
    }

    function getObjectByCode(code, codeType){
        console.log("inspectionTaskList   ", code, codeType);
        var data = stdTaskList.originalData["tables"][0]["value"];
        if(data.length == 0){
            CTOAST.requestToast("没有匹配的规范检查项","","");
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
            CTOAST.requestToast("没有匹配的规范检查项","","");
            return;
        }
        stdTaskList.selectedIndex = i;
        if(codeType == "BarCode"){
            stdTaskList.isBarCodeScan = "1";
            stdTaskList.isRFIDScan = "0";
        } else if(codeType == "RFIDCode"){
            stdTaskList.isBarCodeScan = "0";
            stdTaskList.isRFIDScan = "1";
        }
        var head = {"Objects" : "EXAM_ExamineTaskItemDetail"};
        var para = {"TaskItemDataKey" : data[i]["TaskItemDataKey"]};
        console.log("transmitManager.requestData   EXAM _ExamineTaskItemDetail ")
        transmitManager.resultOfResponse.disconnect(stdTaskList.inspectionResultResponse)
        transmitManager.resultOfResponse.connect(stdTaskList.inspectionResultResponse)
        stdTaskList.actionSequence = inspectionSystemConfig.getSequence();
        transmitManager.requestData(head, para, stdTaskList.actionSequence, "获取规范检查项详情失败");
    }

    onStatusChanged:{
        if(status == CPageStatus.WillShow){
            console.log("InspectionBaseList.qml  refresh ===", refresh);
            if(refresh === true){
                var action = stdTaskList.originalData["head"]["objects"];
                if(action == "EXAM_ExamineTaskList"){
                    var head = {"Objects" : "EXAM_ExamineTaskList"};
                    var para = {"UserID" : inspectionSystemParam.getUserId()};
                    console.log("transmitManager.requestData   EXAM_ExamineTaskList ")
                    stdTaskList.taskActionSequence = inspectionSystemConfig.getSequence();
                    transmitManager.resultOfResponse.disconnect(stdTaskList.inspectionResultResponse)
                    transmitManager.resultOfResponse.connect(stdTaskList.inspectionResultResponse)
                    transmitManager.requestData(head, para, stdTaskList.taskActionSequence, os.i18n.ctr(qsTr("Get standardized inspection list fail")));

                }else if(action == "EXAM_ExamineTaskObjectList"){
                    var head1 = {"Objects" : "EXAM_ExamineTaskObjectList"};
                    var para1 = {"TaskDataKey" : stdTaskList.otherParam["TaskDataKey"],
                        "UserID" : inspectionSystemParam.getUserId()};
                    console.log("transmitManager.requestData   EXAM_ExamineTaskObjectList ")
                    stdTaskList.actionSequence = inspectionSystemConfig.getSequence();
                    transmitManager.resultOfResponse.disconnect(stdTaskList.inspectionResultResponse)
                    transmitManager.resultOfResponse.connect(stdTaskList.inspectionResultResponse)
                    transmitManager.requestData(head1, para1, stdTaskList.actionSequence, os.i18n.ctr(qsTr("Get standardized inspection task suite fail")));
                }else if(action == "EXAM_ExamineTaskItemList"){
                    var head2 = {"Objects" : "EXAM_ExamineTaskItemList"};
                    var para2 = {"TaskObjectDataKey" : stdTaskList.otherParam["TaskObjectDataKey"]};
                    console.log("transmitManager.requestData   EXAM_ExamineTaskItemList ")
                    stdTaskList.actionSequence = inspectionSystemConfig.getSequence();
                    transmitManager.resultOfResponse.disconnect(stdTaskList.inspectionResultResponse)
                    transmitManager.resultOfResponse.connect(stdTaskList.inspectionResultResponse)
                    transmitManager.requestData(head2, para2, stdTaskList.actionSequence, os.i18n.ctr(qsTr("Get standardized inspection device list fail")));
                }
            }
        }else if(status == CPageStatus.Show){
            console.log("StandardBaseList.qml  status == CPageStatus.Show")
        }else if(status == CPageStatus.WillHide){
            console.log("StandardBaseList.qml  status == CPageStatus.WillHide")
            stdTaskList.refresh = true;
        }else if(status == CPageStatus.Hide){
            console.log("StandardBaseList.qml  status == CPageStatus.Hide")
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
        console.log("----stdTaskList-----Response--result, error,action, actionSequence ",
                      result,  error, action, stdTaskList.actionSequence);

        if(action != stdTaskList.actionSequence && action != stdTaskList.taskActionSequence){
            console.log("action != stdTaskList.actionSequence && action != stdTaskList.taskActionSequence return");
            return;
        }
        stdTaskList.refresh = true;
        if(action == stdTaskList.actionSequence){
            transmitManager.resultOfResponse.disconnect(stdTaskList.inspectionResultResponse);
        }
        if(result && error == 0){
            var headObj = map["head"]["objects"];
            if(headObj == "EXAM_ExamineTaskItemDetail"){
                if(map["tables"].length <= 0 || map["tables"][0]["value"].length <= 0){
                    CTOAST.requestToast(os.i18n.ctr(qsTr("No device")),"","");
                    return;
                }
                var dataMap1 = stdTaskList.originalData["tables"][0]["value"][stdTaskList.selectedIndex];
                var paramValue=  {
                    "IsRFIDScan": stdTaskList.isRFIDScan,
                    "IsBarCodeScan": stdTaskList.isBarCodeScan,
                    "LimitMode": stdTaskList.otherParam["LimitMode"],
                    "EnableUploadAttach": stdTaskList.otherParam["EnableUploadAttach"],
                    "TaskObjectDataKey": dataMap1["TaskObjectDataKey"],
                    "parentPage":stdTaskList
                }
                pageStack.push("qrc:/qml/StandardizedTask/DeviceStdCheckForm.qml", {"dataPackageValue": map, "paramValue": paramValue});
            } else if(stdTaskList.originalData["head"]["objects"] == "EXAM_ExamineTaskObjectList" && headObj == "EXAM_ExamineTaskItemList"){
                var dataMap = stdTaskList.originalData["tables"][0]["value"][stdTaskList.selectedIndex];
                console.log("规范检查组列表扫码进入规范检查项列表  otherParam   selectedIndex is ", stdTaskList.selectedIndex);
                var otherParam = {
                    "TaskObjectDataKey": dataMap["TaskObjectDataKey"],
                    "TaskObjectName": dataMap["TaskObjectName"],
                    "RealObjectID" : dataMap["RealObjectID"],
                    "LimitMode": stdTaskList.otherParam["LimitMode"],
                    "EnableUploadAttach": stdTaskList.otherParam["EnableUploadAttach"],
                    "EnabledScanItem": true,
                }
                pageStack.push("qrc:/qml/StandardizedTask/StandardBaseList.qml", { "originalData": map, "otherParam":otherParam});
            }else if(headObj == "EXAM_Off_GetPrePlanIDsByUserID"){
                CTOAST.requestToast("同步未来任务完成","","");
            } else {
                console.log("stdTaskList.model.setListData 00000000000000000000")
                stdTaskList.model.setListData(map["tables"][0]["value"]);
                stdTaskList.originalData = map;
            }
        }
    }

    Component.onCompleted: {
        //页面第一次加载，不刷新
        stdTaskList.refresh = false;
        var action = stdTaskList.originalData["head"]["objects"];
        var roles;
        if(action === "EXAM_ExamineTaskList"){ //任务列表
            //接受二次信号
            console.log("transmitManager.resultOfResponse.connect(stdTaskList.inspectionResultResponse)")
            transmitManager.resultOfResponse.connect(stdTaskList.inspectionResultResponse);
            //<Field Value="TaskIDやTaskDataKeyやTaskCodeやTaskNameやTaskTypeやEnableUploadAttachやLimitModeやCircleやExamObjectTypeやRate"/>
            roles = ["TaskID", "TaskCode", "TaskName", "TaskType", "Circle", "EnableUploadAttach", "TaskDataKey", "ExamObjectType", "LimitMode", "Rate", "SyncStatus"];
            var name;
            var clientPriority = inspectionSystemConfig.getClientPriority();
            if(clientPriority == false){
                name = inspectionSystemParam.getUserPermissionData(stdTaskList.nameId, "ModuleName");
            } else
                name = inspectionFuncsItemModel.getNodeParas(stdTaskList.nameId, "")["NameCN"];
            stdTaskList.titleText = name;
            stdTaskList.emptyText = "无规范检查任务";
            stdTaskList.delegateCom = taskListDelegate;
            inspectionTaskModel.setRolesName(roles);
            inspectionTaskModel.setListData(stdTaskList.originalData["tables"][0]["value"]);
            stdTaskList.model = inspectionTaskModel;
            stdTaskList.titleRightItemIcon = "qrc:/icon/action_bar_menu.png"
            stdTaskList.titleRightItemEnable = true;
            stdTaskList.futureEnable = true
            stdTaskList.offlineEnable = true
        }else if (action === "EXAM_ExamineTaskObjectList"){//套件列表
            roles = ["TaskObjectID", "TaskObjectDataKey","TaskID" ,"TaskObjectName","ExamObjectType", "RealObjectID", "TotalItemCount", "ExamItemCount",
                    "RFIDCode", "BarCode", "QRCode"];
            stdTaskList.titleText = stdTaskList.otherParam["TaskName"];
            stdTaskList.emptyText = "无规范检查任务组";
            stdTaskList.delegateCom = suitListDelegate;
            stdTaskList.enabledScanItem = stdTaskList.otherParam["EnabledScanItem"];
            inspectionTaskSuiteModel.setRolesName(roles);
            inspectionTaskSuiteModel.setListData(stdTaskList.originalData["tables"][0]["value"]);
            stdTaskList.model = inspectionTaskSuiteModel;
        }else if (action === "EXAM_ExamineTaskItemList"){//设备列表
            roles = ["TaskItemID", "TaskItemDataKey", "ItemName", "ItemFullName", "TaskObjectDataKey", "TaskID", "SerialKey",
                    "RFIDCode", "BarCode", "QRCode"];
            stdTaskList.enabledScanItem = stdTaskList.otherParam["EnabledScanItem"];
            stdTaskList.titleText = stdTaskList.otherParam["TaskObjectName"];
            stdTaskList.emptyText = "无规范检查任务对象";
            stdTaskList.delegateCom = deviceListDelegate;
            inspectionTaskItemModel.setRolesName(roles);
            inspectionTaskItemModel.setListData(stdTaskList.originalData["tables"][0]["value"]);
            stdTaskList.model = inspectionTaskItemModel;
        }
    }
    Component.onDestruction: {
        console.log("StandardBaseList.qml destruction")
        transmitManager.resultOfResponse.disconnect(stdTaskList.inspectionResultResponse);
    }

    onSyncPreTask:{
        console.debug("---------------onSyncPreTask-----------dateTime: ", dateTime)
        var head = {"Objects" : "EXAM_Off_GetPrePlanIDsByUserID"};
        var para = {"UserID" : inspectionSystemParam.getUserId(), "PreBuildEndTimeStr" : dateTime};
        console.log("transmitManager.syncPreTask   ", "EXAM_Off_GetPrePlanIDsByUserID");
        stdTaskList.actionSequence = inspectionSystemConfig.getSequence();
        transmitManager.resultOfResponse.disconnect(stdTaskList.inspectionResultResponse)
        transmitManager.resultOfResponse.connect(stdTaskList.inspectionResultResponse)
        transmitManager.syncPreTask(head, para, stdTaskList.actionSequence, "获取规范检查未来任务失败");
    }

    onSyncTask: {
        console.debug("---------------onSyncTask-----------EXAM_ExamineTaskList------")
        var head = {"Objects" : "EXAM_ExamineTaskList"};
        var para = {"UserID" : inspectionSystemParam.getUserId()};
        stdTaskList.taskActionSequence = inspectionSystemConfig.getSequence();
        transmitManager.requestTaskData(head, para, stdTaskList.taskActionSequence, os.i18n.ctr(qsTr("Get standardized inspection list fail")));
    }
}
