/*!
* \file ItemListDelegate.qml
* \brief 设备巡检项列表页面
*
*设备巡检项列表页面
*
*\author 张子健 zhangzijian@syberos.com
*\version 1.0
*\date 2015/9/7
*/
import QtQuick 2.0
import com.syberos.basewidgets 2.0
import "../Common"
/*!
  \class ItemListDelegate
  \inModule 设备巡检
  \brief 设备巡检项列表页面
  \ingroup InspectionTask
  此界面是设备巡检项列表页面，用于显示设备巡检项列表
*/
TwoLineItem {
    id: inspectionDeviceDelegate
    property string actionSequence: "-1"
    /** 0  普通巡检
        1  路线巡检
        2  一键提交*/
    property string method: "0"
//    opacity: (method == "1" && index == 0) || method != "1" ? 1 : 0.5
//    mouseEnable: (method == "1" && index == 0) || method != "1"
    /** type:string 备巡检项名称*/
    firstLeftText: ObjectName
    secondLeftText: "位置:" + Positions
    secondRightText: "型号:" + ModelName
     /** type:string RFID编码*/
    rFIDCode:RFIDCode
    /** type:string 二维码*/
    barCode:BarCode
    /** type:string 是否允许手动操作*/
    twolimitMode:inspectionTaskList.otherParam["LimitMode"]
//    roles = ["PlanObjectID", "ObjectName", "ObjectFullName", "TaskGroupDataKey", "PlanGroupID", "RFIDCode", "BarCode" ,
//            "QRCode",  "RealObjectID", "Positions", "ModelName"];
    onTriggered: {
        if((twolimitMode === "1" && rFIDCode === "" && barCode === "") || twolimitMode === "0") {
            inspectionDeviceDelegate.actionSequence = inspectionSystemConfig.getSequence();
            transmitManager.resultOfResponse.disconnect(inspectionDeviceDelegate.resultResponse)
            transmitManager.resultOfResponse.connect(inspectionDeviceDelegate.resultResponse)

            var head = {"Objects" : "EISM_GetDailyObjectDetail"};
            var para = {"TaskGroupDataKey" : TaskGroupDataKey, "PlanObjectID" : PlanObjectID};

            console.log("transmitManager.requestData    EISM_GetDailyObjectDetail ")
            transmitManager.requestData(head, para, inspectionDeviceDelegate.actionSequence, "获取日常任务对象详情失败");
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
        console.debug("---ItemListDelegate---onResultOfResponse---result, error: action  actionSequece",
                      result, error, action, inspectionDeviceDelegate.actionSequence);
        if(action != inspectionDeviceDelegate.actionSequence){
            console.log("action != inspectionDeviceDelegate  return")
            return;
        }

        transmitManager.resultOfResponse.disconnect(inspectionDeviceDelegate.resultResponse)
        if(result && error == 0){
            var paramValue=  {
                "IsRFIDScan": "0",
                "IsBarCodeScan": "0",
                "TaskDataKey" : inspectionTaskList.otherParam["TaskDataKey"],
                "EnableUploadAttach": inspectionTaskList.otherParam["EnableUploadAttach"],
                "pageFormType":"dailyTask",
                "parentPage":inspectionTaskList
            };
            pageStack.push("qrc:/qml/DailyTask/DailyTaskForm.qml",
                           {
                               "dataPackageValue": map,
                               "paramValue": paramValue
                           });
        }
    }
}
