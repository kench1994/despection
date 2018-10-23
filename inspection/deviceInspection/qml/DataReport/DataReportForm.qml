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
/*!
  \class DeviceInspectionForm
  \inModule 设备巡检
  \brief 设备巡检提交详情页面
  \ingroup InspectionTask
  此界面是设备巡检提交详情页面，用户在该页面进行操作，将填报数据提交服务器
*/
CommonPage {
    id: dataReportPage

    /** type:bool 从服务器获取的数据解析是否正确*/
    property bool   dataError: false

    /** type:string 设备提交时提示文字*/
    property string pageMsgText: ""

    /** type:string 设备提交时提示标题*/
    property string pageMsgTitleText: ""

    /** type:string 信息提交失败时提示文字*/
    property string pageSubmitFaileText: ""

    /** type:var 页面展示需要，但是没有包含在dataPackageValue数据中的数据*/
    property var    paramValue

    /** type:var 从服务器获取的调拨详情数据*/
    property var    dataPackageValue

    /** type:var 设备名称列表*/
    property var    groupNameList:[]

    /** type:var 设备ID列表*/
    property var    groupIDList:[]

    objectName:"deviceInspectionFormPage"
    statusBarHoldEnabled: true
    statusBarHoldItemColor: "white"
    orientationPolicy: CPageOrientation.LockPortrait
    clip: true


    Connections{
        target: rootWindow
        onKeyBackTriggered:{
            if(dataReportPage.status == CPageStatus.Show) {
                if(dataReportPage.dataError == true) {
                    pageStack.pop();
                } else {
                    ensureSubmitDlg.type = "backToCancel";
                    // 确定放弃设备巡检填报吗？
                    dataReportPage.pageMsgText = "放弃数据上报"
                    ensureSubmitDlg.show();
                }
            }
        }
        onToHome:{
            dropDownListChDlg.hide();
            ensureSubmitDlg.type = "homeToCancel";
            dataReportPage.pageMsgText = "放弃数据上报"
            ensureSubmitDlg.show();
        }
    }

    onStatusChanged: {
        if (status == CPageStatus.Show) {
            rootWindow.dialogShow = true;
            dataReportPage.statusBarHoldEnabled = true
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
        formType:"dataReport"
        rootObjName: "deviceInspectionFormPage"
        contentColObjName: "deviceInspectionFormCol"
        choiceDialogItemObjName: "deviceInspectionFormChDlg"
        deviceValue:paramValue
        //后面对数据进行处理后再传入c++
        //dataPackage:dataPackageValue
//        onSendResponseFinished:{
//            console.log("result is ", result, "error is  ", error, "aciton is  ", aciton, "  map  ", map);
//            if(Number(error) == 0){
//                CTOAST.requestToast("数据上报成功", "", "");
//                if(deviceListChDlg.selectedIndex < deviceListChDlg.maxIndex ){
//                    deviceListChDlg.selectedIndex++;
//                    deviceListItem.endTitle = dataReportPage.groupNameList[deviceListChDlg.selectedIndex];
//                    deviceListItem.itemId = dataReportPage.groupIDList[deviceListChDlg.selectedIndex];
//                    dataReportPage.resetTableData();
//                }
//            }
//        }
        onCreateComponentFinished:{
            submmitBtn.visible = true;
        }

        onDataError:{
            dataReportPage.dataError = true;
            CTOAST.requestToast("错误:" + error,"","");
        }

        onCheckLegalityErrorOccurred:{
            //const QString &label, const int &errorType
            // 0:合法  1:必填 2:运行时必填 3:输入不合法
            console.log("label  ", label, "  errorType  ", errorType);
            switch(Number(errorType)){
            case 1:
                CTOAST.requestToast( label + " 必填项", "", "");
                break;
            case 2:
                CTOAST.requestToast( label + " 当设备运行时，此项必填", "", "");
                break;
            case 3:
                if(dataType == "IntType")
                    CTOAST.requestToast( label + " 请输入整数", "", "");
                else if(dataType == "DecimalType"){
                    var num = Number(precision);
                    var showStr = "0.";
                    while(num--)
                        showStr += "0";
                    CTOAST.requestToast( label + " "
                                        + " 请输入浮点数(" + String(showStr) + ")", "", "");
                }
                break;
            }
        }
        Component.onCompleted: {
            console.log("GenerateForm   onCompleted")
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
            //color:"red"

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
                leftItemText:"取消"
                leftItemIcon:""
                titleText:dataReportPage.pageMsgTitleText
                onLeftItemTriggered: {
                    if(dataReportPage.dataError == true)
                        pageStack.pop();
                    else {
                        ensureSubmitDlg.type = "backToCancel";
                        dataReportPage.pageMsgText = "放弃数据上报"
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
                ListItem{
                    id:deviceListItem
                    mainTitle: "上报项列表"
                    onClicked: {
                        deviceListChDlg.select([deviceListChDlg.selectedIndex], true);
                        deviceListChDlg.show();
                    }
                }

                Column{
                    id:contentCol
                    width: parent.width
                    spacing: 5
                    objectName: "deviceInspectionFormCol"
                }

                CommonButton{
                    id:submmitBtn
                    signal submitClicked(string errorText,var imgList)
                    text:"提交"
                    visible:false
                    onClicked:{
                        ensureSubmitDlg.type = "submit"
                        dataReportPage.pageMsgText = "提交后信息不可改变，确认提交吗？"
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
        titleText : dataReportPage.pageMsgTitleText
        messageText:dataReportPage.pageMsgText
        acceptedButtonText: "确定"
        rejectButtonText: "取消"
        onAccepted:{
            if(type == "submit"){
                var otherParamMap = {};
//                var rv = generateFm.sendDeviceInspectinFormRequest("",
//                                                                   dataReportPage.pageSubmitFaileText,
//                                                                   [],
//                                                                   otherParamMap);
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
        objectName:"deviceInspectionFormChDlg"
        onDelegateItemTriggered:{
            dropDownListChDlg.hide();
        }
        function setSelectedItem(selectedIndex) {
            console.log("dropDownListChDlg   setSelectedItem   ", selectedIndex);
            dropDownListChDlg.select([selectedIndex], true);
        }
    }

    CListDialog{
        id: deviceListChDlg
        property int  selectedIndex: 0
        property int maxIndex: -1
        buttonAreaEnabled: false
        maxVisibleItems:4
        titleText:"上报项列表"
        onDelegateItemTriggered:{
            deviceListChDlg.hide();
            if(deviceListChDlg.selectedIndex != index){
                deviceListChDlg.selectedIndex = index;
                dataReportPage.resetTableData();
            }
        }
    }

    Timer{
        id:tm
        interval: 350
        repeat: false
        running: false
        onTriggered: {
            dataReportPage.resetTableData();
        }
    }

    function resetTableData(){
        deviceListItem.endTitle = dataReportPage.groupNameList[deviceListChDlg.selectedIndex];
        deviceListItem.itemId = dataReportPage.groupIDList[deviceListChDlg.selectedIndex];
        var groupID = deviceListItem.itemId;
        var dataPackagemap = {};
        dataPackagemap["head"] = dataReportPage.dataPackageValue["head"];
        dataPackagemap["tables"] = [];
        var tableMap = {};
        tableMap["name"] = dataReportPage.dataPackageValue["tables"][0]["name"];
        tableMap["key"] =  dataReportPage.dataPackageValue["tables"][0]["key"];
        tableMap["value"] = [];
        var i = 0;
        for(;i < dataPackageValue["tables"][0]["value"].length; i++){
            if(groupID == dataPackageValue["tables"][0]["value"][i]["GroupID"]){
                var valueMap = dataPackageValue["tables"][0]["value"][i];
                console.log("var valueMap = dataPackageValue   ", valueMap["ID"])
                tableMap["value"].push(valueMap);
            }
        }
        dataPackagemap["tables"].push(tableMap);
        generateFm.dataPackage = dataPackagemap;
        generateFm.initializeForm();
    }

    Component.onCompleted: {
        console.log("dataReportPage   onCompleted  ")
        rootWindow.toHomeFlag = 1;
        dataReportPage.dataError = true;
        if(dataPackageValue["tables"].length <= 0){
            CTOAST.requestToast("错误：表为空","","");
            return;
        } else if(dataPackageValue["tables"][0]["value"].length <= 0){
            CTOAST.requestToast("错误：table[0].value 为空","","");
            return;
        }
        dataReportPage.dataError = false;
        dataReportPage.pageMsgTitleText = "数据上报";
        dataReportPage.pageSubmitFaileText  = "数据上报提交失败";
        var deviceList = dataPackageValue["tables"][0]["value"];
        var i = 0;
        for(; i < deviceList.length; i++){
            if(arrayContains(dataReportPage.groupIDList, deviceList[i]["GroupID"]) == false){
                dataReportPage.groupIDList.push(deviceList[i]["GroupID"]);
                dataReportPage.groupNameList.push(deviceList[i]["GroupName"]);
            }
        }

        deviceListChDlg.selectedIndex = 0;
        deviceListChDlg.maxIndex = dataReportPage.groupNameList.length - 1;
        deviceListItem.endTitle = dataReportPage.groupNameList[0];
        deviceListItem.itemId = dataReportPage.groupIDList[0];
        deviceListChDlg.model = dataReportPage.groupNameList;
        deviceListChDlg.select([0], true);
        tm.start();
    }

    function arrayContains(a, obj){
        for(var i = 0; i < a.length; i++) {
            if(a[i] === obj){
                return true;
            }
        }
        return false;
    }

    Component.onDestruction: {
        rootWindow.toHomeFlag = 0;
    }
}
