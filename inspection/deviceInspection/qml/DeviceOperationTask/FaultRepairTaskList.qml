import QtQuick 2.0
import com.syberos.basewidgets 2.0
import "../Common"
BaseList{
    id:faultRepairTaskList
    property string actionSequence: ""
    //任务列表代理
    Component{
        id:taskListDelegate
        FaultRepairTaskDelegate{}
    }

    onStatusChanged:{
        if(status == CPageStatus.WillShow){
            console.log("faultRepairTaskList.qml  refresh ===", refresh);
            if(refresh === true){
                var headObj = faultRepairTaskList.originalData["head"]["objects"];
                console.log("InspectionBaseList.qml  refresh === true action  ", headObj);
                if(headObj == "EMF_FaultRepair_Query_Task"){
                    var head = {"Objects" : "EMF_FaultRepair_Query_Task"};
                    var para = {"UserCode" : inspectionSystemParam.getUserPropertyByKey("UserCode"), "Status": faultRepairTaskList.otherParam["TaskStatus"]};
                    console.log("transmitManager.requestData   EMF_FaultRepair_Query_Task ")
                    faultRepairTaskList.actionSequence = inspectionSystemConfig.getSequence();
                    transmitManager.resultOfResponse.disconnect(faultRepairTaskList.faultRepairResultResponse);
                    transmitManager.resultOfResponse.connect(faultRepairTaskList.faultRepairResultResponse);
                    transmitManager.requestData(head, para, faultRepairTaskList.actionSequence,
                                                "获取" + faultRepairTaskList.otherParam["TitleName"] + "任务列表失败");
                }
            }
        }else if(status == CPageStatus.Show){
            console.log("faultRepairTaskList.qml  status == CPageStatus.Show")
        }else if(status == CPageStatus.WillHide){
            console.log("faultRepairTaskList.qml  status == CPageStatus.WillHide  ")
            //默认下次进入此页面刷新
            faultRepairTaskList.refresh = true;
        }else if(status == CPageStatus.Hide){
            console.log("faultRepairTaskList.qml  status == CPageStatus.Hide")
        }
    }

    /**
     * 接受服务器返回数据的槽函数
     * @param result:bool 获取数据是否成功
     * @param error:int 错误代码
     * @param action:string 当前发送数据动作类型
     * @param map:var    服务器返回值
     */
    function faultRepairResultResponse(result, error, action, map){
        console.debug("----faultRepairTaskList-----Response--------------result, error,action, actionSequence ",
                      result,  error, action, faultRepairTaskList.actionSequence);
        if(action != faultRepairTaskList.actionSequence){
            console.log("action != faultRepairTaskList.actionSequence  return");
            return;
        }
        faultRepairTaskList.refresh = true;
        if(action == faultRepairTaskList.actionSequence)
            transmitManager.resultOfResponse.disconnect(faultRepairTaskList.faultRepairResultResponse);
        if(result && error == 0){
            var headObj = map["head"]["objects"];
            if(headObj == "EMF_FaultRepair_Query_Task"){
                if(map["tables"].length <= 0){
                    CTOAST.requestToast("数据错误", "", "");
                    return;
                }
                faultRepairTaskList.model.setListData(map["tables"][0]["value"]);
                faultRepairTaskList.originalData = map;
            }
        }
    }
    Component.onCompleted: {
        //页面第一次加载，不刷新
        faultRepairTaskList.refresh = false;
        var headObj = faultRepairTaskList.originalData["head"]["objects"];
        if (headObj === "EMF_FaultRepair_Query_Task"){//设备列表
            var roles = ["ID", "FaultCode", "Status", "EquitmentName", "ReporterOrganiseName", "FaultType","ReporterName","ReportDate",
                    "Positions", "EnterpriseName", "EquipmentCode", "ModelName"];
            faultRepairTaskList.titleText = faultRepairTaskList.otherParam["TitleName"];
            faultRepairTaskList.emptyText = "无" + faultRepairTaskList.otherParam["TitleName"];
            faultRepairTaskList.delegateCom = taskListDelegate;
            inspectionTaskItemModel.setRolesName(roles);
            console.log("FaultRepairTaskList111111 ",faultRepairTaskList.originalData["tables"][0]["value"].length )
            inspectionTaskItemModel.setListData(faultRepairTaskList.originalData["tables"][0]["value"]);
            faultRepairTaskList.model = inspectionTaskItemModel;
        }
    }
    Component.onDestruction: {
        console.log("FaultRepairTaskList.qml onDestruction ")
        transmitManager.resultOfResponse.disconnect(faultRepairTaskList.faultRepairResultResponse);
    }
}
