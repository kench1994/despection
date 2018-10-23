/*!
* \file MaintainListDelegate.qml
* \brief 设备保养任务列表页面
*
*设备保养任务列表页面
*
*\author 张子健 zhangzijian@syberos.com
*\version 1.0
*\date 2015/9/10
*/
import QtQuick 2.0
import com.syberos.basewidgets 2.0
import "../Common"
/*!
  \class MaintainListDelegate
  \inModule 设备保养
  \brief 设备保养任务列表页面
  \ingroup DeviceMaintainTask
  此界面是设备保养任务列表页面,用于显示设备保养任务列表
*/
TwoLineItem {
    id : maintainTaskDelegate
    property string actionSequence: "-1"
    enabled: (SyncStatus == undefined || SyncStatus==1)?true:false
    /** type:string 任务名称*/
    firstLeftText: TaskName
    /** type:string 保养时间*/
    secondLeftText: os.i18n.ctr(qsTr("Maintenance time: ")) + Circle
    /** type:string 完成比例*/
    secondRightText: {
        if(SyncStatus == undefined || SyncStatus == 1){
            return Rate
        }else if(SyncStatus == 2){
            return "同步失败"
        }else if(SyncStatus == 0){
            return "正在同步中..."
        }
    }
//roles = ["TaskDataKey", "TaskCode", "TaskName", "LimitMode", "EnableUploadAttach", "Circle", "Rate","ModelType"];
    onTriggered: {
        maintainTaskDelegate.actionSequence = inspectionSystemConfig.getSequence();
        transmitManager.resultOfResponse.disconnect(maintainTaskDelegate.resultResponse)
        transmitManager.resultOfResponse.connect(maintainTaskDelegate.resultResponse)

        if(ModelType == "1"){
            var head = {"Objects" : "EMM_GetTaskGroupList"};
            var para = {"UserID" : inspectionSystemParam.getUserId(), "TaskDataKey" : TaskDataKey};
            console.log("transmitManager.requestData   EMM_ GetTaskGroupList ")
            transmitManager.requestData(head, para, maintainTaskDelegate.actionSequence, "获取保养任务组列表失败");
        } else {
            var head1 = {"Objects" : "EMM_GetTaskObjectListByTask"};
            var para1 = {"UserID" : inspectionSystemParam.getUserId(), "TaskDataKey" : TaskDataKey};
            console.log("transmitManager.requestData   EMM_ GetTaskObjectListByTask ")
            transmitManager.requestData(head1, para1, maintainTaskDelegate.actionSequence, "获取保养任务项列表失败");
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
        console.debug("--ListDelegate--onResultOfResponse--result, error: ", result, error);
        if(action != maintainTaskDelegate.actionSequence){
            console.log("action != maintainTaskDelegate.actionSequence  return")
            return;
        }
        transmitManager.resultOfResponse.disconnect(maintainTaskDelegate.resultResponse)
        if(result && error == 0){
            var otherParam = {
                "TaskDataKey" : TaskDataKey,
                "TaskName": TaskName,
                "LimitMode": LimitMode,
                "EnableUploadAttach": EnableUploadAttach,
                "EnabledScanItem": ModelType != "1" ?  true : false,
            }
            pageStack.push("qrc:/qml/DeviceMaintainTask/MaintainBaseList.qml",
                           {
                               "originalData": map,
                               "otherParam":otherParam,
                           });
        }
    }
}
