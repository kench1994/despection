import QtQuick 2.0
import com.syberos.basewidgets 2.0
import "../Common"
import "../Common/constants.js" as UI
CommonPage{
    id :addparts
    statusBarHoldEnabled: true
    statusBarHoldItemColor: "white"
    orientationPolicy: CPageOrientation.LockPortrait
    color: "white"
    /*faultReport  maintain*/
    property string pageType: ""
    property string actionType:"save"
    property string rightitemtext:"保存"
    property string headTitle:"添加零部件"
    /*接受上一级传入的参数*/
    property var resultInfoValuemap:{var map ={} ;return map }
    /*用于将结果返回给上一级*/
    property var returnInfoValuemap:{var map ={} ;return map }
    //PartID,PartCode,PartName,PartManufacturer,PartStyle,ModifiedBy,PartNumber,FaultRepairID,PartUnit,OperType
    property var faultReortInfoList: [
        ["PartCode","零配件编号:"],
        ["PartName", "零配件名称:"],
        ["PartStyle" , "零配件型号:"],
        ["PartUnit" , "零配件单位:"],
        ["PartNumber" , "零配件数量:"],
        ["PartManufacturer" , "零配件厂商:"]]
    property var maintainInfoList:[
        ["PartCode" , "零配件编号:"],
        ["PartName" , "零配件名称:"],
        ["PartStyle" , "零配件型号:"],
        ["PartNumber" , "零配件数量:"],
        ["PartManufacturer" , "零配件厂商:"]]
    property var currentInfoList: []
    signal result(var infomap)
    property var validator: RegExpValidator{regExp:/^[0-9]+$/}
    contentAreaItem: Rectangle {
        id: title
        anchors.fill: parent
        color:"#eeeff1"
        Rectangle{
            id:pageHead
            width: parent.width
            height: pageHeadTitle.height
            CommonTitlebar{
                id: pageHeadTitle
                width: parent.width
                clip:true
                leftItemEnabled: true
                titleItemEnabled: true
                rightItemEnabled: true
                leftItemPixelSize: 22
                titleAreaCentered:true
                leftItemTextColor: "#666666"
                leftItemText:"取消"
                leftItemIcon:""
                titleText:addparts.headTitle
                rightItemText:addparts.rightitemtext
                onLeftItemTriggered: {
                    rootWindow.closeKeyboard()
                    pageStack.pop();
                }
                onRightItemTriggered:{
                    rootWindow.closeKeyboard()
                    sendResult();
                }
                anchors.top: parent.top
                leftItemBackgroundEnabled: false
            }
        }
        ListModel{
            id:datamodel
        }
        Component{
            id :itemdelegate
            Rectangle{
                width :parent.width
                height: 60
                property string rectType: keyType
                Text {
                    id:label
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin:20
                    font.pixelSize: 18
                    color:"#666666"
                    opacity: 0.6
                    text:name
                }
                CLineEdit{
                    id :lineedit
                    anchors{
                        left: label.right
                        leftMargin: 5
                        right:parent.right
                        verticalCenter: parent.verticalCenter
                    }
                    echoMode:TextInput.Normal
                    textColor:"black"
                    text :returnInfoValuemap[keyType]
                    maximumLength:50
                    placeholderText:"请输入相关信息"
                    font.pixelSize: UI.FONT_SIZE_18
                    font.family: UI.DEFAULT_FONT_FAMILY
                    Component.onCompleted: {
                        if(keyType == "PartNumber"){
                            lineedit.inputMethodHints = Qt.ImhDigitsOnly;
                            lineedit.validator = addparts.validator
                        }
                    }
                    onTextChanged:{
                        returnInfoValuemap[keyType] = text;
                    }
                }
                CLine {
                    width:parent.width
                }
            }
        }

        Flickable{
            id :flick
            flickDeceleration: Flickable.VerticalFlick
            width: parent.width
            height: 600
            clip:true
            contentHeight:instruction.height
            boundsBehavior:Flickable.StopAtBounds
            anchors.top:pageHead.bottom
            anchors.topMargin: 20
            anchors.left:parent.left
            anchors.right: parent.right

            Column{
                id:instruction
                width:parent.width
                Repeater{
                    id:repeater
                    model:datamodel
                    delegate: itemdelegate
                }
            }
        }
    }
    function sendResult(){
        if(returnInfoValuemap["PartCode"].length == 0){
            CTOAST.requestToast(os.i18n.ctr(qsTr("零件编号不能为空")),"","");
            return;
        }else if(returnInfoValuemap["PartName"].length == 0){
            CTOAST.requestToast(os.i18n.ctr(qsTr("零件名称不能为空")),"","");
            return;
        }else if(returnInfoValuemap["PartNumber"].length == 0){
            CTOAST.requestToast(os.i18n.ctr(qsTr("零件数量不能为空")),"","");
            return;
        }else if(parseInt(returnInfoValuemap["PartNumber"]) > 255){
            CTOAST.requestToast(os.i18n.ctr(qsTr("零件数量不能超过255")),"","");
            return;
        }
        addparts.result(returnInfoValuemap);
        pageStack.pop();
    }

    Component.onCompleted: {
        if(addparts.pageType == "faultReport"){
            currentInfoList = addparts.faultReortInfoList;
        }else if (addparts.pageType == "maintain"){
            currentInfoList = addparts.maintainInfoList;
        } else {
            CTOAST.requestToast("类型错误", "", "");
            return;
        }
        //初始化返回值
        if(addparts.actionType == "update"){
            for(var key1 in resultInfoValuemap){
                returnInfoValuemap[key1] = resultInfoValuemap[key1];
            }
        } else if(addparts.actionType == "save"){
            for(var i = 0 ; i < currentInfoList.length; i++){
                var key2 = currentInfoList[i][0];
                returnInfoValuemap[key2] = "";
            }
            returnInfoValuemap["PartID"] = inspectionSystemConfig.getUUID();
        }
        //初始化listmodel
        for(var j= 0 ; j < currentInfoList.length; j++){
            var key3 = currentInfoList[j][0];
            var name = currentInfoList[j][1];
            console.log("Component.onCompleted:  key ", key3)
            datamodel.append({"keyType" : key3, "name": name});
        }
    }
}
