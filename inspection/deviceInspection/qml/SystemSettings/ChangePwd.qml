/*!
* \file UserCard.qml
* \brief 用户卡片界面
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
import "../LoginAndHome"



/*!
  \class UserCard
  \inModule 用户卡片模块
  \brief 用于显示登录用户的个人信息
  \ingroup LoginAndHome
  用于显示登录用户的个人信息. 包括：用户名，单位，部门，角色。
*/
CommonPage {
    id: userCardPage;
    statusBarHoldEnabled: true
    statusBarHoldItemColor: "white"
    orientationPolicy: CPageOrientation.LockPortrait
    property int network_error:-8
    property var oldpassword
    property string actionSequence: "-1"
    clip: true
    Connections{
        target: gInputContext
        onSoftwareInputPanelEventChanged:{
            if(gInputContext.softwareInputPanelEvent === "deviceInspection"){
                if(oldpwdInput.isFocus){
                    newpwdInput.forceActiveFocus()
                }
                else if(newpwdInput.isFocus){
                    confirmpwdInput.forceActiveFocus()
                }else{
                    checkpara()
                }
            }
        }
    }

    LoginError{
        id :alertDialog
        visible :false
        z:1001

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
            onLeftItemTriggered: pageStack.pop()
            anchors.top: parent.top
            onLeftItemChanged:leftItem.backgroundEnabled = false
            leftItemBackgroundEnabled: false
            titleText:os.i18n.ctr(qsTr("Modify password"))
        }
        Rectangle {
            id: rectBackGround;
            width: parent.width
            anchors{
                top: title.bottom
                bottom: parent.bottom
            }
            color: "#ededed";
            Flickable{
                id:contentFlickable
                anchors.fill: parent
                boundsBehavior:Flickable.StopAtBounds
                clip:true
                contentHeight: contentColumn.height +180
                Column{
                    id:contentColumn
                    width: parent.width
                    anchors.left: parent.left
                    anchors.top: parent.top;
                    anchors.topMargin: 80;
                    Column{
                        id :colpwd
                        width: parent.width
                        anchors.left: parent.left
                        spacing: 35
                        Row{
                            id :oldpwd
                            anchors.left: parent.left
                            anchors.leftMargin: 44
                            height: 70
                            ListInputItem{
                                id:oldpwdInput
                                height: 70
                                width: 392
                                backcolor :"#cccccc"
                                changepwd :true
                                editPlaceHoldeText:os.i18n.ctr(qsTr("Original password"))
                                lableIconWidth :40
                                lableIconHeight: 40
                                labelIconPath :"qrc:/icon/oldpwd.png"
                                showType:"image"
                                passwordEnabled:true
                                echoMode:TextInput.Password
                                inputMethodHints:Qt.ImhPreferLatin
                                validator: RegExpValidator{regExp: /^[_A-Za-z0-9!@#$%&*()]+$/}
                                inputMethodHintExtensions:{
                                    var args = {};
                                    args["enterKeyText"]="next"
                                    return args
                                }
                            }

                        }
                        Row{
                            anchors.left: parent.left
                            anchors.leftMargin: 44
                            height: 70
                            ListInputItem{
                                id:newpwdInput
                                height: 70
                                width: 392
                                lableIconWidth :40
                                lableIconHeight: 40
                                backcolor :"#cccccc"
                                changepwd :true
                                showType:"image"
                                passwordEnabled:true
                                editPlaceHoldeText:os.i18n.ctr(qsTr("New password"))
                                labelIconPath :"qrc:/icon/newpwd.png"
                                echoMode:TextInput.Password
                                inputMethodHints:Qt.ImhPreferLatin
                                validator: RegExpValidator{regExp: /^[_A-Za-z0-9!@#$%&*()]+$/}
                                inputMethodHintExtensions:{
                                    var args = {};
                                    args["enterKeyText"]="next"
                                    return args
                                }
                            }

                        }
                        Row{
                            anchors.left: parent.left
                            anchors.leftMargin: 44
                            height: 70
                            ListInputItem{
                                id:confirmpwdInput
                                height: 70
                                width: 392
                                changepwd:true
                                lableIconWidth :40
                                lableIconHeight: 40
                                backcolor :"#cccccc"
                                showType:"image"
                                editPlaceHoldeText:os.i18n.ctr(qsTr("Ensure password"))
                                validator: RegExpValidator{regExp: /^[_A-Za-z0-9!@#$%&*()]+$/}
                                labelIconPath :"qrc:/icon/confirmpwd.png"
                                passwordEnabled:true
                                echoMode:TextInput.Password
                                inputMethodHints:Qt.ImhPreferLatin
                                inputMethodHintExtensions:{
                                    var args = {};
                                    args["enterKeyText"]="done"
                                    return args
                                }
                            }

                        }
                    }
                    Item{
                        width: parent.width
                        height: 70
                    }
                    Column{
                        width: parent.width
                        anchors.left: parent.left
                        anchors.leftMargin: 44
                        spacing: 35
                        Button {
                            id: btnConfirm;
                            width: 392;
                            height: 60;
                            opacity: oldpwdInput.inputText === "" || newpwdInput.inputText === "" || confirmpwdInput.inputText === ""? 0.5 : 1
                            enabled: oldpwdInput.inputText === "" || newpwdInput.inputText === "" || confirmpwdInput.inputText === ""? false:true
                            style: ButtonStyle {
                                background: Rectangle {
                                    color:  "red";   //"red";
                                    z:1;
                                }
                            }
                            Text {
                                id: textConfirm
                                text: os.i18n.ctr(qsTr("Modify"))
                                color:  "white";        //"white";
                                font.pixelSize: 22;
                                anchors.centerIn: parent;
                            }
                            onClicked: {
                                checkpara();
                            }
                        }
                        Button {
                            id: btnCancel
                            width: 392;
                            height: 60;
                            style: ButtonStyle {
                                background: Rectangle {
                                    border.color: "red"
                                    color: "transparent";   //"red";
                                    z:1;
                                }
                            }
                            Text {
                                id: textCancel
                                text: os.i18n.ctr(qsTr("Cancel"))
                                color:  "#535353";        //"white";
                                font.pixelSize: 22;
                                anchors.centerIn: parent;
                            }
                            onClicked: {
                                rootWindow.closeKeyboard();
                                pageStack.pop();
                            }
                        }


                    }

                }

            }
        }
    }
    function check(result, error, action, map){
        if (action != userCardPage.actionSequence){
            console.log("action != userCardPage.actionSequence  return")
            return;
        }
        transmitManager.resultOfResponse.disconnect(check);
        alertDialog.errortitle = os.i18n.ctr(qsTr("Modify password failure"))
        alertDialog.errType = -6
        if(result && error == 0){
            var values = map["tables"][0]["value"][0];
            oldpassword = values["Password"];
            var passwd = rootWindow.inspectSysSetting.getPassword(oldpassword,values["PasswordKey"],values["PasswordIV"]);
            if(oldpwdInput.inputText !== passwd){
                alertDialog.errMsg = os.i18n.ctr(qsTr("Original password not right"))
                alertDialog.visible = true
                oldpwdInput.inputText = ""
                newpwdInput.inputText = ""
                confirmpwdInput.inputText = ""
            }else if(oldpwdInput.inputText == newpwdInput.inputText){
                alertDialog.errMsg = os.i18n.ctr(qsTr("Original password as same as New password"))
                alertDialog.visible = true
                newpwdInput.inputText = ""
                confirmpwdInput.inputText = ""
            }

            else if(newpwdInput.inputText.length < 6 || newpwdInput.inputText.length > 14){
                alertDialog.errMsg = os.i18n.ctr(qsTr("Length of password between 6 to 14"))
                alertDialog.visible = true
                newpwdInput.inputText = ""
                confirmpwdInput.inputText = ""
            }else if(newpwdInput.inputText !== confirmpwdInput.inputText){
                alertDialog.errMsg = os.i18n.ctr(qsTr("twice password not same"))
                alertDialog.visible = true
                newpwdInput.inputText = ""
                confirmpwdInput.inputText = ""
            } else {
                userCardPage.actionSequence = inspectionSystemConfig.getSequence();
                transmitManager.resultOfResponse.disconnect(processresult);
                transmitManager.resultOfResponse.connect(processresult);
                var paramap = rootWindow.inspectSysSetting.setPassword(newpwdInput.inputText);

                var head = {"Objects" : "SDMS_User_Change_Pwd"};
                var para = {};
                para["UserID"] = values["UserID"];
                para["PassWord"] = paramap[0];
                para["PasswordKey"] = paramap[1];
                para["PasswordIV"] = paramap [2];
                console.log("transmitManager.requestData   SDMS_User_Change_Pwd ")
                transmitManager.requestData(head, para, userCardPage.actionSequence, "");
            }
        }

    }

    function checkpara(){
        userCardPage.actionSequence = inspectionSystemConfig.getSequence();
        transmitManager.resultOfResponse.disconnect(check);
        transmitManager.resultOfResponse.connect(check);
        rootWindow.closeKeyboard()
        var userCode = inspectionSystemParam.getUserPropertyByKey("UserCode");
        var head = {"Objects" : "User_QueryPhonePrincipal"};
        var para = {"UserCode" : userCode};
        console.log("transmitManager.requestData  ")
        transmitManager.requestData(head, para, userCardPage.actionSequence, "");
    }
    function processresult(result, error, action, map){
        if(action != userCardPage.actionSequence){
            console.log("action != userCardPage.actionSequence return")
            return;
        }
        alertDialog.errType = -6
        //SDMS_User_Change_Pwd
        transmitManager.resultOfResponse.disconnect(processresult);
        if(result && error == 0) {
            alertDialog.errMsg = os.i18n.ctr(qsTr("Password changes, please log in"))
            alertDialog.errortitle = os.i18n.ctr(qsTr("Password modify success"))
            alertDialog.visible = true
            alertDialog.errType = -5

        } else if(error != network_error) {
            oldpwdInput.inputText = ""
            newpwdInput.inputText = ""
            confirmpwdInput.inputText  = ""
            alertDialog.errMsg = os.i18n.ctr(qsTr("Password modify failure, please try again"))
            alertDialog.errortitle = os.i18n.ctr(qsTr("Password modify failure"))
            alertDialog.visible = true
        }
    }
}
