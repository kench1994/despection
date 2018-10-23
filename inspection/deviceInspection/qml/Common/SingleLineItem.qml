/*!
* \file SingleLineItem.qml
* \brief 列表页单行delegate
*
*列表页单行delegate
*
*\author 张子健 zhangzijian@syberos.com
*\version 1.0
*\date 2015/9/10
*/
import QtQuick 2.0
import com.syberos.basewidgets 2.0
import "delegateconstants.js" as UI
/*!
  \class SingleLineItem
  \inModule 公共控件
  \brief 列表页单行delegate
  \ingroup common
  列表页单行delegate
*/
Rectangle {
    id: rootRect

    width: parent.width
    height:rootRectColumn.height+25
    color: mouse.pressed?UI.COLOR_LIST_ITEM_PRESS:UI.COLOR_WHITE

    /** type:alias 标题 */
    property alias leftText: firstLeftText.text
    /** type:alias 备注 */
    property alias rightText: firstRightText.text

    signal triggered()

    Component.onCompleted: {
        if(firstLeftText.text==""){
           secondLeftText.height=0;
        }
        if(firstRightText.text==""){
           firstRightText.height=0;
        }
    }

    Column{
        id:rootRectColumn
        spacing: 10
        width: parent.width
        anchors.top:  parent.top
        anchors.topMargin: UI.TWOLINES_UP_TEXT_TOP

        Text {
            id: firstLeftText
            color: UI.COLOR_BLACK_LOW
            font.pixelSize: UI.FONT_SIZE_22
            font.family: UI.FONT_FAMILY_Z
            anchors {
                left: parent.left
                leftMargin: UI.SINGLELINE_LEFT_TEXT_LEFT
            }
            width: parent.width - UI.TWOLINES_TEXT_LEFT * 2
            wrapMode:Text.WordWrap
        }

        Text {
            id: firstRightText
            font.pixelSize: UI.FONT_SIZE_18
            font.family: UI.FONT_FAMILY_Z
            color: UI.COLOR_GRAY_LOW
            anchors {
                left: parent.left
                leftMargin: UI.SINGLELINE_LEFT_TEXT_LEFT
            }
            width: parent.width - UI.TWOLINES_TEXT_LEFT * 2
            wrapMode:Text.WordWrap
            horizontalAlignment: Text.AlignLeft
        }
    }

    CLine {
        anchors.bottom: parent.bottom
    }

    MouseArea {
        id: mouse
        anchors.fill: parent
        onClicked: {
            triggered();
        }
    }
}
