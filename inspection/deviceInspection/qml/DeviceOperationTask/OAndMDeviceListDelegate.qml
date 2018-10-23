/*!
* \file OAndMDeviceListDelegate.qml
* \brief 维修查询设备列表显示页面
*
*维修查询设备列表显示页面
*
*\author 张子健 zhangzijian@syberos.com
*\version 1.0
*\date 2015/9/7
*/
import QtQuick 2.0
import com.syberos.basewidgets 2.0
import "../Common"
/*!
  \class OAndMDeviceListDelegate
  \inModule 设备维修
  \brief 维修查询设备列表显示页面
  \ingroup DeviceOperationTask
  此界面是维修查询设备列表显示页面,用于显示维修查询设备列表
*/
TwoLineItem {
    id : rootRect
    property string actionSequence: "-1"
    /** type:string 设备名称*/
    firstLeftText: EquipmentName
    /** type:string 类型*/
    firstRightText: os.i18n.ctr(qsTr("FaultStatus: ")) + FaultStatus
    /** type:string 状态*/
    secondLeftText: os.i18n.ctr(qsTr("FaultType: ")) + FaultType
    /** type:string 报告时间*/
    secondRightText: ReportTime

    property var dataMap;
    onTriggered: {
        rootRect.actionSequence = inspectionSystemConfig.getSequence();
        transmitManager.resultOfResponse.disconnect(rootRect.resultResponse)
        transmitManager.resultOfResponse.connect(rootRect.resultResponse)

        var head = {"Objects" : "EMF_FaultRepair_Query_One"};
        var para = {"ID" : ID};
        console.log("transmitManager.requestData   EMF_FaultRepair_Query_ One ")
        transmitManager.requestData(head, para, rootRect.actionSequence, "获取故障申报设备失败");
    }
    function resultResponse(result, error, action, map){
        console.debug("---OAndMDeviceListDelegate----onResultOfResponse-----result, error: ", result, error);
        if(action != rootRect.actionSequence){
            console.log("OAndMDeviceListDelegate  action != rootRect.actionSequence return")
            return;
        }
        transmitManager.resultOfResponse.disconnect(rootRect.resultResponse)
        if(result && error == 0){
            pageStack.push("qrc:/qml/DeviceOperationTask/OAndMDeviceDetail.qml",
                           {"dataPackageValue":map});
        }
    }
}
