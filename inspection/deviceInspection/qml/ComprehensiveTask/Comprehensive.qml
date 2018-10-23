/*!
* \file Comprehensive.qml
* \brief 综合业务功能选择页面
*
*综合业务功能选择页面
*
*\author 张子健 zhangzijian@syberos.com
*\version 1.0
*\date 2015/9/10
*/
import QtQuick 2.0
import com.syberos.basewidgets 2.0
import "../Common"
/*!
  \class Comprehensive
  \inModule 综合业务
  \brief 综合业务功能选择页面
  \ingroup ComprehensiveTask
  此界面是综合业务功能选择页面，用于进入设备领用,备调拨详或关联RFID标签
*/
CommonPage {
    id: comprehensive
    property string actionSequence: "-1"
    property string nameId: ""
    /** type:var   指示窗口对象*/
    property var dialogObj
    statusBarHoldEnabled: true
    statusBarHoldItemColor: "white"
    orientationPolicy: CPageOrientation.LockPortrait
    signal resultRFIDCode(string rfidCode)
    /**
     * 隐藏RFID窗口信号
     * @param /
     */
    signal rfidDialogHide()
    Component.onCompleted: {
        var name;
        var clientPriority = inspectionSystemConfig.getClientPriority();
        //领用
        if(inspectionSystemParam.checkUserPermission("DeviceTaking") == true) {
            if(clientPriority == false){
                name = inspectionSystemParam.getUserPermissionData("DeviceTaking", "ModuleName");
                console.log("inspectionSystemConfig.getClientPriority  name is ", name)
            } else
                name = inspectionFuncsItemModel.getNodeParas("DeviceTaking", comprehensive.nameId)["NameCN"];
            listModel.append({"listText":name,
                                       "portrait":"qrc:/icon/shebeilingyong.png",
                                       "tag" : "0"
                                   });
        }
        //调拨
        if(inspectionSystemParam.checkUserPermission("DeviceAllot") == true) {
            if(clientPriority == false)
                name = inspectionSystemParam.getUserPermissionData("DeviceAllot", "ModuleName");
            else
                name = inspectionFuncsItemModel.getNodeParas("DeviceAllot", comprehensive.nameId)["NameCN"];
            listModel.append({"listText":name,
                                       "portrait":"qrc:/icon/shebeidiaobo.png",
                                       "tag" : "1"
                                   });
        }
        var type = myInspectSysSetting.getOfflineMode();
        //RFID关联
        if(inspectionSystemParam.checkUserPermission("RFIDAssociation") == true && type != "2") {
            if(clientPriority == false)
                name = inspectionSystemParam.getUserPermissionData("RFIDAssociation", "ModuleName");
            else
                name = inspectionFuncsItemModel.getNodeParas("RFIDAssociation", comprehensive.nameId)["NameCN"];
            listModel.append({"listText":name,
                                       "portrait":"qrc:/icon/guanlianrfid.png",
                                       "tag" : "2"
                                   });
        }
        //照片关联
        if(inspectionSystemParam.checkUserPermission("PHOTOAssociation") == true && type != "2"){
            if(clientPriority == false)
                name = inspectionSystemParam.getUserPermissionData("PHOTOAssociation", "ModuleName");
            else
                name = inspectionFuncsItemModel.getNodeParas("PHOTOAssociation", comprehensive.nameId)["NameCN"];
            listModel.append({"listText":name,
                                   "portrait":"qrc:/icon/picture.png",
                                   "tag" : "3"
                               });
        }
        //读取RFID标签信息
        listModel.append({"listText":"读取RFID标签信息",
                               "portrait":"qrc:/icon/guanlianrfid.png",
                               "tag" : "4"
                           });

    }

    onResultRFIDCode: {
        //rfidCode
        dialogObj = Qt.createQmlObject('import QtQuick 2.0;import com.syberos.basewidgets 2.0;CAlertDialog{}', comprehensive);
        dialogObj.titleText = "RFID标签信息";
        dialogObj.messageText = rfidCode;
        dialogObj.alertButtonText = "知道了";
        dialogObj.hideFinished.connect(comprehensive.rfidDialogHide)
        dialogObj.show();
    }
    onRfidDialogHide: {
        console.debug("----------------onFfidDialogHide----------------");
        dialogObj.hideFinished.disconnect(comprehensive.rfidDialogHide);
        dialogObj.destroy();
    }
    contentAreaItem:  Item {
        id: content
        CommonTitlebar{
            id: title
            width: parent.width
            leftItemEnabled: true
            titleItemEnabled: true
            rightItemEnabled: false
            leftItemText:""
            titleText: inspectionSystemParam.getUserPermissionData(comprehensive.nameId, "ModuleName")
            onLeftItemTriggered: pageStack.pop()
            anchors.top: parent.top
            onLeftItemChanged:leftItem.backgroundEnabled = false
            leftItemBackgroundEnabled: false
        }

        ListModel {
            id: listModel
        }

        ListView {
            id: taskList
            width: parent.width
            anchors{
                top: title.bottom
                topMargin: 10
                bottom: parent.bottom
            }
            model: listModel
            delegate: SingleLineImageItem {
                icon: portrait
                text: listText
                onTriggered: {
                    var tag = listModel.get(index).tag;
                    switch(tag){
                    case "0":
                        comprehensive.getRecipientsPlanList()
                        break;
                    case "1":
                        comprehensive.getTransferPlanList()
                        break;
                    case "2":
                        pageStack.push("qrc:/qml/Common/SearchPage.qml",{
                                           "titleText": listText,
                                           "pageActionType": "Associated with RFID tags",
                                           "enabledScanItem": false});

                        break;
                    case "3":
                        pageStack.push("qrc:/qml/Common/SearchPage.qml",{
                                           "titleText": listText,
                                           "pageActionType": "AssociatedWithPhoto",
                                           "enabledScanItem": true});

                        break;
                    case "4":
                        var rfidScanPage = pageStack.push("qrc:/qml/Common/RFIDScan.qml",{"parentPage":comprehensive});
                        rfidScanPage.sendRFIDResult.connect(comprehensive.resultRFIDCode);
                        break;
                    default:
                        break;
                    }
                }
            }
        }
    }

    /**
     * 获取设备领用计划列表
     */
    function getRecipientsPlanList(){
        comprehensive.actionSequence = inspectionSystemConfig.getSequence();
        var organiseUnitIDs = inspectionSystemParam.getOrganiseUnitIDs();
        var departmentIDs = inspectionSystemParam.getUserPropertyByKey("DepartmentIDs");
        transmitManager.resultOfResponse.disconnect(comprehensive.resultResponse)
        transmitManager.resultOfResponse.connect(comprehensive.resultResponse)

        var head = {"Objects" : "EMD_Distribute_PlanList"};
        var para = {};
        para["OrganiseUnitID"] = organiseUnitIDs;
        para["DepartmentID"] = departmentIDs;
        console.log("transmitManager.requestData   EMD_Distribute_PlanList ")
        transmitManager.requestData(head, para, comprehensive.actionSequence, "获取领用任务列表失败");
    }

    /**
     * 获取设备调拨计划列表
     */
    function getTransferPlanList(){
        comprehensive.actionSequence = inspectionSystemConfig.getSequence();
        transmitManager.resultOfResponse.disconnect(comprehensive.resultResponse)
        transmitManager.resultOfResponse.connect(comprehensive.resultResponse)
        var organiseUnitIDs = inspectionSystemParam.getOrganiseUnitIDs();
        var departmentIDs = inspectionSystemParam.getUserPropertyByKey("DepartmentIDs");

        var head = {"Objects" : "EMA_Allocation_PlanList"};
        var para = {};
        para["OrganiseUnitID"] = organiseUnitIDs;
        para["DepartmentID"] = departmentIDs;
        console.log("transmitManager.requestData   EMA_Allocation_PlanList ")
        transmitManager.requestData(head, para, comprehensive.actionSequence, "获取调拨任务列表失败");
    }

    /**
     * 接受服务器返回数据的槽函数
     * @param result:bool 获取数据是否成功
     * @param error:int 错误代码
     * @param action:string 当前发送数据动作类型
     * @param map:var    服务器返回值
     */
    function resultResponse(result, error, action, map){
        console.debug("--Comprehensive--resultResponse-----onResultOfResponse--------------result: ", result, error);
        if(action != comprehensive.actionSequence){
            console.log("action != comprehensive.actionSequence return")
            return;
        }

        transmitManager.resultOfResponse.disconnect(comprehensive.resultResponse)
        if(result && error == 0){
            var headObj = map["head"]["objects"];
            if(headObj == "EMD_Distribute_PlanList"){
                //设备领用计划
                var recipientsOtherParam = {
                    "enabledScanItem": false
                }
                pageStack.push("qrc:/qml/ComprehensiveTask/ComprehensiveBaseList.qml",
                               {
                                   "originalData": map,
                                   "otherParam":recipientsOtherParam,
                                   "taskActionSequence": comprehensive.actionSequence
                               });
            }else if(headObj == "EMA_Allocation_PlanList"){
                //设备调拨计划
                var transferOtherParam = {
                    "enabledScanItem": false
                }
                pageStack.push("qrc:/qml/ComprehensiveTask/ComprehensiveBaseList.qml",
                               {
                                   "originalData": map,
                                   "otherParam":transferOtherParam,
                                   "taskActionSequence": comprehensive.actionSequence
                               });

            }
        }
    }
}
