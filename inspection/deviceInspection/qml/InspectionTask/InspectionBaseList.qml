import QtQuick 2.0
import com.syberos.basewidgets 2.0
import "../Common"
BaseList{
    id:inspectionTaskList
    property string nameId: ""
    property string taskActionSequence: ""
    property bool isDelegateDataSaved: false
    //组对象数据编辑　０，　设备个性化编辑　１
    property string isEditGroupData: "0"
    property var savedDelegateItemData
    property var savedPersonalityData : {return {} }
    property var savedPersonalityAttach: []
    property var saveItemData : {return {} }
    property var saveItemAttach : {return {} }
    property int selectedIndex: -1
    property string delegateObjectDataKey: ""
    leftItemTriggeredDefaultProcess: !isDelegateDataSaved

    Connections{
        target: rootWindow
        onKeyBackTriggered:{
            console.log("rootWindow onKeyBackTriggered")
            if(inspectionTaskList.status == CPageStatus.Show && inspectionTaskList.isDelegateDataSaved == true) {
                ensureSubmitDlg.type = "backToCancel";
                ensureSubmitDlg.show();
            } else if(inspectionTaskList.status == CPageStatus.Show){
                pageStack.pop();
            }
        }
        onToHome:{
            if(inspectionTaskList.status == CPageStatus.Show && inspectionTaskList.isDelegateDataSaved == true) {
                ensureSubmitDlg.type = "homeToCancel";
                ensureSubmitDlg.show();
            }
        }
    }

    //任务列表代理
    Component{
        id:taskListDelegate
        InspectionTaskListDelegate{}
    }
    //套件列表代理
    Component{
        id:suitListDelegate
        InspectionSuiteDelegate{
        }
    }
    //设备列表代理
    Component{
        id:deviceListDelegate
        InspectionDeviceListDelegate{
            id:delegate
        }
    }

    onTitleBarLeftItemTriggered:{
        ensureSubmitDlg.type = "backToCancel";
        ensureSubmitDlg.show();
    }

    onSubmitBtnClicked: {
        ensureSubmitDlg.type = "submit";
        ensureSubmitDlg.show();
    }

    CDialog{
        id: ensureSubmitDlg
        property string type: ""
        titleText : "设备巡检提交"
        messageText:type == "submit" ? "提交之后信息不可修改,确定提交吗?" : "确认放弃数据提交吗?"
        acceptedButtonText: "确定"
        rejectButtonText: "取消"
        onAccepted:{
            if(type == "submit"){
                inspectionTaskList.submitDeviceList();
            }else if(type == "backToCancel"){
                pageStack.pop();
            }else if (type == "homeToCancel")
                rootWindow.toHomeBack();
        }
    }

    function showGroupModelSelect(taskObjectDataKey){
        inspectionTaskList.delegateObjectDataKey = taskObjectDataKey;
        if(inspectionTaskList.isDelegateDataSaved)
            editGroupListDlg.show();
        else
            getObjectDetail(inspectionTaskList.delegateObjectDataKey);
    }

    CListDialog{
        id: editGroupListDlg
        maxVisibleItems: 4
        titleAreaEnabled: false
        buttonAreaEnabled: false
        model:["组对象数据编辑", "设备个性化编辑"]
        onDelegateItemTriggered:{
            editGroupListDlg.hide();
            inspectionTaskList.isEditGroupData = index.toString();
            getObjectDetail(inspectionTaskList.delegateObjectDataKey);
        }
    }

    function getObjectDetail(taskObjectDataKey){
        transmitManager.resultOfResponse.disconnect(inspectionTaskList.objectDetailResultResponse)
        transmitManager.resultOfResponse.connect(inspectionTaskList.objectDetailResultResponse)
        inspectionTaskList.actionSequence = inspectionSystemConfig.getSequence();
        var head = {"Objects" : "EISM_GetTaskObjectDetail"};
        var para = {"TaskObjectDataKey" : taskObjectDataKey};
        transmitManager.requestData(head, para, inspectionTaskList.actionSequence, "获取巡检项详情失败");
    }

    function submitDeviceList(){
        var head = {"Objects" : "EISM_TaskGroupObject_UpdateOne"};
        var table = {"Name":"ISM_InspectionTaskObject", "Key":"TaskObjectDataKey"};
        var valueMapList = [];
        valueMapList.push(inspectionTaskList.savedDelegateItemData);
        console.log("inspectionTaskList.savedPersonalityData length is ", Object.keys(inspectionTaskList.savedPersonalityData).length);
        if(Object.keys(inspectionTaskList.savedPersonalityData).length > 0)
            valueMapList.push(inspectionTaskList.savedPersonalityData);
        for(var key in inspectionTaskList.saveItemData){
            console.log("inspectionTaskList.saveItemData  key is ", key);
            valueMapList.push(inspectionTaskList.saveItemData[key]);
        }
        var attachs = [];
        for(var key1 in saveItemAttach){
            Array.prototype.push.apply(attachs, saveItemAttach[key1]);
        }
        attachs = attachs.concat(savedPersonalityAttach);

        console.log("submitDeviceList  attachs length is    ", attachs.length);
        transmitManager.resultOfResponse.disconnect(inspectionTaskList.submitDeviceListResponse)
        transmitManager.resultOfResponse.connect(inspectionTaskList.submitDeviceListResponse)
        inspectionTaskList.actionSequence = inspectionSystemConfig.getSequence();
        transmitManager.submitData(head, table, valueMapList, attachs, "",
                                   inspectionTaskList.actionSequence, "巡检组对象提交失败");
    }

    onResultBarCode: {
        var headObj = inspectionTaskList.originalData["head"]["objects"];
        if(headObj == "EISM_GetTaskGroupList")
            getObjectListByCode(barCode, "BarCode");
        else
            getObjectByCode(barCode, "BarCode");
    }
    onResultRFIDCode: {
        var headObj = inspectionTaskList.originalData["head"]["objects"];
        if(headObj == "EISM_GetTaskGroupList")
            getObjectListByCode(rfidCode, "RFIDCode");
        else
            getObjectByCode(rfidCode, "RFIDCode");
    }

    function getObjectListByCode(code, codeType){
        var data = inspectionTaskList.originalData["tables"][0]["value"];
        if(data.length == 0){
            CTOAST.requestToast("没有匹配的巡检组","","");
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
            CTOAST.requestToast("没有匹配的巡检组","","");
            return;
        }
        inspectionTaskList.selectedIndex = i;
        transmitManager.resultOfResponse.disconnect(inspectionTaskList.inspectionResultResponse)
        transmitManager.resultOfResponse.connect(inspectionTaskList.inspectionResultResponse)
        inspectionTaskList.actionSequence = inspectionSystemConfig.getSequence();
        var head = {"Objects" : "EISM_GetTaskObjectListByGroup"};
        var para = {"TaskDataKey" : data[i]["TaskDataKey"],
            "TaskGroupDataKey" : data[i]["TaskGroupDataKey"]};
        console.log("getObjectListByCode  EISM_GetTaskObjectListByGroup  TaskDataKey ", data[i]["TaskDataKey"], "    TaskGroupDataKey ", data[i]["TaskGroupDataKey"])
        transmitManager.requestData(head, para, inspectionTaskList.actionSequence, "获取巡检对象列表失败");
    }

    function getObjectByCode(code, codeType){
        console.log("inspectionTaskList   ", code, codeType);
        var data = inspectionTaskList.originalData["tables"][0]["value"];
        if(data.length == 0){
            CTOAST.requestToast("没有匹配的巡检项","","");
            return;
        }
        var headObj = inspectionTaskList.originalData["head"]["objects"];
        if(headObj === "EISM_GetTaskObjectListByTask" ||
                headObj == "EISM_GetTaskObjectListByGroup" ||
                headObj == "EISM_GetTaskObjectListByGroup_RFID"){
            if((inspectionTaskList.otherParam["InspectionMode"] == "3"
                &&(code != data[0]["BarCode"].toLowerCase() && code != data[0]["RFIDCode"].toLowerCase() && code != data[0]["QRCode"].toLowerCase()))
                    || (inspectionTaskList.otherParam["InspectionMode"] == "5"
                        && (code != data[0]["BarCode"].toLowerCase() && code != data[0]["RFIDCode"].toLowerCase() && code != data[0]["QRCode"].toLowerCase())
                        && isDelegateDataSaved == false)){
                //路线巡检
                console.log("InspectionMode is ", inspectionTaskList.otherParam["InspectionMode"]);
                CTOAST.requestToast("没有匹配的巡检项","","");
                return;
            } else {
                console.log("inspectionBaseList data.length is ", data.length);
                var i = 0;
                for( ; i < data.length; i++){
                    var barCode = data[i]["BarCode"].toLowerCase();
                    var qrCode = data[i]["QRCode"].toLowerCase();
                    var rfidCode = String(data[i]["RFIDCode"]).toLowerCase();
                    if(code == barCode || code == rfidCode || code == qrCode)
                        break;
                }
                if(i >= data.length){
                    CTOAST.requestToast("没有匹配的巡检项","","");
                    return;
                }
                inspectionTaskList.selectedIndex = i;
                if(codeType == "BarCode"){
                    inspectionTaskList.isBarCodeScan = "1";
                    inspectionTaskList.isRFIDScan = "0";
                } else if(codeType == "RFIDCode"){
                    inspectionTaskList.isBarCodeScan = "0";
                    inspectionTaskList.isRFIDScan = "1";
                }
                if(inspectionTaskList.otherParam["InspectionMode"] == "5" && data[i]["IsDelegate"] == "1" && inspectionTaskList.isDelegateDataSaved == true)
                    showGroupModelSelect(data[i]["TaskObjectDataKey"]);
                else
                    getObjectDetail(data[i]["TaskObjectDataKey"]);
            }
        }
    }

    onSearchTextChanged: {
        if(!searchInputItem.visible)
            return;
        console.log("onSearchTextChanged :",text, "  inspectionTaskItemModel.count ", inspectionTaskItemModel.rowCount());
        commonProxyModel.setModelFilter(".*" + text.trim() + ".*");
        console.log("commonProxyModel.filterRegExp  ",  ".*" + text.trim() + ".*");
    }

    function setItemDataSaved(selectedIndex){
        inspectionTaskItemModel.setData(selectedIndex, "Saved", "1");
        inspectionTaskItemModel.updateData();
    }

    onStatusChanged:{
        if(status == CPageStatus.WillShow){
            console.log("InspectionBaseList.qml  refresh ===", refresh);
            if(refresh === true){
                var headerObj = inspectionTaskList.originalData["head"]["objects"];
                 //如果时组对象巡检，回退到巡检对象列表时不刷新
                if(  (inspectionTaskList.otherParam["InspectionMode"] == "5")
                        && (headerObj === "EISM_GetTaskObjectListByTask" ||
                            headerObj == "EISM_GetTaskObjectListByGroup" ||
                            headerObj == "EISM_GetTaskObjectListByGroup_RFID")){
                    if(inspectionTaskList.isDelegateDataSaved == true)
                        inspectionTaskList.showSubmitBtn = true;
                    console.log("inspectionTaskList.otherParam[InspectionMode] == 5  isDelegateDataSaved ", inspectionTaskList.isDelegateDataSaved);
                    return;
                }

                inspectionTaskList.actionSequence = inspectionSystemConfig.getSequence();
                if(headerObj == "EISM_GetUserRunningTask"){
                    transmitManager.resultOfResponse.disconnect(inspectionTaskList.inspectionResultResponse)
                    transmitManager.resultOfResponse.connect(inspectionTaskList.inspectionResultResponse)

                    var head = {"Objects" : "EISM_GetUserRunningTask"};
                    var para = {"UserID" : inspectionSystemParam.getUserId()};
                    console.log("transmitManager.requestData   EISM_GetUserRunningTask ")
                    inspectionTaskList.taskActionSequence = inspectionSystemConfig.getSequence();
                    transmitManager.requestData(head, para, inspectionTaskList.taskActionSequence, os.i18n.ctr(qsTr("Get inspection task fail")));

                }else if(headerObj == "EISM_GetTaskGroupList"){
                    console.log("headerObj is EISM GetTaskGroupList");
                    transmitManager.resultOfResponse.disconnect(inspectionTaskList.inspectionResultResponse)
                    transmitManager.resultOfResponse.connect(inspectionTaskList.inspectionResultResponse)
                    var head1 = {"Objects" : "EISM_GetTaskGroupList"};
                    var para1 = {"UserID" : inspectionSystemParam.getUserId(), "TaskDataKey" : inspectionTaskList.otherParam["TaskDataKey"]};
                    console.log("transmitManager.requestData   EISM_GetTaskGroupList ")
                    transmitManager.requestData(head1, para1, inspectionTaskList.actionSequence, "获取巡检组列表失败");
                }else if(headerObj === "EISM_GetTaskObjectListByTask" || headerObj == "EISM_GetTaskObjectListByGroup"){
                    transmitManager.resultOfResponse.disconnect(inspectionTaskList.inspectionResultResponse)
                    transmitManager.resultOfResponse.connect(inspectionTaskList.inspectionResultResponse)
                    var head2 = {"Objects" : headerObj};
                    var para2 = {};
                    if(headerObj === "EISM_GetTaskObjectListByTask")
                        para2 = {"UserID" : inspectionSystemParam.getUserId(), "TaskDataKey" : inspectionTaskList.otherParam["TaskDataKey"]};
                    else if(headerObj == "EISM_GetTaskObjectListByGroup")
                        para2 = {"TaskDataKey" : inspectionTaskList.otherParam["TaskDataKey"],
                            "TaskGroupDataKey" : inspectionTaskList.otherParam["TaskGroupDataKey"]};
                    console.log("transmitManager.requestData   ", headerObj);
                    transmitManager.requestData(head2, para2, inspectionTaskList.actionSequence, "获取巡检对象列表失败");
                }
            }
        }else if(status == CPageStatus.Show){
            rootWindow.dialogShow = true;
            console.log("InspectionBaseList.qml  status == CPageStatus.Show")
        }else if(status == CPageStatus.WillHide){
            rootWindow.dialogShow = false;
            console.log("InspectionBaseList.qml  status == CPageStatus.WillHide")
            inspectionTaskList.refresh = true;
            searchInputItem.visible = false;
            searchInputItem.inputText = "";
            rootWindow.closeKeyboard();
            var headerObj1 = inspectionTaskList.originalData["head"]["objects"] ;
            if(headerObj1 == "EISM_GetTaskObjectListByTask" ||
                    headerObj1 == "EISM_GetTaskObjectListByGroup" ||
                    headerObj1 == "EISM_GetTaskObjectListByGroup_RFID"){
                commonProxyModel.setModelFilter(".*.*")
            }
        }else if(status == CPageStatus.Hide){
            console.log("InspectionBaseList.qml  status == CPageStatus.Hide")
        }
    }

    /**
     * 接受服务器返回数据的槽函数
     * @param result:bool 获取数据是否成功
     * @param error:int 错误代码
     * @param action:string 当前发送数据动作类型
     * @param map:var    服务器返回值
     */
    function objectDetailResultResponse(result, error, action, map){
        console.log("----inspectionTaskList-----objectDetailResultResponse--result, error,action, actionSequence ",
                      result,  error, action, inspectionTaskList.actionSequence);
        if(action != inspectionTaskList.actionSequence){
            console.log("action != inspectionTaskList.actionSequence  return");
            return;
        }
        inspectionTaskList.refresh = true;
        transmitManager.resultOfResponse.disconnect(inspectionTaskList.objectDetailResultResponse)
        if(result && error == 0){
            var headerObj = map["head"]["objects"];
            if(headerObj == "EISM_GetTaskObjectDetail"){
                if(map["tables"].length <= 0 || map["tables"][0]["value"].length <= 0){
                    CTOAST.requestToast(os.i18n.ctr(qsTr("No device")),"","");
                    return;
                }
                var paramValue =  {
                    "IsRFIDScan":  inspectionTaskList.isRFIDScan,
                    "IsBarCodeScan": inspectionTaskList.isBarCodeScan,
                    "EnableUploadAttach": inspectionTaskList.otherParam["EnableUploadAttach"],
                    "InspectionMode":inspectionTaskList.otherParam["InspectionMode"],
                    "PageFormType":"inspection",
                    "IsEditGroupData" : inspectionTaskList.isEditGroupData,
                    "IsDelegate": inspectionTaskList.originalData["tables"][0]["value"][inspectionTaskList.selectedIndex]["IsDelegate"],
                    "SelectedIndex" : inspectionTaskList.selectedIndex,
                    "ParentPage":inspectionTaskList
                };
                pageStack.push("qrc:/qml/InspectionTask/DeviceInspectionForm.qml",
                               {"dataPackageValue": map,
                                   "paramValue": paramValue});
            }
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
        console.log("----inspectionTaskList-----Response--result, error,action, actionSequence ",
                      result,  error, action, inspectionTaskList.actionSequence);
        if(action != inspectionTaskList.actionSequence && action != inspectionTaskList.taskActionSequence){
            console.log("action != inspectionTaskList.actionSequence  return");
            return;
        }
        inspectionTaskList.refresh = true;
        if(action == inspectionTaskList.actionSequence)
            transmitManager.resultOfResponse.disconnect(inspectionTaskList.inspectionResultResponse)
        if(result && error == 0){
            var headerObj = map["head"]["objects"];
            if(headerObj == "EISM_GetTaskObjectDetail"){
                if(map["tables"].length <= 0 || map["tables"][0]["value"].length <= 0){
                    CTOAST.requestToast(os.i18n.ctr(qsTr("No device")),"","");
                    return;
                }
                var paramValue =  {
                    "IsRFIDScan":  inspectionTaskList.isRFIDScan,
                    "IsBarCodeScan": inspectionTaskList.isBarCodeScan,
                    "EnableUploadAttach": inspectionTaskList.otherParam["EnableUploadAttach"],
                    "InspectionMode":inspectionTaskList.otherParam["InspectionMode"],
                    "PageFormType":"inspection",
                    "IsDelegate": inspectionTaskList.originalData["tables"][0]["value"][inspectionTaskList.selectedIndex]["IsDelegate"],
                    "SelectedIndex" : inspectionTaskList.selectedIndex,
                    "ParentPage":inspectionTaskList
                };
                pageStack.push("qrc:/qml/InspectionTask/DeviceInspectionForm.qml",
                               {"dataPackageValue": map,
                                   "paramValue": paramValue});
            } else if((inspectionTaskList.originalData["head"]["objects"] == "EISM_GetTaskGroupList" ||
                       inspectionTaskList.originalData["head"]["objects"] == "EISM_GetTaskObjectListByGroup_RFID")
                      && headerObj == "EISM_GetTaskObjectListByGroup"){
                //巡检点组列表扫码进入设备列表
                var dataMap = inspectionTaskList.originalData["tables"][0]["value"][inspectionTaskList.selectedIndex];
                console.log("EISM_GetTaskObjectListByGroup  otherParam TaskGroupName ", dataMap["TaskGroupName"], "  selectedIndex is ", inspectionTaskList.selectedIndex);
                var otherParam = {
                    "TaskGroupName": dataMap["TaskGroupName"],
                    "TaskGroupDataKey": dataMap["TaskGroupDataKey"],
                    "TaskDataKey" : dataMap["TaskDataKey"],
                    "LimitMode": inspectionTaskList.otherParam["LimitMode"],
                    "EnableUploadAttach": inspectionTaskList.otherParam["EnableUploadAttach"],
                    "EnabledScanItem": true,
                    "InspectionMode":inspectionTaskList.otherParam["InspectionMode"]
                }
                pageStack.push("qrc:/qml/InspectionTask/InspectionBaseList.qml",
                               {
                                   "originalData": map,
                                   "otherParam":otherParam,
                               });
            }else if(headerObj == "EISM_Off_GetPrePlanIDsByUserID"){
                CTOAST.requestToast("同步未来任务完成","","");
            } else {
                console.log("inspectionTaskList.model.setListData 00000000000000000000")
                inspectionTaskList.model.setListData(map["tables"][0]["value"]);
                inspectionTaskList.originalData = map;
                console.log("inspectionResultResponse listMOdel  list  length is  ", inspectionTaskList.originalData["tables"][0]["value"].length)
                if ((headerObj === "EISM_GetTaskObjectListByTask" || headerObj == "EISM_GetTaskObjectListByGroup")
                        && inspectionTaskItemModel.rowCount() == 0) {
                        inspectionTaskList.titleRightItemEnable = false;
                }
            }
        }
    }

    function submitDeviceListResponse(result, error, action, map){
        console.log("----inspectionTaskList-----submitDeviceListResponse--result, error,action, actionSequence ",
                      result,  error, action, inspectionTaskList.actionSequence);
        if(action != inspectionTaskList.actionSequence){
            console.log("action != inspectionTaskList.actionSequence  return");
            return;
        }

        transmitManager.resultOfResponse.disconnect(inspectionTaskList.submitDeviceListResponse)
        if(result && error == 0){
            CTOAST.requestToast("巡检组对象提交成功", "", "");
            pageStack.pop();
        }
    }

    Component.onCompleted: {
        //页面第一次加载，不刷新
        inspectionTaskList.refresh = false;
        var headObj = inspectionTaskList.originalData["head"]["objects"];
        console.log("Component listMOdel  list  length is  ", inspectionTaskList.originalData["tables"][0]["value"].length)
        var roles;
        if(headObj === "EISM_GetUserRunningTask"){ //任务列表
            transmitManager.resultOfResponse.connect(inspectionTaskList.inspectionResultResponse);
            roles = ["TaskDataKey", "TaskName", "TaskCode", "LimitMode", "EnableUploadAttach", "InspectionMode", "SyncStatus", "Circle", "Rate"];
            var name = "";
            var clientPriority = inspectionSystemConfig.getClientPriority();
            if(clientPriority == false){
                name = inspectionSystemParam.getUserPermissionData(inspectionTaskList.nameId, "ModuleName");
            } else
                name = inspectionFuncsItemModel.getNodeParas(inspectionTaskList.nameId, "")["NameCN"];

            if(name == "")
                name = "设备巡检";
            console.log("completed  name is  ", name)
            inspectionTaskList.titleText = name;
            inspectionTaskList.emptyText = "无巡检任务";
            inspectionTaskList.delegateCom = taskListDelegate;
            inspectionTaskModel.setRolesName(roles);
            inspectionTaskModel.setListData(inspectionTaskList.originalData["tables"][0]["value"]);
            inspectionTaskList.model = inspectionTaskModel;
            inspectionTaskList.titleRightItemIcon = "qrc:/icon/action_bar_menu.png"
            inspectionTaskList.titleRightItemEnable = true;
            inspectionTaskList.futureEnable = true
            inspectionTaskList.offlineEnable = true
        }else if (headObj === "EISM_GetTaskGroupList"){//套件列表
            roles = ["TaskGroupDataKey", "TaskDataKey", "TaskGroupName", "PostID", "TotalObjectCount", "ExamObjectCount",
                     "RFIDCode", "BarCode", "QRCode","OrderIndex"];
            inspectionTaskList.titleText = inspectionTaskList.otherParam["TaskName"];
            inspectionTaskList.emptyText = "无巡检任务组";
            inspectionTaskList.delegateCom = suitListDelegate;
            inspectionTaskSuiteModel.setRolesName(roles);
            inspectionTaskSuiteModel.setListData(inspectionTaskList.originalData["tables"][0]["value"]);
            inspectionTaskList.model = inspectionTaskSuiteModel;
        }else if (headObj === "EISM_GetTaskObjectListByTask" || headObj == "EISM_GetTaskObjectListByGroup" || headObj == "EISM_GetTaskObjectListByGroup_RFID") {//设备列表
            roles = ["RealObjectID", "TaskObjectDataKey", "ObjectName", "ObjectFullName", "RealObjectType", "RFIDCode", "BarCode", "QRCode",
                     "OrderIndex", "SerialKey",  "IsDelegate", "Positions", "ModelName", "DeviceLastModifiedDate", "Saved"];
            if(headObj == "EISM_GetTaskObjectListByTask")
                inspectionTaskList.titleText = inspectionTaskList.otherParam["TaskName"];
            else if(headObj == "EISM_GetTaskObjectListByGroup" || headObj == "EISM_GetTaskObjectListByGroup_RFID")
                inspectionTaskList.titleText = inspectionTaskList.otherParam["TaskGroupName"];
            inspectionTaskList.emptyText = "无巡检任务对象"
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
        if(inspectionTaskList.otherParam["InspectionMode"] == "5"){
            var attachs = [];
            for(var key1 in inspectionTaskList.saveItemAttach){
                Array.prototype.push.apply(attachs, inspectionTaskList.saveItemAttach[key1]);
            }
            var imgList = [];
            for(var i = 0; i < attachs.length; i++){
                console.log("InspectionBaseList.qml 8888888", attachs[i]["Path"])
                imgList.push(attachs[i]["Path"]);
            }
            if(imgList.length > 0)
                multimediaUtils.removeMultimediaFile(imgList);
        }
    }

    onSyncPreTask:{
        console.debug("---------------onSyncPreTask-----------dateTime: ", dateTime)
        var head = {"Objects" : "EISM_Off_GetPrePlanIDsByUserID"};
        var para = {"UserID" : inspectionSystemParam.getUserId(), "PreBuildEndTimeStr" : dateTime};
        console.log("transmitManager.syncPreTask   ", "EISM_Off_GetPrePlanIDsByUserID");
        inspectionTaskList.actionSequence = inspectionSystemConfig.getSequence();
        transmitManager.resultOfResponse.disconnect(inspectionTaskList.inspectionResultResponse)
        transmitManager.resultOfResponse.connect(inspectionTaskList.inspectionResultResponse)
        transmitManager.syncPreTask(head, para, inspectionTaskList.actionSequence, "获取巡检未来任务失败");
    }

    onSyncTask: {
        console.debug("---------------onSyncTask-----------EISM_GetUserRunningTask------")
        var head = {"Objects" : "EISM_GetUserRunningTask"};
        var para = {"UserID" : inspectionSystemParam.getUserId()};
        inspectionTaskList.taskActionSequence = inspectionSystemConfig.getSequence();
        transmitManager.requestTaskData(head, para, inspectionTaskList.taskActionSequence, os.i18n.ctr(qsTr("Get inspection task fail")));
    }
}
