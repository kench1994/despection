/*!
* \file DeviceStdCheckFormOnlyEnum.qml
* \brief 标准化检查枚举控件
*
*此界面是标准化检查枚举控件
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
  \class DeviceStdCheckFormOnlyEnum
  \inModule 标准化检查枚举控件
  \brief 标准化检查枚举控件
  \ingroup StandardizedTask
*/
Rectangle {
    id:stdCheckFormOnlyEnum

    /** type:string 控件类型 */
    property string paramType: ""

    /** type:string  检查项后缀（例如处，台等）*/
    property string unit : ""

    /** type:alias  该项是否选中 */
    property alias  checked: checkbox.checked

    /** type:alias   主标题*/
    property alias  mainTitle: listItem.mainTitle

    /** type:alias   右标题*/
    property alias  endTitle: listItem.endTitle

    /** type:alias 控件itemId */
    property alias  itemId: listItem.itemId

    /**
     * 该项选中状态变化信号
     * @param checked  是否选中
     */
    signal changedChecked(bool checked)

    /**
     * 点击信号
     * @param itemId  itemId
     */
    signal clicked(string itemId)
    width: parent.width
    height: selectListItem.height
    Item{
        id:selectListItem
        width: parent.width
        height: listItem.height
        anchors.top: parent.top

        CCheckBox {
            id:checkbox
            anchors.left: parent.left
            anchors.leftMargin: UI.LIST_ICON_MARGIN_LEFT
            anchors.verticalCenter: parent.verticalCenter
            checked: true
            onClicked:{
                stdCheckFormOnlyEnum.changedChecked(checkbox.checked)
            }
        }

        ListItem{
            id:listItem
            anchors.left:checkbox.right
            anchors.right: parent.right
            enabled: checkbox.checked
            topSeparatorHeight:0
            bottomSeparatorHeight: 0
            type:"category"
            onClicked: {
                stdCheckFormOnlyEnum.clicked(itemId);
            }
        }
    }
}
