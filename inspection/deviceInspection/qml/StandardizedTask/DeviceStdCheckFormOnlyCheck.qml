/*!
* \file DeviceStdCheckFormOnlyCheck.qml
* \brief 标准化检查选择控件
*
*此界面是标准化检查选择控件
*
*\author 桓乐乐 huanlele@syberos.com
*\version 1.0
*\date 2015/9/7
*/
import QtQuick 2.0
import com.syberos.basewidgets 2.0
import "../Common"
import "../Common/constants.js" as UI
/*!
  \class DeviceStdCheckFormOnlyCheck
  \inModule 标准化检查选择控件
  \brief 标准化检查选择控件
  \ingroup StandardizedTask
*/
Rectangle {
    id:stdCheckFormOnlyCheck
    /** type:string 控件类型 */
    property string paramType: ""

    /** type:alias   主标题*/
    property string mainTitle : ""

    /** type:alias  该项是否选中 */
    property alias  checked: checkbox.checked

    /**
     * 该项选中状态变化信号
     * @param checked  是否选中
     */
    signal changedChecked(bool checked)
    width: parent.width
    height: unitLabel.height > 80 ? unitLabel.height : 80

    CCheckBox {
        id:checkbox
        anchors.left: parent.left
        anchors.leftMargin: UI.LIST_ICON_MARGIN_LEFT
        anchors.verticalCenter: parent.verticalCenter
        checked: true
        onClicked:{
            stdCheckFormOnlyCheck.changedChecked(checkbox.checked)
        }
    }

    Text {
        id: unitLabel
        anchors.left: checkbox.right
        anchors.leftMargin: UI.LIST_ICON_MARGIN_LEFT
        anchors.right: parent.right
        anchors.rightMargin:UI.LIST_ICON_MARGIN_RIGHT
        anchors.verticalCenter: parent.verticalCenter
        wrapMode: Text.WrapAnywhere
        font.pixelSize: UI.LIST_TITLE_SIZE
        color: checkbox.checked? UI.LIST_TITLE_COLOR:UI.LIST_TITLE_DISABLE_COLOR
        text: stdCheckFormOnlyCheck.mainTitle
    }
}
