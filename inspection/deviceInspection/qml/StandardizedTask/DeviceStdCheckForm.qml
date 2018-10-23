/*!
* \file DeviceStdCheckForm.qml
* \brief 标准化检查提交详情页面
*
*此界面是标准化检查提交详情页面，用户在该页面进行操作，将填报数据提交服务器
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
  \class DeviceStdCheckForm
  \inModule 标准化检查
  \brief 标准化检查提交详情页面
  \ingroup StandardizedTask
  此界面是标准化检查提交详情页面，用户在该页面进行操作，将填报数据提交服务器
*/
CommonPage {
    id: deviceStdCheckFormPage

    /** type:string serialKey*/
    property string serialKey

    /** type:string 页面类型*/
    property string pageFormType

    /** type:string 是否允许拍照*/
    property string enableImage: "0"

    /** type:string 设备提交时提示文字*/
    property string pageMsgText: ""

    /** type:string 设备提交时提示标题*/
    property string pageMsgTitleText: ""

    /** type:string 信息提交失败时提示文字*/
    property string pageSubmitFaileText: ""

    /** type:bool 从服务器获取的数据解析是否正确*/
    property bool   dataError: false

    /** type:bool 页面是否正在显示，主要用来出来back键*/
    property bool   pageShow: false

    /** type:var 页面展示需要，但是没有包含在dataPackageValue数据中的数据*/
    property var    paramValue

    /** type:var 从服务器获取的调拨详情数据*/
    property var    dataPackageValue
    property string actionSequence: "-1"
    objectName:"deviceInspectionFormPage"
    statusBarHoldEnabled: true
    statusBarHoldItemColor: "white"
    orientationPolicy: CPageOrientation.LockPortrait
    clip: true

    Component.onCompleted: {
        rootWindow.toHomeFlag = 1;
        deviceStdCheckFormPage.serialKey = dataPackageValue["tables"][0]["value"][0]["SerialKey"];
        deviceStdCheckFormPage.pageMsgTitleText = os.i18n.ctr(qsTr("Standards check"));
        deviceStdCheckFormPage.pageSubmitFaileText  = os.i18n.ctr(qsTr("Standards check submit fail"));
        tm.start()
    }

    Component.onDestruction: {
        rootWindow.toHomeFlag = 0;

    }

    Connections{
        target: rootWindow
        onKeyBackTriggered:{
            console.log("DeviceStdCheckForm.qml  onKeyBackTriggered  deviceStdCheckFormPage.pageShow  ",
                        deviceStdCheckFormPage.pageShow)
            if(deviceStdCheckFormPage.pageShow == true){
                if(deviceStdCheckFormPage.dataError == true)
                    pageStack.pop();
                else{
                    ensureSubmitDlg.type = "backToCancel";
                    deviceStdCheckFormPage.pageMsgText = "确认放弃规范检查填报吗?"
                    ensureSubmitDlg.show();
                }
            }
        }
        onToHome:{
            ipModelDialog.hide();
            ensureSubmitDlg.type = "homeToCancel";
            deviceStdCheckFormPage.pageMsgText = "确认放弃规范检查填报吗?"
            ensureSubmitDlg.show();
        }
    }

    onStatusChanged: {
        if (status == CPageStatus.WillShow) {
            console.log("DeviceStdCheckForm.qml   status == CPageStatus.WillShow ")
        }else if (status == CPageStatus.Show) {
            console.log("DeviceStdCheckForm.qml   status == CPageStatus.Show ")
            deviceStdCheckFormPage.pageShow = true;
            rootWindow.dialogShow = true;
            deviceStdCheckFormPage.statusBarHoldEnabled = true
            gScreenInfo.setStatusBarStyle("white");
            gScreenInfo.setStatusBar(true)
        }else if (status == CPageStatus.Hide){
            rootWindow.dialogShow = false;
        }else if (status == CPageStatus.WillHide) {
            console.log("DeviceStdCheckForm.qml   status == CPageStatus.WillHide  ")
            deviceStdCheckFormPage.pageShow = false;
            ensureSubmitDlg.hide();
            ipModelDialog.hide();
        }
    }
    GenerateForm{
        id:stdCheckFm
        formType:"standard"
        rootObjName: "deviceInspectionFormPage"
        contentColObjName: "deviceInspectionFormCol"
        choiceDialogItemObjName: "deviceInspectionFormChDlg"
        deviceValue:paramValue
        dataPackage:dataPackageValue
        onCreateComponentFinished:{
            submmitBtn.visible = true;
            if(deviceStdCheckFormPage.paramValue["EnableUploadAttach"] == "1"){
                faultImageList.visible=true
            }
        }
        onDataError:{
            deviceStdCheckFormPage.dataError = true;
            CTOAST.requestToast(os.i18n.ctr(qsTr("error: ")) + error,"","");
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
                leftItemEnabled: true
                titleItemEnabled: true
                rightItemEnabled: false
                leftItemPixelSize: 22
                titleAreaCentered:true
                leftItemTextColor: "#666666"
                leftItemText:os.i18n.ctr(qsTr("Cancel"))
                leftItemIcon:""
                titleText : dataPackageValue["tables"][0]["value"][0]["ItemName"]
                onLeftItemTriggered: {
                    if(deviceStdCheckFormPage.dataError == true)
                        pageStack.pop();
                    else{
                        ensureSubmitDlg.type = "backToCancel";
                        deviceStdCheckFormPage.pageMsgText = "确认放弃规范检查填报吗?"
                        ensureSubmitDlg.visible = true;
                    }
                }
                anchors.top: parent.top
                onLeftItemChanged:leftItem.backgroundEnabled = false
                leftItemBackgroundEnabled: false
            }
        }

        Flickable{
            id:detailInfo
            anchors.top: pageHead.bottom
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            flickDeceleration: Flickable.VerticalFlick
            //maximumFlickVelocity :70
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
                    objectName: "deviceInspectionFormCol"
                }

                MultimediaView{
                    id:faultImageList
                    visible: false
                    parentPageId:deviceStdCheckFormPage
                }

                CommonButton{
                    id:submmitBtn
                    text:os.i18n.ctr(qsTr("Submit"))
                    visible:false
                    signal submitClicked(string errorText)
                    onClicked:{
                        ensureSubmitDlg.type = "submit"
                        deviceStdCheckFormPage.pageMsgText = os.i18n.ctr(qsTr("No change after the submit information, sure to submit?"))
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
        titleText : deviceStdCheckFormPage.pageMsgTitleText
        messageText:deviceStdCheckFormPage.pageMsgText
        acceptedButtonText: os.i18n.ctr(qsTr("Ok"))//确定
        rejectButtonText: os.i18n.ctr(qsTr("Cancel"))//取消
        onAccepted:{
            if(type == "submit"){
                var rv = stdCheckFm.checkFormInputLegality();
                if(rv != 0){
                    console.log("输入不合法");
                    return;
                }
                var attachments = faultImageList.getMultimediaData();
                var otherParam = {"serialKey": deviceStdCheckFormPage.serialKey};
                var dataRow = stdCheckFm.getItemData(otherParam);
                var valueMapList = [];
                valueMapList.push(dataRow);
                deviceStdCheckFormPage.actionSequence = inspectionSystemConfig.getSequence()
                transmitManager.resultOfResponse.disconnect(deviceStdCheckFormPage.resultResponse)
                transmitManager.resultOfResponse.connect(deviceStdCheckFormPage.resultResponse)

                var head = {"Objects" : "EXAM_ExamineTaskItem_Import"};
                var table = {"Name":"EXAM_ExamineTaskItem", "Key":"TaskItemID"};
                console.log("EXAM_ExamineTaskItem_Import   submitData");
                transmitManager.submitData(head, table, valueMapList, attachments, deviceStdCheckFormPage.serialKey, deviceStdCheckFormPage.actionSequence, "规范检查提交失败");
            } else if(type == "backToCancel"){
                pageStack.pop();
            } else if(type == "homeToCancel"){
                rootWindow.toHomeBack();
            }
        }
    }

    CListDialog{
        id: ipModelDialog
        buttonAreaEnabled: false
        maxVisibleItems:4
        objectName:"deviceInspectionFormChDlg"
        onDelegateItemTriggered:{
            ipModelDialog.hide();
        }
        function setSelectedItem(selectedIndex){
            console.log("ipModelDialog   setSelectedItem   ", selectedIndex);
            ipModelDialog.select([selectedIndex], true);
        }
    }

    Timer{
        id:tm
        interval: 350
        repeat: false
        running: false
        onTriggered: {
            stdCheckFm.initializeForm();
        }
    }
    function resultResponse(result, error, action, map){
        console.debug("-deviceStdCheckForm-onResultOfResponse---result, error, action: ", result, error, action);
        if(action != deviceStdCheckFormPage.actionSequence){
            console.log("action != deviceStdCheckFormPage.actionSequence  return")
            return;
        }

        transmitManager.resultOfResponse.disconnect(deviceStdCheckFormPage.resultResponse);
        if(Number(error) != -8){
            if(Number(error) == 0)
                CTOAST.requestToast("提交成功", "", "");
            pageStack.pop();
        }
    }
}
