/*!
* \file CommonList.qml
* \brief 公共页面，通用列表容器页面
*
*公共页面，通用列表容器页面，装载各个业务的任务，套间，设备列表
*
*\author 桓乐乐 huanlele@syberos.com
*\version 1.0
*\date 2015/9/7
*/
import QtQuick 2.0
import com.syberos.basewidgets 2.0
/*!
  \class CommonList
  \inModule 公共控件
  \brief 公共页面，通用列表容器页面
  \ingroup common
  公共页面，通用列表容器页面，装载各个业务的任务，套间，设备列表
*/
CommonPage {
    id: baseTaskList

    property var originalData
    property var otherParam

    /** type:string  页面标题 */
    property alias  titleText: title.titleText

    /** type:string   网络发包序列号*/
    property string actionSequence:"-1"

    /** type:alias listview的数据源（listmode）  */
    property alias  model :taskList.model

    /** type:bool  是否创建扫描功能模块，true，创建； false，不创建  */
    property bool   enabledScanItem: false

    /** type:string  是否是RFID扫描: 0,不是；1，是 */
    property string isRFIDScan: "0"

    /** type:string  是否是一二维码扫描: 0,不是；1，是 */
    property string isBarCodeScan: "0"

    /** type:var  代理生成的component */
    property alias    delegateCom :taskList.delegate

    /** type:alias   页面为空时，提示字*/
    property alias  emptyText: empty.text

    /** type:alias   页面为空时，提示图片*/
    property alias  emptyIcon: empty.iconSource

    /** type:bool   是否刷新列表*/
    property bool   refresh: true

    property var scanItemPage: undefined

    /*标题栏左区域被点击是否使用默认行为,默认为退出,否则发出信号 titleBarLeftItemTriggered*/
    property bool leftItemTriggeredDefaultProcess: true

    /** type:var 菜单选项*/
    property bool futureEnable: false
    property bool offlineEnable: false
    property var menuModel1:["用户卡片", "未来任务数据同步", "离线任务数据同步", "退出用户"]
    property var menuModel2:["用户卡片", "未来任务数据同步", "退出用户"]
    property var menuModel3:["用户卡片", "离线任务数据同步", "退出用户"]
    property var menuModel4:["用户卡片", "退出用户"]
    property var dateModel:["未来1天","未来2天", "未来3天", "未来4天","未来5天", "未来6天","未来7天"]
    property bool titleRightItemEnable: false
    property string titleRightItemText: ""
    property string titleRightItemIcon: ""
    property alias searchInputItem: searchInput
    property bool showSubmitBtn: false
    property bool showAddBtn: false
    statusBarHoldEnabled: true
    orientationPolicy: CPageOrientation.LockPortrait
    clip: true

    signal resultBarCode(string barCode)
    signal resultRFIDCode(string rfidCode)
    signal searchTextChanged(string text)
    signal submitBtnClicked()
    signal syncPreTask(string dateTime)
    signal syncTask()
    signal addBtnClicked()
    signal titleBarLeftItemTriggered()

    contentAreaItem:  Item {
        id: content
        CommonTitlebar{
            id: title
            width: parent.width
            leftItemEnabled: true
            titleItemEnabled: true
            rightItemEnabled: titleRightItemEnable
            rightItemSize:Qt.size(100, 46)
            leftItemText:""
            onLeftItemTriggered:{
                if(leftItemTriggeredDefaultProcess)
                    pageStack.pop()
                else
                    titleBarLeftItemTriggered();
            }
            rightItemComponent: CButton {
                backgroundComponent: Rectangle {
                    border.width: 0
                    border.color: gUiConst.getValue("CB5")
                    color: gUiConst.getValue("CA1")
                    radius: height / 2
                }
                backgroundEnabled: titleRightItemIcon == ""
                labelComponent:Row {
                    spacing:0
                    Image {
                        id: icon
                        source: titleRightItemIcon
                        anchors.verticalCenter: parent.verticalCenter
                        visible: source != ""
                    }
                    Text{
                        id: textId
                        text: titleRightItemText
                        font.pixelSize: 20
                        color: gUiConst.getValue("CT6")
                        visible: text != ""
                    }
                }
            }
            onRightItemTriggered: {
                if(titleRightItemText == "搜索"){
                    searchInput.visible = !searchInput.visible;
                }else if(titleRightItemIcon == "qrc:/icon/action_bar_menu.png"){
                    var model;
                    var type = myInspectSysSetting.getOfflineMode();
                    if(type == "2"){
                        if(futureEnable && offlineEnable){
                            model = menuModel1
                        } else if(futureEnable){
                            model = menuModel2
                        } else if(offlineEnable){
                            model = menuModel3
                        } else {
                            model = menuModel4
                        }
                    } else if(type == "1"){
                        if(futureEnable){
                            model = menuModel2
                        }else{
                            model = menuModel4
                        }
                    } else if(type == "0"){
                        model = menuModel4
                    }
                    var com = menuDialogCom.createObject(baseTaskList, {"model":model});
                    com.show()
                }
            }
            anchors.top: parent.top
            onLeftItemChanged:leftItem.backgroundEnabled = false
            leftItemBackgroundEnabled: false
        }
        ListInputItem{
            id:searchInput
            showType:"searchInput"
            editPlaceHoldeText:"请输入搜索关键词"
            anchors.top: title.bottom
            visible: false
            onTextChanged: {
              baseTaskList.searchTextChanged(text);
            }
        }

        ListView {
            id: taskList
            width: parent.width
            clip:true
            anchors{
                top: searchInput.bottom
                bottom: submitRect.top
            }
        }
        Rectangle{
            id:submitRect
            width: parent.width
            anchors.bottom: parent.bottom
            height: visible? 80:0
            visible:baseTaskList.showSubmitBtn
            CommonButton{
                id:submitListBtn
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                text:"一键提交"
                onClicked:{
                    baseTaskList.submitBtnClicked();
                }
            }
        }

//        CIconLabel {
//            id: empty
//            visible: taskList.count === 0 ? true : false
//            width: parent.width
//            anchors.top: taskList.top
//            //anchors.topMargin:250
//           // anchors.horizontalCenter: parent.horizontalCenter
//            iconSource: "qrc:/icon/empty.png"
//            text: os.i18n.ctr(qsTr("No result"))
//            spacing: 10
//        }
        Item {
            id: empty
            property string text: os.i18n.ctr(qsTr("No result"))
            property string iconSource: "qrc:/icon/empty.png"
            visible: taskList.count === 0 ? true : false
            width: parent.width
            height:200
            anchors.top: taskList.top
            Image{
                id:emptyIconImg
                width: 100
                height: 100
                source: empty.iconSource
                anchors.top: parent.top
                anchors.topMargin: 20
                anchors.horizontalCenter: parent.horizontalCenter
            }
            Text{
                anchors.top:emptyIconImg.bottom
                anchors.topMargin: 10
                font.pixelSize: 30
                color: "grey"
                text:empty.text
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }

        Rectangle{
            id: menu
            anchors.top: title.bottom
            anchors.bottom: parent.bottom
            width: parent.width
            opacity: 0.6
            color: "black"
            visible: false
            MouseArea {
                anchors.fill: parent
                onClicked: {
                   menu.visible = false;
                }
            }
        }
        CButton {
            id: add
            visible: showAddBtn
            width: 50
            height: 50
            anchors.bottomMargin: 20
            anchors.bottom: parent.bottom
            anchors.rightMargin: 20
            anchors.right: parent.right
            iconSource: "qrc:/icon/multimedia_add_attach.png"
            text: ""
            onClicked: {
                addBtnClicked()
            }
        }
        Component {
            id : menuDialogCom
            CListDialog{
                id: menuDialog
                maxVisibleItems: 4
                reverseSelectOnExclusiveMode: true
                titleAreaEnabled: false
                buttonAreaEnabled: false
                itemCheckable: false
                visible: false
                onDelegateItemTriggered:{
                    if(model[index] == "用户卡片"){
                        menuDialog.hide()
                        pageStack.push("qrc:/qml/LoginAndHome/UserCard.qml", {"nameId":"用户卡片"})
                    }else if(model[index] == "未来任务数据同步"){
                        menuDialog.hide()
                        var com = dateDialogCom.createObject(baseTaskList);
                        com.show();
                    }else if(model[index] == "离线任务数据同步"){
                        menuDialog.hide();
                        syncTask();
                    }else if(model[index] == "退出用户"){
                        menuDialog.hide();
                        rootWindow.popToFirstPage();
                    }
                }
            }
        }
        Component {
            id: dateDialogCom
            CListDialog{
                id: dateDialog
                maxVisibleItems: 4
                titleAreaEnabled: false
                buttonAreaEnabled: false
                itemCheckable: false
                model: dateModel
                visible: false
                reverseSelectOnExclusiveMode: true
                onDelegateItemTriggered:{
                    var date = new Date();
                    console.debug("----------------0000------date: ", date)
                    var day = date.getDate();
                    date.setDate(day+(index+1));
                    date.setHours(0);
                    date.setMinutes(0);
                    date.setSeconds(0);
                    console.debug("----------------1111------date: ", date)
                    syncPreTask(rootWindow.formatDate(date))
                    dateDialog.hide()
                }
            }
        }
    }

    Component.onCompleted: {
        if( baseTaskList.otherParam["EnabledScanItem"] !== undefined &&
                baseTaskList.otherParam["EnabledScanItem"] == true){
            var com = Qt.createComponent("qrc:/qml/Common/ScanItem.qml");
            if(com.status == Component.Ready) {
                baseTaskList.scanItemPage = com.createObject(content);
                baseTaskList.scanItemPage.parentPage = baseTaskList;
                baseTaskList.scanItemPage.anchors.top =  searchInput.bottom;
                taskList.anchors.topMargin = baseTaskList.scanItemPage.height;
                //empty.anchors.topMargin = baseTaskList.scanItemPage.height;
            }
        }
    }

    Component.onDestruction: {
        if(baseTaskList.otherParam["EnabledScanItem"] !== undefined &&
                baseTaskList.otherParam["EnabledScanItem"] == true && baseTaskList.scanItemPage){
                baseTaskList.scanItemPage.destroy();
                baseTaskList.scanItemPage = undefined;
        }
    }
}
