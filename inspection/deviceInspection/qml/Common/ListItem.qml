/*!
* \file ListItem.qml
* \brief 公共控件，列表控件
*
*公共控件，列表控件
*
*\author 桓乐乐 huanlele@syberos.com
*\version 1.0
*\date 2015/9/7
*/
import QtQuick 2.1
import com.syberos.basewidgets 2.0
import "./constants.js" as UI
/*!
  \class ListItem
  \inModule 公共控件
  \brief 公共控件，列表控件
  \ingroup common
  公共控件，列表控件
*/
Rectangle{
    id: listItem
    /** type:string 控件的itemId */
    property string itemId: ""

    /** type:string 控件的identity */
    property string identity: ""

    /** type:string  主标题*/
    property string mainTitle: ""

    /** type:string  副标题*/
    property string subTitle: ""

    /** type:string  副标题占位文本*/
    property string hintTitle: ""

    /** type:string  控件右标题*/
    property string endTitle: ""

    /** type:string  控件右图片*/
    property string endImage: "qrc:/icon/xiangxia_03.png"

    /** type:string  控件右图片按下状态图片*/
    property string endImagePressed : "qrc:/icon/xiangxia_03.png"

    /** type:string  控件左图片*/
    property string headImage: ""

    /** type:bool  控件右图片是否接受鼠标事件*/
    property bool endMouseVisible: false

    /** type:bool  是否是鼠标点击改变选择按钮状态*/
    property bool clickToChangeSwitch: true

    /** type:alias  选择按钮状态*/
    property alias switchChecked: autoSwitch.switchOn

    /** type:string 控件类型 */
    property string type: "category"

    /** type:int 控件高度*/
    property int listItemHeight: UI.LIST_ITEM_HEIGHT

    /** type:int 主标题字体大小*/
    property int mainTextFontPixelSize: UI.LIST_TITLE_SIZE

    /** type:string 主标题字体类型*/
    property string mainTextFontFamily: UI.DEFAULT_FONTFAMILY

    /** type:int 主标题颜色*/
    property string mainTextFontColor: UI.LIST_TITLE_COLOR

    /** type:string 主标题按下颜色*/
    property string mainTextPressedColor: UI.LIST_PRESSED_COLOR

    /** type:int 副标题字体大小*/
    property int subTextFontPixelSize: UI.LIST_SUBTILE_SIZE

    /** type:string 副标题字体类型*/
    property string subTextFontFamily: UI.DEFAULT_FONTFAMILY

    /** type:string 副标题颜色*/
    property string subTextFontColor: UI.LIST_SUBTITLE_COLOR

    /** type:string 副标题按下颜色*/
    property string subTextPressedColor: UI.LIST_PRESSED_COLOR

    /** type:int 右标题字体大小*/
    property int endTextPixelSize: UI.LIST_SUBTILE_SIZE

    /** type:string 右标题字体类型*/
    property string endTextFontFamily: UI.DEFAULT_FONTFAMILY

    /** type:string 右标题字体颜色*/
    property string endTextFontColor: UI.LIST_SUBTITLE_COLOR

    /** type:string 右标题字体按下颜色*/
    property string endTextPressedColor: UI.LIST_PRESSED_COLOR

    /** type:int 右图标高度*/
    property int endIconHeight: UI.LIST_RIGHTICON_HEIGHT

    /** type:int 右图标宽度*/
    property int endIconWidth: UI.LIST_RIGHTICON_WIDTH

    /** type:int 上分割线高度*/
    property int topSeparatorHeight: UI.LIST_SEPARETOR_LINE_HEIGHT

    /** type:color 上分割线颜色*/
    property color topSeparatorColor: UI.LIST_SEPARETOR_LINE_COLOR

    /** type:int 下分割线高度*/
    property int bottomSeparatorHeight: UI.LIST_SEPARETOR_LINE_HEIGHT

    /** type:color 下分割线颜色*/
    property color bottomSeparatorColor: UI.LIST_SEPARETOR_LINE_COLOR

    /** type:int 上间隔高度*/
    property int topIntervalHeight: UI.LIST_TOP_INTERVAL_HEIGHT

    /** type:int 下间隔高度*/
    property int  bottomIntervalHeight: UI.LIST_BTOTTOM_INTERVAL_HEIGHT

    /** type:alias 下分割线控件*/
    property alias btmSeparatorItem: bottomSeparetor

    /** type:alias 下分割线控件*/
    property alias topSeparatorItem: topSeparator

    /** type:bool 是否显示背景颜色*/
    property bool showbkground: true
    property var mainTextOpacity:1

    /**
     * 控件点击事件
     * @param 控件的itemId
     */
    signal clicked(string itemId)

    /**
     * 控件选择按钮状态变化信号
     * @param 控件的itemId
     * @param 控件的选择按钮状态
     */
    signal checkedChanged(string itemId, bool checked)

    /**
     * 控件右边图标点击信号
     * @param 控件的itemId
     */
    signal endImgClicked(string itemId)
    width: parent.width
    height: mainText.text != ""?
                ((endText.height > titleCol.height? endText.height: titleCol.height + topSeparator.height +topIntervalHeight +
                  bottomSeparetor.height +bottomIntervalHeight) > listItem.listItemHeight ?
                     (endText.height > titleCol.height? endText.height: titleCol.height + topSeparator.height +topIntervalHeight +
                      bottomSeparetor.height +bottomIntervalHeight) : listItem.listItemHeight) : 0
    visible: mainText.text != "" ? true : false
    Rectangle{
        id: background
        anchors.fill: parent
        visible: mouseArea.pressed && showbkground
        color: UI.LIST_ITEM_BGCOLOR
    }
    Rectangle {
        id: topSeparator
        width: parent.width
        height: topSeparatorHeight
        anchors.top: parent.top
        anchors.topMargin: -2
        color: topSeparatorColor
        visible: topSeparatorHeight > 0
    }
    Item{
        anchors.top: topSeparator.bottom
        anchors.topMargin: topIntervalHeight
        anchors.bottom: bottomSeparetor.top
        anchors.bottomMargin: bottomIntervalHeight
        width: parent.width
        Image{
            id: headImg
            anchors.left: parent.left
            anchors.leftMargin: UI.LIST_ICON_MARGIN_LEFT
            width: UI.LIST_ICON_WIDTH;height: UI.LIST_ICON_HEIGHT
            visible: source != ""
            source: headImage
            anchors.verticalCenter: parent.verticalCenter
        }
        MouseArea {
            id: mouseArea
            anchors.fill: parent
            onClicked: {
                listItem.clicked(listItem.itemId);
                if(type == "switch" && listItem.clickToChangeSwitch === true) {
                    console.log(",,,,,,,,,,,,,,,,,,,,,,,,,,, click item---")
                    autoSwitch.switchOn = !autoSwitch.switchOn;
                    //autoSwitch.switchOnChangedByClicked();
                    autoSwitch.switchOnChangedByUser(autoSwitch.switchOn);
                }
            }
        }
        Item{
            id:containItem
            height: titleCol.height
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: headImg.visible?headImg.right:parent.left
            anchors.leftMargin: headImg.visible?
                                    UI.LIST_ITEM_ROW_SPACEING:UI.LIST_TITLE_MARGIN_LEFT
            anchors.right: parent.right
            //anchors.rightMargin: UI.LIST_ICON_MARGIN_RIGHT
            Column {
                id:titleCol
                anchors.left: parent.left
                anchors.right: endItem.left
                anchors.rightMargin: endItem.width === 0 ? 0 : UI.LIST_ITEM_ROW_SPACEING
                spacing: UI.LIST_MAIN_SUB_SEPARATOR
                Text {
                    id: mainText; text: mainTitle
                    opacity: mainTextOpacity 
                    width: parent.width
                    horizontalAlignment: Text.AlignLeft
                    font.pixelSize: listItem.mainTextFontPixelSize
                    font.family: listItem.mainTextFontFamily
                    //color: enabled ? (mouseArea.pressed? listItem.mainTextPressedColor: listItem.mainTextFontColor) : UI.LIST_TITLE_DISABLE_COLOR //20131231 by caizhenbin
                    color: enabled ? listItem.mainTextFontColor : UI.LIST_TITLE_DISABLE_COLOR
                    onTextChanged: {
                        changeWidth();
                    }
                }

                Text {
                    id: subText
                    text: subTitle
                    width: parent.width
                    //lineHeight: 1.25
                    horizontalAlignment: Text.AlignLeft
                    font.pixelSize: listItem.subTextFontPixelSize
                    font.family: listItem.subTextFontFamily
                    //color: enabled ? (mouseArea.pressed? listItem.subTextPressedColor: listItem.subTextFontColor) : UI.LIST_TITLE_DISABLE_COLOR //20131231 by caizhenbin
                    color: enabled ? listItem.subTextFontColor : UI.LIST_TITLE_DISABLE_COLOR
                    visible: text != ""
                    onTextChanged: {
                        changeWidth();
                    }
                }
                Text {
                    id: hintText
                    text: hintTitle
                    width: parent.width
                    //lineHeight: 1.25
                    horizontalAlignment: Text.AlignLeft
                    font.pixelSize: listItem.subTextFontPixelSize
                    font.family: listItem.subTextFontFamily
                    //color: enabled ? (mouseArea.pressed? listItem.subTextPressedColor: listItem.subTextFontColor) : UI.LIST_TITLE_DISABLE_COLOR //20131231 by caizhenbin
                    color: enabled ? listItem.subTextFontColor : UI.LIST_TITLE_DISABLE_COLOR
                    visible: text != "" && !subText.visible
                    onTextChanged: {
                        changeWidth();
                    }
                }
            }
            Item{
                id:endItem
                //color:"red"
                height: parent.height
                anchors.right: parent.right
                width: endTitleIconItem.visible || autoSwitch.visible ?
                           (endTitleIconItem.visible && autoSwitch.visible?
                                Math.max(endTitleIconItem.width, autoSwitch.width) + UI.LIST_ICON_MARGIN_RIGHT*2:
                                endTitleIconItem.visible ?endTitleIconItem.width + UI.LIST_ICON_MARGIN_RIGHT*2:
                                        autoSwitch.visible?autoSwitch.width + UI.LIST_ICON_MARGIN_RIGHT*2:0):0
                Item{
                    id: endTitleIconItem
                    anchors.right: parent.right
                    anchors.rightMargin: UI.LIST_ICON_MARGIN_RIGHT
                    height: parent.height
                    width: endText.visible || endIcon.visible ?
                               (endText.visible && endIcon.visible?
                                    endText.width + endIcon.width + UI.LIST_ITEM_ROW_SPACEING/2:
                                          endText.visible?endText.width:endIcon.visible?endIcon.width:0): 0
                    Text {
                        id: endText
                        text: endTitle
                        width: endText.contentWidth
                        anchors.right: endIcon.visible? endIcon.left:parent.right
                        anchors.rightMargin: endIcon && endIcon.visible?UI.LIST_ITEM_ROW_SPACEING/2:0
                        anchors.verticalCenter: parent.verticalCenter
                        horizontalAlignment: Text.AlignRight
                        font.pixelSize: listItem.endTextPixelSize
                        font.family: listItem.endTextFontFamily
                        color: mouseArea.pressed ? listItem.endTextPressedColor : listItem.endTextFontColor
                        visible: text != ""
                        onTextChanged: {
                            //endText.elide = Text.ElideNone;
                            changeWidth();
                        }
                    }
                    Image {
                        id:endIcon
                        visible: type != "category" ? false : true
                        source: endImgmouseArea.pressed ? endImagePressed : endImage
                        height: endIconHeight
                        width: endIconWidth
                        fillMode: Image.PreserveAspectFit
                        anchors.right: parent.right
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
                CSwitch {
                    id: autoSwitch
                    visible: type != "switch" ? false : true
                    anchors.right: parent.right
                    anchors.rightMargin: UI.LIST_ICON_MARGIN_RIGHT
                    anchors.verticalCenter: parent.verticalCenter
                    onSwitchOnChangedByUser:{
                        console.log("onSwitchOnChangedByUser  switchOn   ", switchOn)
                        listItem.checkedChanged(listItem.itemId, switchOn)
                    }
                }
                MouseArea {
                    id: endImgmouseArea
                    anchors.fill: parent
                    visible: endMouseVisible
                    onClicked: {
                        listItem.endImgClicked(listItem.itemId);
                    }
                }

            }
            onWidthChanged: {
                if(endItem){
                    changeWidth();
                }
            }
        }
    }//end Item
    Rectangle {
        id: bottomSeparetor
        height: bottomSeparatorHeight; width: parent.width
        anchors.bottom: parent.bottom
        color: UI.LIST_SEPARETOR_LINE_COLOR
        visible: bottomSeparatorHeight > 0
    }
    onVisibleChanged: {
        if(endItem && endItem.width){
            changeWidth();
        }
    }

    /**
     * 文本标题宽度变化, 重新调整控件布局
     */
    function changeWidth(){
        if(containItem.width <= 0)
            return;
        mainText.wrapMode  = Text.NoWrap;
        subText.wrapMode = Text.NoWrap;
        endText.wrapMode = Text.NoWrap;
        //endText.elide = Text.ElideNone;
        var titleMaxLen = Math.max(mainText.contentWidth, subText.contentWidth);
        var itemWidth = Math.round(containItem.width) - UI.LIST_ITEM_ROW_SPACEING;
        if(type == "switch") {
            mainText.wrapMode = Text.WordWrap;
            subText.wrapMode = Text.WordWrap;
            return;
        }
        var endWidth = (endIcon.visible?
                    endIcon.width + UI.LIST_ITEM_ROW_SPACEING / 2 + endText.contentWidth:
                    endText.contentWidth) + UI.LIST_ICON_MARGIN_RIGHT * 2;
        var endIconWidth = endIcon.visible ? endIcon.width + UI.LIST_ITEM_ROW_SPACEING / 2 : 0;
        if(titleMaxLen + endWidth < itemWidth) {
            endText.width = endText.contentWidth;
        }
        if(titleMaxLen + endWidth >= itemWidth){
            if(titleMaxLen <= itemWidth / 2 && endWidth >= itemWidth / 2) {
                endText.width = itemWidth - titleMaxLen - endIconWidth - UI.LIST_ICON_MARGIN_RIGHT * 2;
                console.log("titleMaxLen <= itemWidth/2 && endItem.width >= itemWidth/2")
            } else if(titleMaxLen >= itemWidth / 2 && endWidth <= itemWidth / 2) {
                endText.width = endText.contentWidth;
                console.log("titleMaxLen >= itemWidth/2 && endItem.width <= itemWidth/2")
            } else if(titleMaxLen >= itemWidth / 2 && endWidth >= itemWidth / 2) {
                endText.width = Math.round(itemWidth / 2) - endIconWidth - UI.LIST_ICON_MARGIN_RIGHT * 2;
                console.log("titleMaxLen >= itemWidth/2 && endItem.width >= itemWidth/2")
            }
        }
        mainText.wrapMode  = Text.WrapAnywhere;
        subText.wrapMode = Text.WrapAnywhere;
        //endText.elide = Text.ElideRight;
        endText.wrapMode = Text.WrapAnywhere;
    }
}
