import QtQuick 2.0
import com.syberos.basewidgets 2.0
import "../Common"

CommonPage {
    id: options

    /** type:var 协议头类型列表*/
    property var headModel:["普通协议头", "加密协议头"]

    /** type:var 数据加密类型*/
    property var encryptModel:["无加密", "普通加密", "SSL&TLS"]

    Component.onCompleted: {
    }

    contentAreaItem: Rectangle {
        id: contentRect

        CommonTitlebar {
            id: title
            titleText: "高级选项"
            onLeftItemTriggered: pageStack.pop()
        }

        Rectangle {
            id: titleSeparateLine
            width: parent.width; height: 1
            color: "#000000"
            opacity: 0.1
            z: parent.z + 1
            anchors.top: title.bottom
        }

        Rectangle {
            id: backGroundRect
            color: "#eff1f3"
            anchors.top: title.bottom
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right

            Flickable {
                id: detailInfo
                anchors.fill: parent
                flickDeceleration: Flickable.VerticalFlick
                clip: true
                contentHeight: totalCol.height + topToHeaderSeparatorItem.height

                Item {
                    id:topToHeaderSeparatorItem
                    width: parent.width; height: 10
                }

                Column {
                    id: totalCol
                    width: parent.width

                    ListItem {
                        id: serverAddrItem
                        mainTitle: os.i18n.ctr( qsTr("ServerAddress") )//服务器地址
                        type: "category"
                        endTitle: myInspectSysSetting.serverAddr
                        endImage: "qrc:/icon/xiangyou.png"
                        onClicked: {
                            serverAddressDlg.oldServerAddr = endTitle;
                            serverAddressDlg.setText( endTitle );
                            serverAddressDlg.visible = true;
                        }
                    }
                    ListItem {
                        id: serverPortItem
                        mainTitle: os.i18n.ctr( qsTr("Server port") )  //"服务器端口"
                        type: "category"
                        endTitle: myInspectSysSetting.serverPort
                        endImage: "qrc:/icon/xiangyou.png"
                        onClicked: {
                            serverPortDlg.oldServerPort = endTitle;
                            serverPortDlg.setText(endTitle);
                            serverPortDlg.show();
                        }
                    }
                    ListItem {
                        id: headTypeItem
                        mainTitle: os.i18n.ctr( qsTr("headType") )
                        type:"category"
                        endTitle: headModel[myInspectSysSetting.authType]
                        onClicked: {
                            headDialog.select([myInspectSysSetting.authType], true);
                            headDialog.show()
                        }
                    }
                    ListItem {
                        id: encryptTypeItem
                        mainTitle: os.i18n.ctr( qsTr("encryptType") )
                        endTitle: encryptModel[myInspectSysSetting.encryptType]
                        type:"category"
                        onClicked: {
                            encryptDialog.select([myInspectSysSetting.encryptType], true);
                            encryptDialog.show()
                        }
                    }

                    Column{
                        id: resetParaCol
                        width: parent.width
                        ListItem {
                            id: resetParaItem
                            mainTitle: os.i18n.ctr( qsTr("Reset System Para") )
                            type:"category"
                            onClicked: {
                                accountItem.visible = !accountItem.visible;
                                codeItem.visible = !codeItem.visible;
                                resetParaBtn.visible = !resetParaBtn.visible;
                            }
                        }
                        ListItem {
                            id: accountItem
                            mainTitle: os.i18n.ctr( qsTr("Account") )
                            type: "category"
                            visible: false
                            endImage: "qrc:/icon/xiangyou.png"
                            onClicked: {
                                accountDlg.setText("");
                                accountDlg.visible = true;
                            }
                        }
                        ListItem {
                            id: codeItem
                            mainTitle: os.i18n.ctr( qsTr("Code") )
                            type: "category"
                            visible: false
                            endImage: "qrc:/icon/xiangyou.png"
                            onClicked: {
                                codeDlg.setText("");
                                codeDlg.visible = true;
                            }
                        }
                        CommonButton{
                            id:resetParaBtn
                            visible: false
                            text:os.i18n.ctr( qsTr("Reset") )
                            onClicked:{
                                myInspectSysSetting.setKeyPara(accountItem.itemId, codeItem.itemId);
                            }
                        }
                    }
                }
            }
        }

        Connections{
            target: rootWindow
            onEntryMsgDetailFromNotify:{
                console.log("hidde  all dialog when entry msg from notify")
                serverAddressDlg.hide();
                serverPortDlg.hide();
                checkChangeServerAddrDlg.hide();
            }
        }

        CInputDialog {
            id: serverAddressDlg

            /** type:string  原有服务器地址 */
            property  string oldServerAddr: ""
            titleText: os.i18n.ctr( qsTr("ServerAddress") )//服务器地址
            inputFontPixelSize: 22
            inputMethodHints: Qt.ImhPreferNumbers//默认数字键盘：（支持其他键盘）
            placeholderText: os.i18n.ctr( qsTr("enter server address") )//输入服务器地址
            validator: RegExpValidator {
                regExp: /^(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])$/
            }

            acceptedButtonText: os.i18n.ctr( qsTr("Ok") )//确定
            rejectButtonText: os.i18n.ctr( qsTr("Cancel") )//取消
            onAccepted: {
                var serverAddrRetText = serverAddressDlg.text();

                if(oldServerAddr != serverAddrRetText) {
                    console.log("-------------------serverAddrRetText----------------------",serverAddrRetText);

                    //服务器地址格式校验: xxx.xxx.xxx.xxx(3 split points and 4 Network fields are needed)
                    var addressSplit = serverAddrRetText.split(".");
                    if( (addressSplit.length == 4) && (addressSplit[3] != "") ) {
                        var isLogin = inspectionSystemParam.getLoginStatus();
                        console.log("-------------------inspectionSystemParam loginStatus----------isLogin:",isLogin);
                       if(isLogin) {
                           checkChangeServerAddrDlg.messageText = os.i18n.ctr( qsTr("You have modified the system's server address, and the default will be returned to the login screen. Determine the change?") )
                           checkChangeServerAddrDlg.modifyType = "address";
                           checkChangeServerAddrDlg.show();
                       } else {
                           serverAddrItem.endTitle = serverAddrRetText;
                           myInspectSysSetting.serverAddr =  serverAddrRetText;
                       }
                    } else {
                        CTOAST.requestToast(os.i18n.ctr( qsTr( "Server address error, change failed" ) ), "", "" );//服务器地址错误，修改失败
                    }
                }
            }

            onTextChanged: {
                if(text == "") {
                    acceptButtonEnabled = false;
                } else {
                    acceptButtonEnabled = true;
                }
            }
        }//end of CInputDialog{ id:serverAddressDlg

        CInputDialog {
            id: serverPortDlg
            /** type:string  原有服务器地址 */
            property  string oldServerPort: ""
            titleText: os.i18n.ctr( qsTr("Server port") ) //"服务器端口"
            inputFontPixelSize: 22
            inputMethodHints: Qt.ImhPreferNumbers//默认数字键盘：（支持其他键盘）
            placeholderText: os.i18n.ctr( qsTr("Please  input server port") )   //"请输入服务器端口"
            validator:RegExpValidator{regExp: /^[1-9][0-9]{0,}$/}
            acceptedButtonText: os.i18n.ctr( qsTr("Ok") )//确定
            rejectButtonText: os.i18n.ctr( qsTr("Cancel") )//取消
            onAccepted: {
                var serverPortText = serverPortDlg.text();
                if(oldServerPort != serverPortText) {
                    var isLogin = inspectionSystemParam.getLoginStatus();
                   if(isLogin) {
                       checkChangeServerAddrDlg.messageText = os.i18n.ctr( qsTr("You have modified the system's server port, and the default will be returned to the login screen. Determine the change?") )  //"您已修改当前系统的服务器端口，默认将退回登录界面。确定修改？"
                       checkChangeServerAddrDlg.modifyType = "port";
                       checkChangeServerAddrDlg.show();
                   } else {
                       serverPortItem.endTitle = serverPortText;
                       myInspectSysSetting.serverPort=  serverPortText;
                   }
                }
            }
            onTextChanged: {
                if(text == "") {
                    acceptButtonEnabled = false;
                } else {
                    acceptButtonEnabled = true;
                }
            }
        }//end of CInputDialog{ id:serverPortDlg

        CDialog {
            id: checkChangeServerAddrDlg
            titleAreaEnabled: false
//            messageText:  os.i18n.ctr( qsTr("You have modified the system's server address,
//and the default will be returned to the login screen. Determine the change?") ) //您已修改当前系统的服务器地址，默认将退回登录界面。确定修改？
            acceptedButtonText: os.i18n.ctr( qsTr("Ok") )//确定
            rejectButtonText: os.i18n.ctr( qsTr("Cancel") )//取消
            property string modifyType: ""
            onAccepted: {
                if(modifyType == "address"){
                    serverAddrItem.endTitle = serverAddressDlg.text();
                    myInspectSysSetting.serverAddr =  serverAddressDlg.text();
                }else if(modifyType == "port"){
                    serverPortItem.endTitle = serverPortDlg.text();
                    myInspectSysSetting.serverPort =  serverPortDlg.text();
                }
                //return login page
                rootWindow.popToFirstPage();
            }
        }

        CListDialog{
            id: headDialog
            maxVisibleItems: 3
            titleAreaEnabled: false
            buttonAreaEnabled: false
            objectName:"headDlg"
            model: headModel
            onDelegateItemTriggered:{
                myInspectSysSetting.authType = index;
                headTypeItem.endTitle = headModel[index];
                headDialog.hide();
            }
        }
        CListDialog{
            id: encryptDialog
            maxVisibleItems: 4
            titleAreaEnabled: false
            buttonAreaEnabled: false
            objectName:"encryptDlg"
            model: encryptModel
            onDelegateItemTriggered:{
                myInspectSysSetting.encryptType = index;
                encryptTypeItem.endTitle = encryptModel[index];
                encryptDialog.hide();
            }
        }

        CInputDialog {
            id: accountDlg
            titleText: os.i18n.ctr( qsTr("Account") )
            inputFontPixelSize: 22
            inputMethodHints: Qt.ImhPreferLatin
            placeholderText: os.i18n.ctr( qsTr("enter account") )
            acceptedButtonText: os.i18n.ctr( qsTr("Ok") )
            rejectButtonText: os.i18n.ctr( qsTr("Cancel") )
            onAccepted: {
                var account = accountDlg.text();
                accountItem.endTitle = account;
                accountItem.itemId = account;
            }
            onTextChanged: {
                if(text == "") {
                    acceptButtonEnabled = false;
                } else {
                    acceptButtonEnabled = true;
                }
            }
        }//end of CInputDialog{ id:accountDlg

        CInputDialog {
            id: codeDlg
            titleText: os.i18n.ctr( qsTr("Code") )
            inputFontPixelSize: 22
            inputMethodHints: Qt.ImhPreferNumbers//默认数字键盘：（支持其他键盘）
            placeholderText: os.i18n.ctr( qsTr("enter code") )
            acceptedButtonText: os.i18n.ctr( qsTr("Ok") )
            rejectButtonText: os.i18n.ctr( qsTr("Cancel") )
            onAccepted: {
                var code = codeDlg.text();
                codeItem.endTitle = code;
                codeItem.itemId = code;
            }
            onTextChanged: {
                if(text == "") {
                    acceptButtonEnabled = false;
                } else {
                    acceptButtonEnabled = true;
                }
            }
        }//end of CInputDialog{ id:codeDlg
    }
}
