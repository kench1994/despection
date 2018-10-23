/*!
* \file main.qml
* \brief UI启动文件
*
*
*
*\author 何昆 hekun@syberos.com
*\version 1.0
*\date 2015/9/9
*/
import QtQuick 2.0
import QtQuick.Controls 1.0
import com.syberos.basewidgets 2.0
import QtQuick.Controls.Styles 1.1
import myInspectionSysSetting 1.0  //add by chenr ruihua
import "./Common"
import "./Common/constants.js" as UI
import "./SystemSettings"
import "./LoginAndHome"
import "./DeviceOperationTask"
import "./InspectionTask"
/*!
  \class main
  \brief UI启动文件
  \ingroup qml
  UI启动文件
*/
CPageStackWindow {
    id: rootWindow
    /** type:int  是否正在进行二维码扫描*/
    property int isScaningBarCode:0
    /** type:int  是否正在进行RFID扫描*/
    property int isScaningRFID:0
    /** type:int  是否正在使用照相机*/
    property int cameraIsRuning:0
    /** type:int  是否支持Home键; 0--各个页面不做处理，直接返回到主界面; 1--各个页面做处理*/
    property int toHomeFlag:0
    /** type:int 是否正在下载数据; 0--没有; 1--正在下载*/
    property int dataLoading: 0
    /** type:int  当存在对话框时，HOME键是否可用 0--可用; 1--禁止*/
    /** type:variant  存储主界面在PageStack中的索引位置，用于一步退到主界面*/
    property variant homePageindex
    /** type:bool  是否正在升级*/
    property bool isUpdating: false
    /**升级包路径*/
    property string appFilePath: ""
    /**升级包的MD5值*/
    property string appMD5: ""
    /**升级包的sha1值*/
    property string appSHA1: ""
    /** type:bool  对话框出现时，是否支持BACK键回退*/
    property bool dialogShow: false
    /** type:bool  当前是否正在进行版本检查*/
    property bool isVersionChecked: false;
    property string messageInfoSequence: "-1"
    property string actionSequence: "-1"
    property string checkVersionSequence: "-1"
    /** type:bool  当前是否允许进入系统消息详情页*/
    property bool allowToEnterMsgPage: true

    /** type:int 当前已重复下载软件包的次数*/
    property int  downloadCount: 0;
    property alias inspectSysSetting :myInspectSysSetting

    property var versioninfo
    property bool showDeveloperModel: false
    signal refresh();
    /**
         *发开始拍照信号
         */
    signal openCamera()
    /**
         *发送开始二维码扫描信号
         */
    signal scanBarCode()
    /**
         *发送开始rfid扫描信号
         */
    signal scanRFID()
    /**
         *发送直接退至主界面信号
         */
    signal toHome()
    /**
         *发送直接退至主界面请求信号
         */
    signal toHomeBack()
    /**
         *发送back键请求信号
         */
    signal keyBackTriggered()
    /**
         *从通知栏进入消息详情的信号, 隐藏系统设置页面所有的弹出框
         */
    signal entryMsgDetailFromNotify();

    signal systemMessageUpdated(bool result);

    canDrag:false
    keyHandle: false
    //scale:1.5
    factor:gScreenInfo.displayWidth === 720 ?1.5:1
    width:480
    height:854
    appName: "deviceInspection"
    objectName: "rootWindow"
    InspectionSysSetting{
        id: myInspectSysSetting
    }
    onToHomeBack:{
        rootWindow.pageStack.pop(rootWindow.homePageindex, 1)
        toHomeFlag = 0;
    }

    //如果正在RFID，一维码扫描，拍照，数据下载中，屏蔽快捷键
    Connections{
        target:deviceInspectionEventFilter
        onShorCutPressed:{
            console.log("deviceInspectionEventFilter   ---------- keyType is ", keyType,
                        "  rootWindow.isScaning RFID ", rootWindow.isScaningRFID,
                        "  rootWindow.isScaning BarCode  ", rootWindow.isScaningBarCode,
                        "  rootWindow.data Loading  ", rootWindow.dataLoading,
                        "  rootWindow.camera IsRuning  ", rootWindow.cameraIsRuning)
            if(rootWindow.isScaningRFID == 1 || rootWindow.isScaningBarCode == 1 ||
                    rootWindow.cameraIsRuning == 1 || rootWindow.dataLoading == 1)
                return;
            if(keyType == "KEY_BARSCAN")
                rootWindow.scanBarCode();
            else if(keyType == "KEY_RFID")
                rootWindow.scanRFID();
            else if(keyType == "KEY_CAMERA")
                rootWindow.openCamera();
            else if(keyType == "KEY_HOME"){
                console.log("eyType == deviceInspectionEventFilter.KEY_HOME")
                if(rootWindow.toHomeFlag == 1) {
                    rootWindow.toHome();
                }else {
                    rootWindow.pageStack.pop(rootWindow.homePageindex,1)
                }
            }
        }
    }

    Keys.onReleased: {
        console.log("000000000000000Keys.onReleased0000000000000000000000000000000  ",event.key)
        if(event.key === Qt.Key_Return){
            gInputContext.softwareInputPanelEvent = "deviceInspection"
        }
    }

    onBackKey: {
        console.log("dialogShow = ", rootWindow.dialogShow);
        if(rootWindow.dataLoading == 1){
            console.log("rootWindow.dataLoading == 1  return;")
            return;
        }
        if(rootWindow.dialogShow === true) {
            rootWindow.keyBackTriggered();
            return;
        }

        if(pageStack.depth > 1) {
            if (inputContext.softwareInputPanelVisible)
                rootWindow.focus = true;
            else {
                pageStack.pop()
            }
        }else {
            console.log("rootWindow   quit");
            Qt.quit();
        }
    }
    Connections{
        target:networkStatus
        onNetworkConnectChanged:{
            //networkType
            if( rootWindow.isVersionChecked == false && networkType != "") {
                console.log("nwtwork work type:",networkType);
                rootWindow.getAppVersion(false);
            }
        }
    }

    //界面加载完成后开始获取系统版本，然后再获取系统消息
    Timer{
        id:initTimer
        interval: 1000
        running: false
        repeat: false
        onTriggered: {
            rootWindow.getAppVersion(false);
        }
    }

    initialPage:LoginTemplate{
        Component.onCompleted: {
            initTimer.start();
        }
    }

    CDialog {
        id:versionUpdateid
        anchors.bottom: parent.bottom;
        width: parent.width;
        height: 100;
        visible : false
        titleText: os.i18n.ctr(qsTr("Version update"));   //版本更新
        rejectButtonText: os.i18n.ctr(qsTr("Cancel"));   //取消
        acceptedButtonText: os.i18n.ctr(qsTr("Download"));          //下载
        canceledOnBackKeyReleased:true
        canceledOnOutareaClicked: true
        onAccepted: {
            rootWindow.closeKeyboard()
            //进入升级包下载安装界面。
            rootWindow.pageStack.push("qrc:/qml/SystemSettings/UpgradeProcess.qml");
        }
        onRejected: {
            console.debug("----------------onHideFinished----------------")
            rootWindow.toHomeFlag = 0;
        }
        onCanceled:{
            console.debug("----------------onCanceled----------------")
            rootWindow.toHomeFlag = 0;
        }
    }

    WaitingPage {
        id: checkVersionid
        actionType: "checkVersion"
        z:1000
        visible: false
    }

    Timer{
        id:popToFirstPageTm
        interval: 300
        running: false
        repeat: false
        onTriggered: {
            console.log("pageStack.pop(rootWindow);  ")
            rootWindow.pageStack.pop(rootWindow, true);
            console.log("pageStack.pop(rootWindow); ok ")
        }
    }

    /**
         * 退出当前界面至登录界面
         */
    function popToFirstPage() {
        console.log("0000000000000000000000000000000000")
        inspectionSystemParam.clearLoginUserInfo();
        var longinStatus = {"login":"false"};
        var values = {};
        var permissions = {};
        transmitManager.setStatus("login", longinStatus);
        transmitManager.setStatus("userInfo", values);
        transmitManager.setStatus("permission", permissions);
        popToFirstPageTm.start();
    }

    //-----------add by he   kun

    /**
         * 获取服务器端存储的手机软件版本。
         */
    function getAppVersion(manual) {
        //network is OK? if not, DO Nothing;
        rootWindow.checkVersionSequence = inspectionSystemConfig.getSequence();
        if(manual){
            rootWindow.checkVersionSequence += "_manual";
            showCheckUpgradeVersion(UI.SHOW_CHECK_VERSION_PAGE);
        }else
            rootWindow.checkVersionSequence += "_auto";
        var appId = inspectionSystemConfig.getAppId();
        var head = {"Objects" : "DEX_GetApp_One"};
        var para = {"AppId" : appId};
        console.log("transmitManager.requestData   DEX_GetApp_One")
        transmitManager.resultOfResponse.disconnect(rootWindow.versionCheckResult);
        transmitManager.resultOfResponse.connect(rootWindow.versionCheckResult);
        transmitManager.requestData(head, para, rootWindow.checkVersionSequence, "", true, true);
    }

    /**
       * 获取用户维修流程
       **/
    function getFaultActList(){
        rootWindow.actionSequence = inspectionSystemConfig.getSequence();
        transmitManager.resultOfResponse.disconnect(rootWindow.resultResponse)
        transmitManager.resultOfResponse.connect(rootWindow.resultResponse)

        var head1 = {"Objects" : "EMF_FaultAct_List"};
        var para1 = {"UserID" : inspectionSystemParam.getUserId()};
        console.log("transmitManager.requestData   EMF_FaultAct_List ")
        transmitManager.requestData(head1, para1, rootWindow.actionSequence, "获取用户维修流程失败");
    }

    /**
        * 获取标准化检查任务列表
        */
    function getStandardizedInspectionTaskList() {
        rootWindow.actionSequence = inspectionSystemConfig.getSequence();
        transmitManager.resultOfResponse.disconnect(rootWindow.resultResponse)
        transmitManager.resultOfResponse.connect(rootWindow.resultResponse)
        var head = {};
        head["Objects"] = "EXAM_ExamineTaskList";
        var para = {};
        para["UserID"] = inspectionSystemParam.getUserId();
        console.log("transmitManager.requestData   EXAM_ExamineTaskList ")
        transmitManager.requestData(head, para, rootWindow.actionSequence, os.i18n.ctr(qsTr("Get standardized inspection list fail")));
    }

    /**
         * 获取设备盘点任务列表
         */
    function getInventoryTaskList() {
        rootWindow.actionSequence = inspectionSystemConfig.getSequence();
        transmitManager.resultOfResponse.disconnect(rootWindow.resultResponse)
        transmitManager.resultOfResponse.connect(rootWindow.resultResponse)

        var head = {};
        head["Objects"] = "EMI_InventoryTask_RuningList";
        var para = {};
        para["UserID"] = inspectionSystemParam.getUserId();
        console.log("transmitManager.requestData   EMI_InventoryTask_RuningList ")
        transmitManager.requestData(head, para, rootWindow.actionSequence, os.i18n.ctr(qsTr("Get inventory task fail")));
    }
    /**
         * 获取设备巡检任务列表
         */
    function getInspectionTaskList() {
        rootWindow.actionSequence = inspectionSystemConfig.getSequence();
        transmitManager.resultOfResponse.disconnect(rootWindow.resultResponse);
        transmitManager.resultOfResponse.connect(rootWindow.resultResponse);
        var head = {"Objects" : "EISM_GetUserRunningTask"};
        var para = {"UserID" : inspectionSystemParam.getUserId()};
        console.log("transmitManager.requestData   EISM_GetUserRunningTask ")
        transmitManager.requestData(head, para, rootWindow.actionSequence, os.i18n.ctr(qsTr("Get inspection task fail")));
    }
    /**
         * 获取设备保养任务列表
         */
    function getDeviceMaintainTaskList() {
        rootWindow.actionSequence = inspectionSystemConfig.getSequence();
        transmitManager.resultOfResponse.disconnect(rootWindow.resultResponse)
        transmitManager.resultOfResponse.connect(rootWindow.resultResponse)
        var head = {"Objects" : "EMM_GetUserRunningTask"};
        var para = {"UserID" : inspectionSystemParam.getUserId()};
        console.log("transmitManager.requestData   EMM_GetUserRunningTask ")
        transmitManager.requestData(head, para, rootWindow.actionSequence, "获取保养任务列表失败");
    }

    /**
         * 获取设备保养任务列表
         */
    function getDataReportTaskList(){
        rootWindow.actionSequence = inspectionSystemConfig.getSequence();
        transmitManager.resultOfResponse.disconnect(rootWindow.resultResponse)
        transmitManager.resultOfResponse.connect(rootWindow.resultResponse)
        var head = {"Objects" : "DFI_ReportTask_RuningList"};
        var para = {"UserID" : inspectionSystemParam.getUserId()};
        console.log("transmitManager.requestData   DFI_ReportTask_RuningList ")
        transmitManager.requestData(head, para,  rootWindow.actionSequence, "获取数据上报任务列表失败");
    }

    function getDailyTaskList(){
        rootWindow.actionSequence = inspectionSystemConfig.getSequence();
        transmitManager.resultOfResponse.disconnect(rootWindow.resultResponse)
        transmitManager.resultOfResponse.connect(rootWindow.resultResponse)
        var head = {"Objects" : "EISM_GetDailyRunningTask"};
        var para = {"UserID" : inspectionSystemParam.getUserId()};
        console.log("transmitManager.requestData   EISM_GetDailyRunningTask ")
        transmitManager.requestData(head, para, rootWindow.actionSequence, "获取日常任务列表失败");
    }

    function getKnowledgeTaskList(){
        transmitManager.resultOfResponse.disconnect(rootWindow.resultResponse)
        transmitManager.resultOfResponse.connect(rootWindow.resultResponse)
        var head = {"Objects" : "EMK_knowledge_all"};
        var para = {};
        console.log("transmitManager.requestData   EMK_knowledge_all ")
        transmitManager.requestData(head, para, rootWindow.actionSequence, "获取知识库列表失败");
    }

    function formatDate(d) {
        var years = add_zero(d.getFullYear());
        var month = add_zero(d.getMonth()+1);
        var days = add_zero(d.getDate());
        var hours = add_zero(d.getHours());
        var minutes = add_zero(d.getMinutes());
        var seconds =  add_zero(d.getSeconds());
        var ndate = years+"-"+month+"-"+days+" "+hours+":"+minutes+":"+seconds;
        return ndate;
    }

    /**
         * 获取当前日期及时间.时间格式:“1900-07-05 12：00：00”
         */
    function getCurDate() {
        var d = new Date();
        var years = add_zero(d.getFullYear());
        var month = add_zero(d.getMonth()+1);
        var days = add_zero(d.getDate());
        var hours = add_zero(d.getHours());
        var minutes = add_zero(d.getMinutes());
        var seconds=  add_zero(d.getSeconds());
        var ndate = years+"-"+month+"-"+days+" "+hours+":"+minutes+":"+seconds;
        return ndate;
    }

    /**
         * 如果是个位数，加前缀“0”;
         * @param temp:string 整数值
         * @return string 加前缀“0”之后的整数值
         */
    function add_zero(temp) {
        if(temp<10) return "0"+temp;
        else return temp;
    }

    function versionCheckResult(result, error, action, map){
        if(action != rootWindow.checkVersionSequence){
            console.log("action != rootWindow.checkVersionSequence  return")
            return;
        }
        transmitManager.resultOfResponse.disconnect(rootWindow.versionCheckResult);
        if(result && error == 0) {
            if(map["tables"].length <= 0 || map["tables"][0]["value"] == undefined || map["tables"][0]["value"].length <= 0){
                if(String(action).indexOf("_manual") != -1){
                    CTOAST.requestToast("没有可用更新","","");
                    rootWindow.showCheckUpgradeVersion(UI.CLEAR_VERSION_PAGE);
                }
                return;
            }

            //获取版本
            rootWindow.isVersionChecked = true;
            var val = map["tables"][0]["value"][0];
            var localversion = inspectionSystemConfig.getVersionInfo();
            console.log("local version:",localversion, "  update version ", val["AppVersionCode"]);
            if((val == undefined || localversion == "")) {
                if(String(action).indexOf("_manual") != -1){
                    CTOAST.requestToast(os.i18n.ctr(qsTr("Requeset version failed")),"",""); //获取版本失败
                    rootWindow.showCheckUpgradeVersion(UI.CLEAR_VERSION_PAGE);
                }
            }else if(Number(val["AppVersionCode"] <= Number(localversion))){
                if(String(action).indexOf("_manual") != -1){
                    CTOAST.requestToast(os.i18n.ctr(qsTr("No updates available")),"","");     //没有可用更新
                    rootWindow.showCheckUpgradeVersion(UI.CLEAR_VERSION_PAGE);
                }
            } else {
                rootWindow.appFilePath = val["AppFilePath"];
                rootWindow.appMD5 = val["AppMD5"];
                rootWindow.appSHA1 = val["AppSHA1"];
                rootWindow.showCheckUpgradeVersion(UI.SHOW_VERSION_UPDATE_PAGE);
            }
        } else if((String(action).indexOf("_manual") != -1)){
            if(error == -100) {
                CTOAST.requestToast(os.i18n.ctr(qsTr("Server IP error")),"",""); //服务器ip地址错误
            }else if(error <= -1 && error >= -7) {
                CTOAST.requestToast(os.i18n.ctr(qsTr("Network Connection error")),"",""); //网络连接错误
            } else if(error == 1) {
                CTOAST.requestToast(os.i18n.ctr(qsTr("Data Decode error")),"","");  //数据包解码错误
            } else if(error == -8){
                CTOAST.requestToast(os.i18n.ctr(qsTr("No network connect")),"","");
            }
            //查询升级包失败
            rootWindow.showCheckUpgradeVersion(UI.CLEAR_VERSION_PAGE);
        }
    }

    /**
         * 接受服务器返回数据的槽函数
         * @param result:bool 获取数据是否成功
         * @param error:int 错误代码
         * @param action:string 当前发送数据动作类型
         * @param map:var    服务器返回值
         */
    function resultResponse(result, error, action, map) {
        console.debug("main.qml -onResultOfResponse--------------result, error, action  actionSequence",
                      result, error, action, rootWindow.actionSequence);
        if(action != rootWindow.actionSequence){
            console.log("action != rootWindow.actionSequence  return")
            return;
        }
        transmitManager.resultOfResponse.disconnect(resultResponse)
        if(result && error == 0) {
            if(map["head"]["objects"] == "EISM_GetUserRunningTask") {
                //巡检任务
                var otherParam = {"EnabledScanItem": false}
                pageStack.push("qrc:/qml/InspectionTask/InspectionBaseList.qml",
                               {"originalData": map,
                                   "otherParam": otherParam,
                                   "nameId":"Inspection",
                                    "taskActionSequence" : rootWindow.actionSequence
                               });
            }else if(map["head"]["objects"] == "EMM_GetUserRunningTask") {
                //设备保养
                var maintainOtherParam = { "enabledScanItem": false}
                pageStack.push("qrc:/qml/DeviceMaintainTask/MaintainBaseList.qml",
                               {"originalData": map,
                                   "otherParam": maintainOtherParam,
                                   "nameId":"Maintenance",
                                   "taskActionSequence" : rootWindow.actionSequence
                               });
            }else if(map["head"]["objects"]  == "EMI_InventoryTask_RuningList") {
                //盘点任务
                var inventoryOtherParam = { "enabledScanItem": false}
                pageStack.push("qrc:/qml/InventoryTask/InventoryBaseList.qml",
                               {"originalData": map,
                                   "otherParam": inventoryOtherParam,
                                   "nameId":"Inventory",
                                   "taskActionSequence" : rootWindow.actionSequence
                               });
            } else if(map["head"]["objects"] == "EXAM_ExamineTaskList") {
                //标准化检查
                var stdOtherParam = { "enabledScanItem": false}
                pageStack.push("qrc:/qml/StandardizedTask/StandardBaseList.qml",
                               {"originalData": map,
                                   "otherParam": stdOtherParam,
                                   "nameId":"Standardization",
                                   "taskActionSequence" : rootWindow.actionSequence
                               });
            } else if(map["head"]["objects"] == "DFI_ReportTask_RuningList") {
                //数据上报
                var reportOtherParam = { "enabledScanItem": false}
                pageStack.push("qrc:/qml/DataReport/DataReportBaseList.qml",
                               {"originalData": map,
                                   "otherParam": reportOtherParam,
                                   "nameId":"DataReport"
                               });
            } else if(map["head"]["objects"] == "EISM_GetDailyRunningTask") {
                //日常任务
                console.log("EISM_GetDailyRunningTask    response")
                var dailyOtherParam = { "EnabledScanItem": false}
                pageStack.push("qrc:/qml/DailyTask/DailyTaskBaseList.qml",
                               {"originalData": map,
                                   "otherParam": dailyOtherParam,
                                   "nameId":"DailyTask",
                                   "taskActionSequence" : rootWindow.actionSequence
                               });
            } else if(map["head"]["objects"] == "EMK_knowledge_all"){
                //日常任务
                console.log("EISM_GetDailyRunningTask    response")
                var knowledgeOtherParam = { "EnabledScanItem": false}
                pageStack.push("qrc:/qml/Knowledge/Knowledge.qml",
                               {"originalData": map,
                                   "otherParam": knowledgeOtherParam,
                                   "nameId":"KnowLedge"
                               });
            }

            else if(map["head"]["objects"] == "EMF_FaultAct_List"){
                //获取用户维修流程
                pageStack.push("qrc:/qml/DeviceOperationTask/OperationAndMaintenance.qml",
                                                               {"originalData": map, "nameId":"Operation"});
            }
        }
    }

    /**
         * 根据检测升级步骤显示不同界面
         * @param step:string 升级步骤

         */
    function showCheckUpgradeVersion(step) {
        rootWindow.toHomeFlag = 1;
        if(String(step) == UI.SHOW_CHECK_VERSION_PAGE) {
            console.log("show check version");
            checkVersionid.visible = true;
        } else if(String(step) == UI.SHOW_VERSION_UPDATE_PAGE) {
            //show version update
            console.log("show version update");
            checkVersionid.visible = false;
            if(networkStatus.getNetworkConnectStatus() == "wifi") {
                versionUpdateid.messageText = os.i18n.ctr(qsTr("Is download package?"));
            }else if(networkStatus.getNetworkConnectStatus() == "cellular") {
                versionUpdateid.messageText = os.i18n.ctr(qsTr("You are currently using net data flow, is download package?"));
            }
            versionUpdateid.show();
        } else if(String(step) == UI.CLEAR_VERSION_PAGE) {
            //clear version
            console.log("String(step) == UI.CLEAR_VERSION_PAGE")
            rootWindow.toHomeFlag = 0;
            checkVersionid.visible = false;
            versionUpdateid.hide();
        }
    }

    /**
     * 进入消息详情页
     */
    function entryMsgDetail(uuid) {
        console.log("----------entry MsgDetail:----------messageId:", uuid);
        //when upgardeing , notify user and reture
        if(isUpdating == true) {
            CTOAST.requestToast( os.i18n.ctr(qsTr("while updating system, forbid enter system message page.")),"","");     //系统升级期间，禁止查看系统消息
            return;
        }
        console.log("rootWindow.allowTo EnterMsgPage == ", rootWindow.allowToEnterMsgPage);

        if(rootWindow.allowToEnterMsgPage == false) {
            return;
        }

        versionUpdateid.hide();
        rootWindow.entryMsgDetailFromNotify();
        pageStack.push("qrc:/qml/SystemMessage/SystemMsgs.qml");
    }

    /**
     *解析theme.xml中color字段值。
     */
    function getColor(config) {
        //console.log("config=",config);
        if(config == undefined || config == "") {
            return "";
        }
        var type = config.split("@");
        if(type == undefined) {
            return "";
        }
        if(type[0] == "color") {
            return type[1];
        }
        return "";
    }
    /**
     *将本地文件完整路径转为URL格式
     * @param fileName:string 本地文件完整路径
     */
    function createLocalURL(fileName) {
        console.log("fileName = ", fileName);
        if(fileName == undefined || fileName == "") {
            return "";
        }
        var fullPath = "file:///";
        fullPath +=  inspectionFuncsItemModel.getImagePath();
        fullPath += fileName;
        return fullPath;
    }

    function closeKeyboard(){
        if(gInputContext.softwareInputPanelVisible){
            gAppUtils.closeSoftKeyboard()
        }
    }
}
