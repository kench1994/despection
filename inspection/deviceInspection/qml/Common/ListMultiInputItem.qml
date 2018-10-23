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

    /** type:alias 输入控件的输入最大字数*/
    property alias maximumLength: inputEdit.maximumLength

    /** type:string 最前方显示的图片宽度*/
    property int lableIconWidth: 20

    /** type:string 最前方显示的图片高度*/
    property int lableIconHeight: 20

    /** type:bool 是否启用密码模式*/
    property bool passwordEnabled: false

    /** type:string 占位文本*/
    property string editPlaceHoldeText: "请输入"

    /** type:int 控件高度*/
    property int iniputItemHeight: UI.LIST_ITEM_HEIGHT

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

    /** type:alias 标签宽度 */
    property alias titleWidth: labelRect.width

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
//iniputItemHeight
    width: parent.width
    //height: visible? (titleLabel.height > 200?titleLabel.height:200) : 0
    height: visible? (textRect.height > iniputItem.iniputItemHeight ?
                          (titleLabel.height > textRect.height ? titleLabel.height: textRect.height):
                          iniputItem.iniputItemHeight) : 0

    Item{
        id:labelRect
        width: parent.width * 0.4
        height: parent.height
        Label {
            id: titleLabel
            anchors.verticalCenter: parent.verticalCenter
            text: mainTitle
            horizontalAlignment:Text.AlignLeft
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: UI.LIST_ICON_MARGIN_LEFT
            wrapMode:Text.WrapAnywhere
            font.pixelSize: iniputItem.mainTextFontPixelSize
            font.family:iniputItem.mainTextFontFamily
            color:iniputItem.mainTextFontColor
        }
    } //end of labelRect

    Rectangle{
        id:textRect
        width: parent.width
        anchors.left: labelRect.right
        anchors.right: parent.right
        height: inputEdit.height + 20
        CTextArea{
            id:inputEdit
            height: contentHeight > iniputItem.iniputItemHeight - 20 ? (contentHeight > 180 ? 180 : contentHeight): iniputItem.iniputItemHeight - 20
            font.pixelSize:20
            anchors.top:parent.top
            anchors.topMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter
            width:parent.width - 40
            maximumLength:300
            onContentHeightChanged:{
                console.log("contentHeight  ", contentHeight)
            }

            Text {
                id:placeHoldText
                visible: parent.text == ""
                color: "#dddddd"
                font.pixelSize:parent.font.pixelSize
                text: editPlaceHoldeText
                anchors.verticalCenter : parent.verticalCenter
            }
            onTextChanged: {
                console.log("input test is  ", text)
                iniputItem.textChanged(text);
            }
        }
    }
}//end Item


