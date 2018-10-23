import QtQuick 2.0
import com.syberos.basewidgets 2.0
import "../Common"
import "../Common/constants.js" as UI
import "../DateTime"

Column{
    id:falutrepair
    width: parent.width
    property string actionSequence:"-1"
    property var repairTypeName: ["上门维修", "送厂维修"]
    property var repairTypeId: ["1", "2"]
    property var repairResultName: ["未维修", "已维修"]
    property var repairResultId: ["0", "1"]
    property var partsInfoList: []
    property string serverRepaieDate: ""
    property string serverRepairType: ""
    property string serverRepairResult: ""
    property string serverRepairDescription: ""
    /*0:保存;1:提交*/
    property string resultSaved: "0"

    //["维修登记单位:","RepairOrganiseName"],
    //["维修登记人:","RepairRegisterUser"],
    //["登记时间:","RepairRegisterDate"],
    //["实际维修时间:","RepairDate"],
    //["维修方式:","RepairType"],
    //["维修结果:","RepairResult"],
    //["维修情况:", "RepairDescription"]  ]
    //deviceOperationDetail.originalData
    Component.onCompleted: {
        var repairValue = deviceOperationDetail.originalData["tables"][0]["value"][0];
        serverRepaieDate = repairValue["RepairDate"];
        serverRepairDescription = repairValue["RepairDescription"];
        serverRepairResult = repairValue["RepairResult"];
        serverRepairType = repairValue["RepairType"];
        console.log("实际维修时间 ", repairValue["RepairDate"])
        console.log("维修方式 ", repairValue["RepairType"])
        console.log("维修结果 ", repairValue["RepairResult"])
        console.log("维修情况 ", repairValue["RepairDescription"])
        var orginalPartInfoList = deviceOperationDetail.originalData["tables"][4]["value"];
        var partCount = orginalPartInfoList.length;
        for(var i = 0 ; i < partCount; i++){
            var partInfo = {};
            partInfo["PartID"] =orginalPartInfoList[i]["ID"];
            partInfo["PartCode"] =orginalPartInfoList[i]["PartCode"];
            partInfo["PartName"] =orginalPartInfoList[i]["PartName"];
            partInfo["PartManufacturer"] =orginalPartInfoList[i]["PartManufacturer"];
            partInfo["PartStyle"] =orginalPartInfoList[i]["PartStyle"];
            partInfo["PartNumber"] =orginalPartInfoList[i]["PartNumber"];
            partInfo["PartUnit"] =orginalPartInfoList[i]["PartUnit"];
            partInfo["OperType"] = "2";
            falutrepair.partsInfoList.push(partInfo);
        }
    }

    CListDialog{
        id: dropDownListChDlg
        buttonAreaEnabled: false
        maxVisibleItems:4
        titleText :"维修方式"
        model : repairTypeName
        onDelegateItemTriggered:{
            setSelectedItem(index)
            dropDownListChDlg.hide();
        }

        function setSelectedItem(selectedIndex) {
            dropDownListChDlg.select([selectedIndex], true);
            repairtypedisplay.endTitle =  repairTypeName[selectedIndex];
            repairtypedisplay.itemId =  repairTypeId[selectedIndex];
        }
    }
    CListDialog{
        id: dropDownListChDlg1
        buttonAreaEnabled: false
        maxVisibleItems:4
        titleText :"维修结果"
        model :repairResultName
        onDelegateItemTriggered:{
            setSelectedItem(index)
            dropDownListChDlg1.hide();
        }
        function setSelectedItem(selectedIndex) {
            dropDownListChDlg1.select([selectedIndex], true);
            repairresultdisplay.endTitle = repairResultName[selectedIndex];
            repairresultdisplay.itemId = repairResultId[selectedIndex];
        }
    }

    ListModel{
        id:partModel
        ListElement{
            name: "添加零部件"
        }ListElement{
            name:"零部件信息"
        }
    }

    Component{
        id:partDelegate
        Rectangle{
            width: 200
            height: 60
            border.color: partTitlerow.selectedIndex == index ? "#3995ce" : "#dddddd"
            Image {
                id: partImg
                width: 40
                height: 40
                fillMode: Image.PreserveAspectFit
                source:  partTitlerow.selectedIndex == index ? "qrc:/icon/buttonaddparts-choice.png" : "qrc:/icon/buttonaddparts-normal.png"
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
            }
            Text{
                id:partName
                anchors.left: partImg.right
                anchors.leftMargin: 20
                text:name
                color:partTitlerow.selectedIndex == index ? "#3995ce" : "#dddddd"
                anchors.verticalCenter: parent.verticalCenter
            }
            MouseArea{
                anchors.fill: parent
                onReleased: {
                    partTitlerow.selectedIndex = index;
                    if(index ==0){
                        var addComPage = rootWindow.pageStack.push("qrc:/qml/Common/AddComponents.qml", {"actionType":"save", "pageType" : "faultReport"});
                        addComPage.result.connect(falutrepair.addPartResult);
                    }else if(index == 1){
                        console.log("falutrepair.partsInfoList  ", falutrepair.partsInfoList.length)
                        var modifyComPage = rootWindow.pageStack.push("qrc:/qml/Common/ComponentsInfo.qml",
                                                  {"partInfoList":falutrepair.partsInfoList, "pageType" : "faultReport"});
                        modifyComPage.result.connect(falutrepair.modifyPartResult);
                    }
                }
            }
        }
    }

    Rectangle{
        width: parent.width
        height: 80
        Row{
            id :partTitlerow
            height: parent.height
            anchors.left: parent.left
            anchors.leftMargin: 25
            anchors.top:parent.top
            anchors.topMargin: 10
            spacing: 30
            /*初始化时选中第一个*/
            property int selectedIndex: 0
            Repeater{
                id :repeater
                model: partModel
                delegate: partDelegate
            }
        }
    }
    ListItem{
        id:repairtypedisplay
        mainTitle: "维修方式"
        endImage: "qrc:/icon/icon_08.png"
        endTitle:serverRepairType == "" ? repairTypeName[0] : repairTypeName[Number(serverRepairType) - 1]
        itemId: serverRepairType == "" ? repairTypeId[0]: repairTypeId[Number(serverRepairType) - 1];
        onClicked: {
            console.log("onClicked  Number(itemId) - 1 ", Number(itemId) - 1);
            dropDownListChDlg.select([Number(itemId) - 1], true);
            dropDownListChDlg.show()
        }
    }
    ListItem{
        id:repairresultdisplay
        mainTitle: "维修结果"
        endTitle: serverRepairResult == "" ?  repairResultName[0] : repairResultName[Number(serverRepairResult)]
        itemId: serverRepairResult == "" ?  repairResultId[0] : repairResultId[Number(serverRepairResult)]
        endImage: "qrc:/icon/icon_08.png"
        onClicked: {
            console.log("onClicked  Number(itemId)  ", Number(itemId));
            dropDownListChDlg1.select([Number(itemId)], true);
            dropDownListChDlg1.show()
        }
    }
    ListItem{
        id:repairDate
        mainTitle: "实际维修时间:"
        endTitle: serverRepaieDate == "" ?  rootWindow.getCurDate() : serverRepaieDate
        endImage: "qrc:/icon/icon_08.png"
        onClicked: {
            deviceOperationDetail.dateTimeItem.accepted.connect(repairDate.setText)
            deviceOperationDetail.dateTimeItem.requestHide.connect(repairDate.disconnectSignal)
            if(repairDate.endTitle != ""){
                var date = new Date(repairDate.endTitle);
                deviceOperationDetail.dateTimeItem.setDateTime(date)
            }
            deviceOperationDetail.dateTimeItem.show()
        }
        function disconnectSignal(){
            dateTimeItem.accepted.disconnect(repairDate.setText)
            dateTimeItem.requestHide.disconnect(repairDate.disconnectSignal)
        }
        function setText(){
            repairDate.endTitle = dateTimeItem.dateTimeString()
        }
    }
    Rectangle{
        width: parent.width
        height: 120
        border.color: "gray"
        Text {
            id: maintainCondition
            anchors.left: parent.left
            anchors.leftMargin:20
            anchors.top:parent.top
            anchors.topMargin: 10
            font.pixelSize: 18
            color: "black"
            text: "维修情况:"
        }
        CommonTextArea{
            id:textarea
            height:parent.height - 20
            font.pixelSize:20
            anchors.left:maintainCondition.right
            anchors.right:parent.right
            anchors.leftMargin: 10
            anchors.rightMargin: 20
            anchors.verticalCenter: parent.verticalCenter
            maximumLength:300
            textColor: "black"
            text:serverRepairDescription
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
        width: parent.width
        height: 20
    }

    Item{
        height: 60
        anchors.left:parent.left
        anchors.right: parent.right
        anchors.leftMargin: 20
        anchors.rightMargin: 20
        Item{
            width:200
            anchors.left:parent.left
            CommonButton{
                id:saveBtn
                width:parent.width
                text: "保存"
                onClicked:{
                    rootWindow.closeKeyboard();
                    falutrepair.resultSaved = "0";
                    falutRepairregistercommit(true);
                }
            }
        }
        Item{
            width:200
            anchors.right: parent.right
            CommonButton{
                id:submmitBtn
                width:parent.width
                text: "提交"
                onClicked:{
                    rootWindow.closeKeyboard()
                    ensureSubmitDlg.type = "submit"
                    ensureSubmitDlg.show()
                }
            }
        }
    }

    Connections{
        target: deviceOperationDetail
        onAccepted:{
            console.log("deviceOperationDetail  accepted")
            falutrepair.resultSaved = "1";
            falutRepairregistercommit(false)
        }
    }

    function addPartResult(partInfoMap){
        console.log("addPartResult  ", partInfoMap["PartName"]);
        partInfoMap["OperType"] = "1";
        falutrepair.partsInfoList.push(partInfoMap)
    }

    function modifyPartResult(partInfosList){
        console.log("modifyPartResult   ", partInfosList.length)
        falutrepair.partsInfoList = partInfosList;
    }
    function getPartInfoList(){
        var tmpPartInfoList = [];
        for(var i = 0 ; i < falutrepair.partsInfoList.length; i++){
            if(falutrepair.partsInfoList[i]["OperType"] != "3"){
                falutrepair.partsInfoList[i]["FaultRepairID"] = deviceOperationDetail.otherParam["TaskID"];
                falutrepair.partsInfoList[i]["ModifiedBy"] = inspectionSystemParam.getUserId();
                tmpPartInfoList.push(falutrepair.partsInfoList[i])
            }
        }
        return tmpPartInfoList;
    }

//call EMF_FaultRepair_UpdateRepairByTerminal( @RepairRegisterUser, @RepairType, @RepairResult, @RepairDate, @RepairExplain, @ID, @IsSave)
//call EMF_FaultRepair_UpdatePart( @PartID, @FaultRepairID, @ModifiedBy, @PartCode, @PartName, @PartStyle,@PartUnit,@PartManufacturer,@PartNumber,
//@OperType)
//0 delete 1 save 2 modify

    function falutRepairregistercommit(isSaved){
        var dataRow ={}
        dataRow["ID"] = deviceOperationDetail.otherParam["TaskID"];
        dataRow["RepairRegisterUser"] = inspectionSystemParam.getUserId();
        dataRow["RepairType"] = repairtypedisplay.itemId;
        dataRow["RepairResult"] = repairresultdisplay.itemId;
        dataRow["RepairDate"] = repairDate.endTitle;
        dataRow["RepairExplain"] = textarea.text;
        dataRow["IsSave"] = isSaved ? "0" : "1"

        var head = {"Objects" : "EMF_FaultRepair_Repair_Part_Import"};
        var table = {"Name":"EMF_FaultRepair", "Key":"ID"};
        var attachments = [];
        var dataRowList = [];
        dataRowList.push(dataRow);

        var partTable = {};
        var partDataRow = getPartInfoList();
        if(partDataRow.length > 0){
            partTable = {"Name" : "emf_faultrepairparts", "Key" : "ID"};
        }

        falutrepair.actionSequence = inspectionSystemConfig.getSequence();
        transmitManager.resultOfResponse.connect(resultResponse)
        transmitManager.submitDataWithPart(head, table, dataRowList, attachments, partTable, partDataRow,  "", falutrepair.actionSequence, "维修登记信息提交失败");
    }

    function resultResponse(result, error, action, map){
        if(action != falutrepair.actionSequence){
            console.log("action != falutrepair.actionSequence  return")
            return;
        }
        transmitManager.resultOfResponse.disconnect(resultResponse)
        if(result == true && Number(error) == 0) {
            if(falutrepair.resultSaved == "0")
                CTOAST.requestToast("保存成功", "", "");
            else
                CTOAST.requestToast("提交成功", "", "");
            pageStack.pop();
        }
    }
}
