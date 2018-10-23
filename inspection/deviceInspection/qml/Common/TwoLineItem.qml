/*!
* \file TwoLineItem.qml
* \brief 列表页多行delegate
*
*列表页多行delegate
*
*\author 张子健 zhangzijian@syberos.com
*\version 1.0
*\date 2015/9/10
*/
import QtQuick 2.0
import com.syberos.basewidgets 2.0
import "delegateconstants.js" as UI
/*!
  \class TwoLineItem
  \inModule 公共控件
  \brief 列表页多行delegate
  \ingroup common
  列表页多行delegate
*/
Rectangle {
    id : rootRect

    width: parent.width
    color: mouse.pressed?UI.COLOR_LIST_ITEM_PRESS:UI.COLOR_WHITE
    height: visible ? (twolineColumn.height + 25 < 80 ? 80 : twolineColumn.height + 25): 0

    /** type:alias 第一行标题 */
    property alias  firstLeftText:   firstLeftText.text
    /** type:alias 第一行备注 */
    property alias  firstRightText:  firstRightText.text
    /** type:alias 第二行标题 */
    property alias  secondLeftText:  secondLeftText.text
    /** type:alias 第二行备注 */
    property alias  secondRightText: secondRightText.text
    /** type:alias 鼠标事件是否可用 */
    property alias mouseEnable: mouse.enabled


    /** type:alias rFID编码 */
    property string rFIDCode:""
    /** type:alias 一维码 */
    property string barCode:""
    /** type:alias 二维码 */
    property string qRCode:""
    /** type:string 是否允许手动操作*/
    property string twolimitMode:""
    /**
     *点击信号
     */
    signal triggered()

    Component.onCompleted: {
        if(secondLeftText.text==""){
           secondLeftText.height=0;
        }
        if(firstRightText.text==""){
           firstRightText.height=0;
        }
        if(secondRightText.text==""){
           secondRightText.height=0;
        }
    }

    Column{
        id:twolineColumn
        spacing: 10
        width: parent.width
        anchors.top:  parent.top
        anchors.topMargin: UI.TWOLINES_UP_TEXT_TOP

        Text {
            id: firstLeftText
            color: UI.COLOR_BLACK_LOW
            font.pixelSize: UI.FONT_SIZE_20
            font.family: UI.FONT_FAMILY_Z
            anchors {
                left: parent.left
                leftMargin: UI.TWOLINES_TEXT_LEFT
            }
            width: twolimitMode==1?
                       parent.width - UI.TWOLINES_TEXT_LEFT -100:
                       parent.width - UI.TWOLINES_TEXT_LEFT * 2
            wrapMode:Text.WrapAnywhere
        }

        Text {
            id: secondLeftText
            font.pixelSize: UI.FONT_SIZE_16
            font.family: UI.FONT_FAMILY_Z
            color:  UI.COLOR_GRAY_LOW
            visible: secondLeftText !=""
            anchors {
                left: parent.left
                leftMargin: UI.TWOLINES_TEXT_LEFT
            }
            width: twolimitMode == 1 ?
                       parent.width - UI.TWOLINES_TEXT_LEFT -100:
                       parent.width - UI.TWOLINES_TEXT_LEFT * 2
            wrapMode:Text.WrapAnywhere
            horizontalAlignment: Text.AlignLeft
        }

        Text {
            id: firstRightText
            color: UI.COLOR_BLACK_LOW
            font.pixelSize: UI.FONT_SIZE_16
            font.family: UI.FONT_FAMILY_Z
            visible: firstRightText != ""
            anchors {
                left: parent.left
                leftMargin: UI.TWOLINES_TEXT_LEFT
            }
            width: twolimitMode==1?
                       parent.width - UI.TWOLINES_TEXT_LEFT -100:
                       parent.width - UI.TWOLINES_TEXT_LEFT * 2
            wrapMode:Text.WrapAnywhere
            horizontalAlignment: Text.AlignLeft
        }

        Text {
            id: secondRightText
            font.pixelSize: UI.FONT_SIZE_16
            font.family: UI.FONT_FAMILY_Z
            color: UI.COLOR_GRAY_NUMBER
            visible: secondRightText != ""
            anchors {
                left: parent.left
                leftMargin: UI.TWOLINES_TEXT_LEFT
            }
            width: twolimitMode == 1?
                       parent.width - UI.TWOLINES_TEXT_LEFT -100:
                       parent.width - UI.TWOLINES_TEXT_LEFT * 2
            wrapMode:Text.WrapAnywhere
            horizontalAlignment: Text.AlignLeft
        }
    }

    Rectangle{
        visible: twolimitMode==1
        anchors.top:  parent.top
        anchors.topMargin: UI.TWOLINES_UP_TEXT_TOP
        anchors.right: parent.right
        anchors.rightMargin: 20
        Image {
            id: rfidlittle
            width: 25
            height: 25
            anchors.top:  parent.top
            anchors.right: parent.right
            visible: rFIDCode!=""
            source: "qrc:/icon/rfidlittle.png"
        }
        Image {
            id: barcodelittle
            width: 25
            height: 25
            anchors.top:  parent.top
            anchors.right: rFIDCode!=""?rfidlittle.left:parent.right
            anchors.rightMargin: rFIDCode!=""?20:0
            visible: barCode!=""
            source: "qrc:/icon/barcodelittle.png"
        }
        Image {
            id: touch
            width: 25
            height: 25
            anchors.top:  parent.top
            anchors.right: parent.right
            visible: barCode==""&&rFIDCode==""
            source: "qrc:/icon/touchlittle.png"
        }
    }

    CLine {
       anchors.bottom: parent.bottom
    }

    MouseArea {
        id: mouse
        anchors.fill: parent
        onClicked: {
            rootWindow.closeKeyboard();
            triggered()
        }
    }
}
