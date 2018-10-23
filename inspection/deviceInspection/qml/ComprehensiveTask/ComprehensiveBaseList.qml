import QtQuick 2.0
import com.syberos.basewidgets 2.0
import "../Common"
BaseList{
    id:comprehensiveTaskList
    //获取任务的action
    property string taskActionSequence: ""
    //1，设备编码；2，FIRD码；3，二维码；4，条形码
    property string codeType: ""
    property string eRBQCode: ""
    //领用任务列表代理
    Component{
        id:recipientsPlanListDelegate
        RecipientsPlanListDelegate{}
    }
    //调拨任务列表代理
    Component{
        id:transferTaskListDelegate
        TransferPlanListDelegate{}
    }
    //设备列表代理
    Component{
        id:deviceListDelegate
        ComprehensiveDeviceListDelegate{}
    }

    onResultRFIDCode: {
        console.log("comprehensiveTaskList  onResultRFIDCode  ", rfidCode)
        comprehensiveTaskList.isBarCodeScan = "0";
        comprehensiveTaskList.isRFIDScan = "1";
        comprehensiveTaskList.codeType = "2";
        comprehensiveTaskList.eRBQCode = rfidCode;
        getDeviceDetail(rfidCode, "", true);
    }

    onResultBarCode: {
        console.log("comprehensiveTaskList  onResultBarCode  ", barCode)
        comprehensiveTaskList.isBarCodeScan = "1";
        comprehensiveTaskList.isRFIDScan = "0";
        comprehensiveTaskList.codeType = "3";
        comprehensiveTaskList.eRBQCode = barCode;
        getDeviceDetail("", barCode, false);
    }


    function getDeviceDetail(rfidCode, barCode, rfidORbar){
        var headObj = comprehensiveTaskList.originalData["head"]["objects"];
        if(headObj == "EMD_DistributeEquipmentList"){
            transmitManager.resultOfResponse.disconnect(comprehensiveTaskList.resultResponse)
            transmitManager.resultOfResponse.connect(comprehensiveTaskList.resultResponse)
            comprehensiveTaskList.actionSequence = inspectionSystemConfig.getSequence();
            var head = {};
            head["Objects"] = "EMD_DistributeEquipment_Para";
            var para = {};
            para["DistributeID"] = comprehensiveTaskList.otherParam["DistributeID"];
            para["EquipmentID"] = "";
            if(rfidORbar){
                para["RFIDCode"] = rfidCode;
                para["BarCode"] = "";
            } else {
                para["RFIDCode"] = "";
                para["BarCode"] = barCode;
            }
            console.log("transmitManager.requestData   EMD_DistributeEquipment_Para ")
            transmitManager.requestData(head, para, comprehensiveTaskList.actionSequence, os.i18n.ctr(qsTr("Get distribute equipment para fail")));
        }
    }

    onStatusChanged:{
        if(status == CPageStatus.WillShow){
            console.log("InspectionBaseList.qml  refresh ===", refresh);
            if(refresh === true){
                var headObj = comprehensiveTaskList.originalData["head"]["objects"];
                var organiseUnitIDs = inspectionSystemParam.getOrganiseUnitIDs();
                var departmentIDs = inspectionSystemParam.getUserPropertyByKey("DepartmentIDs");
                if(headObj == "EMD_Distribute_PlanList"){  //设备领用  任务列表
                    transmitManager.resultOfResponse.disconnect(comprehensiveTaskList.resultResponse)
                    transmitManager.resultOfResponse.connect(comprehensiveTaskList.resultResponse)

                    var head = {"Objects" : "EMD_Distribute_PlanList"};
                    var para = {"OrganiseUnitID" : organiseUnitIDs,
                        "DepartmentID" : departmentIDs};
                    console.log("transmitManager.requestData   EMD_Distribute_PlanList ")
                    comprehensiveTaskList.taskActionSequence = inspectionSystemConfig.getSequence();
                    transmitManager.requestData(head, para, comprehensiveTaskList.taskActionSequence, "获取领用任务列表失败");
                }else if(headObj == "EMA_Allocation_PlanList"){ //设备调拨 任务列表
                    transmitManager.resultOfResponse.disconnect(comprehensiveTaskList.resultResponse)
                    transmitManager.resultOfResponse.connect(comprehensiveTaskList.resultResponse)

                    var head2 = {"Objects" : "EMA_Allocation_PlanList"};
                    var para2 = {"OrganiseUnitID" : organiseUnitIDs,
                        "DepartmentID" : departmentIDs};
                    console.log("transmitManager.requestData   EMA_Allocation_PlanList ")
                    comprehensiveTaskList.taskActionSequence = inspectionSystemConfig.getSequence();
                    transmitManager.requestData(head2, para2, comprehensiveTaskList.taskActionSequence, "获取调拨任务列表失败");

                }else if(headObj == "EMD_DistributeEquipmentList"){  //设备领用  设备列表
                    transmitManager.resultOfResponse.disconnect(comprehensiveTaskList.resultResponse)
                    transmitManager.resultOfResponse.connect(comprehensiveTaskList.resultResponse)
                    var head1 = {"Objects" : "EMD_DistributeEquipmentList"};
                    var para1 = {"OrderCode" : comprehensiveTaskList.otherParam["OrderCode"]};
                    console.log("transmitManager.requestData   EMD_DistributeEquipmentList ")
                    comprehensiveTaskList.actionSequence = inspectionSystemConfig.getSequence();
                    transmitManager.requestData(head1, para1, comprehensiveTaskList.actionSequence, "获取领用设备列表失败");
                }
            }

        }else if(status == CPageStatus.Show){
            console.log("ComprehensiveBaseList.qml  status == CPageStatus.Show")
        }else if(status == CPageStatus.WillHide){
            console.log("ComprehensiveBaseList.qml  status == CPageStatus.WillHide")
            comprehensiveTaskList.refresh = true;
        }else if(status == CPageStatus.Hide){
            console.log("ComprehensiveBaseList.qml  status == CPageStatus.Hide")
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
        console.log("----comprehensiveTaskList-----Response--result, error,action, actionSequence ",
                      result,  error, action, comprehensiveTaskList.actionSequence);

        if(action != comprehensiveTaskList.actionSequence && action != comprehensiveTaskList.taskActionSequence){
            console.log("action != comprehensiveTaskList.actionSequence  return && action != comprehensiveTaskList.taskActionSequence return" );
            return;
        }
        comprehensiveTaskList.refresh = true;
        if(action == comprehensiveTaskList.actionSequence){
            transmitManager.resultOfResponse.disconnect(comprehensiveTaskList.resultResponse);
        }
        if(result && error == 0){
            var headObj = map["head"]["objects"];
            if(headObj == "EMD_DistributeEquipment_Para"){
                if(map["tables"].length <= 0 || map["tables"][0]["value"].length <= 0){
                    CTOAST.requestToast(os.i18n.ctr(qsTr("No device")),"","");
                    return;
                }
                var paramValue=  {
                    "CodeType" : comprehensiveTaskList.codeType,
                    "ERBQCode" : comprehensiveTaskList.eRBQCode,
                    "IsRFIDScan": comprehensiveTaskList.isRFIDScan,
                    "IsBarCodeScan": comprehensiveTaskList.isBarCodeScan,
                    "parentPage":comprehensiveTaskList
                };
                pageStack.push("qrc:/qml/ComprehensiveTask/DeviceRecipientsDetail.qml",
                               {"paramValue":paramValue,"dataPackageValue":map});
            }else{
                console.log("comprehensiveTaskList.model.setListData 00000000000000000000")
                comprehensiveTaskList.model.setListData(map["tables"][0]["value"]);
                comprehensiveTaskList.originalData = map;
            }
        }
    }

    Component.onCompleted: {
        //页面第一次加载，不刷新
        comprehensiveTaskList.refresh = false;
        var headObj = comprehensiveTaskList.originalData["head"]["objects"];
        var roles;
        if(headObj === "EMD_Distribute_PlanList"){ //设备领用  任务列表
            //接受二次信号
            console.log("transmitManager.resultOfResponse.connect(comprehensiveTaskList.resultResponse)")
            transmitManager.resultOfResponse.connect(comprehensiveTaskList.resultResponse);
            //<Field Value="IDやOrderCodeやOrderNameやOperatorUserNameや
            //OperatorDateやAuditDateやAuditUserNameやOrganiseUnitIDやDepartMentIDやoperatorrate"/>
            roles = ["ID", "OrderCode", "OrderName", "OrganiseUnitID", "DepartMentID", "OperatorDate", "operatorrate","SyncStatus"];
            comprehensiveTaskList.titleText = "设备领用";
            comprehensiveTaskList.emptyText = "无设备领用任务";
            comprehensiveTaskList.delegateCom = recipientsPlanListDelegate;
            inspectionTaskModel.setRolesName(roles);
            inspectionTaskModel.setListData(comprehensiveTaskList.originalData["tables"][0]["value"]);
            comprehensiveTaskList.model = inspectionTaskModel;
            comprehensiveTaskList.titleRightItemIcon = "qrc:/icon/action_bar_menu.png"
            comprehensiveTaskList.titleRightItemEnable = true;
            comprehensiveTaskList.offlineEnable = true
            if(networkStatus.getNetworkConnectStatus() == ""){
                comprehensiveTaskList.showAddBtn = true
            }
        }else if (headObj === "EMA_Allocation_PlanList"){//设备调拨  任务列表
            //接受二次信号
            console.log("transmitManager.resultOfResponse.connect(comprehensiveTaskList.resultResponse)")
            transmitManager.resultOfResponse.connect(comprehensiveTaskList.resultResponse);
            roles = ["ID", "OrderCode", "OrderName", "OutOrganiseUnit", "OutDepartMent",
                     "InOrganiseUnit", "InDepartMent","OperatorDate","Status",
                     "OutOrganiseUnitName", "InOrganiseUnitName","SyncStatus"];

            comprehensiveTaskList.titleText =  "设备调拨"
            comprehensiveTaskList.emptyText = "无设备调拨任务";
            comprehensiveTaskList.delegateCom = transferTaskListDelegate;
            inspectionTaskModel.setRolesName(roles);
            inspectionTaskModel.setListData(comprehensiveTaskList.originalData["tables"][0]["value"]);
            comprehensiveTaskList.model = inspectionTaskModel;
            comprehensiveTaskList.titleRightItemIcon = "qrc:/icon/action_bar_menu.png"
            comprehensiveTaskList.titleRightItemEnable = true;
            comprehensiveTaskList.offlineEnable = true
            if(networkStatus.getNetworkConnectStatus() == ""){
                comprehensiveTaskList.showAddBtn = true
            }
        }else if(headObj == "EMD_DistributeEquipmentList"){// 设备领用 设备列表
            roles = ["ID", "DistributeID", "EquipmentID", "ReceiveUserID",
                     "ReceiveDate", "IsRFIDScan", "PDAID",
                     "EquipmentName", "EquipmentCode", "ManufacturerName",
                     "EnterpriseID", "ModelName", "ClassName", "ReceiveUserName"];
            comprehensiveTaskList.titleText = comprehensiveTaskList.otherParam["TitleText"];
            comprehensiveTaskList.emptyText = "无领用设备";
            comprehensiveTaskList.delegateCom = deviceListDelegate;
            inspectionTaskItemModel.setRolesName(roles);
            inspectionTaskItemModel.setListData(comprehensiveTaskList.originalData["tables"][0]["value"]);
            comprehensiveTaskList.model = inspectionTaskItemModel;
        }else if(headObj == "EMA_AllocationEquipmentList"){   //设备调拨  设备列表
            roles = ["ID", "AllocationID", "EquipmentID", "EquipmentName",
                                     "EquipmentCode", "EnterpriseName", "ManufacturerName", "ModelName"];
            comprehensiveTaskList.titleText = os.i18n.ctr(qsTr("Transfer device"));
            comprehensiveTaskList.emptyText = os.i18n.ctr(qsTr("No transfer device"));
            comprehensiveTaskList.delegateCom = deviceListDelegate;
            inspectionTaskItemModel.setRolesName(roles);
            inspectionTaskItemModel.setListData(comprehensiveTaskList.originalData["tables"][0]["value"]);
            comprehensiveTaskList.model = inspectionTaskItemModel;
        }
    }
    Component.onDestruction: {
        console.log("ComprehensiveBaseList.qml destruction")
        transmitManager.resultOfResponse.disconnect(comprehensiveTaskList.resultResponse);
    }

    onSyncTask: {
        var headObj = comprehensiveTaskList.originalData["head"]["objects"];
        console.debug("---------------onSyncTask-----------" + headObj + "------")
        var organiseUnitIDs = inspectionSystemParam.getOrganiseUnitIDs();
        var departmentIDs = inspectionSystemParam.getUserPropertyByKey("DepartmentIDs");
        var head = {"Objects" : headObj};
        var para = {"OrganiseUnitID" : organiseUnitIDs, "DepartmentID" : departmentIDs};
        comprehensiveTaskList.taskActionSequence = inspectionSystemConfig.getSequence();
        var errorMsg;
        if(headObj == "EMD_Distribute_PlanList"){
            errorMsg = "获取领用任务列表失败";
        }else if(headObj == "EMA_Allocation_PlanList"){
            errorMsg = "获取调拨任务列表失败";
        }
        transmitManager.requestTaskData(head, para, comprehensiveTaskList.taskActionSequence, errorMsg);
    }

    onAddBtnClicked:{
        var headObj = comprehensiveTaskList.originalData["head"]["objects"];
        if(headObj == "EMD_Distribute_PlanList"){
            pageStack.push("qrc:/qml/ComprehensiveTask/DeviceRecipientsDetailAmbiguous.qml");
        }else if(headObj == "EMA_Allocation_PlanList"){
            pageStack.push("qrc:/qml/ComprehensiveTask/DeviceTransferDetailAmbiguous.qml");
        }
    }
}
