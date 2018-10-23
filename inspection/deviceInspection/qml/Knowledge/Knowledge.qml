import QtQuick 2.0
import com.syberos.basewidgets 2.0
import "../Common"
BaseList{
    id:knowledgeTaskList
    property string nameId: ""
    property int selectedIndex: -1
    property string taskActionSequence: ""
    property var  documentType: {"1":"规章制度", "2" : "产品文档", "3" : "常见故障查询", "4" : "标准规范"}
    property var  documentClass: {"1":"内部资料", "2" : "外部资料"}
    //任务列表代理
    Component{
        id:taskListDelegate
        KnowledgeTaskListDelegate{}
    }

    onSearchTextChanged: {
        if(!searchInputItem.visible)
            return;
        console.log("onSearchTextChanged :",text, "  inspectionTaskItemModel.count ", inspectionTaskItemModel.rowCount());
        commonProxyModel.setModelFilter( ".*" + text.trim() + ".*");
        console.log("commonProxyModel.filterRegExp  ", ".*" + text + ".*", "   proxyModel is  ", commonProxyModel.itemCount());
    }

    onStatusChanged:{
        if(status == CPageStatus.WillShow){
            console.log("DailyTaskBaseList.qml  refresh ===", refresh);
        }else if(status == CPageStatus.Show){
            console.log("DailyTaskBaseList.qml  status == CPageStatus.Show")
        }else if(status == CPageStatus.WillHide){
            console.log("DailyTaskBaseList.qml  status == CPageStatus.WillHide")
//            knowledgeTaskList.refresh = true;
//            searchInputItem.visible = false;
//            searchInputItem.inputText = "";
//            rootWindow.closeKeyboard();
//            var headerObj1 = knowledgeTaskList.originalData["head"]["objects"] ;
//            if(headerObj1 === "EISM_GetDailyObjectListByGroup" || headerObj1 == "EISM_GetDailyObjectListByTask"){
//                commonProxyModel.setModelFilter(".*.*");
//            }
        }else if(status == CPageStatus.Hide){
            console.log("DailyTaskBaseList.qml  status == CPageStatus.Hide")
        }
    }

    /**
     * 接受服务器返回数据的槽函数
     * @param result:bool 获取数据是否成功
     * @param error:int 错误代码
     * @param action:string 当前发送数据动作类型
     * @param map:var    服务器返回值
     */
    function inspectionResultResponse(result, error, action, map){
        console.log("----DailyTaskBaseList-----Response--result, error,action, actionSequence ",
                      result,  error, action, knowledgeTaskList.actionSequence);
        if(action != knowledgeTaskList.actionSequence  && action != knowledgeTaskList.taskActionSequence){
            console.log("action != knowledgeTaskList.actionSequence  return");
            return;
        }
        knowledgeTaskList.refresh = true;
        if(action == knowledgeTaskList.actionSequence)
            transmitManager.resultOfResponse.disconnect(knowledgeTaskList.inspectionResultResponse)
        if(result && error == 0){
            var headerObj = map["head"]["objects"];
            if(headerObj == "EISM_GetDailyObjectDetail"){
                if(map["tables"].length <= 0 || map["tables"][0]["value"].length <= 0){
                    CTOAST.requestToast("没有找到日常任务对象","","");
                    return;
                }
                var paramValue=  {
                    "IsRFIDScan": knowledgeTaskList.isRFIDScan,
                    "IsBarCodeScan": knowledgeTaskList.isBarCodeScan,
                    "TaskDataKey" : knowledgeTaskList.otherParam["TaskDataKey"],
                    "EnableUploadAttach": knowledgeTaskList.otherParam["EnableUploadAttach"],
                    "pageFormType":"dailyTask",
                    "parentPage":knowledgeTaskList
                };
                pageStack.push("qrc:/qml/DailyTask/DailyTaskForm.qml",
                               {"dataPackageValue": map,
                                   "paramValue": paramValue});
            }
        }
    }

    Component.onCompleted: {
        //页面第一次加载，不刷新
        knowledgeTaskList.refresh = false;
        var headObj = knowledgeTaskList.originalData["head"]["objects"];
        var roles;
        if(headObj === "EMK_knowledge_all"){ //任务列表
            roles = ["ID", "Code", "Name", "Type", "Class", "KeyWords", "CreateName"];
            var name = "";
            var clientPriority = inspectionSystemConfig.getClientPriority();
            if(clientPriority == false){
                name = inspectionSystemParam.getUserPermissionData(knowledgeTaskList.nameId, "ModuleName");
                 console.log("completed11111111  name is  ", name)
            } else {
                name = inspectionFuncsItemModel.getNodeParas(knowledgeTaskList.nameId, "")["NameCN"];
                console.log("completed222222222  name is  ", name, " knowledgeTaskList.nameId ", knowledgeTaskList.nameId)
            }

            if(name == ""){
                name = "知识库";
            }
            transmitManager.resultOfResponse.connect(knowledgeTaskList.inspectionResultResponse);
            knowledgeTaskList.titleText = name;
            knowledgeTaskList.emptyText = "无相关内容";
            knowledgeTaskList.delegateCom = taskListDelegate;
            inspectionTaskModel.setRolesName(roles);
            inspectionTaskModel.setListData(knowledgeTaskList.originalData["tables"][0]["value"]);
            knowledgeTaskList.model = inspectionTaskModel;
            knowledgeTaskList.titleRightItemIcon = "qrc:/icon/action_bar_menu.png"
            knowledgeTaskList.titleRightItemEnable = true;
        }
    }

    Component.onDestruction: {
        console.log("InspectionBaselist.qml onDestruction ")
        transmitManager.resultOfResponse.disconnect(knowledgeTaskList.inspectionResultResponse);
    }
}
