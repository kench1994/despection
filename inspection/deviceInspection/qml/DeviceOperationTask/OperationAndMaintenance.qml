/*!
* \file OperationAndMaintenance.qml
* \brief 设备维修功能选择页面
*
*设备维修功能选择页面
*
*\author 张子健 zhangzijian@syberos.com
*\version 1.0
*\date 2015/9/7
*/
import QtQuick 2.0
import com.syberos.basewidgets 2.0
import "../Common"

/*!
  \class OperationAndMaintenance
  \inModule 设备维修
  \brief 设备维修功能选择页面
  \ingroup DeviceOperationTask
  此界面是设备维修功能选择页面，用于进入故障申报或维修查询
*/

CommonPage {
    id: listPage
    property string nameId: ""
    statusBarHoldEnabled: true
    statusBarHoldItemColor: "white"
    property string actionSequence:"-1"
    property string taskStatus : ""
    property string titleName: ""
    property var originalData
    orientationPolicy: CPageOrientation.LockPortrait
    signal faultRepairTasks(int action);
    contentAreaItem:  Rectangle {
        id: content
        color:"#eeeff1"
        Rectangle{
            id:pageHead
            width: parent.width
            height: title.height
            CommonTitlebar{
                id: title
                width: parent.width
                leftItemEnabled: true
                titleItemEnabled: true
                rightItemEnabled: false
                leftItemText:""
                titleText:{
                    var name;
                    var clientPriority = inspectionSystemConfig.getClientPriority();
                    if(clientPriority == false){
                        name = inspectionSystemParam.getUserPermissionData(listPage.nameId, "ModuleName");
                    } else
                        name = inspectionFuncsItemModel.getNodeParas(listPage.nameId, "")["NameCN"];
                    return name;
                }
                onLeftItemTriggered: pageStack.pop()
                anchors.top: parent.top
                onLeftItemChanged:leftItem.backgroundEnabled = false
                leftItemBackgroundEnabled: false
            }
        }


        Item{
            id:deviceGrid
            width:  parent.width - 10;
            anchors.top: pageHead.bottom
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            GridView {
                id: grid
                width:  parent.width;
                anchors.top:parent.top;
                anchors.topMargin: 10;
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 10
                //interactive: false
                cellWidth: 156;
                cellHeight: 156;
                model: listModel;
                delegate: contactDelegate
                clip: true
            }
        }

        ListModel {
            id: listModel
            ListElement {
                iconPath: "qrc:/icon/operation_report_normal.png"
                faultActName:"故障申报"
                modelItemID : "-1"
                orderIndex: "0"
                newCount: "0"
            }
        }

        Component {
            id: contactDelegate
                Item{
                    width: grid.cellWidth
                    height: grid.cellHeight
                    Rectangle {
                        width: grid.cellWidth - 10
                        height: grid.cellHeight -10
                        anchors.centerIn: parent
                        color: modulMouse.pressed ? "#dddddd" : "white";
                        Rectangle {
                            //角标
                            height: 20
                            radius: height/2;
                            anchors.right: parent.right;
                            color: "red"
                            visible: texCount.text != "0"
                            width: texCount.width + 10;
                            Text {
                                id: texCount;
                                text: newCount
                                color: texCount.text != "0"?"#ffffff":"white";
                                font.pixelSize: 14;
                                anchors.centerIn: parent;
                            }
                        }
                        Image {
                            id: img
                            anchors.centerIn: parent;
                            source: iconPath;
                            width: 50;
                            height: 50
                            fillMode: Image.Stretch;
                        }
                        Text {
                            id: texName
                            anchors.horizontalCenter: parent.horizontalCenter;
                            anchors.bottom: parent.bottom
                            anchors.bottomMargin: 10
                            text: faultActName;
                            color: "#333333";
                            font.pixelSize: 22;
                        }
                        MouseArea{
                            id:modulMouse
                            anchors.fill: parent
                            onClicked: {
                                console.log("index is ", index, "  orderIndex   ", orderIndex)
                                switch(orderIndex){
                                case "0":
                                    pageStack.push("qrc:/qml/Common/SearchPage.qml",{
                                    "titleText": faultActName,
                                                 "pageActionType": "Failure to declare",
                                                 "enabledScanItem": true});
                                    break;
                                case "7":
                                    pageStack.push("qrc:/qml/Common/SearchPage.qml",{
                                                       "titleText": faultActName,
                                                       "pageActionType": "Ops query",
                                                       "enabledScanItem": false});
                                    break;
                                case "1":
                                    queryFaultRepairTaskList("1", faultActName, "获取运维受理任务列表失败");
                                    break;
                                case "2":
                                    queryFaultRepairTaskList("2", faultActName, "获取运维审批任务列表失败");
                                    break;
                                case "3":
                                    queryFaultRepairTaskList("3", faultActName, "获取运维派工任务列表失败");
                                    break;
                                case "4":
                                    queryFaultRepairTaskList("4", faultActName, "获取运维登记任务列表失败");
                                    break;
                                case "5":
                                    queryFaultRepairTaskList("5", faultActName, "获取运维费用任务列表失败");
                                    break;
                                case "6":
                                    queryFaultRepairTaskList("6", faultActName, "获取运维评价任务列表失败");
                                    break;
                                }
                            }
                        }
                    }
                }
            }
    }

    //获取指定运维任务列表
    function queryFaultRepairTaskList(status, name, errorText){
        listPage.taskStatus = status;
        listPage.titleName = name;
        listPage.actionSequence = inspectionSystemConfig.getSequence();
        transmitManager.resultOfResponse.disconnect(listPage.resultResponse)
        transmitManager.resultOfResponse.connect(listPage.resultResponse)

        var head1 = {"Objects" : "EMF_FaultRepair_Query_Task"};
        var para1 = {"UserCode" : inspectionSystemParam.getUserPropertyByKey("UserCode"),
            "Status" : status};
        console.log("transmitManager.requestData   EMF_FaultRepair_Query_Task ")
        transmitManager.requestData(head1, para1, listPage.actionSequence, errorText);
    }

    function resultResponse(result, error, action, map){
        console.log("OperationAndMaintenance resultResponse  ", action)
        if(action != listPage.actionSequence){
            console.log("action != listPage.actionSequence return")
            return;
        }
        transmitManager.resultOfResponse.disconnect(listPage.resultResponse)
        if(result == true && Number(error) == 0) {
            var headObj = map["head"]["objects"];
            if(headObj == "EMF_FaultRepair_Query_Task"){
                var otherParam = {
                    "TaskStatus" : listPage.taskStatus,
                    "TitleName" : listPage.titleName,
                    "EnabledScanItem" : false
                };
                pageStack.push("qrc:/qml/DeviceOperationTask/FaultRepairTaskList.qml",
                               {"originalData": map,
                                   "otherParam" : otherParam
                               });
            } else if(headObj == "EMF_FaultRepair_ModelCount"){
//                <Field Value="StatusやTotalCount"/>
//                <D V="1や26"/>
                //listModel
                var dataMap = map["tables"][0]["value"];
                for(var j = 0; j < listModel.count; j++){
                    var orderIndex = listModel.get(j).orderIndex;
                    var find = false;
                    for(var i = 0; i < dataMap.length; i++){
                        var status =dataMap[i]["Status"];
                        if(status == orderIndex){
                            find = true;
                            listModel.set(j, {"newCount" : dataMap[i]["TotalCount"]});
                            break;
                        }
                    }
                    if(!find){
                        listModel.set(j, {"newCount" : "0"});
                    }
                }
            }
        }
    }


    function setListModelData(){
        var data = listPage.originalData["tables"][0]["value"];
        if(!(listPage.originalData["para"]["OfflineModel"] != undefined && listPage.originalData["para"]["OfflineModel"] == "1") ){
            //有网情况下显示查询
            listModel.append({"faultActName" : "维修查询", "modelItemID" : "-1",
                                 "orderIndex" : "7", "iconPath" : "qrc:/icon/operation_query_normal.png", "newCount" : "0"});
        }

        for(var i = 0; i < data.length; i++){
            var iconPath;
            var dataMap = data[i];
            var orderIndex = dataMap["OrderIndex"];
            switch(orderIndex){
            case "1":
                iconPath = "qrc:/icon/operation_acceptance_normal.png";
                break;
            case "2":
                iconPath = "qrc:/icon/operation_approval_normal.png";
                break;
            case "3":
                iconPath = "qrc:/icon/operation_dispatch_normal.png";
                break;
            case "4":
                iconPath = "qrc:/icon/operation_register_normal.png";
                break;
            case "5":
                iconPath = "qrc:/icon/operation_cost_normal.png";
                break;
            case "6":
                iconPath = "qrc:/icon/operation_evaluation_normal.png";
                break;
            }

            listModel.append({"faultActName" : dataMap["FaultActName"], "modelItemID" : dataMap["ModelItemID"],
                                 "orderIndex" : dataMap["OrderIndex"], "iconPath" : iconPath, "newCount" : "0"});
        }
    }
    function getModelCount(){
        listPage.actionSequence = inspectionSystemConfig.getSequence();
        transmitManager.resultOfResponse.disconnect(listPage.resultResponse)
        transmitManager.resultOfResponse.connect(listPage.resultResponse)

        var head1 = {"Objects" : "EMF_FaultRepair_ModelCount"};
        var para1 = {"UserID" : inspectionSystemParam.getUserId()};
        console.log("transmitManager.requestData   EMF_FaultRepair_ModelCount ")
        transmitManager.requestData(head1, para1, listPage.actionSequence, "", true, false);
    }
onStatusChanged: {
    if (status == CPageStatus.Show) {
        getModelCount();
    }
}
    Component.onCompleted: {
        /*
          <Field Value="FaultActNameやModelItemIDやOrderIndexやPostID"/>
        <D V="维修受理やや1や7ace5add-6749-11e5-8729-00163e004c5a"/>
        <D V="运维审批やe0589c82-415e-11e5-bd89-0021ccceca60や2や7ace5add-6749-11e5-8729-00163e004c5a"/>
        <D V="维修派工や0a07fb15-415f-11e5-bd89-0021ccceca60や3や7ace5add-6749-11e5-8729-00163e004c5a"/>
        <D V="维修登记や229e8868-415f-11e5-bd89-0021ccceca60や4や7ace5add-6749-11e5-8729-00163e004c5a"/>
        <D V="维修费用やde9912df-94d3-11e6-ad7b-02004c4f4f50や5や7ace5add-6749-11e5-8729-00163e004c5a"/>
        <D V="维修评价や32a21dba-415f-11e5-bd89-0021ccceca60や6や7ace5add-6749-11e5-8729-00163e004c5a"/>*/
        setListModelData();
    }
}
