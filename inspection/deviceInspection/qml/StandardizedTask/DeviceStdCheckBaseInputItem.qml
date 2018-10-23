/*!
* \file DeviceStdCheckBaseInputItem.qml
* \brief 标准化检查基础输入框控件
*
*此界面是标准化检查基础输入框控件
*
*\author 桓乐乐 huanlele@syberos.com
*\version 1.0
*\date 2015/9/7
*/
import QtQuick 2.1
import QtQuick.Controls 1.0
import com.syberos.basewidgets 2.0
import QtQuick.Controls 1.0
import "../Common"
import "../Common/constants.js" as UI
/*!
  \class DeviceStdCheckBaseInputItem
  \inModule 标准化检查基础输入框控件
  \brief 标准化检查基础输入框控件
  \ingroup StandardizedTask
*/
Rectangle{
    id:iniputItem
    width: parent.width
    height: titleLabel.height > iniputItem.iniputItemHeight?
                titleLabel.height:iniputItem.iniputItemHeight
    border.width: showType == "image" ? 2:0
    border.color: showType == "image" ? "gray":"white"
    radius: showType == "image" ? 5:0

    /** type:string 控件的itemId */
    property string itemId: ""

    /** type:string 标签名*/
    property string mainTitle: ""

     /** type:string 标签字体类型 */
    property string mainTextFontFamily: UI.DEFAULT_FONTFAMILY

    /** type:string 标签字体颜色 */
    property string mainTextFontColor: UI.LIST_TITLE_COLOR

    /** type:string 输入控件的最前方是否显示图片*/
    property string showType: "string"  //string or image

    /** type:string 最前方显示的图片路径*/
    property string labelIconPath: ""

    /** type:string 下拉列表的按钮图片路径*/
    property string dropListImagePath: ""

    /** type:string 占位文本*/
    property string editPlaceHoldeText:os.i18n.ctr(qsTr("Please input"))

    /** type:string 输入字体类型 */
    property string inputTextFontFamily: UI.DEFAULT_FONTFAMILY

    /** type:string 输入字体颜色 */
    property string inputTextFontColor: UI.LIST_SUBTITLE_COLOR

    /** type:alias 输入值*/
    property alias  inputText: inputEdit.text

    /** type:alias 输入控件的正则表达式*/
    property alias  validator: inputEdit.validator

    /** type:alias 输入控件的输入模式，明文或者密文*/
    property alias  echoMode: inputEdit.echoMode

    /** type:alias 输入控件的输入最大字数*/
    property alias  maximumLength: inputEdit.maximumLength

    /** type:alias 调用键盘模式*/
    property alias  inputMethodhints: inputEdit.inputMethodHints

    /** type:int 设置键盘enter键显示文字 */
    property alias  inputMethodHintExtensions: inputEdit.inputMethodHintExtensions

    /** type:alias 标签宽度 */
    property alias  titleWidth: labelRect.width

    /** type:alias 控件设置焦点 */
    property alias  isFocus: inputEdit.focus

    /** type:bool 是否显示下拉列表*/
    property bool   showDropList: false

    /** type:bool 是否启用密码模式*/
    property bool   passwordEnabled: false

    /** type:string 最前方显示的图片宽度*/
    property int    lableIconWidth: 20

    /** type:string 最前方显示的图片高度*/
    property int    lableIconHeight: 20

    /** type:int 控件高度*/
    property int    iniputItemHeight: UI.LIST_ITEM_HEIGHT

    /** type:int 标签字体大小 */
    property int    mainTextFontPixelSize: UI.LIST_TITLE_SIZE

    /** type:int 输入字体大小 */
    property int    inputTextFontPixelSize:UI.LIST_SUBTILE_SIZE

    /**
     * 输入框焦点变化信号
     * @param focus 是否获取焦点
     */
    signal textFocusChanged(bool focus)

    /**
     * 输入框内容变化信号
     * @param text 输入框内容
     */
    signal textChanged(string text)

    Item{
        id:labelRect
        anchors.left: parent.left
        anchors.right: inputRect.left
        anchors.rightMargin: UI.LIST_ICON_MARGIN_RIGHT
        height: parent.height
        Label {
            id: titleLabel
            anchors.verticalCenter: parent.verticalCenter
            text: mainTitle
            visible:iniputItem.showType == "string"
            horizontalAlignment:Text.AlignLeft
            anchors.left: parent.left
            anchors.right: parent.right
            wrapMode: Text.WrapAnywhere
            font.pixelSize: iniputItem.mainTextFontPixelSize
            font.family:iniputItem.mainTextFontFamily
            color:iniputItem.mainTextFontColor
        }
    }

    Rectangle{
        id:inputRect
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        height: 45
        width: 85
        border.color: "#cccccc"
        border.width: 1
        CTextField{
            id: inputEdit
            placeholderText: iniputItem.editPlaceHoldeText
            textColor: "#000000"
            clip: true
            anchors.fill: parent
            font.pixelSize: 16
            onTextChanged: {
                iniputItem.textChanged(text);
            }
        }
    }

    /**
     * 控件设置焦点
     */
    function forceActiveFocus(){
        inputEdit.forceActiveFocus();
    }
}//end Item

