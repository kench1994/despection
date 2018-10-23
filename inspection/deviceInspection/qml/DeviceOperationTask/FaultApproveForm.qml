import QtQuick 2.0
import com.syberos.basewidgets 2.0
import "../Common"
Column{
    id:faultApproveForm
    property string actionSequence: "-1"
    width: parent.width

    ListItem{
        id:auditOrganise
        mainTitle: "审核单位:"
        endImage: ""
        endTitle: inspectionSystemParam.getUserPropertyByKey("OrganiseUnitNames")
    }
    ListItem{
        id:auditUserName
        mainTitle: "确认人:"
        endImage: ""
        endTitle: inspectionSystemParam.getUserPropertyByKey("UserName");
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
            text: "审批意见："
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
            textColor: "black"
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
            ensureSubmitDlg.type = "submit"
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
            faultApproveForm.submitData();
        }
    }

    function submitData(){
        console.log("-----------------------------------------------")
        var dataRow = {};
        dataRow["AuditRegisterUser"] = inspectionSystemParam.getUserId();
        dataRow["AuditExplain"] = noteArea.text;
        dataRow["ID"] = deviceOperationDetail.otherParam["TaskID"];
        dataRow["AuditDate"] = rootWindow.getCurDate();
        faultApproveForm.actionSequence = inspectionSystemConfig.getSequence();
        transmitManager.resultOfResponse.connect(faultApproveForm.resultResponse);
        var head = {"Objects" : "EMF_FaultRepair_Audit_By_Device_Import"};
        var table = {"Name":"EMF_FaultRepair", "Key":"ID"};
        var attachments = [];
        var dataRowList = [];
        dataRowList.push(dataRow);
        transmitManager.submitData(head, table, dataRowList, attachments, "", faultApproveForm.actionSequence, "审批信息提交失败");
    }


    function resultResponse(result, error, action, map){
        if(action != faultApproveForm.actionSequence){
            console.log("action != faultApproveForm.actionSequence return")
            return;
        }
        transmitManager.resultOfResponse.disconnect(faultApproveForm.resultResponse)
        if(result == true && Number(error) == 0) {
            CTOAST.requestToast("审批提交成功", "", "");
            pageStack.pop();
        }
    }
}
