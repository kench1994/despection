import QtQuick 2.0
import com.syberos.basewidgets 2.0
import "../Common"
import "../Common/constants.js" as UI
import "../Multimedia"
Column{
    id:faultRepairDetailList
    visible: deviceOperationDetail.showDetail
    width: parent.width
    property string actionSequence: "-1"
    property var arrayFaultReportDetailModel:
        [["申报单号:","FaultCode"],["设备型号:","EquitmentModelName"],
        ["设备厂商:","EquitmentEnterpriseName"],["设备所属部门:","EquitmentDepartmentName"],
        ["保管人:","EquitmentCustodian"],["申报单位:","ReporterOrganiseName"],["申报人:","ReporterName"],["申报时间:","ReportDate"],
        ["故障类型:","FalutType"],["故障原因:","FaultReason"], ["故障情况的说明:", "FaultDescription"]]
    property var faultTypeMap: {"1":"运行损坏", "2":"意外损坏", "3":"事故损坏", "4":"其他原因"}
    property var arrayFaultAcceptDetailModel:
        [ ["受理单位:","HandleOrganiseName"],["受理人:","HandlerName"],["受理时间:","HandleDate"],["受理意见:", "HandleDescription"] ]
    property var arrayFaultApproveDetailModel:
            [ ["审核单位:","AuditOrganiseName"], ["确认人:","AuditUserName"], ["确认时间:","AuditDate"], ["审批意见:", "AuditDescription"] ]

    property var arrayFaultDispatchDetailModel:
        [ ["派工单位:","DispatchOrganiseName"], ["派工人:","DispatcherName"], ["维修单位:","RepairCompany"],
        ["维修人姓名:","RepairPerson"],["维修联系电话:","RepairPersonTelephone"],
        ["派工时间:","DispatchDate"],["计划维修时间:","PlanRepairDate"], ["派工意见:", "DispatchDescription"] ]
    property var arrayFaultMaintainDetailModel : [
        ["维修登记单位:","RepairOrganiseName"],
        ["维修登记人:","RepairRegisterUser"],
        ["登记时间:","RepairRegisterDate"],
        ["实际维修时间:","RepairDate"],
        ["维修方式:","RepairType"],
        ["维修结果:","RepairResult"],
        ["维修情况:", "RepairDescription"]  ]

    property var arrayFaultCostDetailModel:
        [["费用登记单位:","CostRecordCompanyName"],
        ["费用登记人:","CostRecordUser"],
        ["费用登记时间:","CostRecordDate"],
        ["配件费用(单位:元):","PartsCost"],
        ["人工费用(单位:元):","LaborCost"],
        ["其他费用(单位:元):","OtherCost"],
        ["总费用(单位:元):","TotalCost"],
        ["备注:","CostRemark"] ]
    property var  arrayFaultAppraiseDetailModel:[
        ["评价单位:", "AppraiseOrganiseName"],
        ["评价人:", "AppraiseUser"],
        ["评价时间:", "AppraiseDate"],
        ["评价结果:", "AppraiseResult"],
        ["评价意见:", "AppraiseDescription"] ]
    ListModel{
        id:faultReportListModel
    }
    ListModel{
        id:faultAcceptListModel
    }
    ListModel{
        id:faultApproveListModel
    }
    ListModel{
        id:faultDispatchListModel
    }
    ListModel{
        id:faultMaintainListModel
    }
    ListModel{
        id:faultCostListModel
    }
    ListModel{
        id:faultAppraiseListModel
    }
    Repeater{
        model:faultReportListModel
        delegate:ReadonlyInfoDelegate{}
    }
    MultimediaView{
        id:faultReportMultimediaView
        interactionType:"showMultimedia"
        visible: false
    }
    Item{
        width: parent.width
        height: visible?5:0
        visible: faultReportListModel.count > 0
    }

    Repeater{
        model:faultAcceptListModel
        delegate:ReadonlyInfoDelegate{}
    }
    Item{
        width: parent.width
        height: visible?5:0
        visible: faultAcceptListModel.count > 0
    }
    Repeater{
        model:faultApproveListModel
        delegate:ReadonlyInfoDelegate{}
    }
    Item{
        width: parent.width
        height: visible?5:0
        visible: faultApproveListModel.count > 0
    }
    Repeater{
        model:faultDispatchListModel
        delegate:ReadonlyInfoDelegate{}
    }
    Item{
        width: parent.width
        height: visible?5:0
        visible: faultDispatchListModel.count > 0
    }
    Repeater{
        model:faultMaintainListModel
        delegate:ReadonlyInfoDelegate{}
    }
    Rectangle{
        id:partRect
        width: parent.width
        height: 40
        visible: faultMaintainListModel.count > 0
        Text{
            id:partText
            anchors.left: parent.left
            anchors.leftMargin: 40
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: UI.FONT_SIZE_16
            color:"#999999"
            text:"零部件信息:"
        }
        Rectangle{
            anchors.left: partText.right
            anchors.leftMargin: 20
            border.color: "gray"
            border.width: 2
            radius: 5
            width: clickCheckPartText.width + 20
            height: 30
            anchors.verticalCenter: parent.verticalCenter
            Text{
                id:clickCheckPartText
                anchors.centerIn: parent
                font.pixelSize: UI.FONT_SIZE_16
                text:"点击查看"
            }
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    console.log("part check  click");
                    getPartsInfo();
                }
            }
        }

        CLine {
            width:parent.width
        }
    }

    Item{
        width: parent.width
        height: visible?5:0
        visible: faultMaintainListModel.count > 0
    }
    Repeater{
        model:faultCostListModel
        delegate:ReadonlyInfoDelegate{}
    }
    MultimediaView{
        id:faultCostMultimediaView
        interactionType:"showMultimedia"
        visible: false
    }
    Item{
        width: parent.width
        height: visible?5:0
        visible: faultCostListModel.count > 0
    }
    Repeater{
        model:faultAppraiseListModel
        delegate:ReadonlyInfoDelegate{}
    }
    Item{
        width: parent.width
        height: visible?5:0
        visible: faultAppraiseListModel.count > 0
    }

    function initializeData(){
        var data = deviceOperationDetail.originalData["tables"][0]["value"][0];
        var status = deviceOperationDetail.otherParam["Taskstatus"];
        var permission = deviceOperationDetail.originalData["tables"][0]["value"][0]["UserOrderPermission"];
        var funcArray = [];
        funcArray.push(initializeReportData);
        funcArray.push(initializeAcceptData);
        funcArray.push(initializeApproveData);
        funcArray.push(initializeDispatchData);
        funcArray.push(initializeMaintainData);
        funcArray.push(initializeCostData);
        //显示故障申报的详情
        funcArray[0](data);
        if(status != "1"){
            funcArray[1](data);
            if(permission != ""){
                var userPermission = String(permission).split(",");
                userPermission.sort();
                for(var i = 0; i < userPermission.length; i++){
                    console.log("userPermission   ", userPermission[i])
                    if(status > userPermission[i]){
                        funcArray[Number(userPermission[i])](data);
                    } else
                        break;
                }
            }
        }
        var reportMultimediaData = deviceOperationDetail.originalData["tables"][1]["value"];
        //<Field Value="IDやNameやTitleやUrlやSerialKeyやMIMETypeやDescript"/>
        var reportAttchs = [];
        for(var i = 0; i < reportMultimediaData.length; i++){
            var reportAttch = {};
            reportAttch["Url"] = reportMultimediaData[i]["Url"];
            reportAttch["Path"] = "";
            reportAttch["Title"] = reportMultimediaData[i]["Title"];
            reportAttch["Description"] = reportMultimediaData[i]["Descript"];
            reportAttch["DataIsOk"] = false;
            var reportMimiType = reportMultimediaData[i]["MIMEType"];
            if(String(reportMimiType).indexOf("image/") != -1)
                reportAttch["MultimediaType"] = "image";
            else if(String(reportMimiType).indexOf("video/") != -1)
                reportAttch["MultimediaType"] = "video";
            else if(String(reportMimiType).indexOf("audio/") != -1)
                reportAttch["MultimediaType"] = "audio";
            console.log("faultrepair .setMultimediaData  Url    ", reportMultimediaData[i]["Url"], "  dataIsOk ", reportAttch["DataIsOk"] );
            reportAttchs.push(reportAttch);
        }
        if(reportAttchs.length > 0){
            faultReportMultimediaView.visible = true;
            faultReportMultimediaView.setMultimediaData(reportAttchs);
        }

        var costMultimediaData = deviceOperationDetail.originalData["tables"][2]["value"];
        //<Field Value="IDやNameやTitleやUrlやSerialKeyやMIMETypeやDescript"/>
        var costAttchs = [];
        for(var j = 0; j < costMultimediaData.length; j++){
            var costAttch = {};
            costAttch["Url"] = costMultimediaData[j]["Url"];
            costAttch["Path"] = "";
            costAttch["Title"] = costMultimediaData[j]["Title"];
            costAttch["Description"] = costMultimediaData[j]["Descript"];
            costAttch["DataIsOk"] = false;
            var cosntMimiType = costMultimediaData[j]["MIMEType"];
            if(String(cosntMimiType).indexOf("image/") != -1)
                costAttch["MultimediaType"] = "image";
            else if(String(cosntMimiType).indexOf("video/") != -1)
                costAttch["MultimediaType"] = "video";
            else if(String(cosntMimiType).indexOf("audio/") != -1)
                costAttch["MultimediaType"] = "audio";
            console.log("faultrepair .setMultimediaData  Url    ", costMultimediaData[j]["Url"], "  dataIsOk ", costAttch["DataIsOk"] );
            costAttchs.push(costAttch);
        }
        if(costAttchs.length > 0){
            faultCostMultimediaView.visible = true;
            faultCostMultimediaView.setMultimediaData(costAttchs);
        }
    }

    function initializeReportData(data){
        console.log("initializeReportData ,,,,,,,,,,")
        faultReportListModel.clear()
        var value;
        for(var i = 0; i< arrayFaultReportDetailModel.length; i++){
            if(arrayFaultReportDetailModel[i][1] == "FalutType"){
                var type = data["FalutType"];
                value = faultTypeMap[String(type)];
            } else
                value = data[arrayFaultReportDetailModel[i][1]];

            faultReportListModel.append({
                                            "name":arrayFaultReportDetailModel[i][0],
                                            "itemId":arrayFaultReportDetailModel[i][1],
                                            "value": value,
                                            "type" : "report",
                                            "enableEdit":false
                                        });
        }
    }

    function initializeAcceptData(data){
            for(var i = 0; i< arrayFaultAcceptDetailModel.length; i++)
                faultAcceptListModel.append({
                                                  "name":arrayFaultAcceptDetailModel[i][0],
                                                  "itemId":arrayFaultAcceptDetailModel[i][1],
                                                  "value":data[arrayFaultAcceptDetailModel[i][1]],
                                                  "type" : "accept",
                                                  "enableEdit":false
                                              });
    }
    function initializeApproveData(data){
            for(var i = 0; i< arrayFaultApproveDetailModel.length; i++)
                faultApproveListModel.append({
                                                  "name":arrayFaultApproveDetailModel[i][0],
                                                  "itemId":arrayFaultApproveDetailModel[i][1],
                                                  "value":data[arrayFaultApproveDetailModel[i][1]],
                                                  "type" : "approve",
                                                  "enableEdit":false
                                              });
    }
    function initializeDispatchData(data){
        for(var i = 0; i< arrayFaultDispatchDetailModel.length; i++)
            faultDispatchListModel.append({
                                                "name":arrayFaultDispatchDetailModel[i][0],
                                                "itemId":arrayFaultDispatchDetailModel[i][1],
                                                "value":data[arrayFaultDispatchDetailModel[i][1]],
                                                "type" : "dispatch",
                                                "enableEdit": false
                                            });
    }

    function initializeMaintainData(data){
        for(var i = 0; i< arrayFaultMaintainDetailModel.length; i++){
            var value ;
            if(i == 4 || i == 5){
                var type = i - 3;
                var index = data[arrayFaultMaintainDetailModel[i][1]];
                value = typetoString(type, index);
            } else
                value = data[arrayFaultMaintainDetailModel[i][1] ];

            faultMaintainListModel.append({
                                                    "name":arrayFaultMaintainDetailModel[i][0],
                                                    "itemId":arrayFaultMaintainDetailModel[i][1],
                                                    "value":value,
                                                    "type" : "maintain",
                                                    "enableEdit":false
                                                });
        }
    }
    function initializeCostData(data){
        for(var i = 0;i< arrayFaultCostDetailModel.length;i++){
            var value = data[arrayFaultCostDetailModel[i][1]];
            faultCostListModel.append({
                                          "name":arrayFaultCostDetailModel[i][0],
                                          "itemId":arrayFaultCostDetailModel[i][1],
                                          "value":value,
                                          "type" : "fee",
                                          "enableEdit":false
                                      });
        }
    }
    function initializeAppraiseData(data){
        for(var i = 0; i< arrayFaultAppraiseDetailModel.length; i++)
            faultAppraiseListModel.append({
                                                "name":arrayFaultAppraiseDetailModel[i][0],
                                                "itemId":arrayFaultAppraiseDetailModel[i][1],
                                                "value":data[arrayFaultAppraiseDetailModel[i][1]],
                                                "type" : "appraise",
                                                "enableEdit": false
                                            });
    }

    function typetoString(type,index){
        var string ;
        if(type == 1){
            if(index == 1)
                string = "上门维修"
            else if(index == 2)
                string = "送厂维修"
        }else if(type == 2){
            if(index == 0)
                string = "未修复"
            if(index == 1)
                string = "已修复"
        }
        return string;
    }

    function getPartsInfo(){
        transmitManager.resultOfResponse.connect(faultRepairDetailList.requestResponse)
        var head = {"Objects":"EMF_FaultRepair_Query_Parts"};
        var para = {"ID" : deviceOperationDetail.otherParam["TaskID"]};
        console.log("transmitManager.requestData   EMF_FaultRepair_Query_Parts ")
        faultRepairDetailList.actionSequence = inspectionSystemConfig.getSequence();
        transmitManager.requestData(head, para, faultRepairDetailList.actionSequence, "获取维修零部件列表失败");
    }

    function requestResponse(result, error, action, map){
        if(action  != faultRepairDetailList.actionSequence)
            return;
        transmitManager.resultOfResponse.disconnect(faultRepairDetailList.requestResponse)
        if(result && error == 0){
            if(map["tables"][0]["value"].length <= 0){
                CTOAST.requestToast("没有相关零部件信息","","");
            } else {
                var otherParam = {"TaskID" : deviceOperationDetail.otherParam["TaskID"]};
                rootWindow.pageStack.push("qrc:/qml/DeviceOperationTask/CheckComponentInfo.qml",
                                          {"originalData":map,
                                              "otherParam": otherParam});
            }
        }
    }

    Component.onCompleted: {
        initializeData();
    }
}
