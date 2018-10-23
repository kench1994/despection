import QtQuick 2.0
import com.syberos.basewidgets 2.0
import "../Common"
import "../Common/constants.js" as UI
import "../Multimedia"
CommonPage {
    id: deviceRepairArchives
    property alias titleText: pageHeadTitle.titleText
    property string action
    property real textLeftMargin: 40
    property real textRightMargin: 20
    property real textTopMargin: 25
    property real textSpacing: 14
    property real textSize: 18
    property color textColor: "#666666"
    property var info
    property var paramValue
    property var dataPackageValue
    statusBarHoldEnabled: true
    statusBarHoldItemColor: "white"
    orientationPolicy: CPageOrientation.LockPortrait
    clip: true

    contentAreaItem:  Rectangle {
        id: content
        anchors.fill: parent
        color:"#eeeff1"
        Rectangle{
            id:pageHead
            width: parent.width
            height: pageHeadTitle.height
            CommonTitlebar{
                id: pageHeadTitle
                width: parent.width
                leftItemEnabled: true
                titleItemEnabled: true
                rightItemEnabled: false
                leftItemText:""
                titleText: "设备维修明细"
                onLeftItemTriggered: pageStack.pop()
                anchors.top: parent.top
                onLeftItemChanged:leftItem.backgroundEnabled = false
                leftItemBackgroundEnabled: false
            }
        }
        Flickable{
            id: contentFlick
            width: parent.width
            anchors.top: pageHead.bottom
            anchors.bottom: sperateItem.top
            flickDeceleration: Flickable.VerticalFlick
            contentHeight: contentCol.height + topToHeaderSeparatorItem.height
            clip: true
            Item{
                id:topToHeaderSeparatorItem
                width: parent.width;height: UI.GROUP_SEPARATOR_HIGHT
            }
            Column{
                id:contentCol
                anchors.top: topToHeaderSeparatorItem.bottom
                width: parent.width
                spacing: 15
                Text {
                    id: deviceName
                    anchors.left: parent.left
                    anchors.leftMargin: textLeftMargin
                    anchors.right: parent.right
                    anchors.rightMargin: textRightMargin
                    wrapMode: Text.WrapAnywhere
                    text: "设备名称：" + dataPackageValue["tables"][0]["value"][0]["EquipmentName"]
                    font.pixelSize: textSize
                    font.family: UI.DEFAULT_FONT_FAMILY
                    color: textColor
                }

                Text {
                    id: deviceModelName
                    anchors.left: parent.left
                    anchors.leftMargin: textLeftMargin
                    anchors.right: parent.right
                    anchors.rightMargin: textRightMargin
                    wrapMode: Text.WrapAnywhere
                    text: "设备型号：" + dataPackageValue["tables"][0]["value"][0]["ModelName"]
                    font.pixelSize: textSize
                    font.family: UI.DEFAULT_FONT_FAMILY
                    color: textColor
                }

                Text {
                    id: deviceEquipmentCode
                    anchors.left: parent.left
                    anchors.leftMargin: textLeftMargin
                    anchors.right: parent.right
                    anchors.rightMargin: textRightMargin
                    wrapMode: Text.WrapAnywhere
                    text: "设备编码：" + dataPackageValue["tables"][0]["value"][0]["EquipmentCode"]
                    font.pixelSize: textSize
                    font.family: UI.DEFAULT_FONT_FAMILY
                    color: textColor
                }

                Text {
                    id: deviceType
                    anchors.left: parent.left
                    anchors.leftMargin: textLeftMargin
                    anchors.right: parent.right
                    anchors.rightMargin: textRightMargin
                    wrapMode: Text.WrapAnywhere
                    text: "设备类型：" + dataPackageValue["tables"][0]["value"][0]["ClassName"]
                    font.pixelSize: textSize
                    font.family: UI.DEFAULT_FONT_FAMILY
                    color: textColor
                }

                Text {
                    id: deviceEnterpriseName
                    anchors.left: parent.left
                    anchors.leftMargin: textLeftMargin
                    anchors.right: parent.right
                    anchors.rightMargin: textRightMargin
                    wrapMode: Text.WrapAnywhere
                    text: "生产厂商：" + dataPackageValue["tables"][0]["value"][0]["EnterpriseName"]
                    font.pixelSize: textSize
                    font.family: UI.DEFAULT_FONT_FAMILY
                    color: textColor
                }
                Text {
                    id: deviceEquipmentOrganiseUnitName
                    anchors.left: parent.left
                    anchors.leftMargin: textLeftMargin
                    anchors.right: parent.right
                    anchors.rightMargin: textRightMargin
                    wrapMode: Text.WrapAnywhere
                    text: "设备所属单位：" + dataPackageValue["tables"][0]["value"][0]["EquipmentOrganiseUnitName"]
                    font.pixelSize: textSize
                    font.family: UI.DEFAULT_FONT_FAMILY
                    color: textColor
                }
                Text {
                    id: deviceFaultType
                    anchors.left: parent.left
                    anchors.leftMargin: textLeftMargin
                    anchors.right: parent.right
                    anchors.rightMargin: textRightMargin
                    wrapMode: Text.WrapAnywhere
                    text: "故障类型：" + dataPackageValue["tables"][0]["value"][0]["FaultType"]
                    font.pixelSize: textSize
                    font.family: UI.DEFAULT_FONT_FAMILY
                    color: textColor
                }
                Text {
                    id: deviceFaultStatus
                    anchors.left: parent.left
                    anchors.leftMargin: textLeftMargin
                    anchors.right: parent.right
                    anchors.rightMargin: textRightMargin
                    wrapMode: Text.WrapAnywhere
                    text: "故障状态：" + dataPackageValue["tables"][0]["value"][0]["FaultStatus"]
                    font.pixelSize: textSize
                    font.family: UI.DEFAULT_FONT_FAMILY
                    color: textColor
                }
                Text {
                    id: deviceReason
                    anchors.left: parent.left
                    anchors.leftMargin: textLeftMargin
                    anchors.right: parent.right
                    anchors.rightMargin: textRightMargin
                    wrapMode: Text.WrapAnywhere
                    text: "故障原因：" + dataPackageValue["tables"][0]["value"][0]["Reason"]
                    font.pixelSize: textSize
                    font.family: UI.DEFAULT_FONT_FAMILY
                    color: textColor
                }
                Text {
                    id: deviceExplain
                    anchors.left: parent.left
                    anchors.leftMargin: textLeftMargin
                    anchors.right: parent.right
                    anchors.rightMargin: textRightMargin
                    wrapMode: Text.WrapAnywhere
                    text: "故障说明：" + dataPackageValue["tables"][0]["value"][0]["Explain"]
                    font.pixelSize: textSize
                    font.family: UI.DEFAULT_FONT_FAMILY
                    color: textColor
                }

                Text {
                    id: deviceReportTime
                    anchors.left: parent.left
                    anchors.leftMargin: textLeftMargin
                    anchors.right: parent.right
                    anchors.rightMargin: textRightMargin
                    wrapMode: Text.WrapAnywhere
                    text: "上报时间：" + dataPackageValue["tables"][0]["value"][0]["ReportTime"]
                    font.pixelSize: textSize
                    font.family: UI.DEFAULT_FONT_FAMILY
                    color: textColor
                }

                Text {
                    id: devicePlanRepairDate
                    anchors.left: parent.left
                    anchors.leftMargin: textLeftMargin
                    anchors.right: parent.right
                    anchors.rightMargin: textRightMargin
                    wrapMode: Text.WrapAnywhere
                    text: "计划维修时间：" + dataPackageValue["tables"][0]["value"][0]["PlanRepairDate"]
                    font.pixelSize: textSize
                    font.family: UI.DEFAULT_FONT_FAMILY
                    color: textColor
                }

                Text {
                    id: deviceCustodian
                    anchors.left: parent.left
                    anchors.leftMargin: textLeftMargin
                    anchors.right: parent.right
                    anchors.rightMargin: textRightMargin
                    wrapMode: Text.WrapAnywhere
                    text: "保管人：" + dataPackageValue["tables"][0]["value"][0]["Custodian"]
                    font.pixelSize: textSize
                    font.family: UI.DEFAULT_FONT_FAMILY
                    color: textColor
                }
            }
        }
        Item{
            id:sperateItem
            width: parent.width
            height: 20
            anchors.bottom: showPhoto.top
        }

        MultimediaView{
            id:showPhoto
            visible: false
            anchors.bottom: parent.bottom
            interactionType:"showMultimedia"
        }
    }
    onStatusChanged: {
        if (status == CPageStatus.Show) {
            deviceRepairArchives.statusBarHoldEnabled = true
            gScreenInfo.setStatusBarStyle("white");
            gScreenInfo.setStatusBar(true)
        }
    }
    Component.onCompleted: {
        var multimediaData = dataPackageValue["tables"][1]["value"];
        //<Field Value="IDやNameやTitleやUrlやSerialKeyやMIMETypeやDescript"/>
        var attchs = [];
        for(var i = 0; i < multimediaData.length; i++){
            var attch = {};
            attch["Url"] = multimediaData[i]["Url"];
            attch["Path"] = "";
            attch["Title"] = multimediaData[i]["Title"];
            attch["Description"] = multimediaData[i]["Descript"];
            attch["DataIsOk"] = false;
            var mimiType = multimediaData[i]["MIMEType"];
            if(String(mimiType).indexOf("image/") != -1)
                attch["MultimediaType"] = "image";
            else if(String(mimiType).indexOf("video/") != -1)
                attch["MultimediaType"] = "video";
            else if(String(mimiType).indexOf("audio/") != -1)
                attch["MultimediaType"] = "audio";
            console.log("faultrepair .setMultimediaData  Url    ", multimediaData[i]["Url"], "  dataIsOk ", attch["DataIsOk"] );
            attchs.push(attch);
        }
        if(attchs.length > 0){
            showPhoto.visible = true;
            showPhoto.setMultimediaData(attchs);
        }
    }
}
