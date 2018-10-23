/*!
* \file MaintainSuiteDelegate.qml
* \brief 设备保养套件列表页面
*
*设备保养套件列表页面
*
*\author 张子健 zhangzijian@syberos.com
*\version 1.0
*\date 2015/9/10
*/
import QtQuick 2.0
import com.syberos.basewidgets 2.0
import "../Common"
/*!
  \class MaintainSuiteDelegate
  \inModule 设备保养
  \brief 设备保养套件列表页面
  \ingroup DeviceMaintainTask
  此界面是设备保养套件列表页面,用于显示设备保养套件列表
*/
SingleLineItem {
    id: maintainSuitDelegate
    property string actionSequence: "-1"
    /** type:string 套件名称*/
    leftText: TaskGroupName
    /** type:string 完成比例*/
    rightText: ExamObjectCount + "/" + TotalObjectCount

    //roles = ["TaskGroupDataKey", "TaskDataKey", "TaskGroupName", "PostID", "TotalObjectCount", "ExamObjectCount", "OrderIndex"];
    onTriggered: {
        maintainSuitDelegate.actionSequence = inspectionSystemConfig.getSequence();
        transmitManager.resultOfResponse.disconnect(maintainSuitDelegate.resultResponse)
        transmitManager.resultOfResponse.connect(maintainSuitDelegate.resultResponse)

        var head2 = {"Objects" : "EMM_GetTaskObjectListByGroup"};
        var para2 = {"TaskGroupDataKey" : TaskGroupDataKey};
        console.log("transmitManager.requestData   EMM_GetTaskObjectListByGroup")
        transmitManager.requestData(head2, para2, maintainSuitDelegate.actionSequence, "获取保养任务项列表失败");
    }

    /**
     * 接受服务器返回数据的槽函数
     * @param result:bool 获取数据是否成功
     * @param error:int 错误代码
     * @param action:string 当前发送数据动作类型
     * @param map:var    服务器返回值
     */
    function resultResponse(result, error, action, map){
        console.debug("--SuiteDelegate--onResultOfResponse--result, error: ", result, error);
        if(action != maintainSuitDelegate.actionSequence){
            console.log("action != maintainSuitDelegate.actionSequence  return")
            return;
        }

        transmitManager.resultOfResponse.disconnect(maintainSuitDelegate.resultResponse)
        if(result && error == 0){
            var otherParam = {
                "TaskGroupDataKey" : TaskGroupDataKey,
                "TaskGroupName": TaskGroupName,
                "LimitMode": maintainTaskList.otherParam["LimitMode"],
                "EnableUploadAttach": maintainTaskList.otherParam["EnableUploadAttach"],
                "EnabledScanItem": true
            }
            pageStack.push("qrc:/qml/DeviceMaintainTask/MaintainBaseList.qml",
                           {
                               "originalData": map,
                               "otherParam":otherParam,
                           });
        }
    }
}
