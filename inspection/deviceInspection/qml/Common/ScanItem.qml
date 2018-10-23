/*!
* \file ScanItem.qml
* \brief 设备扫描控件.
*
*
*\author 何昆 hekun@syberos.com
*\version 1.0
*\date 2015/9/8
*/
import QtQuick 2.0
import com.syberos.basewidgets 2.0
import "./constants.js" as UI



/*!
  \class ScanItem
  \inModule 公共控件
  \brief 设备扫描控件
  \ingroup common
  包括RFID扫描和二维码扫描
*/

Rectangle {
    id: scanRect

     /** type:int  图标距父控件顶端的距离*/
    property int imageTopMargin: 26
     /** type:int 文本距父控件顶端的距离*/
    property int textbottomMargin: 10
    property bool showLine: true

    property var parentPage
    property alias rfidVisible: rfidScanRect.visible
    property alias barCodeVisible: barCodeScanRect.visible

    property alias rfidEnabled: rfidScanRect.enabled
    property alias barCodeEnabled: barCodeScanRect.enabled
    /**
     *发送RFID扫描结果
     * @param RFIDCode RFID扫描结果
     */
    signal resultRFIDCode(string RFIDCode)

    /**
     *发送二维码扫描结果
     * @param BarCode 二维码扫描结果
     */

    signal resultBarCode(string BarCode)

    width: parent.width
    height: UI.SCAN_ITEM_H
    color: "#e8e8e8"

    Connections {
        target: rootWindow
        onScanRFID:{
            console.log("====ScanItem=====onScanRFID parentPage status ", scanRect.parentPage.status,
                        "rfidScanRect.enabled ", rfidScanRect.enabled,
                        "rfidScanRect.visible ", rfidScanRect.visible);
            if(scanRect.parentPage.status == CPageStatus.Show && rfidScanRect.enabled && rfidScanRect.visible){
                var rfidScanPage = pageStack.push("qrc:/qml/Common/RFIDScan.qml",{"parentPage":scanRect.parentPage});
                rfidScanPage.sendRFIDResult.connect(scanRect.parentPage.resultRFIDCode);
            }
        }
        onScanBarCode:{
            console.log("===ScanItem=====Bar CodeScan parentPage status", scanRect.parentPage.status,
                        "barCodeScanRect.enabled  " , barCodeScanRect.enabled,
                        "barCodeScanRect.visible  " , barCodeScanRect.visible);
            if(scanRect.parentPage.status == CPageStatus.Show && barCodeScanRect.enabled && barCodeScanRect.visible){
                var barCodePage = pageStack.push("qrc:/qml/Common/BarCodeScan.qml", {"parentPage": scanRect.parentPage});
                barCodePage.decodeFinished.connect(scanRect.parentPage.resultBarCode);
            }
        }
    }

    Rectangle {
        id:rfidScanRect
        clip: true
        width: visible ?(barCodeScanRect.visible? parent.width/2: parent.width):0
        height: parent.height
        anchors {
            top: parent.top
            left: parent.left
        }
        color: "#232936"
        Image {
            id: rfidImage
            anchors {
                top: parent.top
                topMargin:imageTopMargin
                horizontalCenter: parent.horizontalCenter
            }
            source: "qrc:/icon/rfid.png"
        }
        Text {
            anchors {
                bottom: parent.bottom
                bottomMargin: textbottomMargin
                horizontalCenter: parent.horizontalCenter
            }
            text: os.i18n.ctr(qsTr("RFID Scan"))
            color: "#ffae20"
            font.pixelSize: 16
            font.family: UI.DEFAULT_FONT_FAMILY
        }
        MouseArea {
            id: rfidMouse
            anchors.fill: parent
            onClicked: {
                var rfidScanPage = pageStack.push("qrc:/qml/Common/RFIDScan.qml", {"parentPage":scanRect.parentPage});
                rfidScanPage.sendRFIDResult.connect(scanRect.parentPage.resultRFIDCode);
            }
        }
    }
    Rectangle {
        id:barCodeScanRect
        clip: true
        width: visible ?(rfidScanRect.visible? parent.width/2: parent.width):0
        height: parent.height
        anchors {
            top: parent.top
            right: parent.right
        }
        color: "#0e111d"
        Image {
            id: barcodeImage
            anchors {
                top: parent.top
                topMargin: imageTopMargin
                horizontalCenter: parent.horizontalCenter
            }
            source: "qrc:/icon/barcode.png"
        }
        Text {
            anchors {
                bottom: parent.bottom
                bottomMargin: textbottomMargin
                horizontalCenter: parent.horizontalCenter
            }
            text: os.i18n.ctr(qsTr("Barcode Scan"))
            color: "#44acf4"
            font.pixelSize: 16
            font.family: UI.DEFAULT_FONT_FAMILY
        }
        MouseArea {
            id: barCodeMouse
            anchors.fill: parent
            onClicked: {
                // entry barcode page
                console.log("push   qrc:/qml/Common/BarCodeScan.qml")
                var barScanpage = pageStack.push("qrc:/qml/Common/BarCodeScan.qml", {"parentPage":scanRect.parentPage});
                barScanpage.decodeFinished.connect(scanRect.parentPage.resultBarCode);
            }
        }
    }
    Rectangle{
        color:"#e8e8e8"
        width: 2
        height:parent.height
        visible: rfidScanRect.visible && barCodeScanRect.visible
        x: (parent.width-width)/2
        radius: 2
    }

    CLine {
        visible: showLine
        anchors.bottom: parent.bottom
    }
    Component.onDestruction: {
        console.debug("----scanRect----------Component.onDestruction-------")
    }
}
