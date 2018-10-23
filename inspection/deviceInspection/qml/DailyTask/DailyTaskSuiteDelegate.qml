/*!
* \file SuiteDelegate.qml
* \brief 设备巡检套件列表页面
*
*设备巡检套件列表页面
*
*\author 张子健 zhangzijian@syberos.com
*\version 1.0
*\date 2015/9/10
*/
import QtQuick 2.0
import com.syberos.basewidgets 2.0
import "../Common"
/*!
  \class SuiteDelegate
  \inModule 设备巡检
  \brief 设备巡检套件列表页面
  \ingroup InspectionTask
  此界面是设备巡检套件列表页面,用于显示设备巡检套件列表
*/
SingleLineItem {
    id: inspectionSuitDelegate
    property string actionSequence: "-1"
    /** type:string 套件名称*/
    leftText: TaskGroupName
    rightText: "日常任务"
//roles = ["TaskGroupDataKey", "TaskDataKey", "TaskGroupName", "TotalObjectCount", "RFIDCode", "BarCode", "QRCode"];
    onTriggered: {
        inspectionSuitDelegate.actionSequence = inspectionSystemConfig.getSequence();
        transmitManager.resultOfResponse.disconnect(inspectionSuitDelegate.resultResponse)
        transmitManager.resultOfResponse.connect(inspectionSuitDelegate.resultResponse)

        var head = {"Objects" : "EISM_GetDailyObjectListByGroup"};
        var para = {"TaskDataKey" : TaskDataKey,"TaskGroupDataKey" : TaskGroupDataKey };
        console.log("transmitManager.requestData   EISM_GetDailyObjectListByGroup ")
        transmitManager.requestData(head, para, inspectionSuitDelegate.actionSequence, "获取日常任务对象列表失败");
    }

    /**
     * 接受服务器返回数据的槽函数
     * @param result:bool 获取数据是否成功
     * @param error:int 错误代码
     * @param action:string 当前发送数据动作类型
     * @param map:var    服务器返回值
     */
    function resultResponse(result, error, action, map){
        console.debug("----SuiteDelegate-----onResultOfResponse--------------result, error: ", result, error);
        if(action != inspectionSuitDelegate.actionSequence){
            console.log("action != inspectionSuitDelegate.actionSequence  return")
            return;
        }
        transmitManager.resultOfResponse.disconnect(inspectionSuitDelegate.resultResponse)
        if(result && error === 0){
            var otherParam = {
                "TaskGroupName": TaskGroupName,
                "TaskGroupDataKey": TaskGroupDataKey,
                "TaskDataKey" : TaskDataKey,
                "LimitMode": inspectionTaskList.otherParam["LimitMode"],
                "EnableUploadAttach": inspectionTaskList.otherParam["EnableUploadAttach"],
                "EnabledScanItem": true,
                "InspectionMode":inspectionTaskList.otherParam["InspectionMode"]
            }
            pageStack.push("qrc:/qml/DailyTask/DailyTaskBaseList.qml",
                           {
                               "originalData": map,
                               "otherParam":otherParam,
                           });
        }
    }
}
