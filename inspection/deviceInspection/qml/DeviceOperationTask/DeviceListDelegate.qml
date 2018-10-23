/*!
* \file DeviceListDelegate.qml
* \brief 设备查询or故障申报设备列表显示页面
*
*设备查询or故障申报设备列表显示页面
*
*\author 张子健 zhangzijian@syberos.com
*\version 1.0
*\date 2015/9/7
*/
import QtQuick 2.0
import com.syberos.basewidgets 2.0
import "../Common"
/*!
  \class DeviceListDelegate
  \inModule 设备维修
  \brief 设备查询or故障申报设备列表显示页面
  \ingroup DeviceOperationTask
  此界面是设备查询or故障申报设备列表显示页面,用于显示设备查询or故障申报设备列表
*/
TwoLineItem {
    id : deviceListDelegate
    property string actionSequence: "-1"
    /** type:string 设备名称*/
    firstLeftText: ModelName == "" ? EquipmentName: EquipmentName + "("+ModelName+")"
    /** type:string 设备编码*/
    secondLeftText:  "设备编号:" + EquipmentCode
    /** type:string 使用人*/
    secondRightText:  "生产厂商:" + ManufacturerName

//    roles = ["ID", "EquipmentCode", "EquipmentName", "EquipmentSeries",
//             "EquipmentModelName", "Positions","ClassName",
//             "ModelName", "ManufacturerName"];

    onTriggered: {
        deviceListDelegate.actionSequence = inspectionSystemConfig.getSequence();
        transmitManager.resultOfResponse.disconnect(deviceListDelegate.resultResponse);
        transmitManager.resultOfResponse.connect(deviceListDelegate.resultResponse);
        var unitIDs = inspectionSystemParam.getOrganiseUnitIDs();
        if(searchBaseList.otherParam["pageActionType"] == "device search"){

            var head1 = {};
            head1["Objects"] = "Equipment_QueryOne";
            var para1 = {};
            para1["EquipmentID"] = ID;
            para1["KeyCode"] = "";
            para1["EquipmentCode"] = "";
            para1["OrganiseUnitID"] = unitIDs;
            console.log("transmitManager.requestData    Equipment_ QueryOne ")
            transmitManager.requestData(head1, para1, deviceListDelegate.actionSequence, os.i18n.ctr(qsTr("Get device list fail")));
        }else if(searchBaseList.otherParam["pageActionType"] == "Failure to declare"){
            var head2 = {"Objects" : "EMF_FaultRepair_Para"};
            var para2 = {};
            para2["EquipmentID"] = ID;
            para2["KeyCode"] = "";
            para2["EquipmentCode"] = EquipmentCode;
            para2["OrganiseUnitID"] = unitIDs;
            console.log("transmitManager.requestData    EMF_FaultRepair_ Para ")
            transmitManager.requestData(head2, para2, deviceListDelegate.actionSequence, os.i18n.ctr(qsTr("Get fault repair para fail")));
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
        console.debug("---Device---ListDelegate---onResultOfResponse---result, error, action: ",
                      result, error, action);
        if(action != deviceListDelegate.actionSequence){
            console.log("action != deviceListDelegate.actionSequence return")
            return;
        }
        transmitManager.resultOfResponse.disconnect(deviceListDelegate.resultResponse)

        if(result && error == 0){
            if(map["head"]["objects"] == "Equipment_QueryOne"){
                //设备明细
                var titleText = os.i18n.ctr(qsTr("Equipment archives"))
                pageStack.push("qrc:/qml/DeviceArchives.qml", {"titleText":titleText,
                                   "action":searchBaseList.otherParam["pageActionType"],
                                   "info":map["tables"][0]["value"][0]});
            }else if(map["head"]["objects"] == "EMF_FaultRepair_Para"){
                //维修参数
                var paramValue =  {"IsRFIDScan": "0",
                    "IsBarCodeScan": "0",
                    "EquipmentID":ID,
                    "ERBQCode": EquipmentCode,
                    "CodeType": "1"}
                pageStack.push("qrc:/qml/DeviceOperationTask/FaultReport.qml", {"dataPackageValue": map,
                                   "paramValue": paramValue});
            }
        }
    }
}
