/*!
* \file ListInputItem.qml
* \brief 公共控件，输入控件
*
*公共控件，输入控件
*
*\author 桓乐乐 huanlele@syberos.com
*\version 1.0
*\date 2015/9/7
*/
import QtQuick 2.1
import QtQuick.Controls 1.0
import com.syberos.basewidgets 2.0
import QtQuick.Controls 1.0
import "constants.js" as UI
/*!
  \class ListInputItem
  \inModule 公共控件
  \brief 公共控件，输入控件
  \ingroup common
  公共控件，输入控件
*/
Rectangle{
    id:iniputItem
    /** type:string 控件的itemId */
    property string itemId: ""

    /** type:string 标签名*/
    property string mainTitle: ""

    /** type:alias 输入值*/
    property alias inputText: inputEdit.text

    /** type:alias 输入控件的正则表达式*/
    property alias validator: inputEdit.validator

    /** type:alias 输入控件的输入模式，明文或者密文*/
    property alias echoMode: inputEdit.echoMode

    /** type:alias 输入控件的输入最大字数*/
    property alias maximumLength: inputEdit.maximumLength

    /** type:string 输入控件的最前方是否显示图片*/
    property string showType: "string"  //string or image

    /** type:string 最前方显示的图片路径*/
    property string labelIconPath: ""

    /** type:string 最前方显示的图片宽度*/
    property int lableIconWidth: 20

    /** type:string 最前方显示的图片高度*/
    property int lableIconHeight: 20

    /** type:string 下拉列表的按钮图片路径*/
    property string dropListImagePath: ""

    /** type:bool 是否显示下拉列表*/
    property bool showDropList: false

    /** type:bool 是否启用密码模式*/
    property bool passwordEnabled: false

    /** type:string 占位文本*/
    property string editPlaceHoldeText: "请输入"

    /** type:int 控件高度*/
    property int iniputItemHeight: UI.LIST_ITEM_HEIGHT

    /** type:alias 调用键盘模式*/
    property alias inputMethodhints: inputEdit.inputMethodHints

    /** type:int 标签字体大小 */
    property int mainTextFontPixelSize: UI.LIST_TITLE_SIZE

    /** type:string 标签字体类型 */
    property string mainTextFontFamily: UI.DEFAULT_FONTFAMILY

    /** type:string 标签字体颜色 */
    property string mainTextFontColor: UI.LIST_TITLE_COLOR

    /** type:int 输入字体大小 */
    property int inputTextFontPixelSize: UI.LIST_SUBTILE_SIZE

    /** type:string 输入字体类型 */
    property string inputTextFontFamily: UI.DEFAULT_FONTFAMILY

    /** type:string 输入字体颜色 */
    property string inputTextFontColor: UI.LIST_SUBTITLE_COLOR

    /** type:int 设置键盘enter键显示文字 */
    property alias inputMethodHintExtensions: inputEdit.inputMethodHintExtensions

    /** type:alias 标签宽度 */
    property alias titleWidth: labelRect.width

    /** type:alias 控件设置焦点 */
    property alias isFocus: inputEdit.focusActive

    /** type:alias 设置键盘的初始页面 */
    property alias inputMethodHints:inputEdit.inputMethodHints


    property alias backcolor:imageback.color

    property bool changepwd:false
    property alias dropListDown: dropListRect.dropdown

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

    /**
     * 下拉按钮点击信号
     */
    signal dropListClick()

    width: parent.width
    height: visible? (titleLabel.height > iniputItem.iniputItemHeight?
                titleLabel.height:iniputItem.iniputItemHeight) : 0
    border.width: showType == "image" ? (changepwd ?1:2):0
    border.color: showType == "image" ? (changepwd ?"#cccccc":"gray"):"white"
    radius: showType == "image" ? 5 : 0

    /**
     * 控件设置焦点
     */
    function forceActiveFocus(){
        console.log("inputEdit.forceActiveFocus()-------------------------------------;  ")
        inputEdit.forceActiveFocus();
    }

    /**
     * 设置下拉按钮状态
     */
    function resetDropListIcon(){
        dropListRect.dropdown = !dropListRect.dropdown
    }

    Item{
        id:labelRect
        width: iniputItem.showType == "image" ? labelIcon.width + 40 :
                                                (iniputItem.showType == "string"? parent.width * 0.4 : 20)
        height: parent.height
        Label {
            id: titleLabel
            anchors.verticalCenter: parent.verticalCenter
            text: mainTitle
            visible:iniputItem.showType == "string"
            horizontalAlignment:Text.AlignLeft
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: UI.LIST_ICON_MARGIN_LEFT
            wrapMode:Text.WrapAnywhere
            font.pixelSize: iniputItem.mainTextFontPixelSize
            font.family:iniputItem.mainTextFontFamily
            color:iniputItem.mainTextFontColor
        }
        Rectangle{
            id :imageback
            width: 70
            height: 60
            visible: iniputItem.showType == "image"
            color: backcolor
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.leftMargin: changepwd ?1:2
            anchors.topMargin: changepwd ?1:2
            anchors.bottomMargin: changepwd ?1:2

            Image{
                id:labelIcon
                visible: iniputItem.showType == "image"
                width: iniputItem.lableIconWidth
                height: iniputItem.lableIconHeight
                source: iniputItem.labelIconPath
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
            }
        }
    } //end of labelRect

    Item{
        id:inputRect
        anchors.left: labelRect.right
        anchors.right: dropListRect.left
        height: parent.height
        //color:"blue"
        CLineEdit {
            id: inputEdit
            placeholderText: iniputItem.editPlaceHoldeText
            passwordLabelEnabled: iniputItem.passwordEnabled
            autoHandleReturnKey: false
            clip: true
            textColor: "#333333"
            anchors.fill: parent
            font.pixelSize: 22
            onTextChanged: {
                console.log("input test is  ", text)
                iniputItem.textChanged(text);
            }
            onFocusChanged: {
                console.log("onFocusChanged ------------------  ", focus)
                textFocusChanged(focus)
            }
        }
    } //end of inputRect
    Item{
        id:dropListRect
        property bool dropdown: false
        anchors.right: parent.right
        height: parent.height
        width: visible? dropListImage.width + 40:0
        visible: iniputItem.showDropList

        Image {
            id: dropListImage
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            width: sourceSize.width
            height: sourceSize.height
            source: iniputItem.dropListImagePath
            rotation: dropListRect.dropdown?180:0
            Behavior on rotation {
                RotationAnimation { duration: 150;}
            }
        }
        MouseArea{
            anchors.fill: parent
            onClicked: {
                dropListRect.dropdown = !dropListRect.dropdown
                console.log("dropListRect.dropdown   ", dropListRect.dropdown)
                iniputItem.dropListClick();
            }
        }
    }//end of dropListRect
}//end Item

