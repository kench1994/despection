/*!
* \file MaintainItemListDelegate.qml
* \brief 设备保养项列表页面
*
*设备保养项列表页面
*
*\author 张子健 zhangzijian@syberos.com
*\version 1.0
*\date 2015/9/10
*/
import QtQuick 2.0
import com.syberos.basewidgets 2.0
import "../Common"
/*!
  \class MaintainItemListDelegate
  \inModule 设备保养
  \brief 设备保养项列表页面
  \ingroup DeviceMaintainTask
  此界面是设备保养项列表页面,用于显示设备保养项列表
*/
TwoLineItem {
    id: maintainDeviceDelegate
    property string actionSequence: "-1"
    firstLeftText: ObjectName
    secondLeftText: "型号:" + ModelName
    secondRightText: "位置:" + Positions
    rFIDCode:RFIDCode
    barCode:BarCode
    twolimitMode:maintainTaskList.otherParam["LimitMode"]
    //roles = ["TaskObjectDataKey", "ObjectName", "ObjectFullName", "RFIDCode", "BarCode", "Positions", "ModelName"];

    onTriggered: {
        if((twolimitMode == "1" && rFIDCode == ""&& barCode == "") || twolimitMode != "1"){
            maintainDeviceDelegate.actionSequence = inspectionSystemConfig.getSequence();
            transmitManager.resultOfResponse.disconnect(maintainDeviceDelegate.resultResponse)
            transmitManager.resultOfResponse.connect(maintainDeviceDelegate.resultResponse)

            var head = {"Objects" : "EMM_GetTaskObjectDetail"};
            var para = {"TaskObjectDataKey" : TaskObjectDataKey};
            console.log("transmitManager.requestData   EMM_GetTaskObjectDetail")
            transmitManager.requestData(head, para, maintainDeviceDelegate.actionSequence, "获取保养任务项明细失败");
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
        console.debug("--ItemListDelegate--onResultOfResponse--result, error: ", result, error);
        if(action != maintainDeviceDelegate.actionSequence){
            console.log("action != maintainDeviceDelegate.actionSequence  return")
            return;
        }
        transmitManager.resultOfResponse.disconnect(maintainDeviceDelegate.resultResponse)
        if(result && error == 0){
            console.debug("---map:", map)
            var paramValue=  {
                "IsRFIDScan": "0",
                "IsBarCodeScan": "0",
                "EnableUploadAttach": maintainTaskList.otherParam["EnableUploadAttach"],
                "pageFormType":"maintain",
                "parentPage":maintainTaskList,
                "TaskObjectDataKey":TaskObjectDataKey
            }
            pageStack.push("qrc:/qml/DeviceMaintainTask/DeviceMaintainForm.qml", {"dataPackageValue": map,
                               "paramValue": paramValue});
        }
    }
}
