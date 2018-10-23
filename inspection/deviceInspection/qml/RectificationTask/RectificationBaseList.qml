import QtQuick 2.0
import com.syberos.basewidgets 2.0
import "../Common"
BaseList{
    id:rectificationTaskList
    //任务列表代理
    Component{
        id:rectificationTaskListDelegate
        RectificationTaskDelegate{}
    }
    //套件列表代理
    Component{
        id:rectificationConfTaskListDelegate
        RfconfirmListDelegate{}
    }

    onStatusChanged:{
        if(status == CPageStatus.WillShow){
            console.log("rectificationTaskList.qml  refresh ===", refresh);
            if(refresh === true){
                var headerObj = rectificationTaskList.originalData["head"]["objects"];
                rectificationTaskList.actionSequence = inspectionSystemConfig.getSequence();
                if(headerObj == "EXAM_ExamineRectifyProcList"){
                    transmitManager.resultOfResponse.disconnect(rectificationTaskList.resultResponse)
                    transmitManager.resultOfResponse.connect(rectificationTaskList.resultResponse)
                    if(rectificationTaskList.otherParam["actionType"] == "0"){

                        var head = {};
                        head["Objects"] = "EXAM_ExamineRectifyProcList";
                        var para = {};
                        para["ProcType"] = "1";
                        para["UserCompany"] = inspectionSystemParam.getOrganiseUnitIDs();
                        para["UserDepartment"] = inspectionSystemParam.getUserPropertyByKey("DepartmentIDs");
                        console.log("transmitManager.requestData   EXAM_ExamineRectifyProcList ")
                        transmitManager.requestData(head, para, rectificationTaskList.actionSequence, os.i18n.ctr(qsTr("Get rectification list fail")));

                    }else if(rectificationTaskList.otherParam["actionType"] == "1"){

                        var head1 = {"Objects" : "EXAM_ExamineRectifyProcList"};
                        var para1 = {};
                        para1["ProcType"] = "2";
                        para1["UserCompany"] = inspectionSystemParam.getOrganiseUnitIDs();
                        para1["UserDepartment"] = inspectionSystemParam.getUserPropertyByKey("DepartmentIDs");
                        console.log("transmitManager.requestData   EXAM_ExamineRectifyProcList ")
                        transmitManager.requestData(head1, para1, rectificationTaskList.actionSequence, os.i18n.ctr(qsTr("Get rectification confirmation  list fail")));
                    }
                }
            }

        }else if(status == CPageStatus.Show){
            console.log("RectificationBaseList.qml  status == CPageStatus.Show")
        }else if(status == CPageStatus.WillHide){
            console.log("RectificationBaseList.qml  status == CPageStatus.WillHide")
            rectificationTaskList.refresh = true;
        }else if(status == CPageStatus.Hide){
            console.log("RectificationBaseList.qml  status == CPageStatus.Hide")
        }
    }

    /**
     * 接受服务器返回数据的槽函数
     * @param result:bool 获取数据是否成功
     * @param error:int 错误代码
     * @param action:string 当前发送数据动作类型
     * @param map:var    服务器返回值
     */
    function resultResponse(result, error, action, map){
        console.log("----rectificationTaskList-----Response--result, error,action, actionSequence ",
                      result,  error, action, rectificationTaskList.actionSequence);

        if(action != rectificationTaskList.actionSequence){
            console.log("action != rectificationTaskList.actionSequence  return");
            return;
        }
        rectificationTaskList.refresh = true;
        if(result && error == 0){
            var headerObj = map["head"]["objects"];
            console.log("rectificationTaskList.model.setListData 00000000000000000000")
            rectificationTaskList.model.setListData(map["tables"][0]["value"]);
            rectificationTaskList.originalData = map;
        }
    }

    //actionType  0  整改   1  整改确认
    Component.onCompleted: {
        //页面第一次加载，不刷新
        rectificationTaskList.refresh = false;
        var headObj = rectificationTaskList.originalData["head"]["objects"];
        var roles = ["OrderID", "OrderCode", "OrderName","DataSrcType", "Circle", "SyncStatus"];
        if(headObj === "EXAM_ExamineRectifyProcList"){ //任务列表
            //接受二次信号
            console.log("transmitManager.resultOfResponse.connect(rectificationTaskList.resultResponse)")
            transmitManager.resultOfResponse.connect(rectificationTaskList.resultResponse);
            var title, emptyTitle;
            if(rectificationTaskList.otherParam["actionType"] == "0"){
                title = os.i18n.ctr(qsTr("Rectification task"));
                emptyTitle = os.i18n.ctr(qsTr("No RectificationTask"));
                rectificationTaskList.delegateCom = rectificationTaskListDelegate;
            }else if(rectificationTaskList.otherParam["actionType"] == "1"){
                title = qsTr("Rectification to confirm");
                emptyTitle = os.i18n.ctr(qsTr("No Rectification confirm"));
                rectificationTaskList.delegateCom = rectificationConfTaskListDelegate;
            }
            rectificationTaskList.titleText = title;
            rectificationTaskList.emptyText = emptyTitle;
            inspectionTaskModel.setRolesName(roles);
            inspectionTaskModel.setListData(rectificationTaskList.originalData["tables"][0]["value"]);
            rectificationTaskList.model = inspectionTaskModel;
            rectificationTaskList.titleRightItemIcon = "qrc:/icon/action_bar_menu.png"
            rectificationTaskList.titleRightItemEnable = true;
            rectificationTaskList.offlineEnable = true

        }
    }
    Component.onDestruction: {
        console.log("RectificationBaseList.qml destruction")
        transmitManager.resultOfResponse.disconnect(rectificationTaskList.resultResponse);
    }

    onSyncTask: {
        console.debug("---------------onSyncTask-----------EXAM_ExamineRectifyProcList------")
        var type = rectificationTaskList.otherParam["actionType"];
        var head = {};
        head["Objects"] = "EXAM_ExamineRectifyProcList";
        var para = {};
        if(type == "0"){
            para["ProcType"] = "1";
        }else if(type == "1"){
            para["ProcType"] = "2";
        }
        para["UserCompany"] = inspectionSystemParam.getOrganiseUnitIDs();
        para["UserDepartment"] = inspectionSystemParam.getUserPropertyByKey("DepartmentIDs");
        rectificationTaskList.actionSequence = inspectionSystemConfig.getSequence();
        transmitManager.requestTaskData(head, para, rectificationTaskList.actionSequence, os.i18n.ctr(qsTr("Get rectification list fail")));
    }
}
