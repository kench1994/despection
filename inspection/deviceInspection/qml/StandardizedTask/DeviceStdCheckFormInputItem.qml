/*!
* \file DeviceStdCheckFormInputItem.qml
* \brief 标准化检查输入控件
*
*此界面是标准化检查输入控件
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
  \class DeviceStdCheckFormInputItem
  \inModule 标准化检查输入控件
  \brief 标准化检查输入控件
  \ingroup StandardizedTask
*/
Rectangle {
    id:stdCheckForminputItem

    width: parent.width
    height: inputItem.height

    /** type:string 控件类型 */
    property string paramType: ""

    /** type:string  检查项后缀（例如处，台等）*/
    property string unit : ""

    /** type:alias  该项是否选中 */
    property alias  checked: checkbox.checked

    /** type:alias   检查项标签名 */
    property alias  mainTitle: inputItem.mainTitle

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

    CCheckBox {
        id:checkbox
        anchors.left: parent.left
        anchors.leftMargin: UI.LIST_ICON_MARGIN_LEFT
        anchors.verticalCenter: parent.verticalCenter
        checked: true
        onClicked:{
            stdCheckForminputItem.changedChecked(checkbox.checked)
        }
    }

    DeviceStdCheckBaseInputItem{
        id:inputItem
        enabled: checkbox.checked
        anchors.left: checkbox.right
        anchors.leftMargin: 15
        anchors.right: unitLabel.left
        anchors.rightMargin: 15
        validator:RegExpValidator{regExp: /^[1-9][0-9]{0,4}$/}
        inputMethodhints:Qt.ImhHiddenText|Qt.ImhFormattedNumbersOnly
        mainTextFontColor: checkbox.checked? UI.LIST_TITLE_COLOR:UI.LIST_TITLE_DISABLE_COLOR
        editPlaceHoldeText: "1"
        onTextChanged: {
            stdCheckForminputItem.textChanged(text);
        }
    }
    Text {
        id: unitLabel
        color: checkbox.checked? UI.LIST_TITLE_COLOR:UI.LIST_TITLE_DISABLE_COLOR
        anchors.right: parent.right
        anchors.rightMargin:UI.LIST_ICON_MARGIN_RIGHT
        anchors.verticalCenter: parent.verticalCenter
        text: stdCheckForminputItem.unit
    }
}
