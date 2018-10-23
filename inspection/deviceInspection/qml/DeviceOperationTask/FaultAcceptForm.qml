import QtQuick 2.0
import com.syberos.basewidgets 2.0
import "../Common"
//运维受理页面
Column{
    id:faultAcceptDetail
    width: parent.width
    property bool enableEdit: false
    property string actionSequence: "-1"
    property var arrayFaultAcceptDetailModel:
        [ ["受理单位：","HandleOrganiseName"],["受理人：","HandlerName"],["受理时间：","HandleDate"] ]
    property var processNameList: []
    property var processIDList: []
    ListItem{
        id:handleOrganise
        mainTitle: "受理单位:"
        endImage: ""
        endTitle: inspectionSystemParam.getUserPropertyByKey("OrganiseUnitNames")
    }
    ListItem{
        id:handlerName
        mainTitle: "受理人:"
        endImage: ""
        endTitle: inspectionSystemParam.getUserPropertyByKey("UserName");
    }

    ListItem{
        id:processItem
        mainTitle: "维修流程"
        type:"category"
        endImage: "qrc:/icon/icon_08.png"
        onClicked: {
            dropDownListChDlg1.show();
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
            text: "受理意见："
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
            textColor:"black"
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
            ensureSubmitDlg.show();
        }
    }

    Connections{
        target: deviceOperationDetail
        onAccepted:{
            console.log("deviceOperationDetail  accepted")
            faultAcceptDetail.submitData();
        }
    }

    Item{
        height: 20
        width: parent.width
    }

    CListDialog{
        id: dropDownListChDlg1
        buttonAreaEnabled: false
        maxVisibleItems:4
        titleText :"维修流程"
        onDelegateItemTriggered:{
            setSelectedItem(index)
            dropDownListChDlg1.hide();
        }
        function setSelectedItem(selectedIndex) {
            dropDownListChDlg1.select([selectedIndex], true);
            processItem.endTitle = processNameList[selectedIndex];
            processItem.itemId = processIDList[selectedIndex];
        }
    }

    function submitData(){
        var dataRow = {};
        dataRow["UserID"] = inspectionSystemParam.getUserId();
        dataRow["HandleExplain"] = noteArea.text;
        dataRow["ID"] = deviceOperationDetail.otherParam["TaskID"];
        dataRow["FaultFlowID"] = processItem.itemId;
        dataRow["HandleDate"] = rootWindow.getCurDate();
        faultAcceptDetail.actionSequence = inspectionSystemConfig.getSequence();
        transmitManager.resultOfResponse.disconnect(faultAcceptDetail.resultResponse);
        transmitManager.resultOfResponse.connect(faultAcceptDetail.resultResponse);

        var head = {"Objects" : "EMF_FaultRepair_Handle_By_Device_Import"};
        var table = {"Name":"EMF_FaultRepair", "Key":"ID"};
        var attachments = [];
        var dataRowList = [];
        dataRowList.push(dataRow);
        transmitManager.submitData(head, table, dataRowList, attachments, "", faultAcceptDetail.actionSequence, "受理信息提交失败");
    }

    function resultResponse(result, error, action, map) {
        if(action != faultAcceptDetail.actionSequence){
            console.log("action != faultAcceptDetail.actionSequence return")
            return;
        }
        transmitManager.resultOfResponse.disconnect(faultAcceptDetail.resultResponse)
        if(result == true && Number(error) == 0) {
            CTOAST.requestToast("受理提交成功", "", "");
            pageStack.pop();
        }
    }


    function initializeData(){
        processNameList.length = 0;
        processIDList.length = 0;
        var data = deviceOperationDetail.originalData["tables"][3]["value"][0];
            var flowID = String(data["FlowID"]).split(",");
            var flowName = String(data["FlowName"]).split(",");
            for(var j = 0; j < flowID.length; j++){
                processNameList.push(flowName[j]);
                processIDList.push(flowID[j])
            }
            processItem.endTitle = flowName[0];
            processItem.itemId = flowID[0];
            dropDownListChDlg1.model = processNameList;
            dropDownListChDlg1.select([0], true);
    }
    Component.onCompleted: {
        initializeData();
    }
}
