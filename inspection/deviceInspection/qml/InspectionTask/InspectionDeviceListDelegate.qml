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
    property string inspectionModel:  inspectionTaskList.otherParam["InspectionMode"]
    /*
      InspectionMode: 1:普通设备；2：复合对象巡检；3：路线巡检; 4：巡检点巡检;5：组对象巡检;
      为1时，直接进入设备列表
      */
    opacity: mouseEnable? 1: 0.5
    mouseEnable: (inspectionModel == "3" && inspectionTaskList.model.getSourceIndex(index) == 0)
    ||(inspectionModel == "5" &&(IsDelegate == "1" || inspectionTaskList.isDelegateDataSaved == true))
    ||(inspectionModel != "3" && inspectionModel != "5")
    /** type:string 备巡检项名称*/
    firstLeftText: ObjectName +
                   (inspectionModel == "5" && IsDelegate == "1" ?
                        (inspectionTaskList.isDelegateDataSaved ? "(组代表,已填报)" : "(组代表,请优先填报)") : "")
    secondLeftText:  "位置:" + Positions
    //按照要求，和安卓一样，不显示已保存
    secondRightText: "型号:" + ModelName
    //secondRightText: Saved == "1" ? "已保存" : ""
     /** type:string RFID编码*/
    rFIDCode:RFIDCode
    /** type:string 二维码*/
    barCode:BarCode
    /** type:string 是否允许手动操作*/
    twolimitMode : inspectionTaskList.otherParam["LimitMode"]
//    roles = ["RealObjectID", "TaskObjectDataKey", "ObjectName", "ObjectFullName", "RealObjectType", "RFIDCode", "BarCode", "QRCode",
//             "OrderIndex", "SerialKey",  "IsDelegate", "Positions", "ModelName", "DeviceLastModifiedDate", "Saved"];
    onTriggered: {
        console.debug("---TaskObjectDataKey: ",TaskObjectDataKey, "  twolimitMode ", twolimitMode, " rFIDCode  ", rFIDCode, " barCode ", barCode,
                      " judge is ", (twolimitMode === "1" && rFIDCode === "" && barCode === "") || twolimitMode === "0");
        if((twolimitMode == "1" && rFIDCode === "" && barCode === "")|| twolimitMode != "1") {
            if(inspectionTaskList.otherParam["InspectionMode"] == "5" && IsDelegate == "1"){
                inspectionTaskList.selectedIndex = inspectionTaskList.model.getSourceIndex(index);
                inspectionTaskList.showGroupModelSelect(TaskObjectDataKey);
            }else{
                inspectionDeviceDelegate.actionSequence = inspectionSystemConfig.getSequence();
                transmitManager.resultOfResponse.disconnect(inspectionDeviceDelegate.resultResponse)
                transmitManager.resultOfResponse.connect(inspectionDeviceDelegate.resultResponse)
                var head = {"Objects" : "EISM_GetTaskObjectDetail"};
                var para = {"TaskObjectDataKey" : TaskObjectDataKey};
                console.log("transmitManager.requestData   EISM_GetTaskObjectDetail ")
                transmitManager.requestData(head, para, inspectionDeviceDelegate.actionSequence, os.i18n.ctr(qsTr("Get inspction device fail")));
            }
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
                "EnableUploadAttach": inspectionTaskList.otherParam["EnableUploadAttach"],
                "PageFormType":"inspection",
                "InspectionMode":inspectionTaskList.otherParam["InspectionMode"],
                "isEditGroupData" : "0",
                "IsDelegate": "0",
                "SelectedIndex" : index,
                "ParentPage":inspectionTaskList
            };
            pageStack.push("qrc:/qml/InspectionTask/DeviceInspectionForm.qml",
                           {
                               "dataPackageValue": map,
                               "paramValue": paramValue
                           });
        }
    }
}
