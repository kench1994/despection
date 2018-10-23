/*!
* \file FaultRepairTaskDelegate.qml
* \brief 维修查询设备列表显示页面
*
*维修查询设备列表显示页面
*
*\author 张子健 zhangzijian@syberos.com
*\version 1.0
*\date 2015/9/7
*/
import QtQuick 2.0
import com.syberos.basewidgets 2.0
import "../Common"
/*!
  \class FaultRepairTaskDelegate
  \inModule 设备维修
  \brief 维修查询设备列表显示页面
  \ingroup DeviceOperationTask
  此界面是维修查询设备列表显示页面,用于显示维修查询设备列表
*/
TwoLineItem {
    id : faultRepairTaskDelegate
    property var  statusMap: {"1":"受理" ,"2":"审批", "3":"派工", "4":"维修", "5":"费用登记","6":"评价"}
    property var  faultTypeMap: {"1":"运行损坏" ,"2":"意外损坏", "3":"事故损坏", "4":"其他原因"}
    property string actionSequence: "-1"
    /** type:string 设备名称*/
    firstLeftText: EquitmentName
    /** type:string 类型*/
    firstRightText: "型号:" + ModelName
    secondLeftText: "编号:" + EquipmentCode
    /** type:string 报告时间*/
    secondRightText: "厂商:" + EnterpriseName
//    var roles = ["ID", "FaultCode", "Status", "EquitmentName", "ReporterOrganiseName", "FaultType","ReporterName","ReportDate",
//            "Positions", "EnterpriseName", "EquipmentCode", "ModelName"];
    onTriggered: {
        faultRepairTaskDelegate.actionSequence = inspectionSystemConfig.getSequence();
        transmitManager.resultOfResponse.disconnect(faultRepairTaskDelegate.resultResponse)
        transmitManager.resultOfResponse.connect(faultRepairTaskDelegate.resultResponse)

        var head1= {"Objects" : "EMF_FaultRepair_TaskAllInfo"};
        var para1 = {"ID" : ID, "UserCode" : inspectionSystemParam.getUserPropertyByKey("UserCode"), "Status":Status};
        console.log("transmitManager.requestData   EMF_FaultRepair_TaskAllInfo ")
        transmitManager.requestData(head1, para1, faultRepairTaskDelegate.actionSequence, "获取"   + faultRepairTaskList.otherParam["TitleName"] + "失败");
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
        if(action != faultRepairTaskDelegate.actionSequence){
            console.log("action != faultRepairTaskDelegate.actionSequence return")
            return;
        }
        transmitManager.resultOfResponse.disconnect(faultRepairTaskDelegate.resultResponse)
        if(result && error == 0){
            var otherParam = {
                "EquipmentCode" : EquipmentCode,
                "Taskstatus": Status,
                "TitleName": faultRepairTaskList.otherParam["TitleName"],
                "TaskID" : ID,
                "ParentPage":faultRepairTaskList};
            pageStack.push("qrc:/qml/DeviceOperationTask/FaultRepairMainPage.qml",
                           {"originalData": map,
                               "otherParam": otherParam
                           });
        }
    }
}

