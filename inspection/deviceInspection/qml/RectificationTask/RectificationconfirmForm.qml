/*!
* \file RectificationconfirmForm.qml
* \brief 整改确认任务提交详情页面
*
*整改确认任务提交详情页面
*
*\author 张子健 zhangzijian@syberos.com
*\version 1.0
*\date 2015/9/10
*/
import QtQuick 2.0
import com.syberos.basewidgets 2.0
import "../Common"
import "../Common/constants.js" as UI
import "../Multimedia"
/*!
  \class RectificationconfirmForm
  \inModule 检查整改
  \brief 整改确认任务提交详情页面
  \ingroup RectificationTask
  此界面是整改确认任务提交详情页面,用户在该页面进行操作，将填报数据提交服务器
*/
CommonPage {
    id: rectificationConfFormPage
    statusBarHoldEnabled: true
    statusBarHoldItemColor: "white"
    orientationPolicy: CPageOrientation.LockPortrait
    clip: true

    /** type:string 整改登记人*/
    property string procRecordUserName:""
    /** type:string 登记时间*/
    property string procRecordDate:""
    /** type:string 开始时间*/
    property string starttime:""
    /** type:string 结束时间*/
    property string endtime:""
    /** type:string 制单单位*/
    property string formUnit:""
    /** type:string 制单部门*/
    property string formDepartment:""
    /** type:string 制单人*/
    property string formMakePeople:""
    /** type:string 制单时间*/
    property string formMakeTime:""
    /** type:string 审批时间*/
    property string approvaltime:""
    /** type:string 整改情况*/
    property string procDescript:""
    /** type:string 整改内容*/
    property string rectifyContent:""
    /** type:string 数据源类型(0整改任务 1整改确认)*/
    property string dataSrcType:""
    /** type:string 整改状态*/
    property string rectifyStatus:""
    /** type:string 任务名称*/
    property string orderName: ""
    /** type:string 整改单位*/
    property string rectifyOrgName: ""
    /** type:string 任务编码*/
    property string orderCode: ""
    /** type:string 设备厂商*/
    property string equipmentEnterpriseNameStr: ""
    /** type:string 设备分类*/
    property string equipmentClassNameStr: ""
    /** type:string 设备单位名称*/
    property string equipmentOrganiseUnitNameStr: ""
    /** type:string 设备保管人*/
    property string equipmentCustodianStr: ""
    property bool   dataError: false
    /** type:string 下拉显示状态*/
    property int    flagUporDown:0
    /** type:string 整改详情下拉显示状态*/
    property int    rectificationUporDown:0
    /** type:var 页面展示需要，但是没有包含在dataPackageValue数据中的数据*/
    property var    paramValue
    /** type:var 从服务器获取的整改详情数据*/
    property var    dataPackageValue

    property string actionSequence: "-1"

    Connections{
        target: rootWindow
        onKeyBackTriggered:{
            if(rectificationConfFormPage.status == CPageStatus.Show){
                if(rectificationConfFormPage.dataError == true){
                    pageStack.pop();
                }else{
                    ensureSubmitDlg.type = "backToCancel";
                    ensureSubmitDlg.messageText = os.i18n.ctr(qsTr("Abandon rectification to confirm information"));
                    ensureSubmitDlg.show();
                }
            }
        }
        onToHome:{
            if(rectificationConfFormPage.status == CPageStatus.Show){
                ensureSubmitDlg.type = "homeToCancel";
                ensureSubmitDlg.messageText = os.i18n.ctr(qsTr("Abandon rectification to confirm information"));
                ensureSubmitDlg.show();
            }
        }
    }

    onStatusChanged: {
        if (status == CPageStatus.Show) {
            rootWindow.dialogShow = true;
            rectificationConfFormPage.statusBarHoldEnabled = true
            gScreenInfo.setStatusBarStyle("white");
            gScreenInfo.setStatusBar(true)
        }else if (status == CPageStatus.Hide){
            rootWindow.dialogShow = false;
        }else if (status == CPageStatus.WillHide){
        }
    }

    contentAreaItem: Rectangle {
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
                leftItemEnabled: true
                titleItemEnabled: true
                rightItemEnabled: false
                anchors.top: parent.top
                onLeftItemChanged:leftItem.backgroundEnabled = false
                leftItemBackgroundEnabled: false
                leftItemPixelSize: 22
                titleAreaCentered:true
                titleText:rectificationConfFormPage.orderName
                onLeftItemTriggered: {
                    if(rectificationConfFormPage.dataError == true)
                        pageStack.pop()
                    else{
                        ensureSubmitDlg.type = "backToCancel";
                        ensureSubmitDlg.messageText = os.i18n.ctr(qsTr("Abandon rectification to confirm information"));
                        ensureSubmitDlg.show();
                    }
                }
            }
        }

        Flickable{
            id:detailInfoFlick
            anchors.top: pageHead.bottom
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            flickDeceleration: Flickable.VerticalFlick
            visible: false
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

                ListItem{
                    id:taskInfo
                    property bool showDetail: false
                    mainTitle: "任务信息"
                    endImage: showDetail ?"qrc:/icon/icon_08_up.png" :  "qrc:/icon/xiangxia_03.png"
                    onClicked: {
                        showDetail = !showDetail;
                    }
                }
                Column{
                    id:detailsCol
                    visible: taskInfo.showDetail
                    width: parent.width
                    Item{
                        width: parent.width
                        height:Math.max(starttimetitle.height, starttimetext.height) + 5
                        Text {
                            id:starttimetitle
                            anchors.left: parent.left
                            anchors.leftMargin: 40
                            anchors.verticalCenter: parent.verticalCenter
                            font.pixelSize: 16
                            color:"#999999"
                            text:"开始时间:"
                        }
                        Text {
                            id: starttimetext
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: starttimetitle.right
                            anchors.right: parent.right
                            anchors.rightMargin: 20
                            horizontalAlignment:Text.AlignRight
                            wrapMode: Text.WrapAnywhere
                            font.pixelSize: 16
                            color:"#999999"
                            text: rectificationConfFormPage.starttime
                        }
                        CLine {
                            width:parent.width
                        }
                    }
                    Item{
                        width: parent.width
                        height:Math.max(endtimetitle.height, endtimetext.height) + 5
                        Text {
                            id:endtimetitle
                            anchors.left: parent.left
                            anchors.leftMargin: 40
                            anchors.verticalCenter: parent.verticalCenter
                            font.pixelSize: 16
                            color:"#999999"
                            text:"结束时间:"
                        }
                        Text {
                            id: endtimetext
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: endtimetitle.right
                            anchors.right: parent.right
                            anchors.rightMargin: 20
                            horizontalAlignment:Text.AlignRight
                            wrapMode: Text.WrapAnywhere
                            font.pixelSize: 16
                            color:"#999999"
                            text: rectificationConfFormPage.endtime
                        }
                        CLine {
                            width:parent.width
                        }
                    }
                    Item{
                        width: parent.width
                        height:Math.max(formDepartmentTitle.height, formDepartmentText.height) + 5
                        Text {
                            id:formDepartmentTitle
                            anchors.left: parent.left
                            anchors.leftMargin: 40
                            anchors.verticalCenter: parent.verticalCenter
                            font.pixelSize: 16
                            color:"#999999"
                            text:"制单部门:"
                        }
                        Text {
                            id: formDepartmentText
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: formDepartmentTitle.right
                            anchors.right: parent.right
                            anchors.rightMargin: 20
                            horizontalAlignment:Text.AlignRight
                            wrapMode: Text.WrapAnywhere
                            font.pixelSize: 16
                            color:"#999999"
                            text: rectificationConfFormPage.formDepartment
                        }
                        CLine {
                            width:parent.width
                        }
                    }

                    Item{
                        width: parent.width
                        height:Math.max(formUnitTitle.height, formUnitText.height) + 5
                        Text {
                            id:formUnitTitle
                            anchors.left: parent.left
                            anchors.leftMargin: 40
                            anchors.verticalCenter: parent.verticalCenter
                            font.pixelSize: 16
                            color:"#999999"
                            text:"制单单位:"
                        }
                        Text {
                            id: formUnitText
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: formUnitTitle.right
                            anchors.right: parent.right
                            anchors.rightMargin: 20
                            horizontalAlignment:Text.AlignRight
                            wrapMode: Text.WrapAnywhere
                            font.pixelSize: 16
                            color:"#999999"
                            text: rectificationConfFormPage.formUnit
                        }
                        CLine {
                            width:parent.width
                        }
                    }
                    Item{
                        width: parent.width
                        height:Math.max(formMakeTimeTitle.height, formMakeTimeText.height) + 5
                        Text {
                            id:formMakeTimeTitle
                            anchors.left: parent.left
                            anchors.leftMargin: 40
                            anchors.verticalCenter: parent.verticalCenter
                            font.pixelSize: 16
                            color:"#999999"
                            text:"制单时间:"
                        }
                        Text {
                            id: formMakeTimeText
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: formMakeTimeTitle.right
                            anchors.right: parent.right
                            anchors.rightMargin: 20
                            horizontalAlignment:Text.AlignRight
                            wrapMode: Text.WrapAnywhere
                            font.pixelSize: 16
                            color:"#999999"
                            text: rectificationConfFormPage.formMakeTime
                        }
                        CLine {
                            width:parent.width
                        }
                    }
                    Item{
                        width: parent.width
                        height:Math.max(formMakePeopleTitle.height, formMakePeopleText.height) + 5
                        Text {
                            id:formMakePeopleTitle
                            anchors.left: parent.left
                            anchors.leftMargin: 40
                            anchors.verticalCenter: parent.verticalCenter
                            font.pixelSize: 16
                            color:"#999999"
                            text:"制单人:"
                        }
                        Text {
                            id: formMakePeopleText
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: formMakePeopleTitle.right
                            anchors.right: parent.right
                            anchors.rightMargin: 20
                            horizontalAlignment:Text.AlignRight
                            wrapMode: Text.WrapAnywhere
                            font.pixelSize: 16
                            color:"#999999"
                            text: rectificationConfFormPage.formMakePeople
                        }
                        CLine {
                            width:parent.width
                        }
                    }
                    Item{
                        width: parent.width
                        height:Math.max(formApprovalTimeTitle.height, formApprovalTimeText.height) + 5
                        Text {
                            id:formApprovalTimeTitle
                            anchors.left: parent.left
                            anchors.leftMargin: 40
                            anchors.verticalCenter: parent.verticalCenter
                            font.pixelSize: 16
                            color:"#999999"
                            text:"审批时间:"
                        }
                        Text {
                            id: formApprovalTimeText
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: formApprovalTimeTitle.right
                            anchors.right: parent.right
                            anchors.rightMargin: 20
                            horizontalAlignment:Text.AlignRight
                            wrapMode: Text.WrapAnywhere
                            font.pixelSize: 16
                            color:"#999999"
                            text: rectificationConfFormPage.approvaltime
                        }
                        CLine {
                            width:parent.width
                        }
                    }

                    Item{
                        width: parent.width
                        height:Math.max(rectificationPeopleTitle.height, rectificationPeopleText.height) + 5
                        Text {
                            id:rectificationPeopleTitle
                            anchors.left: parent.left
                            anchors.leftMargin: 40
                            anchors.verticalCenter: parent.verticalCenter
                            font.pixelSize: 16
                            color:"#999999"
                            text:"整改登记人:"
                        }
                        Text {
                            id: rectificationPeopleText
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: rectificationPeopleTitle.right
                            anchors.right: parent.right
                            anchors.rightMargin: 20
                            horizontalAlignment:Text.AlignRight
                            wrapMode: Text.WrapAnywhere
                            font.pixelSize: 16
                            color:"#999999"
                            text: rectificationConfFormPage.procRecordUserName
                        }
                        CLine {
                            width:parent.width
                        }
                    }
                }

                ListItem{
                    id:rectificationContent
                    property bool showDetail: false
                    mainTitle: "整改内容"
                    endImage: showDetail ?"qrc:/icon/icon_08_up.png" :  "qrc:/icon/xiangxia_03.png"
                    onClicked: {
                        showDetail = !showDetail;
                    }
                }
                Item{
                    visible: rectificationContent.showDetail
                    width: parent.width
                    height: contentText.height + 20
                    Text{
                        id:contentText
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.leftMargin: 40
                        anchors.rightMargin: 40
                        wrapMode: Text.WrapAnywhere
                        text:rectificationConfFormPage.rectifyContent
                    }
                }


                ListItem{
                    id:rectificationRemarkItem
                    property bool showDetail: false
                    mainTitle: "整改登记"
                    endImage: showDetail ?"qrc:/icon/icon_08_up.png" :  "qrc:/icon/xiangxia_03.png"
                    onClicked: {
                        showDetail = !showDetail;
                    }
                }
                Item{
                    visible: rectificationRemarkItem.showDetail
                    width: parent.width
                    height: remarkText.height + 20
                    Text{
                        id:remarkText
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.leftMargin: 40
                        anchors.rightMargin: 40
                        wrapMode: Text.WrapAnywhere
                        text:rectificationConfFormPage.procDescript == "" ? "无" : rectificationConfFormPage.procDescript
                    }
                }

                ListItem{
                    id:rectificationCheckItem
                    property bool showDetail: true
                    mainTitle: "整改确认"
                    endImage: showDetail ?"qrc:/icon/icon_08_up.png" :  "qrc:/icon/xiangxia_03.png"
                    onClicked: {
                        showDetail = !showDetail;
                    }
                }

                Rectangle{
                    width: parent.width
                    visible: rectificationCheckItem.showDetail
                    height: 200
                    CommonTextArea{
                        id:deviceInventoryNote
                        height:parent.height - 20
                        font.pixelSize:20
                        anchors.bottom:parent.bottom
                        anchors.horizontalCenter: parent.horizontalCenter
                        width:parent.width - 40
                        maximumLength:300
                        Text {
                            id:placeHoldText
                            visible: parent.text == ""
                            color: "#dddddd"
                            font.pixelSize:parent.font.pixelSize
                            text: "填写整改说明"
                        }
                    }
                }

                MultimediaView{
                    id:faultImageList
                    parentPageId: rectificationConfFormPage
                    visible: rectificationConfFormPage.paramValue["enableImage"] == "1"
                }
                Item{
                    width: parent.width
                    height: UI.GROUP_SEPARATOR_HIGHT
                }
                CommonButton{
                    id:submmitBtn
                    text:os.i18n.ctr(qsTr("Submit"))
                    objectName:"deviceInventoryFormSbmBtn"
                    signal submitClicked()
                    onClicked:{
                        ensureSubmitDlg.type = "submit";
                        ensureSubmitDlg.messageText = os.i18n.ctr(qsTr("No change after the submit information, sure to submit?"))
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
        titleText : os.i18n.ctr(qsTr("Rectification to confirm information"))
        acceptedButtonText: "确定"
        rejectButtonText: "取消"
        onAccepted:{
            if(type == "submit"){
                sendRequest();
            }else if(type == "backToCancel"){
                pageStack.pop();
            }else if(type == "homeToCancel"){
                rootWindow.toHomeBack();
            }
        }
    }

    /**
     * 提交整改确认任务数据
     */
    function sendRequest(){
        var valueMap = {};
        valueMap["OrderID"] = dataPackageValue["tables"][0]["value"][0]["OrderID"];
        valueMap["Proctype"] = "2";
        valueMap["RectifyStatus"] = "3";
        valueMap["ProcDescript"] = rectificationConfFormPage.procDescript;
        valueMap["ProcVerifyDescript"] = deviceInventoryNote.text;
        valueMap["ModifiedBy"] = inspectionSystemParam.getUserId();
        rectificationConfFormPage.actionSequence = inspectionSystemConfig.getSequence();
        transmitManager.resultOfResponse.disconnect(rectificationConfFormPage.resultResponse)
        transmitManager.resultOfResponse.connect(rectificationConfFormPage.resultResponse)

        var head = {"Objects" : "EXAM_ExamineRectify_Import"};
        var table = {"Name":"EXAM_ExamineRectify", "Key":"OrderID"};
        var attachments = faultImageList.getMultimediaData();
        var serialKey = inspectionSystemConfig.getUUID();
        var valueMapList = [];
        valueMapList.push(valueMap);
        transmitManager.submitData(head, table, valueMapList, attachments, serialKey,
                                   rectificationConfFormPage.actionSequence, os.i18n.ctr(qsTr("Rectification confirm submit fail")));
    }

    /**
     * 接受服务器返回数据的槽函数
     * @param result:bool 获取数据是否成功
     * @param error:int 错误代码
     * @param action:string 当前发送数据动作类型
     * @param map:var    服务器返回值
     */
    function resultResponse(result, error, action, map){
        console.debug("---Device----------------ListDelegate-----onResultOfResponse--------------result, error, action: "
                      , result, error, action);
        if(action != rectificationConfFormPage.actionSequence){
            console.log("action != rectificationConfFormPage.actionSequence return")
            return;
        }
        transmitManager.resultOfResponse.disconnect(rectificationConfFormPage.resultResponse);
        if(Number(error) != -8){
            if(Number(error) == 0)
                CTOAST.requestToast(os.i18n.ctr(qsTr("Rectification confirm success")), "", "");
            pageStack.pop();
        }

    }

    Component.onCompleted: {
        rootWindow.toHomeFlag = 1;
        console.log("inventory  dataPackageValue[tables].length", dataPackageValue["tables"].length)
        rectificationConfFormPage.dataError = true;
        if(dataPackageValue["tables"][0]["value"].length <= 0){
            CTOAST.requestToast(os.i18n.ctr(qsTr("error: tables[0].value is empty")),"","");
            return;
        }
        rectificationConfFormPage.dataError = false;
        detailInfoFlick.visible = true;

        rectificationConfFormPage.procRecordUserName = dataPackageValue["tables"][0]["value"][0]["ProcRecordUserName"];
        rectificationConfFormPage.procRecordDate = dataPackageValue["tables"][0]["value"][0]["ProcRecordDate"];

        rectificationConfFormPage.orderName = dataPackageValue["tables"][0]["value"][0]["OrderName"];
        rectificationConfFormPage.rectifyOrgName = dataPackageValue["tables"][0]["value"][0]["RectifyOrgName"];
        rectificationConfFormPage.orderCode = dataPackageValue["tables"][0]["value"][0]["OrderCode"];

        rectificationConfFormPage.starttime = dataPackageValue["tables"][0]["value"][0]["StartTime"];
        rectificationConfFormPage.endtime = dataPackageValue["tables"][0]["value"][0]["EndTime"];
        rectificationConfFormPage.formUnit = dataPackageValue["tables"][0]["value"][0]["OrderCompanyName"];
        rectificationConfFormPage.formDepartment = dataPackageValue["tables"][0]["value"][0]["OrderDepartmentName"];
        rectificationConfFormPage.formMakePeople = dataPackageValue["tables"][0]["value"][0]["OrderUserName"];
        rectificationConfFormPage.formMakeTime = dataPackageValue["tables"][0]["value"][0]["OrderDate"];
        rectificationConfFormPage.approvaltime = dataPackageValue["tables"][0]["value"][0]["OrderAuditDate"];
        rectificationConfFormPage.procDescript = dataPackageValue["tables"][0]["value"][0]["ProcDescript"];
        rectificationConfFormPage.rectifyContent = dataPackageValue["tables"][0]["value"][0]["RectifyContent"];
        rectificationConfFormPage.dataSrcType = dataPackageValue["tables"][0]["value"][0]["DataSrcType"];
        rectificationConfFormPage.rectifyStatus = dataPackageValue["tables"][0]["value"][0]["RectifyStatus"];
    }

    Component.onDestruction: {
        console.log("RectificationconfirmFrom.qml destruction")
        rootWindow.toHomeFlag = 0;
        transmitManager.resultOfResponse.disconnect(rectificationConfFormPage.resultResponse);
    }
}
