/*!
* \file RecipientsPlanListDelegate.qml
* \brief 设备领用计划列表页面
*
*设备领用计划列表页面
*
*\author 张子健 zhangzijian@syberos.com
*\version 1.0
*\date 2015/9/10
*/
import QtQuick 2.0
import com.syberos.basewidgets 2.0
import "../Common"
/*!
  \class RecipientsPlanListDelegate
  \inModule 综合业务
  \brief 设备领用计划列表页面
  \ingroup ComprehensiveTask
  此界面是设备领用计划列表页面,用于显示设备领用计划列表
*/
TwoLineItem {
    id: recipientsPlanDelegate
    property string actionSequence: "-1"
    enabled: (SyncStatus == undefined || SyncStatus==1)?true:false
    /** type:string 计划名称*/
    firstLeftText:  OrderName
    /** type:string 日期*/
    secondLeftText: {
        if(SyncStatus == undefined || SyncStatus == 1){
            return "日期:" + OperatorDate
        }else if(SyncStatus == 2){
            return "同步失败"
        }else if(SyncStatus == 0){
            return "正在同步中..."
        }
    }
    /** type:string 完成比例*/
    secondRightText: {
        if(SyncStatus == undefined || SyncStatus == 1){
            return "领用:" + operatorrate
        }else{
            return ""
        }
    }

//roles = ["ID", "OrderCode", "OrderName", "OrganiseUnitID", "DepartMentID", "OperatorDate", "operatorrate"];
    //EMD_DistributeEquipmentList
    onTriggered: {
        recipientsPlanDelegate.actionSequence = inspectionSystemConfig.getSequence();
        transmitManager.resultOfResponse.disconnect(recipientsPlanDelegate.resultResponse)
        transmitManager.resultOfResponse.connect(recipientsPlanDelegate.resultResponse)

        var head1 = {};
        head1["Objects"] = "EMD_DistributeEquipmentList";
        var para1 = {"OrderCode" : OrderCode};
        console.log("transmitManager.requestData   EMD_DistributeEquipmentList ")
        transmitManager.requestData(head1, para1, recipientsPlanDelegate.actionSequence, "获取领用设备列表失败");
    }

    /**
     * 接受服务器返回数据的槽函数
     * @param result:bool 获取数据是否成功
     * @param error:int 错误代码
     * @param action:string 当前发送数据动作类型
     * @param map:var    服务器返回值
     */
    function resultResponse(result, error, action, map){
        console.debug("-recipientsPlanDelegate-onResultOfResponse-result, error: ", result, error);
        if(action != recipientsPlanDelegate.actionSequence){
            console.log("action != recipientsPlanDelegate.actionSequence return");
            return;
        }

        transmitManager.resultOfResponse.disconnect(recipientsPlanDelegate.resultResponse)
        if(result && error == 0){
            var recipientsOtherParam = {
                "DistributeID" : ID,
                "OrderCode" : OrderCode,
                "TitleText":OrderName,
                "EnabledScanItem": true
            }
            pageStack.push("qrc:/qml/ComprehensiveTask/ComprehensiveBaseList.qml",
                           {
                               "originalData": map,
                               "otherParam":recipientsOtherParam,
                           });

        }
    }
}
