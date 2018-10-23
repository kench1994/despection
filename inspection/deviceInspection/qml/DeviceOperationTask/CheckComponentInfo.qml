/*!
* \file CheckComponentInfo.qml
* \brief 运维流程查看零部件信息页面
*
*运维流程查看零部件信息页面
*
*\author 桓乐乐 huanlele@syberos.com
*\version 1.0
*\date 2015/9/7
*/
import QtQuick 2.0
import com.syberos.basewidgets 2.0
import "../Common"
import "../Common/constants.js" as UI
import "../Multimedia"
/*!
  \class FaultReport
  \inModule 运维流程查看零部件信息页面
  \brief 运维流程查看零部件信息页面
  \ingroup ComprehensiveTask
  运维流程查看零部件信息页面
*/
CommonPage {
    id: checkPartInfoPage
    statusBarHoldEnabled: true
    statusBarHoldItemColor: "white"
    orientationPolicy: CPageOrientation.LockPortrait
    clip: true

    /** type:var 页面展示需要，但是没有包含在dataPackageValue数据中的数据*/
    property var    otherParam

    /** type:var 从服务器获取的调拨详情数据*/
    property var    originalData

    property int  selectedIndex: -1
    contentAreaItem:  Rectangle {
        id: rect
        anchors.fill: parent
        color:"#eeeff1"

        Rectangle{
            id:pageHead
            width: parent.width
            height: pageHeadTitle.height
            //color:"red"

            CommonTitlebar{
                id: pageHeadTitle
                width: parent.width
                clip:true
                //visible:false
                leftItemEnabled: true
                titleItemEnabled: true
                rightItemEnabled: false
                leftItemPixelSize: 22
                titleAreaCentered:true
                leftItemTextColor: "#666666"
                titleText:"零部件信息"
                onLeftItemTriggered: {
                    pageStack.pop();
                }
                anchors.top: parent.top
            }
        }
        Item{
            id:topToHeaderSeparatorItem
            width: parent.width;height: UI.GROUP_SEPARATOR_HIGHT
            anchors.top: pageHead.bottom
        }
        ListView{
            id:partListView
            width: parent.width
            anchors.top: topToHeaderSeparatorItem.bottom
            anchors.bottom: parent.bottom
            model:partListModel
            delegate: ListItem{
                mainTitle: PartName
                subTitle: "编号:" + PartCode
                onClicked: {
                    partInfoDialog.partCode = partListModel.get(index).PartCode
                    partInfoDialog.partName = partListModel.get(index).PartName
                    partInfoDialog.partStyle = partListModel.get(index).PartStyle
                    partInfoDialog.partNumber = partListModel.get(index).PartNumber + partListModel.get(index).PartUnit
                    partInfoDialog.partManufacturer = partListModel.get(index).PartManufacturer
                    partInfoDialog.show();
                }
            }
        }
    }
    ListModel{
        id:partListModel
    }
    PopDialog{
        id:partInfoDialog
        property string partCode: ""
        property string partName: ""
        property string partStyle: ""
        property string partNumber: ""
        property string partManufacturer: ""
        Rectangle{
            anchors.centerIn: parent
            width: parent.width * 0.6
            height: contentCol.height + 60
            radius: 10
            Column{
                id:contentCol
                width: parent.width
                anchors.verticalCenter: parent.verticalCenter
                spacing: 5
                Item{
                    width: parent.width
                    height: Math.max(partCodeText.height, partCodeContentText.height)
                    Text {
                        id: partCodeText
                        anchors.left: parent.left
                        anchors.leftMargin: 20
                        text: "编号:"
                    }
                    Text{
                        id:partCodeContentText
                        anchors.left: partCodeText.right
                        anchors.leftMargin: 20
                        anchors.right: parent.right
                        anchors.rightMargin: 20
                        wrapMode: Text.WrapAnywhere
                        text:partInfoDialog.partCode
                    }
                }
                Item{
                    width: parent.width
                    height: Math.max(partNameText.height, partNameContentText.height)
                    Text {
                        id: partNameText
                        anchors.left: parent.left
                        anchors.leftMargin: 20
                        text: "名称:"
                    }
                    Text{
                        id:partNameContentText
                        anchors.left: partNameText.right
                        anchors.leftMargin: 20
                        anchors.right: parent.right
                        anchors.rightMargin: 20
                        wrapMode: Text.WrapAnywhere
                        text:partInfoDialog.partName
                    }
                }
                Item{
                    width: parent.width
                    height: Math.max(partStyleText.height, partStyleContentText.height)
                    Text {
                        id: partStyleText
                        anchors.left: parent.left
                        anchors.leftMargin: 20
                        text: "型号:"
                    }
                    Text{
                        id:partStyleContentText
                        anchors.left: partStyleText.right
                        anchors.leftMargin: 20
                        anchors.right: parent.right
                        anchors.rightMargin: 20
                        wrapMode: Text.WrapAnywhere
                        text:partInfoDialog.partStyle
                    }
                }
                Item{
                    width: parent.width
                    height: Math.max(partNumberText.height, partNumberContentText.height)
                    Text {
                        id: partNumberText
                        anchors.left: parent.left
                        anchors.leftMargin: 20
                        text: "数量:"
                    }
                    Text{
                        id:partNumberContentText
                        anchors.left: partNumberText.right
                        anchors.leftMargin: 20
                        anchors.right: parent.right
                        anchors.rightMargin: 20
                        wrapMode: Text.WrapAnywhere
                        text:partInfoDialog.partNumber
                    }
                }
                Item{
                    width: parent.width
                    height: Math.max(partManufacturerText.height, partManufacturerContentText.height)
                    Text {
                        id: partManufacturerText
                        anchors.left: parent.left
                        anchors.leftMargin: 20
                        text: "厂商:"
                    }
                    Text{
                        id:partManufacturerContentText
                        anchors.left: partManufacturerText.right
                        anchors.leftMargin: 20
                        anchors.right: parent.right
                        anchors.rightMargin: 20
                        wrapMode: Text.WrapAnywhere
                        text:partInfoDialog.partManufacturer
                    }
                }
            }
        }
    }

    Component.onCompleted: {
        console.log("fault  dataPackageValue[tables].length", originalData["tables"].length)
        if(originalData["tables"].length <= 0){
            CTOAST.requestToast("数据错误","","");
            return;
        }
  //"IDやFaultRepairIDやPartCodeやPartNameやPartStyleやPartUnitやPartNumberやPartManufacturer
        //やCreatedByやCreatedDateやModifiedByやModifiedDate"/>
        var data = originalData["tables"][0]["value"];
        for(var i = 0; i < data.length; i++){
              partListModel.append({"ID" : data[i]["ID"],
                                   "FaultRepairID" : data[i]["FaultRepairID"],
                                   "PartCode" : data[i]["PartCode"],
                                   "PartName" : data[i]["PartName"],
                                   "PartStyle" : data[i]["PartStyle"],
                                   "PartUnit" : data[i]["PartUnit"],
                                   "PartNumber" : data[i]["PartNumber"],
                                   "PartManufacturer" : data[i]["PartManufacturer"],
                                   "CreatedBy" : data[i]["CreatedBy"],
                                   "CreatedDate" : data[i]["CreatedDate"],
                                   "ModifiedBy" : data[i]["ModifiedBy"],
                                   "ModifiedDate" : data[i]["ModifiedDate"]});
        }
        partListView.update();
    }
}
