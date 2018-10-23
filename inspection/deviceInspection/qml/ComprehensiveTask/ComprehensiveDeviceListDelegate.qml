/*!
* \file ItemListDelegate.qml
* \brief 设备领用计划的设备列表页面
*
*设备领用计划的设备列表页面
*
*\author 张子健 zhangzijian@syberos.com
*\version 1.0
*\date 2015/9/10
*/
import QtQuick 2.0
import com.syberos.basewidgets 2.0
import "../Common"
/*!
  \class ItemListDelegate
  \inModule 综合业务
  \brief 设备领用计划的设备列表页面
  \ingroup ComprehensiveTask
  此界面是设备领用计划的设备列表页面,用于显示设备领用计划的设备列表
*/
TwoLineItem {
    id : comprehensiveDeviceListDelegate
    property string actionSequence: "-1"
    /** type:string 设备名称*/
    firstLeftText: EquipmentName+"("+ModelName+")"
    /** type:string 设备编码*/
    secondLeftText: os.i18n.ctr(qsTr("EquipmentCode: ")) + EquipmentCode
    /** type:string 生产厂商*/
    secondRightText: os.i18n.ctr(qsTr("ManufacturerName: ")) + ManufacturerName

    //EMD_DistributeEquipmentList
//    roles = ["ID", "DistributeID", "EquipmentID", "ReceiveUserID",
//             "ReceiveDate", "IsRFIDScan", "PDAID",
//             "EquipmentName", "EquipmentCode", "ManufacturerName",
//             "EnterpriseID", "ModelName", "ClassName", "ReceiveUserName"];
    onTriggered: {
        var headObj = comprehensiveTaskList.originalData["head"]["objects"];
        if(headObj == "EMD_DistributeEquipmentList"){ //设备领用  进入设备领用详情页面
            comprehensiveDeviceListDelegate.actionSequence = inspectionSystemConfig.getSequence();
            transmitManager.resultOfResponse.disconnect(comprehensiveDeviceListDelegate.resultResponse)
            transmitManager.resultOfResponse.connect(comprehensiveDeviceListDelegate.resultResponse)

            var head = {"Objects" : "EMD_DistributeEquipment_Para"};
            var para = {};
            para["DistributeID"] = DistributeID;
            para["EquipmentID"] = EquipmentID;
            para["RFIDCode"] = "";
            para["BarCode"] = "";
            console.log("transmitManager.requestData   EMD_DistributeEquipment_Para ")
            transmitManager.requestData(head, para, comprehensiveDeviceListDelegate.actionSequence, "获取领用设备详情失败");
        }else if(headObj == "EMA_AllocationEquipmentList"){  //设备调拨，察看设备详情，进入设备详情页面
            comprehensiveDeviceListDelegate.actionSequence = inspectionSystemConfig.getSequence();
            transmitManager.resultOfResponse.disconnect(comprehensiveDeviceListDelegate.resultResponse);
            transmitManager.resultOfResponse.connect(comprehensiveDeviceListDelegate.resultResponse);

            var head1 = {};
            head1["Objects"] = "Equipment_QueryOne";
            var para1 = {};
            para1["EquipmentID"] = EquipmentID;
            para1["KeyCode"] = "";
            para1["EquipmentCode"] = "";
            para1["OrganiseUnitID"] = comprehensiveTaskList.otherParam["outOrganiseUnit"];
            console.log("transmitManager.requestData    Equipment_ QueryOne ")
            transmitManager.requestData(head1, para1, comprehensiveDeviceListDelegate.actionSequence, os.i18n.ctr(qsTr("Get transfer device list fail")));
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
        console.debug("----ListDelegate-----onResultOfResponse--------------result, error: ", result, error);
        if(action != comprehensiveDeviceListDelegate.actionSequence){
            console.log("action != comprehensiveDeviceListDelegate.actionSequence return");
            return;
        }

        transmitManager.resultOfResponse.disconnect(comprehensiveDeviceListDelegate.resultResponse)
        if(result && error == 0){
            var headObj = map["head"]["objects"];
            if(headObj == "EMD_DistributeEquipment_Para"){
                var paramValue = {
                    "CodeType" : "1",
                    "ERBQCode" : EquipmentCode,
                    "IsRFIDScan":"0",
                    "IsBarCodeScan":"0",
                    "parentPage":comprehensiveTaskList};
                pageStack.push("qrc:/qml/ComprehensiveTask/DeviceRecipientsDetail.qml",
                               {"paramValue":paramValue,"dataPackageValue":map});
            }else if (headObj == "Equipment_QueryOne"){
                pageStack.push("qrc:/qml/DeviceArchives.qml",
                               {"titleText":titleText, "pageActionType":"device transfer",
                                "info":map["tables"][0]["value"][0]});
            }
        }
    }
}
