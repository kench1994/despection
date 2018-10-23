/*!
* \file DeviceTitle.qml
* \brief 公共控件，在各个业务提交页面用于显示设备的一些详情信息
*
*公共控件，在各个业务提交页面用于显示设备的一些详情信息
*
*\author 桓乐乐 huanlele@syberos.com
*\version 1.0
*\date 2015/9/7
*/
import QtQuick 2.0
/*!
  \class DeviceTitle
  \inModule 公共控件
  \brief 公共控件，在各个业务提交页面用于显示设备的一些详情信息
  \ingroup common
  公共控件，在各个业务提交页面用于显示设备的一些详情信息
*/
Rectangle {
    id:deviceCommonParamList
    /** type:string 设备详情的属性名, (deviceLabel1~deviceLabel8)*/
    property string deviceLabel1:""

    /** type:string 设备详情的属性值, (deviceName1~deviceName8)*/
    property string deviceName1:""
    property string deviceLabel2:""
    property string deviceName2:""

    property string deviceLabel3:""
    property string deviceName3:""

    property string deviceLabel4:""
    property string deviceName4:""

    property string deviceLabel5:""
    property string deviceName5:""

    property string deviceLabel6:""
    property string deviceName6:""

    property string deviceLabel7:""
    property string deviceName7:""

    property string deviceLabel8:""
    property string deviceName8:""
    width: parent.width
    height: devCol.height
    Column{
        id: devCol
        width: parent.width
        Rectangle{
            width: parent.width
            height: 15
        }

        Rectangle{
            height: devName1.visible? devName1.height + 10:0
            width: parent.width
            Text {
                id:devLabel1
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: visible? 20 : 0
                font.pixelSize: 16
                color:"#333333"
                opacity: 0.6
                visible: text != ""
                text: deviceCommonParamList.deviceLabel1
            }
            Text {
                id: devName1
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: devLabel1.right
                anchors.leftMargin: 20
                anchors.right: parent.right
                anchors.rightMargin: 20
                wrapMode: Text.WrapAnywhere
                font.pixelSize: 20
                color:"#333333"
                visible: text != ""
                text: deviceCommonParamList.deviceName1
            }
        }

        Rectangle{
            height: devLabel2.visible? (devName2.text  != "" ? devName2.height + 10: devLabel2.height + 10):0
            width: parent.width
            Text {
                id:devLabel2
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: visible? 20 : 0
                font.pixelSize: 16
                color:"#333333"
                opacity: 0.6
                visible: text != ""
                text: deviceCommonParamList.deviceLabel2
            }
            Text {
                id: devName2
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: devLabel2.right
                anchors.leftMargin: 20
                anchors.right: parent.right
                anchors.rightMargin: 20
                wrapMode: Text.WrapAnywhere
                font.pixelSize: 16
                color:"#333333"
                opacity: 0.6
                visible: text != ""
                text: deviceCommonParamList.deviceName2
            }
        }
        Rectangle{
            height: devLabel3.visible?  (devName3.text  != "" ? devName3.height + 10: devLabel3.height + 10):0
            width: parent.width
            Text {
                id:devLabel3
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: visible? 20 : 0
                font.pixelSize: 16
                color:"#333333"
                opacity: 0.6
                visible: text != ""
                text: deviceCommonParamList.deviceLabel3
            }
            Text {
                id: devName3
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: devLabel3.right
                anchors.leftMargin: 20
                anchors.right: parent.right
                anchors.rightMargin: 20
                wrapMode: Text.WrapAnywhere
                font.pixelSize: 16
                color:"#333333"
                opacity: 0.6
                visible: text != ""
                text: deviceCommonParamList.deviceName3
            }
        }
        Rectangle{
            height: devLabel4.visible? (devName3.text  != "" ? devName4.height + 10: devLabel4.height + 10):0
            width: parent.width
            Text {
                id:devLabel4
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: visible? 20 : 0
                font.pixelSize: 16
                color:"#333333"
                opacity: 0.6
                visible: text != ""
                text: deviceCommonParamList.deviceLabel4//"设备保管人:"
            }
            Text {
                id: devName4
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: devLabel4.right
                anchors.leftMargin: 20
                anchors.right: parent.right
                anchors.rightMargin: 20
                wrapMode: Text.WrapAnywhere
                font.pixelSize: 16
                color:"#333333"
                opacity: 0.6
                visible: text != ""
                text: deviceCommonParamList.deviceName4
            }
        }
        Rectangle{
            height: devLabel5.visible? (devName5.text  != "" ? devName5.height + 10: devLabel5.height + 10):0
            width: parent.width
            Text {
                id:devLabel5
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: visible? 20 : 0
                font.pixelSize: 16
                color:"#333333"
                opacity: 0.6
                visible: text != ""
                text: deviceCommonParamList.deviceLabel5
            }
            Text {
                id: devName5
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: devLabel5.right
                anchors.leftMargin: 20
                anchors.right: parent.right
                anchors.rightMargin: 20
                wrapMode: Text.WrapAnywhere
                font.pixelSize: 16
                color:"#333333"
                opacity: 0.6
                visible: text != ""
                text: deviceCommonParamList.deviceName5
            }
        }
        Rectangle{
            height: devLabel6.visible? (devName6.text  != "" ? devName6.height + 10: devLabel6.height + 10):0
            width: parent.width
            Text {
                id:devLabel6
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: visible? 20 : 0
                font.pixelSize: 16
                color:"#333333"
                opacity: 0.6
                visible: text != ""
                text: deviceCommonParamList.deviceLabel6
            }
            Text {
                id: devName6
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: devLabel6.right
                anchors.leftMargin: 20
                anchors.right: parent.right
                anchors.rightMargin: 20
                wrapMode: Text.WrapAnywhere
                font.pixelSize: 16
                color:"#333333"
                opacity: 0.6
                visible: text != ""
                text: deviceCommonParamList.deviceName6
            }
        }
        Rectangle{
            height: devLabel7.visible? (devName7.text  != "" ? devName7.height + 10: devLabel7.height + 10):0
            width: parent.width
            Text {
                id:devLabel7
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: visible? 20 : 0
                font.pixelSize: 16
                color:"#333333"
                opacity: 0.6
                visible: text != ""
                text: deviceCommonParamList.deviceLabel7
            }
            Text {
                id: devName7
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: devLabel7.right
                anchors.leftMargin: 20
                anchors.right: parent.right
                anchors.rightMargin: 20
                wrapMode: Text.WrapAnywhere
                font.pixelSize: 16
                color:"#333333"
                opacity: 0.6
                visible: text != ""
                text: deviceCommonParamList.deviceName7
            }
        }
        Rectangle{
            height: devLabel8.visible? (devName8.text  != "" ? devName8.height + 10: devLabel8.height + 10):0
            width: parent.width
            Text {
                id:devLabel8
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: visible? 20 : 0
                font.pixelSize: 16
                color:"#333333"
                opacity: 0.6
                visible: text != ""
                text: deviceCommonParamList.deviceLabel8
            }
            Text {
                id: devName8
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: devLabel8.right
                anchors.leftMargin: 20
                anchors.right: parent.right
                anchors.rightMargin: 20
                wrapMode: Text.WrapAnywhere
                font.pixelSize: 16
                color:"#333333"
                opacity: 0.6
                visible: text != ""
                text: deviceCommonParamList.deviceName8
            }
        }
        Rectangle{
            width: parent.width
            height: 15
        }
    }
}
