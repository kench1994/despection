import QtQuick 2.0
import com.syberos.basewidgets 2.0
import "../Common"
import "../Common/constants.js" as UI

CommonPage{
    id:partInfoPage
    statusBarHoldEnabled: true
    statusBarHoldItemColor: "white"
    orientationPolicy: CPageOrientation.LockPortrait
    property var selectedPartIndexIDMap : { return {}}
    color: "white"
    property var partInfoList : []
    property int selectedForEditIndex: -1
    property int partscount:-1
    property string pageType
    signal result(var partList)

    ListModel{
        id :partsmodel
    }

    Component.onCompleted:{
        initModelData()
    }
    Component.onDestruction: {
        partInfoPage.result(partInfoPage.partInfoList);
    }

    function modifyComponetsParts(map){
        partInfoPage.partInfoList[partInfoPage.selectedForEditIndex] = map;
        console.log("modifyComponetsParts  ", map["PartID"], "  selectedIndex is ", partInfoPage.selectedForEditIndex);
        initModelData();
    }

    function initModelData(){
        partInfoPage.selectedPartIndexIDMap = {};
        partInfoPage.partscount = partInfoPage.partInfoList.length;
        partsmodel.clear();
        for(var i = 0; i < partInfoPage.partscount; i++){
            if(partInfoPage.partInfoList[i]["OperType"] == "0" || partInfoPage.partInfoList[i]["OperType"] == "3"){
                console.log("partInfoPage.partInfoList[i][OperType] ", partInfoPage.partInfoList[i]["OperType"])
                continue;
            }
            partsmodel.append({
                                  "name" : partInfoPage.partInfoList[i]["PartName"],
                                  "code" : partInfoPage.partInfoList[i]["PartCode"],
                                  "partId" : partInfoPage.partInfoList[i]["PartID"],
                                  "listIndex" : i,
                                  "count" : partInfoPage.partInfoList[i]["PartNumber"],
                              });
        }
    }


    function deletePart(){
        if(Object.keys(partInfoPage.selectedPartIndexIDMap).length == 0){
            CTOAST.requestToast("请选择要删除的零部件","","")
            return;
        }
        for(var key in partInfoPage.selectedPartIndexIDMap){
            if(partInfoPage.partInfoList[key]["OperType"] == "2")
                partInfoPage.partInfoList[key]["OperType"] = "0";
            else {
                partInfoPage.partInfoList[key]["OperType"] = "3";
            }
        }
        initModelData();
    }

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
                rightItemText:"删除"
                leftItemIcon:""
                titleText:"零部件信息"
                onLeftItemTriggered: {
                    pageStack.pop()
                }
                onRightItemTriggered:{
                    deletePart();
                }
                anchors.top: parent.top
                leftItemBackgroundEnabled: false
            }
        }
        Component{
            id:itemdelegate
            Column{
                width:parent.width
                Row{
                    width: parent.width
                    anchors.left: parent.left
                    Rectangle{
                        width: 50
                        height: partInfoOperItem.height -2
                        CCheckBox {
                            id:checkbox
                            anchors.verticalCenter: parent.verticalCenter
                            checked: false
                            onClicked:{
                                var listIndex = partInfoOperItem.dataListIndex;
                                console.log("select list index is ", listIndex);
                                if(checkbox.checked)
                                    partInfoPage.selectedPartIndexIDMap[listIndex] = partInfoOperItem.itemId;
                                else
                                    delete partInfoPage.selectedPartIndexIDMap[listIndex];
                            }
                        }
                    }

                    ListItem{
                        id:partInfoOperItem
                        width:parent.width -50
                        property int dataListIndex: listIndex
                        itemId: partId
                        mainTitle:name
                        subTitle: "编号:" + code + "\n数量:" + count
                        type:"category"
                        endImage: "qrc:/icon/xiangyou.png"
                        onClicked: {
                            //进入修改页面
                            var infoMap = {};
                            var listIndex = partInfoOperItem.dataListIndex;
                            console.log("select list index is ", listIndex);
                            partInfoPage.selectedForEditIndex = listIndex;
                            infoMap = partInfoPage.partInfoList[listIndex];
                            var addpartPage = rootWindow.pageStack.push("qrc:/qml/Common/AddComponents.qml",
                                                                        {"resultInfoValuemap":infoMap,"actionType":"update", "pageType" : partInfoPage.pageType, "headTitle" : "修改零部件"});
                            addpartPage.result.connect(partInfoPage.modifyComponetsParts);
                        }
                    }
                }
            }
        }

        Flickable{
            id :flick
            flickDeceleration: Flickable.VerticalFlick
            width: parent.width
            height: 700
            clip:true
            contentHeight:componentsinfo.height
            boundsBehavior:Flickable.StopAtBounds
            anchors.top:pageHead.bottom
            anchors.topMargin: 20
            anchors.left:parent.left
            anchors.right: parent.right
            Column{
                id :componentsinfo
                width: parent.width
                Repeater{
                    model:partsmodel
                    delegate:itemdelegate
                }
            }
        }
        CIconLabel {
            id: empty
            visible: partsmodel.count == 0
            width: parent.width
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.left: parent.left
            iconSource: "qrc:/icon/empty.png"
            text: "没有零件信息"
            spacing: 40
        }
    }
}
