/*!
* \file ListDelegate.qml
* \brief 整改任务列表页面
*
*整改任务列表页面
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
  \inModule 检查整改
  \brief 整改任务列表页面
  \ingroup RectificationTask
  此界面是整改任务列表页面,用于显示整改任务列表
*/
TwoLineItem {
    id : rectificationTaskDelegate

    property string actionSequence: "-1"
    enabled: (SyncStatus == undefined || SyncStatus==1)?true:false
    /** type:string 任务名称*/
    firstLeftText: OrderName
    /** type:string 整改时间*/
    secondLeftText: {
        if(SyncStatus == undefined || SyncStatus == 1){
            return os.i18n.ctr(qsTr("rectification time: ")) + Circle
        }else if(SyncStatus == 2){
            return "同步失败"
        }else if(SyncStatus == 0){
            return "正在同步中..."
        }
    }

    onTriggered: {
        rectificationTaskDelegate.actionSequence = inspectionSystemConfig.getSequence();
        transmitManager.resultOfResponse.disconnect(rectificationTaskDelegate.resultResponse)
        transmitManager.resultOfResponse.connect(rectificationTaskDelegate.resultResponse)

        var head = {};
        head["Objects"] = "EXAM_ExamineRectifyProcDetail";
        var para = {};
        para["OrderId"] = OrderID;
        para["RFIDCode"] = "";
        para["BarCode"] = "";
        console.log("transmitManager.requestData   EXAM_ExamineRectifyProcDetail ")
        transmitManager.requestData(head, para, rectificationTaskDelegate.actionSequence, os.i18n.ctr(qsTr("Get rectification task fail")));
    }

    /**
     * 接受服务器返回数据的槽函数
     * @param result:bool 获取数据是否成功
     * @param error:int 错误代码
     * @param action:string 当前发送数据动作类型
     * @param map:var    服务器返回值
     */
    function resultResponse(result, error, action, map){
        console.debug("----ItemListDelegate-----onResultOfResponse-result, error: ", result, error);
        if(action != rectificationTaskDelegate.actionSequence){
            console.log("action != rectificationTaskDelegate.actionSequence return")
            return;
        }

        transmitManager.resultOfResponse.disconnect(rectificationTaskDelegate.resultResponse)
        if(result && error == 0){
            var paramValue=  {"IsRFIDScan": "0",
                "IsBarCodeScan": "0",
                "LimitMode": 0,
                "EnableUploadAttach": 1,
            }
            pageStack.push("qrc:/qml/RectificationTask/RectificationForm.qml", {"dataPackageValue": map,
                                                                 "paramValue": paramValue});
        }
    }
}
