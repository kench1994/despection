/*!
* \file HomePage.qml
* \brief 主界面
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
import myInspectionSysSetting 1.0
import "../Common"
import "../Common/constants.js" as UI
/*!
  \class HomePage
  \inModule 主界面模块
  \brief 主界面
  \ingroup LoginAndHome
  主界面，根据登录用户权限显示其所有可用功能。
*/

CommonPage {
    id: homePage
    /** type:Component  主页面组件*/
    property Component component: null;
    property string actionSequence: "-1"

    /** type:alias  主界面标题背景图片位置*/
    property alias titleImage: imgTitle.source;
    /** type:alias  主界面标题名称*/
    property alias titleText: titleName.text;
    /** type:alias  主界面退出按钮文本内容*/
    property alias logoutText: exitBtn.text;
    /** type:alias  主界面退出按钮文本颜色*/
    property alias logoutTextColor: exitBtn.textColor;
    /** type:string  主界面退出按钮颜色*/
    property string logoutBtnColor;
    /** type:double  主界面正常状态下透明度*/
    property double logoutBtnNormalOpacity: 1.0;
    /** type:double  主界面点击状态下透明度*/
    property double logoutBtnPressedOpacity: 0.0;

    statusBarHoldEnabled: false
    orientationPolicy: CPageOrientation.LockPortrait
    clip: true

    contentAreaItem:Rectangle{
        anchors.fill: parent
        color:"#eeeff1"
        Image {
            id: imgTitle
            anchors.top: parent.top;
            width: parent.width;
            height: 110;
            Text {
                id: titleName;
                font.pixelSize: 24;
                anchors.centerIn: parent;
                color: "white";
            }
        }
        Component {
            id: contactDelegate
            Item {
                //单个表格
                width: grid.cellWidth;
                height: grid.cellHeight;
                Rectangle {
                    width: parent.width-10
                    height: parent.height - 10
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    color: modulMouse.pressed ? cellPressedColor : cellNormalColor;
                    Column {
                        width: parent.width
                        Rectangle {
                            //角标
                            height: 20
                            radius: height/2;
                            anchors.right: parent.right;
                            color: texCount.text != "0"?"red":"#00000000"
                            width: texCount.width + 10;
                            Text {
                                id: texCount;
                                text: newsCount
                                color: texCount.text != "0"?"#ffffff":"#00000000";
                                font.pixelSize: 14;
                                anchors.centerIn: parent;
                            }
                        }
                        Rectangle {
                            width: parent.width;
                            height: 10;
                            color: modulMouse.pressed ? cellPressedColor : cellNormalColor;
                        }

                        Rectangle {
                            //图标
                            width: parent.width;
                            height: 40;
                            color: modulMouse.pressed ? cellPressedColor : cellNormalColor ;
                            Image {
                                id: img
                                anchors.centerIn: parent;
                                source: portrait;
                                width: 40;
                                fillMode: Image.Stretch;
                            }
                        }
                        Rectangle {
                            width: parent.width;
                            height: 16;
                            color: modulMouse.pressed ? cellPressedColor : cellNormalColor ;
                        }
                        Rectangle {
                            //功能名称
                            width: parent.width;
                            height: 60;
                            color: modulMouse.pressed ? cellPressedColor : cellNormalColor;
                            Text {
                                anchors.horizontalCenter: parent.horizontalCenter;
                                id: texName;
                                text: name;
                                color: "#333333";
                                font.pixelSize: 22;
                            }
                        }
                    }
                    MouseArea{
                        id:modulMouse
                        anchors.fill: parent
                        onClicked: {
                            console.log("index is ", index, "  nameid   ", nameid)
                            if(nameid == UI.THEME_NAMEID_INSPECTION)   {
                                //设备巡检
                                rootWindow.getInspectionTaskList();
                            } else if(nameid == UI.THEME_NAMEID_INVENTORY) {
                                //设备盘点,"Inventory"
                                rootWindow.getInventoryTaskList();
                            } else if(nameid == UI.THEME_NAMEID_DEVICE_QUERY) {
                                //设备查询,DeviceQuery
                                pageStack.push("qrc:/qml/Common/SearchPage.qml",{
                                                   "titleText": name,
                                                   "pageActionType": "device search",
                                                   "enabledScanItem": true});
                            } else if(nameid == UI.THEME_NAMEID_OPERATION) {
                                //设备维修, Operation
                                rootWindow.getFaultActList()
                            } else if(nameid == UI.THEME_NAMEID_COMPREHENSIVE_BUSINESS) {
                                //综合业务, ComprehensiveBusiness
                                pageStack.push("qrc:/qml/ComprehensiveTask/Comprehensive.qml", {"nameId":nameid})
                            } else if(nameid == UI.THEME_NAMEID_SETTINGS) {
                                //系统设置Settings
                                console.log("rootWindow.entry SystemSettingPage(); ")
                                pageStack.push("qrc:/qml/SystemSettings/SystemSetting.qml", {"nameId":nameid})
                            } else if(nameid == UI.THEME_NAMEID_RECTIFICATION) {
                                //检查整改,Rectification
                                pageStack.push("qrc:/qml/RectificationTask/Rectification.qml", {"nameId":nameid});
                            } else if(nameid == UI.THEME_NAMEID_STANDARDIZATION) {
                                //标准化检查,Standardization
                                rootWindow.getStandardizedInspectionTaskList()
                            } else if(nameid == UI.THEME_NAMEID_USER_CARD) {
                                //用户卡片,UserCard
                                pageStack.push("qrc:/qml/LoginAndHome/UserCard.qml", {"nameId":nameid});
                            } else if( nameid == UI.THEME_NAMEID_MESSAGES) {
                                //系统消息,Messages
                                console.log("entry rootWindow.entrySystemMsgsPage() ")
                                pageStack.push("qrc:/qml/SystemMessage/SystemMsgs.qml", {"nameId":nameid})
                            } else if( nameid == UI.THEME_NAMEID_MAINTENANCE) {
                                //设备保养,Maintenance
                                rootWindow.getDeviceMaintainTaskList();
                            } else if( nameid == UI.THEME_NAMEID_DATAREPORT) {
                                //数据上报, DataReport
                                rootWindow.getDataReportTaskList();
                            }else if( nameid == UI.THEME_NAMEID_DAILYTASK) {
                                //数据上报, DataReport
                                rootWindow.getDailyTaskList();
                            }else if( nameid == UI.THEME_NAMEID_SCANTASK) {
                                //扫码任务, ScanTask
                                pageStack.push("qrc:/qml/ScanTask/ScanTask.qml")
                            }else if( nameid == UI.THEME_NAMEID_KNOWLEDGE) {
                                //知识库, KnowLedge
                                rootWindow.getKnowledgeTaskList();
                            }
                        }
                    }
                }
            }
        }

        Item{
            id:deviceGrid
            width:  parent.width - 10;
            height: parent.height-imgTitle.height;
            anchors.top: imgTitle.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            GridView {
                id: grid
                width:  parent.width;
                height: cellWidth * 4;
                anchors.top:parent.top;
                anchors.topMargin: 10;
                //interactive: false
                cellWidth: 156;
                cellHeight: 156;
                model: deviceListModel;
                delegate: contactDelegate
                clip: true
            }

            CommonButton {
                id: exitBtn
                backgroundColor:logoutBtnColor
                pixelSize:22
                anchors {
                    top: grid.bottom
                    topMargin: 20
                    horizontalCenter: parent.horizontalCenter
                }
                onClicked: {
                    inspectionSystemParam.clearLoginUserInfo();
                    var longinStatus = {"login":"false"};
                    var values = {};
                    var permissions = {};
                    transmitManager.setStatus("login", longinStatus);
                    transmitManager.setStatus("userInfo", values);
                    transmitManager.setStatus("permission", permissions);
                    pageStack.pop();
                }
            }
        }

        ListModel {
            id: deviceListModel;
        }
    }

    /**
     * 读取theme.xml，设置主界面皮肤
     */

    function createListElement() {
        console.log("createListElement.--------------------------------------------------");
        var homeLogo = inspectionFuncsItemModel.getStaticParas("HomeLogo");
        homePage.titleImage = rootWindow.createLocalURL(homeLogo["Display_normal"]);
        homePage.titleText = homeLogo["NameCN"];

        var logoutConfig = inspectionFuncsItemModel.getStaticParas("LogoutBtn");
        homePage.logoutText = logoutConfig["NameCN"];
        homePage.logoutBtnColor = rootWindow.getColor(logoutConfig["Display_normal"]);
        homePage.logoutBtnNormalOpacity = Number(logoutConfig["Opacity_normal"]).toPrecision(2);
        homePage.logoutBtnPressedOpacity = Number(logoutConfig["Opacity_press"]).toPrecision(2);
        homePage.logoutTextColor = rootWindow.getColor(logoutConfig["Name_color"]);
        var i = 0;
        var type = myInspectSysSetting.getOfflineMode();
        for(; i<inspectionFuncsItemModel.getIconCount(); i++) {
            var node = inspectionFuncsItemModel.getNodeParas(i, "");
            var nameid = node["ID"];
            var name = node["NameCN"];
            var cellNormalColor = rootWindow.getColor(node["Background_normal"]);
            var cellPressedColor = rootWindow.getColor(node["Background_press"]);
            var portrait = rootWindow.createLocalURL(node["Display_normal"]);
            console.log(" 1111111111 nameid is ", nameid);
            if(name == undefined || nameid == undefined || cellNormalColor == "" || cellPressedColor == ""
                    || inspectionSystemParam.checkUserPermission(nameid) == false) {
                console.log("name == undefined || nameid == undefined continue || Permission fail ", nameid)
                continue;
            }
            if((nameid == UI.THEME_NAMEID_KNOWLEDGE || nameid == UI.THEME_NAMEID_SCANTASK) && type == "2"){
                continue;
            }
            var moduleID = inspectionSystemParam.getUserPermissionData(nameid, "ModuleID");
            if(inspectionSystemConfig.getClientPriority() == false){
                name = inspectionSystemParam.getUserPermissionData(nameid, "ModuleName");
                console.log("client priority false name is ", name);
            }
            if(name == "")
                name = node["NameCN"];

            if(nameid == UI.THEME_NAMEID_INSPECTION){
                deviceListModel.append({"name":"日常任务", "portrait":portrait, "nameid":UI.THEME_NAMEID_DAILYTASK,
                                           "cellNormalColor":cellNormalColor, "cellPressedColor":cellPressedColor,  "newsCount":"0", "moduleID":"default"});
            }

            deviceListModel.append({"name":name, "portrait":portrait, "nameid":nameid,
                                       "cellNormalColor":cellNormalColor, "cellPressedColor":cellPressedColor, "newsCount":"0", "moduleID":moduleID});
        }
    }

    /**
     * 对服务器返回数据进行处理的槽函数，用于生成新任务角标
     * @param result:bool 获取数据是否成功
     * @param error:int 错误代码
     * @param action:string 当前发送数据动作类型
     * @param map:var    服务器返回值
     */
    function tasksResultResponse(result, error, action, map) {
        console.log("----Module_Query_ItemCounts_ForAndroid----onResultOfResponse------result, error, action:", result, error, action);
        if(action  != homePage.actionSequence) {
            console.log("action  != homePage.actionSequence return")
            return;
        }
        transmitManager.resultOfResponse.disconnect(homePage.tasksResultResponse);
        if(result && error == 0) {
            var i;
            console.log("set  corner    ",  map["tables"][0]["value"].length)
            for (i = 0; i < map["tables"][0]["value"].length; i++) {
                var values = map["tables"][0]["value"][i];
                var moduleID = values["ModuleID"];
                var number = values["ItemCount"];
                console.log("Set Corner Icon ------ ", moduleID, " : ", number);
                var j;
                for(j = 0; j < deviceListModel.count; j++) {
                    if(deviceListModel.get(j).moduleID == moduleID) {
                        console.log("Set ", deviceListModel.get(j).nameid, " to ", number);
                        deviceListModel.setProperty(j, "newsCount", number);
                        break;
                    }
                }
            }
            grid.update();
        }
        //读取系统消息角标
        var head = {"Objects" : "Module_Query_MessageCounts_ForAndroid"};
        var para = {};
        console.log("transmitManager.requestData   requestUserTaskCount Module_Query_MessageCounts_ForAndroid")
        homePage.actionSequence = inspectionSystemConfig.getSequence();
        transmitManager.resultOfResponse.disconnect(homePage.systemMessageCountResponse);
        transmitManager.resultOfResponse.connect(homePage.systemMessageCountResponse);
        transmitManager.requestData(head, para, homePage.actionSequence, "", true, false);
    }

    function systemMessageCountResponse(result, error, action, map) {
        console.log("----Module_Query_ItemCounts_ForAndroid----onResultOfResponse------result, error, action:", result, error, action);
        if(action  != homePage.actionSequence) {
            console.log("action  != homePage.actionSequence return")
            return;
        }
        transmitManager.resultOfResponse.disconnect(homePage.systemMessageCountResponse);
        //Messages
        if(result && error == 0) {
            var msgCount = map["tables"][0]["value"][0]["MessageCount"];
            console.log("get msgCOunt i s ", msgCount)
            for(var j = 0; j < deviceListModel.count; j++) {
                if(deviceListModel.get(j).nameid == UI.THEME_NAMEID_MESSAGES) {
                    console.log("Set ", deviceListModel.get(j).nameid, " to ", msgCount);
                    deviceListModel.setProperty(j, "newsCount", msgCount);
                    break;
                }
            }
            grid.update();
        }
    }

    onStatusChanged: {
        if (status == CPageStatus.Show) {
            console.log(" homePage ttttttttttttttttttttttttt CPageStatus.Show ", status)
            rootWindow.dialogShow = true;
            homePage.statusBarHoldEnabled = false
            gScreenInfo.setStatusBarStyle("transblack");
            gScreenInfo.setStatusBar(true);
            // Fetch tasks count information from server..
            homePage.actionSequence = inspectionSystemConfig.getSequence();
            transmitManager.resultOfResponse.disconnect(homePage.tasksResultResponse);
            transmitManager.resultOfResponse.connect(homePage.tasksResultResponse);
            var head = {"Objects" : "Module_Query_ItemCounts_ForAndroid"};
            var para = {"UserID" : inspectionSystemParam.getUserId()};
            console.log("transmitManager.requestData   requestUserTaskCount Module_Query_ItemCounts_ForAndroid")
            transmitManager.requestData(head, para, homePage.actionSequence, "", true, false);
            console.log("homePage begine gc")
            gc();
            console.log("homePage  gc  ok")
        } else if (status == CPageStatus.WillHide) {
            rootWindow.dialogShow = false;
            console.log("HomePage.qml  willHide")
        }
    }

    Component.onCompleted: {
        console.log("HomePage.qml   Component.onCompleted ")
        var currentStyle = inspectionFuncsItemModel.getStyle();
        if(currentStyle != "SyberOS"){
            console.log("unsupported style:", currentStyle);
            return;
        }
        rootWindow.homePageindex = homePage;
        homePage.createListElement();
    }

    Component.onDestruction: {
        console.log("HomePage.qml   Component.onDestruction disconnect homePage.tasksResultResponse ")
        transmitManager.resultOfResponse.disconnect(homePage.tasksResultResponse);
    }
}

