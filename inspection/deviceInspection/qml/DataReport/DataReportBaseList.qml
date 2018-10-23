import QtQuick 2.0
import com.syberos.basewidgets 2.0
import "../Common"
BaseList{
    id:dataReportTaskList
    property string nameId: ""
    //任务列表代理
    Component{
        id:taskListDelegate
        DataReportTaskDelegate{}
    }

    onStatusChanged:{
        if(status == CPageStatus.WillShow){
            console.log("dataReportTaskList.qml  refresh ===", refresh);
        }else if(status == CPageStatus.Show){
            console.log("dataReportTaskList.qml  status == CPageStatus.Show")
        }else if(status == CPageStatus.WillHide){
            console.log("dataReportTaskList.qml  status == CPageStatus.WillHide  ")
            //默认下次进入此页面刷新
            dataReportTaskList.refresh = true;
        }else if(status == CPageStatus.Hide){
            console.log("dataReportTaskList.qml  status == CPageStatus.Hide")
        }
    }
    /**
     * 接受服务器返回数据的槽函数
     * @param result:bool 获取数据是否成功
     * @param error:int 错误代码
     * @param action:string 当前发送数据动作类型
     * @param map:var    服务器返回值
     */
    function dataReportResultResponse(result, error, action, map){
        console.debug("----dataReportTaskList-----Response--------------result, error,action, actionSequence ",
                      result,  error, action, dataReportTaskList.actionSequence);
        if(action != dataReportTaskList.actionSequence){
            console.log("action != dataReportTaskList.actionSequence  return");
            return;
        }
        dataReportTaskList.refresh = true;
        transmitManager.resultOfResponse.disconnect(dataReportTaskList.dataReportResultResponse);
        if(result && error == 0){
            console.log("dataReportTaskList.model.setListData 00000000000000000000")
            dataReportTaskList.model.setListData(map["tables"][0]["value"]);
            dataReportTaskList.originalData = map;
        }
    }

    Component.onCompleted: {
        //页面第一次加载，不刷新
        dataReportTaskList.refresh = false;
        var action = dataReportTaskList.originalData["head"]["objects"];
        var roles;
        if(action === "DFI_ReportTask_RuningList"){ //任务列表
            roles = ["ID", "TaskCode", "TaskName", "TaskStatus", "ProjectName", "ShortName"];
            var name;
            var clientPriority = inspectionSystemConfig.getClientPriority();
            if(clientPriority == false){
                name = inspectionSystemParam.getUserPermissionData(dataReportTaskList.nameId, "ModuleName");
            } else
                name = inspectionFuncsItemModel.getNodeParas(dataReportTaskList.nameId, "")["NameCN"];
            dataReportTaskList.titleText = name;
            dataReportTaskList.emptyText = "无数据上报任务";
            dataReportTaskList.delegateCom = taskListDelegate;
            inspectionTaskModel.setRolesName(roles);
            inspectionTaskModel.setListData(dataReportTaskList.originalData["tables"][0]["value"]);
            dataReportTaskList.model = inspectionTaskModel;
        }
    }

    Component.onDestruction: {
        console.log("dataReportTaskList.qml onDestruction ")
        transmitManager.resultOfResponse.disconnect(dataReportTaskList.dataReportResultResponse);
    }
}
