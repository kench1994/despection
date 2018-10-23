/*!
* \file TransferPlanListDelegate.qml
* \brief 设备调拨计划列表页面
*
*设备调拨计划列表页面
*
*\author 张子健 zhangzijian@syberos.com
*\version 1.0
*\date 2015/9/10
*/
import QtQuick 2.0
import com.syberos.basewidgets 2.0
import "../Common"
/*!
  \class TransferPlanListDelegate
  \inModule 设备调拨
  \brief 设备调拨计划列表页面
  \ingroup ComprehensiveTask
  设备设备调拨计划列表页面,用于显示设备调拨计划列表
*/
TwoLineItem {
    id: rootRect
    enabled: (SyncStatus == undefined || SyncStatus==1)?true:false
    /** type:string 计划名称*/
    firstLeftText:  OrderName
    /** type:string 日期*/
    secondLeftText: {
        if(SyncStatus == undefined || SyncStatus == 1){
            return os.i18n.ctr(qsTr("date: ")) + OperatorDate
        }else if(SyncStatus == 2){
            return "同步失败"
        }else if(SyncStatus == 0){
            return "正在同步中..."
        }
    }
//    roles = ["ID", "OrderCode", "OrderName", "OutOrganiseUnit", "OutDepartMent",
//             "InOrganiseUnit", "InDepartMent","OperatorDate","Status",
//             "OutOrganiseUnitName", "InOrganiseUnitName", "SyncStatus"];

    onTriggered: {
        var taskValues = [ID, OrderCode, OrderName, OutOrganiseUnit, OutDepartMent,
                          InOrganiseUnit, InDepartMent, OperatorDate,Status,
                          OutOrganiseUnitName, InOrganiseUnitName];
        pageStack.push("qrc:/qml/ComprehensiveTask/DeviceTransferDetail.qml",{"paramValue":taskValues});
    }
}
