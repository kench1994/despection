/*!
* \file StandardizedSuiteDelegate.qml
* \brief 标准化检查套件列表页面
*
*标准化检查套件列表页面
*
*\author 张子健 zhangzijian@syberos.com
*\version 1.0
*\date 2015/9/9
*/
import QtQuick 2.0
import com.syberos.basewidgets 2.0
import "../Common"
/*!
  \class StandardizedSuiteDelegate
  \inModule 标准化检查
  \brief 标准化检查套件列表页面
  \ingroup StandardizedTask
  此界面是标准化检查套件列表页面,用于显示标准化检查套件列表
*/
SingleLineItem {
    id: stdSuitDelegate
    property string actionSequence: "-1"
    /** type:string 套件名称*/
    leftText: TaskObjectName
    /** type:string 完成比例*/
    rightText: ExamItemCount + "/" + TotalItemCount
    //        roles = ["TaskObjectID", "TaskObjectDataKey","TaskID" ,"TaskObjectName","ExamObjectType", "RealObjectID", "TotalItemCount", "ExamItemCount",
    //                "RFIDCode", "BarCode", "QRCode"];
    onTriggered: {
        stdSuitDelegate.actionSequence = inspectionSystemConfig.getSequence();
        transmitManager.resultOfResponse.disconnect(stdSuitDelegate.resultResponse)
        transmitManager.resultOfResponse.connect(stdSuitDelegate.resultResponse)
        var head2 = {"Objects" : "EXAM_ExamineTaskItemList"};
        var para2 = {"TaskObjectDataKey" : TaskObjectDataKey};
        console.log("transmitManager.requestData   EXAM_ExamineTaskItemList ")
        transmitManager.requestData(head2, para2, stdSuitDelegate.actionSequence, "获取规范检查项列表失败");
    }
    /**
     * 接受服务器返回数据的槽函数
     * @param result:bool 获取数据是否成功
     * @param error:int 错误代码
     * @param action:string 当前发送数据动作类型
     * @param map:var    服务器返回值
     */
    function resultResponse(result, error, action, map){
        console.debug("---SuiteDelegate---onResultOfResponse---result, error: ", result, error);
        if(action != stdSuitDelegate.actionSequence){
            console.log("action != stdSuitDelegate.actionSequence return");
            return;
        }
        transmitManager.resultOfResponse.disconnect(stdSuitDelegate.resultResponse)
        if(result && error == 0){
            var otherParam = {
                "TaskObjectDataKey": TaskObjectDataKey,
                "TaskObjectName": TaskObjectName,
                "RealObjectID" : RealObjectID,
                "LimitMode": stdTaskList.otherParam["LimitMode"],
                "EnableUploadAttach": stdTaskList.otherParam["EnableUploadAttach"],
                "EnabledScanItem": true,
            }
            pageStack.push("qrc:/qml/StandardizedTask/StandardBaseList.qml",
                           {
                               "originalData": map,
                               "otherParam":otherParam,
                           });
        }
    }
}
