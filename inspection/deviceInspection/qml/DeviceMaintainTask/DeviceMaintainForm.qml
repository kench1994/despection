/*!
* \file DeviceMaintainForm.qml
* \brief 设备保养提交详情页面
*
*此界面是设备保养提交详情页面，用户在该页面进行操作，将填报数据提交服务器
*
*\author 桓乐乐 huanlele@syberos.com
*\version 1.0
*\date 2015/9/7
*/
import QtQuick 2.0
import com.syberos.basewidgets 2.0
import com.syberos.generateForm 1.0
import "../Common"
import "../Common/constants.js" as UI
import "../Multimedia"
/*!
  \class DeviceMaintainForm
  \inModule 设备保养
  \brief 设备保养提交详情页面
  \ingroup DeviceMaintainTask
  此界面是设备保养提交详情页面，用户在该页面进行操作，将填报数据提交服务器
*/
CommonPage {
    id: deviceMaintainPage

    /** type:string serialKey*/
    property string serialKey

    /** type:bool 从服务器获取的数据解析是否正确*/
    property bool dataError: false

    /** type:string 设备提交时提示文字*/
    property string pageMsgText: ""

    /** type:string 设备提交时提示标题*/
    property string pageMsgTitleText: ""

    /** type:string 信息提交失败时提示文字*/
    property string pageSubmitFaileText: ""

    /** type:bool 页面是否正在显示，主要用来出来back键*/
    property bool pageShow: false

    /** type:var 页面展示需要，但是没有包含在dataPackageValue数据中的数据*/
    property var paramValue

    /** type:var 从服务器获取的调拨详情数据*/
    property var dataPackageValue
    property string actionSequence: "-1"
    property var  partsInfoList: []
    objectName:"deviceInspectionFormPage"
    statusBarHoldEnabled: true
    statusBarHoldItemColor: "white"
    orientationPolicy: CPageOrientation.LockPortrait
    clip: true

    Component.onCompleted: {
        rootWindow.toHomeFlag = 1;
        deviceMaintainPage.serialKey = dataPackageValue["tables"][0]["value"][0]["SerialKey"];
        deviceMaintainPage.pageMsgTitleText = os.i18n.ctr(qsTr("Equipment maintain"));
        deviceMaintainPage.pageSubmitFaileText  = os.i18n.ctr(qsTr("Maintain submit fail"));
        console.log("deviceMaintainPage.pageMsgTitleText   ", deviceMaintainPage.pageMsgTitleText);
        tm.start()
    }

    Component.onDestruction: {
        rootWindow.toHomeFlag = 0;
    }

    Connections{
        target: rootWindow
        onKeyBackTriggered:{
            if(deviceMaintainPage.pageShow == true) {
                if(deviceMaintainPage.dataError == true) {
                    pageStack.pop();
                } else {
                    ensureSubmitDlg.type = "backToCancel";
                    deviceMaintainPage.pageMsgText = os.i18n.ctr(qsTr("Abandon equipment maintain"))
                    ensureSubmitDlg.show();
                }
            }
        }
        onToHome:{
            dropDownListChDlg.hide();
            ensureSubmitDlg.type = "homeToCancel";
            deviceMaintainPage.pageMsgText = os.i18n.ctr(qsTr("Abandon equipment maintain"))
            ensureSubmitDlg.show();
        }
    }

    onStatusChanged: {
        if (status == CPageStatus.Show) {
            deviceMaintainPage.pageShow = true;
            rootWindow.dialogShow = true;
            deviceMaintainPage.statusBarHoldEnabled = true
            gScreenInfo.setStatusBarStyle("white");
            gScreenInfo.setStatusBar(true)
        } else if (status == CPageStatus.Hide) {
            rootWindow.dialogShow = false;
        }else if (status == CPageStatus.WillHide) {
            deviceMaintainPage.pageShow = false;
            dropDownListChDlg.hide();
            ensureSubmitDlg.hide();
        }
    }

    GenerateForm{
        id:generateFm
        formType:"maintain"
        rootObjName: "deviceInspectionFormPage"
        contentColObjName: "deviceInspectionFormCol"
        choiceDialogItemObjName: "deviceInspectionFormChDlg"
        deviceValue:paramValue
        dataPackage:dataPackageValue
        onCreateComponentFinished:{
            submmitBtn.visible = true;
            if(deviceMaintainPage.paramValue["enableImage"] == "1") {
                faultImageList.visible=true
            }
        }
        onDataError: {
            deviceMaintainPage.dataError = true;
            CTOAST.requestToast(os.i18n.ctr(qsTr("error: ")) + error,"","");
        }
        onCheckLegalityErrorOccurred:{
            //const QString &label, const int &errorType
            // 0:合法  1:必填 2:运行时必填 3:输入不合法
            console.log("label  ", label, "  errorType  ", errorType);
            switch(Number(errorType)) {
            case 1:
                CTOAST.requestToast( label + " "  + os.i18n.ctr(qsTr("Must fill in")), "", "");
                break;
            case 2:
                CTOAST.requestToast( label + " "  + os.i18n.ctr(qsTr("When equipment is running, must fill in")), "", "");
                break;
            case 3:
                if(dataType == "IntType")
                    CTOAST.requestToast( label + " "  + os.i18n.ctr(qsTr("Input is not legal, please input integer")),
                                    "", "");
                else if(dataType == "DecimalType") {
                    var num = Number(precision);
                    var showStr = "0.";
                    while(num--)
                        showStr += "0";
                    CTOAST.requestToast( label + " "
                                        + os.i18n.ctr(qsTr("Input is not legal, please input floating number")
                                                      +  " (" + String(showStr) + ")"), "", "");
                }
                break;
            }
        }
    }

    contentAreaItem:  Rectangle {
        id: rect
        anchors.fill: parent
        color:"#eeeff1"
        Rectangle{
            id:pageHead
            width: parent.width
            height: pageHeadTitle.height
            CommonTitlebar{
                id: pageHeadTitle
                width: parent.width
                clip:true
                //visible:false
                leftItemEnabled: true
                titleItemEnabled: true
                rightItemEnabled: false
                leftItemPixelSize: 22
                titleAreaCentered:true
                leftItemTextColor: "#666666"
                leftItemText:os.i18n.ctr(qsTr("Cancel"))
                leftItemIcon:""
                titleText:deviceMaintainPage.pageMsgTitleText
                onLeftItemTriggered: {
                    if(deviceMaintainPage.dataError == true)
                        pageStack.pop();
                    else {
                        ensureSubmitDlg.type = "backToCancel";
                        deviceMaintainPage.pageMsgText = os.i18n.ctr(qsTr("Abandon equipment maintain"))
                        ensureSubmitDlg.show();
                    }
                }
                anchors.top: parent.top
                onLeftItemChanged:leftItem.backgroundEnabled = false
                leftItemBackgroundEnabled: false
                Component.onCompleted: {
                    pageHeadTitle.titleItem.horizontalAlignment = Text.AlignHCenter
                    pageHeadTitle.titleItem.width = 300
                }
            }
        }
        ListModel{
            id:titlemodel
            ListElement{
                name: "添加零部件"
            }ListElement{
                name:"零部件信息"
            }
        }

        Component{
            id:titledelegate
            Rectangle{
                width: 200
                height: 60
                border.color: partTitlerow.selectedIndex == index ? "#3995ce" : "#dddddd"
                Image {
                    id: titleimg
                    width: 40
                    height: 40
                    fillMode: Image.PreserveAspectFit
                    source:partTitlerow.selectedIndex == index ? "qrc:/icon/buttonaddparts-choice.png" : "qrc:/icon/buttonaddparts-normal.png"
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    anchors.verticalCenter: parent.verticalCenter
                }
                Text{
                    id:titlename
                    anchors.left: titleimg.right
                    anchors.leftMargin: 20
                    text:name
                    color: partTitlerow.selectedIndex == index ? "#3995ce" : "#dddddd"
                    anchors.verticalCenter: parent.verticalCenter
                }
                MouseArea{
                    anchors.fill: parent
                    onReleased: {
                        partTitlerow.selectedIndex = index;
                        if(index ==0){
                            var addComPage = rootWindow.pageStack.push("qrc:/qml/Common/AddComponents.qml", {"actionType":"save", "pageType" : "maintain"});
                            addComPage.result.connect(deviceMaintainPage.addPartResult);
                        } else if(index == 1){
                            console.log("falutrepair.partsInfoList  ", deviceMaintainPage.partsInfoList.length)
                            var modifyComPage = rootWindow.pageStack.push("qrc:/qml/Common/ComponentsInfo.qml",
                                                      {"partInfoList":deviceMaintainPage.partsInfoList, "pageType" : "maintain"});
                            modifyComPage.result.connect(deviceMaintainPage.modifyPartResult);
                        }
                    }
                }
            }
        }


        Flickable{
            id:detailInfo
            anchors.top: pageHead.bottom
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            flickDeceleration: Flickable.VerticalFlick
            clip:true
            contentHeight:totalCol.height
            Item{
                id:topToHeaderSeparatorItem
                width: parent.width;height: UI.GROUP_SEPARATOR_HIGHT
            }
            Column{
                id:totalCol
                anchors.top: topToHeaderSeparatorItem.bottom
                width: parent.width
                spacing: 20
                //<Field Value="TaskObjectDataKeyやObjectNameやObjectFullNameやItemSchemaやRealObjectTypeやRealObjectIDや
//SerialKeyやTaskDataKeyやTaskGroupDataKeyやRFIDCodeやBarCodeやRealObjectModifiedDateやCustodianやModelName"/>
                DeviceTitle{
                    id:deviceTitle
                    deviceName1: dataPackageValue["tables"][0]["value"][0]["ObjectName"]
                    deviceLabel2: "设备型号:"
                    deviceName2: dataPackageValue["tables"][0]["value"][0]["ModelName"]
                    deviceLabel3: "设备保管人:"
                    deviceName3: dataPackageValue["tables"][0]["value"][0]["Custodian"]
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
                            model:titlemodel
                            delegate: titledelegate
                        }
                    }
                }

                Column{
                    id:contentCol
                    width: parent.width
                    spacing: 5
                    objectName: "deviceInspectionFormCol"
                }

                MultimediaView{
                    id:faultImageList
                    parentPageId:deviceMaintainPage
                    visible: deviceMaintainPage.paramValue["EnableUploadAttach"] == "1"
                }

                CommonButton{
                    id:submmitBtn
                    text:os.i18n.ctr(qsTr("Submit"))
                    visible:false
                    onClicked: {
                            ensureSubmitDlg.type = "submit"
                            deviceMaintainPage.pageMsgText = os.i18n.ctr(qsTr("No change after the submit information, sure to submit?"))
                            ensureSubmitDlg.show();
                    }
                }
                Item{
                    width: parent.width
                    height: UI.GROUP_SEPARATOR_HIGHT*3
                }
            }
        }
    }

    CDialog{
        id:ensureSubmitDlg
        property string type: ""
        titleText : deviceMaintainPage.pageMsgTitleText
        messageText:deviceMaintainPage.pageMsgText
        acceptedButtonText: os.i18n.ctr(qsTr("Ok"))
        rejectButtonText: os.i18n.ctr(qsTr("Cancel"))
        onAccepted:{
            if(type == "submit") {
                var rv = generateFm.checkFormInputLegality();
                if(rv != 0){
                    console.log("输入不合法");
                    return;
                }
                var otherParamMap = {};
                otherParamMap["serialKey"] = deviceMaintainPage.serialKey;
                var dataRow = generateFm.getItemData(otherParamMap);
                var valueMapList = [];
                valueMapList.push(dataRow);

                deviceMaintainPage.actionSequence = inspectionSystemConfig.getSequence()
                transmitManager.resultOfResponse.disconnect(deviceMaintainPage.submitResultResponse)
                transmitManager.resultOfResponse.connect(deviceMaintainPage.submitResultResponse)

                var head = {"Objects" : "EMM_TaskObject_Parts_Import"};
                var table = {"Name":"EMM_MaintenanceTaskObject", "Key":"TaskObjectDataKey"};
                var attachments = faultImageList.getMultimediaData();

                var partTable = {};
                var partDataRow = getPartInfoList();
                if(partDataRow.length > 0){
                    partTable = {"Name" : "EMM_MaintenanceParts", "Key" : "PartID"};
                }

                console.log("EMM_TaskObject_Parts_Import   submitData");
                transmitManager.submitDataWithPart(head, table, valueMapList, attachments, partTable, partDataRow,
                                           deviceMaintainPage.serialKey, deviceMaintainPage.actionSequence, "保养任务提交失败");
            } else if(type == "backToCancel") {
                pageStack.pop();
            }else if(type == "homeToCancel"){
                rootWindow.toHomeBack();
            }
        }
    }

    CListDialog{
        id: dropDownListChDlg
        buttonAreaEnabled: false
        maxVisibleItems:4
        objectName:"deviceInspectionFormChDlg"
        onDelegateItemTriggered:{
            dropDownListChDlg.hide();
        }

        function setSelectedItem(selectedIndex) {
            console.log("dropDownListChDlg   setSelectedItem   ", selectedIndex);
            dropDownListChDlg.select([selectedIndex], true);
        }
    }

    Timer{
        id:tm
        interval: 350
        repeat: false
        running: false
        onTriggered: {
            generateFm.initializeForm();
        }
    }

    function addPartResult(partInfoMap){
        console.log("addPartResult  ", partInfoMap["PartName"]);
        partInfoMap["OperType"] = "1";
        deviceMaintainPage.partsInfoList.push(partInfoMap)
    }

    function modifyPartResult(partInfosList){
        console.log("modifyPartResult   ", partInfosList.length)
        deviceMaintainPage.partsInfoList = partInfosList;
    }
//PartID,PartCode,PartName,PartManufacturer,PartStyle,ModifiedBy,PartCount,TaskObjectDataKey,CreatedBy
    function getPartInfoList(){
        var tmpPartInfoList = [];
        for(var i = 0 ; i < deviceMaintainPage.partsInfoList.length; i++){
            if(deviceMaintainPage.partsInfoList[i]["OperType"] != "3"){
                var partInfo = deviceMaintainPage.partsInfoList[i];
                partInfo["PartCount"] = partInfo["PartNumber"];
                delete partInfo["PartNumber"] ;
                partInfo["TaskObjectDataKey"] = paramValue["TaskObjectDataKey"];
                partInfo["ModifiedBy"] = inspectionSystemParam.getUserId();
                partInfo["CreatedBy"] = inspectionSystemParam.getUserId();
                tmpPartInfoList.push(partInfo)
            }
        }
        return tmpPartInfoList;
    }

    function submitResultResponse(result, error, action, map){
        console.debug("DeviceMaintainForm-onResultOfResponse---result, error, action: ", result, error, action);
        if(action != deviceMaintainPage.actionSequence){
            console.log("action != DeviceMaintainForm.actionSequence  return")
            return;
        }
        transmitManager.resultOfResponse.disconnect(deviceMaintainPage.submitResultResponse);
        if(Number(error) != -8){
            if(Number(error) == 0)
                CTOAST.requestToast("提交成功", "", "");
            pageStack.pop();
        }
    }
}
