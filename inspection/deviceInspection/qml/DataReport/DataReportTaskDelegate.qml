/*!
* \file ListDelegate.qml
* \brief 设备盘点任务列表页面
*
*设备盘点任务列表页面
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
  \inModule 设备盘点
  \brief 设备盘点任务列表页面
  \ingroup InventoryTask
  此界面是设备盘点任务列表页面,用于显示设备盘点任务列表
*/
TwoLineItem {
    id : dataReportTaskDelegate
    property string actionSequence: "-1"
    /** type:string 任务名称*/
    firstLeftText: TaskName
    /** type:string 盘点时间*/
    secondLeftText: "方案: " + ProjectName

     //var roles = ["ID", "TaskCode", "TaskName", "TaskStatus", "ProjectName", "ShortName"];

    onTriggered: {
        dataReportTaskDelegate.actionSequence = inspectionSystemConfig.getSequence();
        console.log("dataReportTaskDelegate.actionSequence is ", dataReportTaskDelegate.actionSequence)
        transmitManager.resultOfResponse.disconnect(dataReportTaskDelegate.resultResponse)
        transmitManager.resultOfResponse.connect(dataReportTaskDelegate.resultResponse)

        //int requestData(const QMap<QString, QString> &head, const QMap<QString, QString> &para, const QString &action, const QString &errorText);
        var head1 = {};
        head1["Objects"] = "DFI_ReportTaskItem_GetOne";
        var para1 = {};
        para1["TaskID"] = ID;
        console.log("transmitManager.requestData   DFI_ReportTaskItem_GetOne ")
        transmitManager.requestData(head1, para1, dataReportTaskDelegate.actionSequence, "获取数据上报任务列表失败");

//        transmitManager.requestDataReportTaskDeviceList(ID, dataReportTaskDelegate.actionSequence,
//                                                  "获取数据上报任务列表失败"
//                                                  );
    }

    /**
     * 接受服务器返回数据的槽函数
     * @param result:bool 获取数据是否成功
     * @param error:int 错误代码
     * @param action:string 当前发送数据动作类型
     * @param map:var    服务器返回值
     */
    function resultResponse(result, error, action, map){
        console.debug("---dataReportTaskDelegate---onResultOfResponse---result, error: ", result, error);
        if(action != dataReportTaskDelegate.actionSequence){
            console.log("action != dataReportTaskDelegate.actionSequence return")
            return;
        }
        transmitManager.resultOfResponse.disconnect(dataReportTaskDelegate.resultResponse)
        if(result && error == 0){
            var paramValue=  {
                "parentPage":dataReportTaskList,
            };
            pageStack.push("qrc:/qml/DataReport/DataReportForm.qml",
                           {
                               "dataPackageValue": map,
                               "paramValue": paramValue

                           });
        }
    }
}
