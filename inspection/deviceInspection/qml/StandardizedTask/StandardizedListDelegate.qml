/*!
* \file StandardizedListDelegate.qml
* \brief 标准化检查任务列表页面
*
*标准化检查任务列表页面
*
*\author 张子健 zhangzijian@syberos.com
*\version 1.0
*\date 2015/9/9
*/
import QtQuick 2.0
import com.syberos.basewidgets 2.0
import "../Common"
/*!
  \class StandardizedListDelegate
  \inModule 标准化检查
  \brief 标准化检查任务列表页面
  \ingroup StandardizedTask
  此界面是标准化检查任务列表页面,用于显示标准化检查任务列表
*/
TwoLineItem {
    id : stdTaskDelegate
    property string actionSequence: "-1"
    enabled: (SyncStatus == undefined || SyncStatus==1)?true:false
    /** type:string 任务名称*/
    firstLeftText: TaskName
    secondLeftText: Circle
    secondRightText: {
        if(SyncStatus == undefined || SyncStatus == 1){
            return Rate
        }else if(SyncStatus == 2){
            return "同步失败"
        }else if(SyncStatus == 0){
            return "正在同步中..."
        }
    }
//roles = ["TaskID", "TaskCode", "TaskName", "TaskType", "Circle", "EnableUploadAttach", "TaskDataKey", "ExamObjectType", "LimitMode", "Rate"];
    onTriggered: {
        stdTaskDelegate.actionSequence = inspectionSystemConfig.getSequence();
        transmitManager.resultOfResponse.disconnect(stdTaskDelegate.resultResponse)
        transmitManager.resultOfResponse.connect(stdTaskDelegate.resultResponse)

        var head = {"Objects" : "EXAM_ExamineTaskObjectList"};
        var para = {"TaskDataKey" : TaskDataKey, "UserID" : inspectionSystemParam.getUserId()};
        console.log("transmitManager.requestData   EXAM_ExamineTaskObjectList ")
        transmitManager.requestData(head, para, stdTaskDelegate.actionSequence, os.i18n.ctr(qsTr("Get standardized inspection task suite fail")));
    }

    /**
     * 接受服务器返回数据的槽函数
     * @param result:bool 获取数据是否成功
     * @param error:int 错误代码
     * @param action:string 当前发送数据动作类型
     * @param map:var    服务器返回值
     */
    function resultResponse(result, error, action, map){
        console.debug("---ListDelegate---onResultOfResponse---result, error: ", result, error);
        if(action != stdTaskDelegate.actionSequence){
            console.log("action != stdTaskDelegate.actionSequence return")
            return;
        }

        transmitManager.resultOfResponse.disconnect(stdTaskDelegate.resultResponse)
        if(result && error == 0){
            var otherParam = {
                "TaskDataKey": TaskDataKey,
                "TaskName" : TaskName,
                "TaskType" : TaskType,
                "LimitMode": LimitMode,
                "EnableUploadAttach": EnableUploadAttach,
                "EnabledScanItem": ExamObjectType == "2",
            }
            pageStack.push("qrc:/qml/StandardizedTask/StandardBaseList.qml",
                           {
                               "originalData": map,
                               "otherParam":otherParam,
                           });
        }
    }
}
