/*!
* \file ListDelegate.qml
* \brief 设备巡检任务列表页面
*
*设备巡检任务列表页面
*
*\author 张子健 zhangzijian@syberos.com
*\version 1.0
*\date 2015/9/10
*/
import QtQuick 2.0
import com.syberos.basewidgets 2.0
import "../Common"
/*!
  \class ListDelegate
  \inModule 设备巡检
  \brief 张子健 zhangzijian@syberos.com
  \ingroup InspectionTask
  此界面是设备巡检任务列表页面,用于显示设备巡检任务列表
*/
/*
  InspectionMode: 1:普通设备；2：组件；3：路线；4：巡检点；5：设备组;
  为1时，直接进入设备列表
  */
TwoLineItem {
    id : inspectionTaskDelegate
    property string actionSequence: "-1"
    /** type:string 任务名称*/
    firstLeftText: TaskName
    mouseEnable: SyncStatus == undefined ||SyncStatus == "1"
    secondLeftText: "巡检周期:"+ Circle
    secondRightText: {
        if(SyncStatus == undefined || SyncStatus == "1"){
            return Rate
        }else if(SyncStatus == "0"){
            return "正在同步中..."
        }else if(SyncStatus == "2"){
            return "同步失败"
        }
    }
//roles = ["TaskDataKey", "TaskName", "TaskCode", "LimitMode", "EnableUploadAttach", "InspectionMode", "SyncStatus", "Circle", "Rate"];
    onTriggered: {
        transmitManager.resultOfResponse.disconnect(inspectionTaskDelegate.resultResponse)
        transmitManager.resultOfResponse.connect(inspectionTaskDelegate.resultResponse)
        inspectionTaskDelegate.actionSequence = inspectionSystemConfig.getSequence();
        if(InspectionMode != "1"){
            var head = {"Objects" : "EISM_GetTaskGroupList"};
            var para = {"UserID" : inspectionSystemParam.getUserId(), "TaskDataKey" : TaskDataKey};
            console.log("transmitManager.requestData   EISM_GetTaskGroupList ")
            transmitManager.requestData(head, para, inspectionTaskDelegate.actionSequence, os.i18n.ctr(qsTr("Get inspection task suite fail")));
        } else {
            var head1 = {"Objects" : "EISM_GetTaskObjectListByTask"};
            var para1 = {"UserID" : inspectionSystemParam.getUserId(), "TaskDataKey" : TaskDataKey};
            console.log("transmitManager.requestData   EISM_GetTaskObjectListByTask ")
            transmitManager.requestData(head1, para1, inspectionTaskDelegate.actionSequence, os.i18n.ctr(qsTr("Get inspection task suite fail")));
        }
    }

    /**
     * 接受服务器返回数据的槽函数,加载巡检套件列表
     * @param result:bool 获取数据是否成功
     * @param error:int 错误代码
     * @param action:string 当前发送数据动作类型
     * @param map:var    服务器返回值
     */
    function resultResponse(result, error, action, map){
        console.debug("---ListDelegate---onResultOfResponse---result, error: action  actionSeqence ",
                      result, error, action, inspectionTaskDelegate.actionSequence);
        if(action != inspectionTaskDelegate.actionSequence){
            console.log("action != inspectionTaskDelegate.actionSequence  return")
            return;
        }

        transmitManager.resultOfResponse.disconnect(inspectionTaskDelegate.resultResponse)
        if(result && error === 0){
            var otherParam = {
                "TaskDataKey" : TaskDataKey,
                "TaskName": TaskName,
                "LimitMode": LimitMode,
                "EnableUploadAttach": EnableUploadAttach,
                "EnabledScanItem": InspectionMode == "1" || InspectionMode == "4" ?  true : false,
                "InspectionMode":InspectionMode
            }
            console.log("InspectionMode  is ", InspectionMode)
            pageStack.push("qrc:/qml/InspectionTask/InspectionBaseList.qml",{
                               "originalData": map,
                               "otherParam":otherParam,
                           });

        }
    }
}
