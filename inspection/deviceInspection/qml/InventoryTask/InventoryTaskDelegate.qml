/*!
* \file ListDelegate.qml
* \brief 设备盘点任务列表页面
*
*设备盘点任务列表页面
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
  \inModule 设备盘点
  \brief 设备盘点任务列表页面
  \ingroup InventoryTask
  此界面是设备盘点任务列表页面,用于显示设备盘点任务列表
*/
TwoLineItem {
    id : inventoryTaskDelegate
    property string actionSequence: "-1"
    enabled: (SyncStatus == undefined || SyncStatus==1)?true:false
    /** type:string 任务名称*/
    firstLeftText: TaskName
    /** type:string 盘点时间*/
    secondLeftText: "盘点时间:" + Circle
    /** type:string 完成比例*/
    secondRightText: {
        if(SyncStatus == undefined || SyncStatus == 1){
            return Rate
        }else if(SyncStatus == 0){
            return "正在同步中..."
        }else if(SyncStatus == 2){
            return "同步失败"
        }
    }

//roles = ["ID", "TaskCode", "TaskName", "LimitMode", "EnableImage", "StartDate", "EndDate", "Circle", "SyncStatus", "Rate"];

    onTriggered: {
        inventoryTaskDelegate.actionSequence = inspectionSystemConfig.getSequence();
        transmitManager.resultOfResponse.disconnect(inventoryTaskDelegate.resultResponse)
        transmitManager.resultOfResponse.connect(inventoryTaskDelegate.resultResponse)

        var head1 = {"Objects" : "EMI_InventoryEquipmentList"};
        var para1 = {};
        para1["TaskID"] = ID;
        para1["UserID"] = inspectionSystemParam.getUserId();
        para1["EquipmentName"] = "";
        console.log("transmitManager.requestData   EMI_InventoryEquipmentList ")
        transmitManager.requestData(head1, para1, inventoryTaskDelegate.actionSequence, "获取盘点项列表失败");
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
        if(action != inventoryTaskDelegate.actionSequence){
            console.log("action != inventoryTaskDelegate.actionSequence return")
            return;
        }
        transmitManager.resultOfResponse.disconnect(inventoryTaskDelegate.resultResponse)
        if(result && error == 0){
            var otherParam = {
                "TaskID":ID,
                "TaskName":TaskName,
                "LimitMode": LimitMode,
                "EnableImage": EnableImage,
                "EnabledScanItem": true,
                "parentPage":inventoryTaskList
            }
            pageStack.push("qrc:/qml/InventoryTask/InventoryBaseList.qml",
                           {"originalData": map,
                               "otherParam": otherParam,
                           });
        }
    }
}
