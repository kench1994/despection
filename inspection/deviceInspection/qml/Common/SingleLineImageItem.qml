/*!
* \file SingleLineImageItem.qml
* \brief 功能页delegate
*
*功能页delegate
*
*\author 张子健 zhangzijian@syberos.com
*\version 1.0
*\date 2015/9/10
*/
import QtQuick 2.0
import com.syberos.basewidgets 2.0
import "delegateconstants.js" as UI
/*!
  \class SingleLineImageItem
  \inModule 公共控件
  \brief 功能页delegate
  \ingroup common
  功能页delegate
*/
Rectangle {
    id: rootRect

    width: parent.width
    height: UI.LIST_ITEM_H
    color: mouse.pressed?UI.COLOR_LIST_ITEM_PRESS:UI.COLOR_WHITE

    /** type:alias 行首图标 */
    property alias icon: firstImage.source
    /** type:alias 标题 */
    property alias text: firstText.text

    signal triggered()

    Image {
        id: firstImage
        width: 30
        height: 30
        anchors {
            left: parent.left
            leftMargin: UI.SINGLELINE_IMAGE_LEFT
            verticalCenter: parent.verticalCenter
        }
    }

    Text {
        id: firstText
        font.pixelSize: UI.FONT_SIZE_22
        font.family: UI.FONT_FAMILY_Z
        color: UI.COLOR_BLACK_LOW
        anchors {
            left: firstImage.right
            leftMargin: UI.SINGLELINE_TEXT_LEFT
            verticalCenter: parent.verticalCenter
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
