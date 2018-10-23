/*!
* \file ItemListDelegate.qml
* \brief 设备盘点项列表页面
*
*设备盘点项列表页面
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
  \inModule 设备盘点
  \brief 设备盘点项列表页面
  \ingroup InventoryTask
  此界面是设备盘点项列表页面,用于显示设备盘点项列表
*/
TwoLineItem {
    id: inventoryDeviceDelegate
    property string actionSequence: "-1"
    /** type:string 设备名称*/
    firstLeftText:  EquipmentName
    /** type:string 设备编码*/
    secondLeftText: "编码: " + EquipmentCode
    /** type:string 保管人*/
    secondRightText: "保管人:" + Custodian
    /** type:string RFID编码*/
    rFIDCode: RFIDCode
    /** type:string 二维码*/
    barCode: BarCode
    /** type:string 是否允许手动操作*/
    twolimitMode:inventoryTaskList.otherParam["LimitMode"]
//<Field Value="IDやEquipmentIDやEquipmentNameやEquipmentCodeやModelNameやCustodianやRFIDCodeやBarCodeやDeviceLastModifiedDate"/>
    onTriggered: {
        console.debug("---inventoryDeviceDelegate.TaskID: ", inventoryDeviceDelegate.TaskID)
        console.debug("---ID: ",ID)
        if((twolimitMode == 1 && rFIDCode=="" && barCode == "") ||
                twolimitMode == 0){
            inventoryDeviceDelegate.actionSequence = inspectionSystemConfig.getSequence();
            transmitManager.resultOfResponse.disconnect(inventoryDeviceDelegate.resultResponse)
            transmitManager.resultOfResponse.connect(inventoryDeviceDelegate.resultResponse)

            var head = {"Objects" : "EMI_InventoryEquipment_Para"};
            var para = {"TaskID" : inventoryTaskList.otherParam["TaskID"],
                "ID" : ID,
                "RFIDCode" : "",
                "BarCode" : ""};
            console.log("transmitManager.requestData   EMI_InventoryEquipment_Para ")
            transmitManager.requestData(head, para, inventoryDeviceDelegate.actionSequence, os.i18n.ctr(qsTr("Get inventory equipment para fail")));
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
        console.debug("---ItemListDelegate---onResultOfResponse---result, error: ", result, error);
        if(action != inventoryDeviceDelegate.actionSequence){
            console.log("action != inventoryDeviceDelegate.actionSequence return")
            return;
        }
        transmitManager.resultOfResponse.disconnect(inventoryDeviceDelegate.resultResponse)
        if(result && error == 0){
            console.debug("---map:", map)
            var paramValue =  {"IsRFIDScan": "0",
                "IsBarCodeScan": "0",
                "enableImage": 0,
                "limitMode":twolimitMode,
                "TaskID":inventoryTaskList.otherParam["TaskID"],
                "parentPage":inventoryTaskList}
            pageStack.push("qrc:/qml/InventoryTask/DeviceInventoryForm.qml", {"dataPackageValue": map,
                               "paramValue": paramValue});

        }
    }
}
