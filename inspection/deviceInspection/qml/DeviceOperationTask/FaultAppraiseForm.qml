import QtQuick 2.0
import com.syberos.basewidgets 2.0
import "../Common"
import "../Common/constants.js" as UI
Column{
    id :appriseForm
    width: 480
    property string actionSequence: "-1"
    property var appriseresulteName:["非常满意","满意","基本满意","一般","较差","很差"]
    property var appriseresulteId:["1","2","3","4","5","6"]

        ListItem{
            id:appriseresult
            mainTitle: "评价结果"
            type:"category"
            endImage: "qrc:/icon/icon_08.png"
            endTitle: appriseresulteName[0]
            itemId: appriseresulteId[0]
            onClicked: {
                dropDownListChDlg.show()
            }
        }
        CListDialog{
            id: dropDownListChDlg
            buttonAreaEnabled: false
            maxVisibleItems:4
            titleText:"评价结果"
            model:appriseresulteName
            onDelegateItemTriggered:{
                setSelectedItem(index)
                dropDownListChDlg.hide();
            }
            function setSelectedItem(selectedIndex) {
                dropDownListChDlg.select([selectedIndex], true);
                appriseresult.endTitle = appriseresulteName[selectedIndex];
                appriseresult.itemId = appriseresulteId[selectedIndex];
            }
        }

        Rectangle{
            width: parent.width
            height: 100
            border.color: "#e8e8e8"
            CommonTextArea{
                id:textarea
                height:100
                font.pixelSize:20
                width:parent.width - 40
                maximumLength:300
                anchors.top: parent.top
                anchors.topMargin: 2
                Text {
                    id:placeHoldText
                    visible: parent.text == ""
                    color: "#dddddd"
                    font.pixelSize:UI.FONT_SIZE_18
                    text:"请添加评价反馈意见:"
                    anchors.top: parent.top
                    anchors.topMargin: 2
                    onTextChanged: {
                    }
                }
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }
        Item{
            width: parent.width
            height: 20
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

        Connections{
            target: deviceOperationDetail
            onAccepted:{
                console.log("deviceOperationDetail  accepted")
                falutRepairApprisecommit();
            }
        }

    function falutRepairApprisecommit(){
        var dataRow ={}
        dataRow["ID"] = deviceOperationDetail.otherParam["TaskID"];
        dataRow["AppraiseUser"] = inspectionSystemParam.getUserId()
        dataRow["AppraiseResult"] = appriseresult.itemId
        dataRow["AppraiseExplain"] = textarea.text

        var head = {"Objects" : "EMF_FaultRepair_Appraise_By_Device_Import"};
        var table = {"Name":"EMF_FaultRepair", "Key":"ID"};
        var attachments = [];
        var dataRowList = [];
        dataRowList.push(dataRow);
        appriseForm.actionSequence = inspectionSystemConfig.getSequence();
        transmitManager.resultOfResponse.disconnect(resultResponse);
        transmitManager.resultOfResponse.connect(resultResponse);
        transmitManager.submitData(head, table, dataRowList, attachments, "", appriseForm.actionSequence, "评价信息提交失败");
    }
    function resultResponse(result, error, action, map){
        if(action != appriseForm.actionSequence)
            return;
        transmitManager.resultOfResponse.disconnect(resultResponse)
        if(result == true && Number(error) == 0) {
            CTOAST.requestToast("提交成功", "", "");
            pageStack.pop();
        }
    }
}
