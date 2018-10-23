/*!
* \file DeviceStdCheckFormListItem.qml
* \brief 标准化检查枚举加输入控件
*
*此界面是标准化检查枚举加输入控件
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
  \class DeviceStdCheckFormListItem
  \inModule 标准化检查枚举加输入控件
  \brief 标准化检查枚举加输入控件
  \ingroup StandardizedTask
*/
Rectangle {
    id:stdCheckFormListItem

    width: parent.width
    height: selectListItem.height + inputItemItem.height

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

    /** type:alias 占位文本*/
    property alias  editPlaceHoldeText: inputItem.editPlaceHoldeText

    /** type:alias 输入值*/
    property alias  inputText: inputItem.inputText

    /**
     * 输入框内容变化信号
     * @param text 输入框内容
     */
    signal textChanged(string text)

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
                stdCheckFormListItem.changedChecked(checkbox.checked)
            }
        }
        ListItem{
            id:listItem
            anchors.left:checkbox.right
            anchors.right: parent.right
            enabled: checkbox.checked

            topSeparatorHeight:0
            bottomSeparatorHeight: 0
            //mainTitle: "aa"
            type:"category"
            //endTitle: "bbbbbb"
            onClicked: {
                stdCheckFormListItem.clicked(itemId);
            }
        }
    }
    Item{
        id:inputItemItem
        anchors.top: selectListItem.bottom
        width: parent.width
        height: inputItem.height
        enabled: checkbox.checked

        DeviceStdCheckBaseInputItem{
            id:inputItem
            mainTextFontColor: checkbox.checked? UI.LIST_TITLE_COLOR:UI.LIST_TITLE_DISABLE_COLOR
            anchors.left: parent.left
            anchors.right: unitLabel.left
            anchors.rightMargin: 15
            validator:RegExpValidator{regExp: /^[1-9][0-9]{0,4}$/}
            inputMethodhints:Qt.ImhHiddenText|Qt.ImhFormattedNumbersOnly
            editPlaceHoldeText: "1"
            onTextChanged: {
                stdCheckFormListItem.textChanged(text)
            }
        }

        Text {
            id: unitLabel
            anchors.right: parent.right
            anchors.rightMargin:UI.LIST_ICON_MARGIN_RIGHT
            font.pixelSize: 16
            color: checkbox.checked? UI.LIST_TITLE_COLOR:UI.LIST_TITLE_DISABLE_COLOR
            anchors.verticalCenter: parent.verticalCenter
            text: stdCheckFormListItem.unit
        }
    }
}
