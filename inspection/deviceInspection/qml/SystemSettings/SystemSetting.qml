/*!
* \file SystemSetting.qml
* \brief 系统设置页面
*
*系统设置页面: 用于显示系统设置项列表
*
*\author 陈瑞华 chenruihua@syberos.com
*\version 1.0
*\date 2015/9/9
*/
import QtQuick 2.0
import com.syberos.basewidgets 2.0
import com.syberos.generateForm 1.0
import "../Common"
import "../Common/constants.js" as UI

/*!
  \class SystemSetting
  \inModule 系统设置
  \brief 系统设置，系统设置页面
  \ingroup SystemSettings
  系统设置, 提供网络/VPN/GPS/亮度/服务器地址/软件更新/重启/关机等系统设置功能
*/
CommonPage {
    id: setSystemParaPage
    /** 模块名称 */
    property string nameId: ""
    /** type:var  可选RFID类型 */
    property var  rfidType: [os.i18n.ctr( qsTr("HF") ), os.i18n.ctr( qsTr("UHF") ) ]

    /** type:var  RFID类型显示字符 */
    property var  rfidTypeString: ["HF", "UHF"]

    /** type:var  运行模式显示字符 */
    property var workModel: ["在线模式" , "在线&离线混合模式", "离线模式"]

    /**
     *用户设置触发的蜂窝移动网络连接状态变化信号
     * @param 连接状态
     */
    signal cellularConnectedChanged(bool connected)

    //    objectName:"systemSettingPage"
    statusBarHoldEnabled: true
    statusBarHoldItemColor: "white"
    orientationPolicy: CPageOrientation.LockPortrait
    clip: true

    /**
     *当用户单击WIFI设置Item后，执行跳转进入WIFI管理页面操作功能
     * @param 无
     */
    function wifiHandle() {
        //        gApp.openUrl("settings://mainpage?operation=store");
        gApp.openUrl("settings://AppletPage?operation=com.syberos.settings.wlan");
    }

    /**
     *当用户单击VPN设置Item后，执行跳转进入VPN管理页面操作功能
     * @param 无
     */
    function vpnHandle() {
        gApp.openUrl("settings://AppletPage?operation=com.syberos.settings.more");
    }

    /**
     *当用户单击GPS-test设置Item后，执行跳转进入GPS功能测试界面
     * @param 无
     */
    function gpsTestHandle() {
        //CTOAST.requestToast(os.i18n.ctr( qsTr( "Enter into test page of GPS function " ) ), "", "" );//warning toast: 进入GPS功能测试界面
        gApp.openUrl("syberos-gpstest-uiapp://AppletPage?operation=homePage");
    }

    contentAreaItem: Rectangle {
        id: contentRect

        CommonTitlebar {
            id: title
            titleText: "系统设置"
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

        Connections {
            target: myInspectSysSetting
            onSglSim_OfflinePropertyChanged: {
                console.log("------------------onSglSimPropertyChanged---------------bOfflineFlag, bSimFlag", bOfflineFlag, bSimFlag );
                cellularNet.visible = (  !bOfflineFlag &&  bSimFlag );//visible ? 显示 : 隐藏
                console.log("-----------cellularNet.enabled:----------",  (  !bOfflineFlag &&  bSimFlag ) );
            }
            onSglcellularEnabledChanged: {
                console.debug("------------------onSglcellularEnabledChanged---------------flag",flag);
                cellularNet.switchChecked = flag;
                console.log("-----------cellularNet.switchChecked:----------", cellularNet.switchChecked);
            }
            onSglLocateModeChanged: {
                if( (modeStr == "SatelliteMode") || (modeStr == "SatelliteNetworkMode") ) {
                    gpsServiceItem.switchChecked = true;
                } else {
                    gpsServiceItem.switchChecked = false;
                }
            }
            onSglBrightnessChanged: {
                console.log("----------nCurBrightnessValue:----------", nCurBrightnessValue);
                console.log("----------brightness.value:----------", brightness.value);
                if( nCurBrightnessValue != brightness.value ) {
                    brightness.value = nCurBrightnessValue;
                }
            }
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
                    anchors.top: topToHeaderSeparatorItem.bottom
                    width: parent.width
                    spacing: 10

                    Column {
                        id: contentCol
                        width: parent.width

                        ListItem {
                            id: cellularNet
                            mainTitle: os.i18n.ctr( qsTr("CellularNet") )//数据流量
                            type: "switch"
                            endImage: "qrc:/icon/kaiguan_03.png"
                            switchChecked: false
                            onCheckedChanged: {
                                console.log("----------cellularNet----------onCheckedChanged----------itemid, checked",itemId, checked);
                                var ret = myInspectSysSetting.setCellularEnabled(checked);
                                console.log("----------cellularNet----------onCheckedChanged----------setCellularEnabled----ret",ret);
                            }
                             visible: ( ( !myInspectSysSetting.isAirplaneMode() ) && ( myInspectSysSetting.hasSIM() ) )
                        }

                        ListItem {
                            id: wifiItem
                            mainTitle: "WLAN"
                            type: "category"
                            endTitle: ""
                            endImage: "qrc:/icon/xiangyou.png"
                            onClicked: {
                                setSystemParaPage.wifiHandle();
                                console.log("----------setSystemParaPage.wifiHandle()----------");
                            }
                        }

                        ListItem {
                            id: vpnItem
                            mainTitle: "VPN"
                            type: "category"
                            endTitle: ""
                            endImage: "qrc:/icon/xiangyou.png"
                            onClicked: {
                                setSystemParaPage.vpnHandle();
                                console.log("----------setSystemParaPage.vpnHandle()----------");
                            }
                        }

                        ListItem {
                            id: gpsServiceItem
                            mainTitle: "GPS"
                            type: "switch"
                            endImage: "qrc:/icon/kaiguan_03.png"
                            switchChecked: false
                            onCheckedChanged: {
                                var modeStr = "NoneMode";
                                if(checked == true) {
                                    modeStr = "SatelliteMode";
                                } else {
                                    modeStr = "NoneMode";
                                }
                                myInspectSysSetting.setLocateMode(modeStr);
                            }
                        }

                        ListItem {
                            id: gpsTestItem
                            mainTitle: "GPS-test"
                            type: "category"
                            endTitle: ""
                            endImage: "qrc:/icon/xiangyou.png"
                            visible: myInspectSysSetting.getGPSTestItemShowStatus();
                            onClicked: {
                                setSystemParaPage.gpsTestHandle();
                                console.log("----------setSystemParaPage.gpsTestHandle()----------");
                            }
                        }

                        ListItem {
                            id: rfidItem
                            mainTitle: os.i18n.ctr( qsTr("RFIDScanType") )//RFID扫描类型
                            type: "category"
                            endImage: "qrc:/icon/xiangyou.png"
                            visible: false
                            onClicked: {
                                rfidDlg.show();
                            }
                        }

                        Rectangle {
                            id: brightnessRect
                            width: parent.width
                            height: 40
                            color: "#eff1f3"
                            Text {
                                id: sectionTitle
                                font.pixelSize: 16
                                color: "#999999"

                                anchors.left: parent.left
                                anchors.top: parent.top
                                anchors.topMargin: 12
                                anchors.leftMargin: 20
                                text: os.i18n.ctr( qsTr("Brightness") )//亮度
                            }
                        }

                        Rectangle{
                            id: brightnessCtrlRect
                            width: parent.width; height: 80
                            color: "white"
                            Image {
                                id: brightnessLeftImage
                                source: "qrc:/icon/liangdu_10.png"
                                width: 25; height: 25
                                anchors.left: parent.left
                                anchors.leftMargin: 20
                                anchors.verticalCenter: parent.verticalCenter
                            }

                            Rectangle {
                                id: brightnessCSliderRect
                                color: "white"
                                height: UI.LIST_ITEM_HEIGHT
                                anchors.left: brightnessLeftImage.right
                                anchors.leftMargin: 15
                                anchors.right: brightnessRightImage.left
                                anchors.rightMargin: 15
                                visible: true
                                CSlider {
                                    id: brightness

                                    /** type:int  用于记录上一次用户设置的亮度值 */
                                    property int oldvalue: 0
                                    height: 80
                                    anchors.left: parent.left
                                    anchors.right: parent.right
                                    anchors.verticalCenter: parent.verticalCenter                                   
                                    stepSize: 1
                                    minimumValue: 0
                                    maximumValue: myInspectSysSetting.getMaximumBrightness()
                                    value: myInspectSysSetting.getBrightness()
                                    enabled: true
                                    onValueChanged: {
                                        if(value != oldvalue) {
                                            oldvalue = value;
                                            setBrightnessValueTimer.restart();
                                        }
                                    }
                                }
                                Timer {
                                    id: setBrightnessValueTimer
                                    interval: 20
                                    onTriggered: {
                                        console.debug("set brightness to:" , brightness.value);
                                        myInspectSysSetting.setBrightness(brightness.value);
                                        console.debug("set brightness to:", brightness.value, "  ok");
                                    }
                                }
                            }

                            Image {
                                id: brightnessRightImage
                                source: "qrc:/icon/liangdu_07.png"
                                width: 37; height:37
                                anchors.right: parent.right
                                anchors.rightMargin: 20
                                anchors.verticalCenter: parent.verticalCenter
                            }
                        }
                    }

                    Column {
                        id: contentCol2
                        width: parent.width
                        spacing: 10

                        ListItem {
                            id: deviceInfoItem
                            mainTitle: os.i18n.ctr( qsTr("DeviceInfo") )//设备信息
                            type: "category"
                            endImage: "qrc:/icon/xiangyou.png"
                            onClicked: {
                                var com = Qt.createComponent( "qrc:/qml/SystemSettings/DeviceInfo.qml" );
                                if(com.status == Component.Ready) {
                                    var obj = com.createObject(setSystemParaPage);
                                    pageStack.push(obj);
                                }
                            }
                        }

                        ListItem {
                            id: checkUpdateItem
                            mainTitle: os.i18n.ctr( qsTr("CheckUpdate") )//检查更新
                            type: "category"
                            endImage: "qrc:/icon/xiangyou.png"
                            onClicked: {
                                //check update
                                rootWindow.getAppVersion(true);
                            }

                            Component.onCompleted: {
                                var versionNum = myInspectSysSetting.versionNum;
                                endTitle = os.i18n.ctr( qsTr("Current version %1").arg(versionNum) );//当前版本
                            }
                        }

                        ListItem {
                            id: workModeItem
                            mainTitle: os.i18n.ctr( qsTr("OffLine Mode") )//启用离线模式
                            type: "category"
                            endTitle: {
                                var selectedIndex = 0;
                                if(myInspectSysSetting.getOfflineMode() == "0"){
                                    selectedIndex = 0;
                                }else if(myInspectSysSetting.getOfflineMode() == "1"){
                                    selectedIndex = 1;
                                }else if(myInspectSysSetting.getOfflineMode() == "2"){
                                    selectedIndex = 2;
                                }
                                return workModel[selectedIndex];
                            }
                            onClicked: {
                                var selectedIndex = 0;
                                if(myInspectSysSetting.getOfflineMode() == "0"){
                                    selectedIndex = 0;
                                }else if(myInspectSysSetting.getOfflineMode() == "1"){
                                    selectedIndex = 1;
                                }else if(myInspectSysSetting.getOfflineMode() == "2"){
                                    selectedIndex = 2;
                                }
                                moduleDlg.select(selectedIndex, true);
                                moduleDlg.show();
                            }
                        }

                        ListItem {
                            id: rebootItem
                            mainTitle: os.i18n.ctr( qsTr("Reboot") )//重启
                            mainTextFontColor: "#cc181e"
                            type:""
                            onClicked: {
                                console.debug("-------reboot -----------onClicked---------")
                                rebootDlg.titleText = os.i18n.ctr( qsTr("Reboot") )//重启
                                rebootDlg.messageText = os.i18n.ctr( qsTr("Sure to reboot?") )//确定重启设备吗？
                                rebootDlg.show();
                            }
                        }

                        ListItem {
                            id: shutDownItem
                            mainTitle: os.i18n.ctr( qsTr("ShutDown") )//关机
                            mainTextFontColor: "#cc181e"
                            type:""
                            onClicked: {
                                console.debug("-------power on -----------onClicked---------")
                                rebootDlg.titleText = os.i18n.ctr( qsTr("ShutDown") )//关机
                                rebootDlg.messageText = os.i18n.ctr( qsTr("Sure to shutdown?") )//确定关机吗？
                                rebootDlg.show();
                            }
                        }
                    }
                    Column {
                        id: contentCol3
                        width: parent.width
                        spacing: 10
                        visible: rootWindow.showDeveloperModel

                        ListItem {
                            id: optionItem
                            mainTitle: os.i18n.ctr( qsTr("Options") )
                            mainTextFontColor: "#cc181e"
                            type:""
                            onClicked: {
                                console.debug("-------Options-----------onClicked---------")
                                var com = Qt.createComponent( "qrc:/qml/SystemSettings/Options.qml" );
                                if(com.status == Component.Ready) {
                                    var obj = com.createObject(setSystemParaPage);
                                    pageStack.push(obj);
                                }
                            }
                        }
                    }
                    Item {
                        id: bottomSeparatorItem
                        width: parent.width
                        height: UI.GROUP_SEPARATOR_HIGHT
                    }
                }
            } // end of  Flickable { id:detailInfo
        } //end of Rectangle{ id: backGroundRect

        Connections{
            target: rootWindow
            onEntryMsgDetailFromNotify:{
                console.log("hidde  all dialog when entry msg from notify")
                rfidDlg.hide();
                rebootDlg.hide();
            }
        }

        CListDialog {
            id: rfidDlg
            titleText: os.i18n.ctr( qsTr("RFIDScanType") )//RFID扫描类型
            delegateHeight: 80
            itemFontSize: 22
            itemTextColor: "#333333"
            //reverseSelectOnExclusiveMode: true
            buttonAreaEnabled: false
            model: setSystemParaPage.rfidType
            onDelegateItemTriggered: {
                rfidItem.endTitle = setSystemParaPage.rfidType[index];
                myInspectSysSetting.setRfidScanType( setSystemParaPage.rfidTypeString[index] );
            }

            Component.onCompleted: {
                var type = myInspectSysSetting.rfidScanType;
                var selectedIndex = -1;
                for( var i = 0; i < setSystemParaPage.rfidTypeString.length; i++ ) {
                    if( setSystemParaPage.rfidTypeString[i] == type ) {
                        selectedIndex = i;
                        break;
                    }
                }
                rfidItem.endTitle = setSystemParaPage.rfidType[selectedIndex];
                select( [selectedIndex], true );
            }
        }//end of CListDialog{ id: rfidDlg




        CDialog {
            id: rebootDlg
            acceptedButtonText: os.i18n.ctr( qsTr("Ok") )//确定
            rejectButtonText: os.i18n.ctr( qsTr("Cancel") )//取消
            onAccepted: {
                //设备重启
                if( rebootDlg.titleText == os.i18n.ctr( qsTr("Reboot") ) ) {//重启
                    console.log("***************确定重启*******************");
                    myInspectSysSetting.reBootDevice();
                    rebootDlg.hide();
                }

                //设备关机
                if( rebootDlg.titleText == os.i18n.ctr( qsTr("ShutDown") ) ) {//关机
                    console.log("***************确定关机*******************");
                    myInspectSysSetting.shutDownDevice();
                    rebootDlg.hide();
                }
            }//end of onAccepted:{
        }//end of CDialog{ id:rebootDlg

        CListDialog{
            id: moduleDlg
            maxVisibleItems: 4
            titleAreaEnabled: false
            buttonAreaEnabled: false
            visible: false
            model: workModel
            onDelegateItemTriggered:{
                if(index == 0){
                    myInspectSysSetting.setOfflineMode("0")
                }else if(index == 1){
                    myInspectSysSetting.setOfflineMode("1")
                }else if(index == 2){
                    myInspectSysSetting.setOfflineMode("2")
                }
                rootWindow.popToFirstPage();
            }
            Component.onCompleted: {
                var selectedIndex = 0;
                if(myInspectSysSetting.getOfflineMode() == "0"){
                    selectedIndex = 0;
                }else if(myInspectSysSetting.getOfflineMode() == "1"){
                    selectedIndex = 1;
                }else if(myInspectSysSetting.getOfflineMode() == "2"){
                    selectedIndex = 2;
                }
                moduleDlg.select(selectedIndex, true);
            }
        }
    }//end of contentAreaItem: Rectangle { id:contentRect

    Component.onCompleted: {
        var ret = myInspectSysSetting.isCellularConnected();
        console.log( "-------huanlele   ----myInspectSysSetting.isCellularConnected()-----------ret:", ret );
         cellularNet.switchChecked = ret;

        var modeStr = myInspectSysSetting.getLocateMode();
        if( (modeStr == "SatelliteMode") || (modeStr == "SatelliteNetworkMode") ) {
            gpsServiceItem.switchChecked = true;
        }
        else {
            gpsServiceItem.switchChecked = false;
        }
    }
}
