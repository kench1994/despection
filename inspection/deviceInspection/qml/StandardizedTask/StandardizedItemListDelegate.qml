/*!
* \file StandardizedItemListDelegate.qml
* \brief 标准化检查项列表页面
*
*标准化检查项列表页面
*
*\author 张子健 zhangzijian@syberos.com
*\version 1.0
*\date 2015/9/9
*/
import QtQuick 2.0
import com.syberos.basewidgets 2.0
import "../Common"
/*!
  \class StandardizedItemListDelegate
  \inModule 标准化检查
  \brief 标准化检查项列表页面
  \ingroup StandardizedTask
  此界面是标准化检查项列表页面,用于显示标准化检查项列表
*/
TwoLineItem {
    id: stdDeviceDelegate
    property string actionSequence: "-1"
    /** type:string 标准化检查项名称*/
    firstLeftText: ItemName
    secondLeftText: ItemFullName
    rFIDCode: RFIDCode
    barCode:  BarCode
    twolimitMode : stdTaskList.otherParam["LimitMode"]
//    roles = ["TaskItemID", "TaskItemDataKey", "ItemName", "ItemFullName", "TaskObjectDataKey", "TaskID", "SerialKey",
    //            "RFIDCode", "BarCode", "QRCode"];
    onTriggered: {
        if((twolimitMode == "1" && rFIDCode === "" && barCode === "")|| twolimitMode != "1"){
            stdDeviceDelegate.actionSequence = inspectionSystemConfig.getSequence();
            transmitManager.resultOfResponse.disconnect(stdDeviceDelegate.resultResponse)
            transmitManager.resultOfResponse.connect(stdDeviceDelegate.resultResponse)
            var head = {"Objects" : "EXAM_ExamineTaskItemDetail"};
            var para = {"TaskItemDataKey" : TaskItemDataKey};
            console.log("transmitManager.requestData   EXAM_ExamineTaskItemDetail ")
            transmitManager.requestData(head, para, stdDeviceDelegate.actionSequence, os.i18n.ctr(qsTr("Get standardized inspection fail")));
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
        console.debug("---stdDeviceDelegate ---Response---result, error: ", result, error);
        if(action != stdDeviceDelegate.actionSequence){
            console.log("action != stdDeviceDelegate.actionSequence  return");
            return;
        }
        transmitManager.resultOfResponse.disconnect(stdDeviceDelegate.resultResponse)
        if(result && error == 0){
            var paramValue=  {
                "IsRFIDScan": "0",
                "IsBarCodeScan": "0",
                "TaskObjectDataKey": TaskObjectDataKey,
                "LimitMode": stdTaskList.otherParam["LimitMode"],
                "EnableUploadAttach": stdTaskList.otherParam["EnableUploadAttach"],
                "parentPage":stdTaskList
            }
            pageStack.push("qrc:/qml/StandardizedTask/DeviceStdCheckForm.qml", {"dataPackageValue": map,
                               "paramValue": paramValue});
        }
    }
}
