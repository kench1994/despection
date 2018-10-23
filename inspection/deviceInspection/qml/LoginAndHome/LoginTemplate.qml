/*!
* \file LoginTemplate.qml
* \brief 登录界面默认模板
*
*
*
*\author 何昆 hekun@syberos.com
*\version 1.0
*\date 2015/9/9
*/

import QtQuick 2.0
import com.syberos.basewidgets 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.1
import "../Common"
import "../Common/constants.js" as UI
/*!
  \class LoginTemplate
  \inModule 换肤模块
  \brief 登录界面默认模板
  \ingroup default
  登录界面，支持换肤功能
*/
CommonPage {
    id:loginTemplate
    /** type:var  登录界面LOGO位置*/
    property var logoImage;
    /** type:var  登录按钮文本内容*/
    property var btnContent;
    /** type:var  登录按钮文本颜色*/
    property var btnContentColor;
    /** type:var  登录按钮正常显示透明度*/
    property var btnNormalOpacity;
    /** type:var  登录按钮按压时显示的透明度*/
    property var btnPressOpacity;
    /** type:var  登录按钮颜色*/
    property var btnColor;
    /** type:var  登录按钮按压时的文本内容*/
    property var btnContentPress;
    /** type:var  公司名称*/
    property alias companyName: textCompanyName.text;
    /** type:var  产品名称*/
    property alias productName: textProductName.text;
    /** type:var  用户名输入框左侧图标*/
    property alias accountInputLabelImg: accountInput.labelIconPath;
    /** type:var  用户名输入框下拉列表图标*/
    property alias accountInputDropListImg: accountInput.dropListImagePath;
    /** type:var  密码输入框左侧图标*/
    property alias pwdInputLabelImg: pwdInput.labelIconPath;
    /** type:var  用户名列表是否显示*/
    property alias userNameVisible: rectUserName.visible;

    property string actionSequence : "-1"
    property string historyUserActionSequence : "-1"
    color:"#eeeff1"
    statusBarHoldEnabled: false
    statusBarHoldItemColor: "white"
    Connections{
        target: gInputContext
        onSoftwareInputPanelEventChanged:{
            if(gInputContext.softwareInputPanelEvent === "deviceInspection"){
                if(accountInput.isFocus){
                    pwdInput.forceActiveFocus()
                } else if(pwdInput.isFocus){
                    loginTemplate.login()
                }
            }
        }
    }

    contentAreaItem:  Item {
        anchors.fill: parent
        Flickable {
            anchors.fill: parent
            boundsBehavior: Flickable.StopAtBounds
            contentHeight: contentCol.height
            MouseArea {
                id:allMouse
                anchors.fill: parent
                onClicked: {
                    console.log("all mouse   click")
                    //点击登录界面的任何地方，用户列表收起
                    if(accountInput.dropListDown == true)
                        accountInput.dropListDown = false;
                }
            }
            Column{
                id:contentCol
                width: parent.width
                Item{
                    id:logo
                    width: parent.width
                    height: rootWindow.height/3
                    Image {
                        id:productImg
                        width: 128;
                        height: 128;
                        source: logoImage;
                        anchors.top:parent.top
                        anchors.topMargin: 60
                        anchors.horizontalCenter: parent.horizontalCenter
                        fillMode: Image.Stretch;
                    }
                    Text {
                        id: textCompanyName
                        anchors.top:productImg.bottom
                        anchors.topMargin: 25
                        anchors.horizontalCenter: parent.horizontalCenter;
                        font.pixelSize: 22;
                        color: "#333333";
                    }
                    Text {
                        id: textProductName
                        anchors.top: textCompanyName.bottom
                        anchors.topMargin: 8
                        anchors.horizontalCenter: parent.horizontalCenter;
                        font.pixelSize: 18;
                        color: "#999999";
                    }
                }

                Item{
                    id:loginItem
                    width: parent.width
                    height: rootWindow.height/3
                    /*帐号输入部分*/
                    ListInputItem {
                        id:accountInput
                        height: 70
                        width: 440
                        anchors.horizontalCenter: parent.horizontalCenter
                        showType:"image"
                        showDropList: true
                        z:pwdInput.z + 1
                        inputMethodHints:Qt.ImhPreferLatin
                        inputMethodHintExtensions:{
                            var args = {};
                            args["enterKeyText"]="next"
                            return args
                        }
                        onTextChanged: {

                            var len = text.length;
                            var i = 0;
                            rectUserName.showCount = nameModel.count;
                            if(text == ""){
                                for(; i < nameModel.count; i++){
                                    nameModel.setProperty(i, "hide", false);
                                }
                            }

                            for(; i < nameModel.count; i++){
                                var prefixeStr = String(nameModel.get(i).name).substr(0, len);
                                if(text != prefixeStr){
                                    nameModel.setProperty(i, "hide", true);
                                    rectUserName.showCount--;
                                }else{
                                    nameModel.setProperty(i, "hide", false);
                                }
                            }
                        }

                        Component {
                            id: userItem;
                            Rectangle {
                                width: rectUserName.width;
                                visible: !hide
                                height: visible? 70 : 0;
                                color: userItmeMouse.pressed ? "#ededed" : "white";
                                border.color: "#ededed"
                                Text {
                                    id: texName
                                    anchors.verticalCenter: parent.verticalCenter;
                                    anchors.left: parent.left;
                                    anchors.leftMargin: 20;
                                    text: name;
                                    color: "#333333";
                                    font.pixelSize: 22;
                                }
                                MouseArea {
                                    id:userItmeMouse;
                                    anchors.fill: parent;
                                    onClicked: {
                                        accountInput.inputText = name;
                                        accountInput.dropListDown = false;
                                        pwdInput.forceActiveFocus();
                                    }
                                }
                            }
                        }

                        ListModel{
                            id:nameModel
                        }
                        ListView {
                            id: rectUserName;
                            anchors.top: accountInput.bottom;
                            width: accountInput.width;
                            visible: accountInput.dropListDown
                            property int showCount: 0
                            model: nameModel;
                            delegate: userItem;
                            boundsBehavior:Flickable.StopAtBounds
                            height: showCount * 70 > 280 ? 280 : showCount * 70
                            clip: true;
                            Component.onCompleted: {
                                historyUsers(true, "", "");
                            }
                        }

                    }

                    ListInputItem{
                        id:pwdInput
                        height: 70
                        width: 440
                        anchors.top: accountInput.bottom
                        anchors.topMargin: 20
                        anchors.horizontalCenter: parent.horizontalCenter
                        showType:"image"
                        passwordEnabled:true
                        echoMode:TextInput.Password
                        inputMethodHints:Qt.ImhPreferLatin
                        inputMethodHintExtensions:{
                            var args = {};
                            args["enterKeyText"]="done"
                            return args
                        }
                    }

                    CommonButton {
                        id: btnLogin
                        backgroundColor:btnColor
                        pixelSize:22
                        text:btnContent
                        textColor:btnContentColor
                        enabled:accountInput.inputText !== "" && pwdInput.inputText !== ""
                        anchors {
                            top: pwdInput.bottom
                            topMargin: 40
                            horizontalCenter: parent.horizontalCenter
                        }

                        onClicked: {
                            loginTemplate.login()

                        }
                    }
                }

                Item{
                    id:enterSettingsItem
                    width: parent.width
                    height: rootWindow.height/3
                    CommonButton{
                        id:rfidLoginBtn
                        width:180
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 150
                        text:"扫描员工卡登录"
                        pixelSize:22
                        onClicked:{
                            nfcLogin.visible = true;
                            rootWindow.allowToEnterMsgPage = false;
                        }
                    }
                    Item {
                        id: rectSysSetting;
                        width: texSysConfig.width + 80;
                        height: texSysConfig.height + 60;
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 30
                        anchors.horizontalCenter: parent.horizontalCenter;
                        Text {
                            id: texSysConfig;
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter
                            text: os.i18n.ctr(qsTr("Settings"));
                            font.pixelSize: 20;
                            color: "#333333";
                            font.underline: true;
                        }
                        MouseArea {
                            id:settingMouse
                            anchors.fill: parent;
                            onClicked:
                            {
                                if(accountInput.dropListDown == true)
                                    accountInput.dropListDown = false;
                                rootWindow.pageStack.push("qrc:/qml/SystemSettings/SystemSetting.qml");
                            }
                        }
                    }
                }
            }
        }
        LoginError {
            id: errNotification
        }
        NFCLogin{
            id:nfcLogin
            visible: false
        }
    }

    Timer{
        id:translateTm
        interval: 500
        running: false
        repeat: false
        triggeredOnStart: true
        onTriggered: {
            accountInput.editPlaceHoldeText = os.i18n.ctr(qsTr("Please input user name"));
            pwdInput.editPlaceHoldeText = os.i18n.ctr(qsTr("Please input password")) ;
        }
    }

    function loginByNfc(rfidCode){
        rootWindow.allowToEnterMsgPage = false;
        btnLogin.text = btnContentPress;
        errNotification.errType = -4;
        errNotification.visible = true;
        loginTemplate.actionSequence = inspectionSystemConfig.getSequence();
        transmitManager.resultOfResponse.disconnect(loginTemplate.resultResponse);
        transmitManager.resultOfResponse.connect(loginTemplate.resultResponse);
        var head = {"Objects" : "User_QueryPhonePrincipalByRFIDCode"};
        var para = {"UserRFIDCode" : rfidCode};
        transmitManager.requestData(head, para, loginTemplate.actionSequence, "");
    }

    /**
     * 登陆服务器
     */
    function login() {
        rootWindow.closeKeyboard();
        if(accountInput.inputText.length > 0 && pwdInput.inputText.length > 0) {
            rootWindow.allowToEnterMsgPage = false;
            btnLogin.text = btnContentPress;
            errNotification.errType = -4;
            errNotification.visible = true;
            loginTemplate.actionSequence = inspectionSystemConfig.getSequence();
            transmitManager.resultOfResponse.disconnect(loginTemplate.resultResponse);
            transmitManager.resultOfResponse.connect(loginTemplate.resultResponse);
            var head = {"Objects" : "User_QueryPhonePrincipal"};
            var para = {"UserCode" : accountInput.inputText};
            transmitManager.requestData(head, para, loginTemplate.actionSequence, "");
        }
    }

    function historyUsers(query, userCode, password){
        loginTemplate.historyUserActionSequence = inspectionSystemConfig.getSequence();
        transmitManager.resultOfResponse.disconnect(loginTemplate.historyUserResultResponse);
        transmitManager.resultOfResponse.connect(loginTemplate.historyUserResultResponse);
        var head = {};
        var para = {};
        if(query){
            head = {"Objects" : "User_QueryHistoryUsers_Local"};
        } else {
            head = {"Objects" : "User_SetHistoryUsers_Local"};
            para = {"UserCode" : userCode, "Password" : password};
        }
        console.log("history Users(query, userCode, password)", query, userCode, password);
        transmitManager.requestData(head, para, loginTemplate.historyUserActionSequence, "", true);
    }

    function historyUserResultResponse(result, error, action, map) {
        if(action  !=  loginTemplate.historyUserActionSequence) {
            console.log("historyUser ResultResponse action  !=  loginTemplate.actionSequence  return")
            return;
        }
        transmitManager.resultOfResponse.disconnect(loginTemplate.historyUserResultResponse);
        if(result && error == 0){
            console.log("historyUser ResultResponse object is ", map["head"]["objects"]);
            if(map["head"]["objects"] == "User_QueryHistoryUsers_Local"){
                nameModel.clear();
                console.log("historyUser ResultResponse  value ", map["tables"][0]["value"].length);
                var usesMapList = map["tables"][0]["value"];
                for (var i = 0; i < usesMapList.length; i++)
                    nameModel.append({"name": usesMapList[i]["UserCode"], "hide": false});
                console.log("historyUser ResultResponse  nameModel ", nameModel.count);
                rectUserName.showCount = nameModel.count;
            } else {
                historyUsers(true, "", "");
            }
        }
    }

    /**
     * 接收接收服务器返回的，处理认证结果的槽函数
     * 并根据权限，更新本地主界面功能项
     * @param result 获取结果是否成功
     * @param error 如果获取失败，存储失败编码
     * @param action ”User_QueryPhonePrincipal“
     * @param map 经过解析的服务器返回的数据
     */
    function resultResponse(result, error, action, map) {
        console.debug("----LoginTemplate.qml-----onResultOfResponse--------------result, error, action ", result, error, action);
        if(action  !=  loginTemplate.actionSequence) {
            console.log("action  !=  loginTemplate.actionSequence  return")
            return;
        }
        transmitManager.resultOfResponse.disconnect(loginTemplate.resultResponse);
        rootWindow.allowToEnterMsgPage = true;
        btnLogin.text = btnContent;
        errNotification.errType = 0;
        errNotification.visible = false;

        if(result && error == 0) {
            if(map["tables"] == undefined || map["tables"].length <= 0 ||
                    map["tables"][0]["value"] == undefined || map["tables"][0]["value"].length <= 0) {
                //用户名或密码错误
                console.log("user name is not exist.");
                loginTemplate.loginCheck(-1);
                return;
            }
            processLoginResponse(map)
        } else {
            // 网络问题
            loginTemplate.loginCheck(-2);
        }
    }

    /**
     * 处理登陆map
     * @param map 经过解析的服务器返回的数据
     */
    function processLoginResponse(map){
        var pwdInputText = pwdInput.inputText;
        pwdInput.inputText = "";
        var objects = map["head"]["objects"];
        var values = map["tables"][0]["value"][0];
        var passwd = myInspectSysSetting.getPassword(values["Password"], values["PasswordKey"], values["PasswordIV"]);
        if(values["Status"] == 1) {
            if(objects  ==  "User_QueryPhonePrincipal" && passwd  !=  pwdInputText){
                //用户名或密码错误
                console.log("input passwd:  ", pwdInput.inputText, "   server password:  ", passwd, " user name is not exist.");
                loginTemplate.loginCheck(-1);
                return;
            }
            historyUsers(false, values["UserCode"], passwd);
            var longinStatus = {"login":"true", "user": values["UserID"], "password":passwd};
            transmitManager.setStatus("login", longinStatus);
            transmitManager.setStatus("userInfo", values);
            loginSuccess(map)
        }else{
            console.log("account disable")
            loginTemplate.loginCheck(-3);
        }
    }


    function loginSuccess(map) {
        var values = map["tables"][0]["value"][0];
        inspectionSystemParam.setLoginUserInfo(map);
        // Check Theme information
        values = map["tables"][1]["value"][0];

        if(values["Theme"] != inspectionSystemConfig.getCurrentTheme()) {
            inspectionFuncsItemModel.reflesh(values["Theme"]);
        }
        // Check Permission
        var permissionList = map["tables"][2]["value"];
        for (var i = 0; i < permissionList.length; i++) {
            values = permissionList[i];
            var shortName = values["ShortName"];
            console.log("Set Permission ------ ", shortName, " : ", true);
            var permissions = {"ShortName":shortName, "Permission": "true"};
            transmitManager.setStatus("permission", permissions);
        }
        loginTemplate.loginCheck(0);
        loginTemplate.updateToken();
    }
    /**
     * 增加更新token的接口
     */
    function updateToken(){
        var head = {"Objects" : "User_AppToken_Update"};
        var table = {"Name":"Token_update", "Key":"ID"};
        var attachments = [];
        var dataRow = {};
        var dataRowList = [];
        dataRow["UserAccount"] = inspectionSystemParam.getUserId();
        dataRow["DeviceTokenKey"] = inspectionSystemParam.getPadId();
        dataRow["AppId"] = inspectionSystemConfig.getAppId();
        dataRowList.push(dataRow);
        loginTemplate.actionSequence = inspectionSystemConfig.getSequence();
        transmitManager.resultOfResponse.disconnect(loginTemplate.tokenResultResponse);
        transmitManager.resultOfResponse.connect(loginTemplate.tokenResultResponse);
        transmitManager.submitData(head, table, dataRowList, attachments, "", loginTemplate.actionSequence, "更新token失败", true, false);
    }

    //0 success   -1  account or password  error   -2  network error  -3  account disable
    function loginCheck(loginType){
        switch(loginType) {
        case 0:
            console.log("open system functions interface.");
            rootWindow.pageStack.push("qrc:/qml/LoginAndHome/HomePage.qml");
            break;
        case -3:
            errNotification.visible = true;
            errNotification.errMsg = os.i18n.ctr(qsTr("Account has been disabled."));
            errNotification.errType = -3;
            break;
        case -2:
            errNotification.visible = true;
            errNotification.errMsg = os.i18n.ctr(qsTr("Network connection is failed, please check the network configuration"));
            errNotification.errType = -2;
            break;
        case -1:
            errNotification.visible = true;
            errNotification.errMsg = os.i18n.ctr(qsTr("Account or password isn`t corrected, please input again"));
            errNotification.errType = -1;
            break;
        default:
            console.log("login check para failed.");
            break;
        }
    }

    /**
     * 设置登录皮肤
     */
    function setLoginTheme() {
        var config = inspectionFuncsItemModel.getStaticParas("LoginLogo");
        loginTemplate.logoImage = rootWindow.createLocalURL(config["Display_normal"]);
        config = inspectionFuncsItemModel.getStaticParas("LoginTitle");
        loginTemplate.companyName = config["NameCN"];
        config= inspectionFuncsItemModel.getStaticParas("LoginSubTitle");
        loginTemplate.productName = config["NameCN"];
        config= inspectionFuncsItemModel.getStaticParas("LoginUserLeftIcon");
        loginTemplate.accountInputLabelImg = rootWindow.createLocalURL(config["Display_normal"]);
        config = inspectionFuncsItemModel.getStaticParas("LoginUserRightIcon");
        loginTemplate.accountInputDropListImg = rootWindow.createLocalURL(config["Display_normal"]);
        config = inspectionFuncsItemModel.getStaticParas("LoginPasswrdLeftIcon");
        loginTemplate.pwdInputLabelImg = rootWindow.createLocalURL(config["Display_normal"] );
        config = inspectionFuncsItemModel.getStaticParas("LoginBtn");
        loginTemplate.btnContent = config["NameCN"];
        loginTemplate.btnContentColor = rootWindow.getColor(config["Name_color"]);
        loginTemplate.btnColor = rootWindow.getColor(config["Display_normal"]);
        loginTemplate.btnNormalOpacity = Number(config["Opacity_normal"]).toPrecision(2);
        loginTemplate.btnPressOpacity = Number(config["Opacity_press"]).toPrecision(2);
        loginTemplate.btnContentPress = config["NameCN_press"];
        console.log("loginTemplate.logoImage   is ", loginTemplate.logoImage);
    }

    /**
     * 对服务器返回数据进行处理的槽函数，用于生成新任务角标
     * @param result:bool 获取数据是否成功
     * @param error:int 错误代码
     * @param action:string 当前发送数据动作类型
     * @param map:var    服务器返回值
     */
    function tokenResultResponse(result, error, action, map) {
        console.log("----User_AppToken_Update----tokenResultResponse------result, error, action:",
                    result, error,action);
        if(action != loginTemplate.actionSequence) {
            console.log("action  != loginTemplate.actionSequence return")
            return;
        }
        transmitManager.resultOfResponse.disconnect(loginTemplate.tokenResultResponse);

    }

    onStatusChanged: {
            if (status == CPageStatus.WillShow) {
                gScreenInfo.setStatusBarStyle("white");
                gScreenInfo.setStatusBar(true)
            }
        }

    Component.onCompleted:{
        var currentStyle = inspectionFuncsItemModel.getStyle();
        if(currentStyle != "SyberOS"){
            console.log("unsupported style:", currentStyle);
            return;
        }
        translateTm.start();
        setLoginTheme();
    }
}

