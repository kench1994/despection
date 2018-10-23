/*!
* \file RfconfirmListDelegate.qml
* \brief 整改确认任务列表页面
*
*整改确认任务列表页面
*
*\author 张子健 zhangzijian@syberos.com
*\version 1.0
*\date 2015/9/10
*/
import QtQuick 2.0
import com.syberos.basewidgets 2.0
import "../Common"
/*!
  \class RfconfirmListDelegate
  \inModule 检查整改
  \brief 整改确认任务列表页面
  \ingroup RectificationTask
  此界面是整改确认任务列表页面,用于显示整改确认任务列表
*/
TwoLineItem {
    id : rfConfirmTaskDelegate

    property string actionSequence: "-1"
    /** type:string 任务名称*/
    firstLeftText: OrderName
    /** type:string 整改时间*/
    secondLeftText: os.i18n.ctr(qsTr("rectification time: ")) + Circle

    onTriggered: {
        rfConfirmTaskDelegate.actionSequence = inspectionSystemConfig.getSequence();
        transmitManager.resultOfResponse.disconnect(rfConfirmTaskDelegate.resultResponse)
        transmitManager.resultOfResponse.connect(rfConfirmTaskDelegate.resultResponse)
        var head = {};
        head["Objects"] = "EXAM_ExamineRectifyProcDetail";
        var para = {};
        para["OrderId"] = OrderID;
        para["RFIDCode"] = "";
        para["BarCode"] = "";
        console.log("transmitManager.requestData   EXAM_ExamineRectifyProcDetails ")
        transmitManager.requestData(head, para, rfConfirmTaskDelegate.actionSequence, os.i18n.ctr(qsTr("Get rectification confirm task fail")));
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
        if(action != rfConfirmTaskDelegate.actionSequence){
            console.log("action != rfConfirmTaskDelegate.actionSequence return")
            return;
        }

        transmitManager.resultOfResponse.disconnect(rfConfirmTaskDelegate.resultResponse)
        if(result && error == 0){
            var paramValue=  {"IsRFIDScan": "0",
                "IsBarCodeScan": "0",
                "limitMode": 0,
                "enableImage": 0,}
            pageStack.push("qrc:/qml/RectificationTask/RectificationconfirmForm.qml", {"dataPackageValue": map,
                                                                 "paramValue": paramValue});
        }
    }
}
