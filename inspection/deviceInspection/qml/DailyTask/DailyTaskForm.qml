/*!
* \file DeviceInspectionForm.qml
* \brief 设备巡检提交详情页面
*
*此界面是设备巡检提交详情页面，用户在该页面进行操作，将填报数据提交服务器
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
  \class DeviceInspectionForm
  \inModule 设备巡检
  \brief 设备巡检提交详情页面
  \ingroup InspectionTask
  此界面是设备巡检提交详情页面，用户在该页面进行操作，将填报数据提交服务器
*/
CommonPage {
    id: deviceInspectionPage
    /** type:string serialKey*/
      property string serialKey
    /** type:bool 从服务器获取的数据解析是否正确*/
    property bool   dataError: false
    /** type:string 设备提交时提示文字*/
    property string pageMsgText: ""
    /** type:string 设备提交时提示标题*/
    property string pageMsgTitleText: ""
    /** type:var 页面展示需要，但是没有包含在dataPackageValue数据中的数据*/
    property var    paramValue
    /** type:var 从服务器获取的调拨详情数据*/
    property var    dataPackageValue
    /** type:var 设备运行状态*/
    property var runStatusStr: ["运行", "停机"]
    property var runStatusType: ["1", "0"]
    property string actionSequence: "-1"
    objectName:"dailyTaskFormPage"
    statusBarHoldEnabled: true
    statusBarHoldItemColor: "white"
    orientationPolicy: CPageOrientation.LockPortrait
    clip: true

    Connections{
        target: rootWindow
        onKeyBackTriggered:{
            if(deviceInspectionPage.status == CPageStatus.Show) {
                if(deviceInspectionPage.dataError == true) {
                    pageStack.pop();
                } else {
                    ensureSubmitDlg.type = "backToCancel";
                    // 确定放弃设备巡检填报吗？
                    deviceInspectionPage.pageMsgText = "确定放弃日常任务填报吗?"
                    ensureSubmitDlg.show();
                }
            }
        }
        onToHome:{
            dropDownListChDlg.hide();
            ensureSubmitDlg.type = "homeToCancel";
            deviceInspectionPage.pageMsgText = "确定放弃日常任务填报吗?"
            ensureSubmitDlg.show();
        }
    }

    onStatusChanged: {
        if (status == CPageStatus.Show) {
            rootWindow.dialogShow = true;
            deviceInspectionPage.statusBarHoldEnabled = true
            gScreenInfo.setStatusBarStyle("white");
            gScreenInfo.setStatusBar(true)
        } else if (status == CPageStatus.Hide) {
            rootWindow.dialogShow = false;
        }else if (status == CPageStatus.WillHide) {
            dropDownListChDlg.hide();
            ensureSubmitDlg.hide();
        }
    }

    GenerateForm{
        id:generateFm
        formType:"dailyTask"
        rootObjName: "dailyTaskFormPage"
        contentColObjName: "dailyTaskFormCol"
        choiceDialogItemObjName: "dailyTaskFormChDlg"
        deviceValue:paramValue
        dataPackage:dataPackageValue
        onCreateComponentFinished:{
            submmitBtn.visible = true;
            if(deviceInspectionPage.paramValue["EnableUploadAttach"] == "1") {
                faultImageList.visible = true
            }
        }
        onDataError:{
            deviceInspectionPage.dataError = true;
            CTOAST.requestToast("错误: " + error,"","");
        }
        onCheckLegalityErrorOccurred:{
            //const QString &label, const int &errorType
            // 0:合法  1:必填 2:运行时必填 3:输入不合法
            console.log("label  ", label, "  errorType  ", errorType);
            switch(Number(errorType)){
            case 1:
                CTOAST.requestToast( label + " 必须填写", "", "");
                break;
            case 2:
                CTOAST.requestToast( label + " 运行时必填", "", "");
                break;
            case 3:
                if(dataType == "IntType")
                    CTOAST.requestToast( label + " 输入不合法,请输入整数", "", "");
                else if(dataType == "DecimalType"){
                    var num = Number(precision);
                    var showStr = "0.";
                    while(num--)
                        showStr += "0";
                    CTOAST.requestToast( label + " 输入不合法,请输入浮点数" +  " (" + String(showStr) + ")", "", "");
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
                leftItemText: "取消"
                leftItemIcon:""
                titleText:dataPackageValue["tables"][0]["value"][0]["ObjectName"]
                onLeftItemTriggered: {
                    if(deviceInspectionPage.dataError == true)
                        pageStack.pop();
                    else {
                        ensureSubmitDlg.type = "backToCancel";
                        deviceInspectionPage.pageMsgText = "确定放弃日常任务填报吗?"
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
                Column{
                    id:contentCol
                    width: parent.width
                    spacing: 5
                    objectName: "dailyTaskFormCol"
//<Field Value="PlanObjectIDやObjectNameやObjectFullNameやPlanGroupIDやTaskGroupDataKeyや
//                    ItemSchemaやRealObjectIDやEquipmentModelNameやCustodianやDetaileやEorPCode"/>
                    DeviceTitle{
                        id:deviceTitle
                        deviceName1: dataPackageValue["tables"][0]["value"][0]["ObjectName"]
                        deviceLabel2: "保管人:"
                        deviceName2: dataPackageValue["tables"][0]["value"][0]["Custodian"]
                        deviceLabel3: "型号:"
                        deviceName3: dataPackageValue["tables"][0]["value"][0]["EquipmentModelName"]
//                        deviceLabel4: "设备编码:"
//                        deviceName4: dataPackageValue["tables"][0]["value"][0]["EorPCode"]
//                        deviceLabel5: "功能描述:"
//                        deviceName5: dataPackageValue["tables"][0]["value"][0]["Detaile"]
                    }
                    ListItem{
                        id:deviceRunStatus
                        mainTitle: "设备状态"
                        endTitle: "运行"
                        itemId:"1"
                        onClicked: {
                            runStatusAndquesitonDlg.type = "runStatus"
                            runStatusAndquesitonDlg.titleText = "设备状态";
                            runStatusAndquesitonDlg.model = [];
                            runStatusAndquesitonDlg.model = deviceInspectionPage.runStatusStr
                            runStatusAndquesitonDlg.select([1 - Number(deviceRunStatus.itemId)], true);
                            runStatusAndquesitonDlg.show();
                        }
                    }
                }

                MultimediaView{
                    id:faultImageList
                    parentPageId: deviceInspectionPage
                    visible: false
                }

                CommonButton{
                    id:submmitBtn
                    signal submitClicked(string errorText,var imgList)
                    text:"提交"
                    visible:false
                    onClicked:{
                        ensureSubmitDlg.type = "submit"
                        deviceInspectionPage.pageMsgText = "提交后信息不可修改，确认提交吗？"
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
        titleText : deviceInspectionPage.pageMsgTitleText
        messageText:deviceInspectionPage.pageMsgText
        acceptedButtonText: "确定"
        rejectButtonText: "取消"
        onAccepted:{
            if(type == "submit"){
                var rv = generateFm.checkFormInputLegality();
                if(rv != 0){
                    console.log("输入不合法");
                    return;
                }
                var otherParamMap = {};
                otherParamMap["runStatus"] = deviceRunStatus.itemId;
                otherParamMap["serialKey"] = deviceInspectionPage.serialKey;
                var dataRow = generateFm.getItemData(otherParamMap);
                var valueMapList = [];
                valueMapList.push(dataRow);
                deviceInspectionPage.actionSequence = inspectionSystemConfig.getSequence()
                transmitManager.resultOfResponse.disconnect(deviceInspectionPage.resultResponse)
                transmitManager.resultOfResponse.connect(deviceInspectionPage.resultResponse)

                var head = {"Objects" : "EISM_TaskDailyObject_ADDOne"};
                var table = {"Name":"ISM_InspectionTaskObject", "Key":"PlanObjectID"};
                var attachments = faultImageList.getMultimediaData();
                console.log("EISM_TaskDailyObject_ADDOne   submitData");
                transmitManager.submitData(head, table, valueMapList, attachments, deviceInspectionPage.serialKey,
                                           deviceInspectionPage.actionSequence, "日常任务提交失败");
            } else if(type == "backToCancel"){
                pageStack.pop();
            } else if(type == "homeToCancel"){
                rootWindow.toHomeBack();
            }
        }
    }

    CListDialog{
        id: dropDownListChDlg
        buttonAreaEnabled: false
        maxVisibleItems:4
        objectName:"dailyTaskFormChDlg"
        onDelegateItemTriggered:{
            dropDownListChDlg.hide();
        }
        function setSelectedItem(selectedIndex) {
            console.log("dropDownListChDlg   setSelectedItem   ", selectedIndex);
            dropDownListChDlg.select([selectedIndex], true);
        }
    }
    CListDialog{
        id: runStatusAndquesitonDlg
        property string type: ""
        buttonAreaEnabled: false
        maxVisibleItems:4
        objectName:"dailyTaskFormChDlg"
        onDelegateItemTriggered:{
            runStatusAndquesitonDlg.hide();
            if(type == "runStatus"){
                deviceRunStatus.endTitle = deviceInspectionPage.runStatusStr[index];
                deviceRunStatus.itemId = deviceInspectionPage.runStatusType[index];
            }
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

    function resultResponse(result, error, action, map){
        console.debug("-DailyTaskForm-onResultOfResponse---result, error, action: ", result, error, action);
        if(action != deviceInspectionPage.actionSequence){
            console.log("action != DailyTaskForm.actionSequence  return")
            return;
        }

        transmitManager.resultOfResponse.disconnect(deviceInspectionPage.resultResponse);
        if(Number(error) != -8){
            if(Number(error) == 0)
                CTOAST.requestToast("提交成功", "", "");
            pageStack.pop();
        }
    }

    Component.onCompleted: {
        rootWindow.toHomeFlag = 1;
        deviceInspectionPage.dataError = true;
        if(dataPackageValue["tables"].length <= 0){
            CTOAST.requestToast("错误: tables is empty","","");
            return;
        } else if(dataPackageValue["tables"][0]["value"].length <= 0){
            CTOAST.requestToast("错误: tables[0].value is empty","","");
            return;
        }
        deviceInspectionPage.dataError = false;
        deviceInspectionPage.serialKey = inspectionSystemConfig.getUUID();
        deviceInspectionPage.pageMsgTitleText = "日常任务";
        tm.start()
    }

    Component.onDestruction: {
        rootWindow.toHomeFlag = 0;
    }
}
