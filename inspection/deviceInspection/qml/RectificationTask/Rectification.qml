/*!
* \file Rectification.qml
* \brief 检查整改功能选择列表页
*
*检查整改功能选择列表页
*
*\author 张子健 zhangzijian@syberos.com
*\version 1.0
*\date 2015/9/10
*/
import QtQuick 2.0
import com.syberos.basewidgets 2.0
import "../Common"
/*!
  \class Rectification
  \inModule 检查整改
  \brief 检查整改功能选择列表页
  \ingroup RectificationTask
  此界面是检查整改功能选择列表页,用于进入整改任务或整改确认
*/
CommonPage {
    id: rectification
    property string actionSequence: "-1"
    statusBarHoldEnabled: true
    statusBarHoldItemColor: "white"
    orientationPolicy: CPageOrientation.LockPortrait
    /** type:string 检查整改标志位(0进入整改任务 1进入整改确认)*/
    property string actionType: "0"
    property string nameId: ""

    contentAreaItem:  Item {
        id: content
        CommonTitlebar{
            id: title
            width: parent.width
            leftItemEnabled: true
            titleItemEnabled: true
            rightItemEnabled: false
            leftItemText:""
            //titleText: qsTr("Check the rectification")
            titleText:{
                var name;
                var clientPriority = inspectionSystemConfig.getClientPriority();
                if(clientPriority == false){
                    name = inspectionSystemParam.getUserPermissionData(rectification.nameId, "ModuleName");
                } else
                    name = inspectionFuncsItemModel.getNodeParas(rectification.nameId, "")["NameCN"];
                return name;
            }

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
                        rectification.getRectificationTaskList()
                        break;
                    case "1":
                        rectification.getAllocationPlanList()
                        break;
                    default:
                        break;
                    }
                }
            }
        }
    }
    function getRectificationTaskList(){
        actionType = "0";
        rectification.actionSequence = inspectionSystemConfig.getSequence();
        transmitManager.resultOfResponse.disconnect(rectification.resultResponse)
        transmitManager.resultOfResponse.connect(rectification.resultResponse)

        var head = {};
        head["Objects"] = "EXAM_ExamineRectifyProcList";
        var para = {};
        para["ProcType"] = "1";
        para["UserCompany"] = inspectionSystemParam.getOrganiseUnitIDs();
        para["UserDepartment"] = inspectionSystemParam.getUserPropertyByKey("DepartmentIDs");
        console.log("transmitManager.requestData   EXAM_ExamineRectifyProcList ")
        transmitManager.requestData(head, para, rectification.actionSequence, os.i18n.ctr(qsTr("Get rectification list fail")));
    }

    function getAllocationPlanList(){
        actionType = "1";
        rectification.actionSequence = inspectionSystemConfig.getSequence();
        transmitManager.resultOfResponse.disconnect(rectification.resultResponse)
        transmitManager.resultOfResponse.connect(rectification.resultResponse)
        var head = {};
        head["Objects"] = "EXAM_ExamineRectifyProcList";
        var para = {};
        para["ProcType"] = "2";
        para["UserCompany"] = inspectionSystemParam.getOrganiseUnitIDs();
        para["UserDepartment"] = inspectionSystemParam.getUserPropertyByKey("DepartmentIDs");
        console.log("transmitManager.requestData   EXAM_ExamineRectifyProcList ")
        transmitManager.requestData(head, para, rectification.actionSequence, os.i18n.ctr(qsTr("Get rectification confirmation  list fail")));
    }

    function resultResponse(result, error, action, map){
        console.debug("1---resultResponse---onResultOfResponse---result: ", result, error);
        if(action != rectification.actionSequence){
            console.log("action != rectification.actionSequence return")
            return;
        }

        transmitManager.resultOfResponse.disconnect(rectification.resultResponse);
        if(result && error == 0){
            var headObj = map["head"]["objects"];
            if(headObj == "EXAM_ExamineRectifyProcList" && actionType == "0"){
                //整改任务
                var otherParam = {
                    "actionType": actionType,
                    "enabledScanItem":false
                }
                pageStack.push("qrc:/qml/RectificationTask/RectificationBaseList.qml",
                               {
                                   "originalData": map,
                                   "otherParam": otherParam,
                                   "actionSequence": rectification.actionSequence
                               });
            }else if(headObj == "EXAM_ExamineRectifyProcList"&&actionType == "1"){
                //整改确认

                var confirmOtherParam = {
                    "actionType": actionType,
                    "enabledScanItem":false
                }
                pageStack.push("qrc:/qml/RectificationTask/RectificationBaseList.qml",
                               {
                                   "originalData": map,
                                   "otherParam": confirmOtherParam,
                                   "actionSequence": rectification.actionSequence
                               });
            }
        }
    }

    Component.onCompleted: {
        var name;
        var clientPriority = inspectionSystemConfig.getClientPriority();
        if(inspectionSystemParam.checkUserPermission("RectificationTasks") == true) {
            if(clientPriority == false){
                name = inspectionSystemParam.getUserPermissionData("RectificationTasks", "ModuleName");
            } else
                name = inspectionFuncsItemModel.getNodeParas("RectificationTasks", rectification.nameId)["NameCN"];
            listModel.append({"listText":name,
                                       "portrait":"qrc:/icon/zhenggairenwu.png",
                                       "tag" : "0"
                                   });
        }
        if(inspectionSystemParam.checkUserPermission("RectificationConfirm") == true) {
            if(clientPriority == false){
                name = inspectionSystemParam.getUserPermissionData("RectificationConfirm", "ModuleName");
            } else
                name = inspectionFuncsItemModel.getNodeParas("RectificationConfirm", rectification.nameId)["NameCN"];
            listModel.append({"listText":name,
                                       "portrait":"qrc:/icon/zhenggaiqueren.png",
                                       "tag" : "1"
                                   });
        }
    }
    Component.onDestruction: {
        console.log("rectification.qml  destruction")
        transmitManager.resultOfResponse.disconnect(rectification.resultResponse);
    }
}
