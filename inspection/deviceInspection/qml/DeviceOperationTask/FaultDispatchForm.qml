import QtQuick 2.0
import com.syberos.basewidgets 2.0
import "../Common"
Column{
    id:faultDispatchDetail
    property string actionSequence:"-1"
    width: parent.width

    ListItem{
        id:dispatchOrganiseName
        mainTitle: "派工单位:"
        endImage: ""
        endTitle: inspectionSystemParam.getUserPropertyByKey("OrganiseUnitNames")
    }
    ListItem{
        id:dispatcherName
        mainTitle: "派工人:"
        endImage: ""
        endTitle: inspectionSystemParam.getUserPropertyByKey("UserName");
    }
    ListInputItem{
        id:repairCompany
        mainTitle: "维修单位:"
    }
    Item{
        width: parent.width
        height: 2
    }

    ListInputItem{
        id:repairPerson
        mainTitle: "维修人:"
    }
    Item{
        width: parent.width
        height: 2
    }
    ListInputItem{
        id:repairphone
        mainTitle: "维修联系电话:"
    }
    ListItem{
        id:planRepairDate
        mainTitle: "计划维修时间:"
        endTitle: rootWindow.getCurDate();
        onClicked: {
            deviceOperationDetail.dateTimeItem.accepted.connect(planRepairDate.setText)
            deviceOperationDetail.dateTimeItem.requestHide.connect(planRepairDate.disconnectSignal)
            if(planRepairDate.endTitle != ""){
                var date = new Date(planRepairDate.endTitle);
                deviceOperationDetail.dateTimeItem.setDateTime(date)
            }
            deviceOperationDetail.dateTimeItem.show()
        }
        function disconnectSignal(){
            dateTimeItem.accepted.disconnect(planRepairDate.setText)
            dateTimeItem.requestHide.disconnect(planRepairDate.disconnectSignal)
        }
        function setText(){
            planRepairDate.endTitle = dateTimeItem.dateTimeString()
        }
    }

    Rectangle{
        width: parent.width
        height: 120
        border.color: "gray"
        Text {
            id: instruction
            anchors.left: parent.left
            anchors.leftMargin:20
            anchors.top:parent.top
            anchors.topMargin: 10
            font.pixelSize: 18
            text: "派工意见："
        }
        CommonTextArea{
            id:noteArea
            height:parent.height - 20
            font.pixelSize:20
            anchors.left:instruction.right
            anchors.right:parent.right
            anchors.leftMargin: 10
            anchors.rightMargin: 20
            anchors.verticalCenter: parent.verticalCenter
            maximumLength:300
            Text {
                id:placeHoldText
                visible: parent.text == ""
                color: "#dddddd"
                font.pixelSize:parent.font.pixelSize
                text: "请填写意见"
            }
        }
    }

    Item{
        height: 20
        width: parent.width
    }

    CommonButton{
        id:submmitBtn
        text: "提交"
        onClicked:{
            rootWindow.closeKeyboard()
            ensureSubmitDlg.type = "submit";
            ensureSubmitDlg.show()
        }
    }

    Item{
        height: 20
        width: parent.width
    }

    Connections{
        target: deviceOperationDetail
        onAccepted:{
            console.log("deviceOperationDetail  accepted")
            faultDispatchDetail.submitData();
        }
    }

    function submitData(){
        console.log("-----------------------------------------------")
        var dataRow = {};
        dataRow["DisPatchUser"] = inspectionSystemParam.getUserId();
        dataRow["UserID"]= inspectionSystemParam.getUserId();
        dataRow["DisPatchExplain"] = noteArea.text;
        dataRow["ID"] = deviceOperationDetail.otherParam["TaskID"];
        dataRow["PlanRepairDate"] = planRepairDate.endTitle
        dataRow["RepairPerson"] = repairPerson.inputText;
        dataRow["Telephone"] = repairphone.inputText;
        dataRow["RepairCompany"] = repairCompany.inputText;
        faultDispatchDetail.actionSequence = inspectionSystemConfig.getSequence();
        transmitManager.resultOfResponse.connect(faultDispatchDetail.resultResponse);

        var head = {"Objects" : "EMF_FaultRepair_Dispatch_By_Device_Import"};
        var table = {"Name":"EMF_FaultRepair", "Key":"ID"};
        var attachments = [];
        var dataRowList = [];
        dataRowList.push(dataRow);
        transmitManager.submitData(head, table, dataRowList, attachments, "", faultDispatchDetail.actionSequence, "派工信息提交失败");
    }

    function resultResponse(result, error, action, map){
        if(action != faultDispatchDetail.actionSequence){
            console.log("action != faultDispatchDetail.actionSequence return")
            return;
        }
        transmitManager.resultOfResponse.disconnect(faultDispatchDetail.resultResponse)
        if(result == true && Number(error) == 0) {
            CTOAST.requestToast("派工提交成功", "", "");
            pageStack.pop();
        }
    }
    Component.onDestruction: {
        transmitManager.resultOfResponse.disconnect(faultDispatchDetail.resultResponse);
    }
}
