import QtQuick 2.0
import com.syberos.basewidgets 2.0
import "../Common"
import "../Multimedia"
Column{
    id:faultCostForm
    property string actionSequence:"-1"
    width: parent.width
//           [["费用登记单位：","CostRecordCompanyName"],["费用登记人：","CostRecordUser"],
//           ["费用登记时间:","CostRecordDate"],["配件费用(单位:元):","PartsCost"],
//           ["人工费用(单位:元):","LaborCost"],
//           ["其他费用(元):","OtherCost"],["总费用(元):",""] ]


    ListItem{
        id:dispatchOrganiseName
        mainTitle: "费用登记单位:"
        endImage: ""
        endTitle: inspectionSystemParam.getUserPropertyByKey("OrganiseUnitNames")
    }
    ListItem{
        id:dispatcherName
        mainTitle: "费用登记人:"
        endImage: ""
        endTitle: inspectionSystemParam.getUserPropertyByKey("UserName");
    }
    ListInputItem{
        id:partsCost
        inputMethodhints:Qt.ImhHiddenText|Qt.ImhFormattedNumbersOnly
        mainTitle: "配件费用(元):"
    }
    Item{
        width: parent.width
        height: 2
    }

    ListInputItem{
        id:laborCost
        inputMethodhints:Qt.ImhHiddenText|Qt.ImhFormattedNumbersOnly
        mainTitle: "人工费用(元):"
    }
    Item{
        width: parent.width
        height: 2
    }
    ListInputItem{
        id:otherCost
        inputMethodhints:Qt.ImhHiddenText|Qt.ImhFormattedNumbersOnly
        mainTitle: "其他费用(元):"
    }
    Item{
        width: parent.width
        height: 2
    }
    ListItem{
        id:totalCost
        mainTitle: "总费用(元):"
        type:""
        endTitle: Number(partsCost.inputText) + Number(laborCost.inputText) + Number(otherCost.inputText)
    }
    ListItem{
        id:costRecordDate
        mainTitle: "费用登记时间:"
        endTitle: rootWindow.getCurDate();
        onClicked: {
            deviceOperationDetail.dateTimeItem.accepted.connect(costRecordDate.setText)
            deviceOperationDetail.dateTimeItem.requestHide.connect(costRecordDate.disconnectSignal)
            if(costRecordDate.endTitle != ""){
                var date = new Date(costRecordDate.endTitle);
                deviceOperationDetail.dateTimeItem.setDateTime(date)
            }
            deviceOperationDetail.dateTimeItem.show()
        }
        function disconnectSignal(){
            dateTimeItem.accepted.disconnect(costRecordDate.setText)
            dateTimeItem.requestHide.disconnect(costRecordDate.disconnectSignal)
        }
        function setText(){
            costRecordDate.endTitle = dateTimeItem.dateTimeString()
        }
    }
    MultimediaView{
        id:faultImageList
        parentPageId: deviceOperationDetail
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
            text: "备注:"
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
                text: "请填写备注"
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
            faultCostForm.submitData()
        }
    }

    function submitData(){
        var dataRow = {};
        var serialKey = inspectionSystemConfig.getUUID();
        dataRow["ID"] = deviceOperationDetail.otherParam["TaskID"];
        dataRow["PartsCost"] = partsCost.inputText;
        dataRow["LaborCost"]= laborCost.inputText;
        dataRow["OtherCost"] = otherCost.inputText;
        dataRow["CostRemark"] = noteArea.text;
        dataRow["CostRecordUser"] = inspectionSystemParam.getUserId();
        dataRow["CostRecordCompanyID"] = inspectionSystemParam.getOrganiseUnitIDs();
        dataRow["CostRecordDate"] = rootWindow.getCurDate();
        dataRow["CostSerialKey"] = serialKey;

        faultCostForm.actionSequence = inspectionSystemConfig.getSequence();
        transmitManager.resultOfResponse.connect(faultCostForm.resultResponse);

        var head = {"Objects" : "EMF_FaultRepair_Cost_By_Device_Import"};
        var table = {"Name":"EMF_FaultRepair", "Key":"ID"};
        var attachments = faultImageList.getMultimediaData();
        var dataRowList = [];
        dataRowList.push(dataRow);
        transmitManager.submitData(head, table, dataRowList, attachments, serialKey, faultCostForm.actionSequence, "费用信息提交失败");
    }

    function resultResponse(result, error, action, map){
        if(action != faultCostForm.actionSequence){
            console.log("action != faultCostForm.actionSequence return")
            return;
        }
        transmitManager.resultOfResponse.disconnect(faultCostForm.resultResponse)
        if(result == true && Number(error) == 0) {
            CTOAST.requestToast("费用提交成功", "", "");
            pageStack.pop();
        }
    }
    Component.onDestruction: {
        transmitManager.resultOfResponse.disconnect(faultCostForm.resultResponse);
    }
}
