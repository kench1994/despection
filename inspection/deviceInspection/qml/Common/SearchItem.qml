/*!
* \file SearchItem.qml
* \brief 设备查询控件
*查询界面支持三种查找方式：设备名称， 设备编码, 设备型号
*\author 何昆 hekun@syberos.com
*\version 1.0
*\date 2015/9/8
*/
import QtQuick 2.0
import com.syberos.basewidgets 2.0
import "./constants.js" as UI

/*!
  \class SearchItem
  \inModule 公共控件
  \brief 设备查询控件
  \ingroup common
  设备查询控件支持三种查找方式：设备名称， 设备编码, 设备型号
  其中“设备型号”查询，是被收起的.
  目前用于“故障申报”，“运维查询”，“故障申报”界面
*/

Rectangle {
    id: searchItemID;
    property alias nameRectVisible: nameRect.visible
    property alias codeRectVisible: codeRect.visible
    property alias modelRectVisible: modelRect.visible
    property alias exRectVisible: exRect.visible

    /** type:bool  是否其支持更多搜索条件，默认支持*/
    property bool enabledMoreItem: true
    /** type:bool 是否显示更多搜索条件。默认不显示*/
    property bool dropdown: false
     /** type:int 每个搜索条件控件的高度*/
    property int rectHeight: 80
    /** type:int 每个搜索条件控件的左侧偏移度*/
    property int rectLeftMargin: 40
    /** type:int 每个搜索条件控件的右侧偏移度*/
    property int rectRighttMargin: 40
    /** type:int 每个搜索条件控件的输入框左侧偏移度*/
    property int editLeftMargin: 20
    /** type:int 每个搜索条件控件的输入框右侧偏移度*/
    property int editRighttMargin: 40
    /** type:int 每个输入框可输入的最大字符个数*/
    property int editMaxLen: 100
    /** type:int 隐藏的搜索条件控件高度*/
    property int exrectHeight: 40
    /** type:int 隐藏的搜索条件控件顶部偏移度*/
    property int exrectTopMargin: 50
    /** type:int “查找”按钮的顶部偏移度*/
    property int buttonTopMargin: 40

    /**
     *发送用户输入的设备搜索条件
     * @param name:string 设备名称
     * @param code:string 设备编码
     * @param model:string 设备型号,如果处于隐藏状态，赋值为"".
     */
    signal clickSearchButton(string name, string code, string model)

    width: parent.width
    height: nameRect.height+codeRect.height+exRect.height+modelRect.height+searchButton.height+buttonTopMargin
    color: "white"

    Rectangle{
        id: nameRect
        height: visible? rectHeight:0
        color: "white"
        anchors{
            left: parent.left
            leftMargin: rectLeftMargin
            right: parent.right
            rightMargin: rectRighttMargin
        }
        Image {
            id: nameImage
            anchors{
                left: parent.left
                verticalCenter: parent.verticalCenter
            }
            source: "qrc:/icon/device.png"
        }

        CLineEdit{
            id: nameEdit
            anchors{
                left: nameImage.right
                leftMargin: editLeftMargin
                right: parent.right
                verticalCenter: parent.verticalCenter
            }
            echoMode:TextInput.Normal
            textColor:"black"
            placeholderText:os.i18n.ctr(qsTr("Please input device name"))
            font.pixelSize: UI.FONT_SIZE_22
            font.family: UI.DEFAULT_FONT_FAMILY
            validator: RegExpValidator{regExp: /^[^ ]+$/}
            overMaximumLengthRequestToast: os.i18n.ctr(qsTr("max overflow"))
            Component.onCompleted: {
                maximumLength = editMaxLen
            }
        }
        CLine {
            anchors.bottom: parent.bottom
        }
    }
    Rectangle{
        id: codeRect
        height: visible? rectHeight:0
        color: "white"

        anchors{
            left: parent.left
            leftMargin: rectLeftMargin
            right: parent.right
            rightMargin: rectRighttMargin
            top: nameRect.bottom
            topMargin: nameRect.visible?0:40
        }

        Image {
            id: codeImage
            anchors{
                left: parent.left
                verticalCenter: parent.verticalCenter
            }
            source: "qrc:/icon/key.png"
        }

        CLineEdit{
            id: codeEdit
            anchors{
                left: codeImage.right
                leftMargin: editLeftMargin
                right: parent.right
                verticalCenter: parent.verticalCenter
            }
            inputMethodHints:Qt.ImhPreferLatin
            echoMode:TextInput.Normal
            //validator: RegExpValidator{regExp: /^[_A-Za-z0-9]+$/}
            //validator: RegExpValidator{regExp: /^[_A-Za-z0-9_\-\#\$&*\[\]()]+$/}
            textColor:"black"
            placeholderText:os.i18n.ctr(qsTr("Please input device code"))
            font.pixelSize: UI.FONT_SIZE_22
            font.family: UI.DEFAULT_FONT_FAMILY
            overMaximumLengthRequestToast: os.i18n.ctr(qsTr("max overflow"))
            Component.onCompleted: {
                maximumLength = editMaxLen
            }
        }
        CLine {
            anchors.bottom: parent.bottom
        }
    }
    Rectangle{
        id: modelRect
        clip: true
        height: enabledMoreItem && dropdown?(visible? rectHeight:0):0
        color: "white"

        Behavior on height{
            NumberAnimation {duration: 200}
        }

        anchors{
            left: parent.left
            leftMargin: rectLeftMargin
            right: parent.right
            rightMargin: rectRighttMargin
            top: dropdown?codeRect.bottom:codeRect.bottom
        }

        Image {
            clip: true
            id: modelImage
            anchors{
                left: parent.left
                verticalCenter: parent.verticalCenter
            }
            source: "qrc:/icon/model.png"
        }

        CLineEdit{
            id: modelEdit
            clip: true
            anchors{
                left: modelImage.right
                leftMargin: editLeftMargin
                right: parent.right
                verticalCenter: parent.verticalCenter
            }
            echoMode:TextInput.Normal
            textColor:"black"
            placeholderText:os.i18n.ctr(qsTr("Please device model"))
            font.pixelSize: UI.FONT_SIZE_22
            font.family: UI.DEFAULT_FONT_FAMILY
            validator: RegExpValidator{regExp: /^[^ ]+$/}
            overMaximumLengthRequestToast: os.i18n.ctr(qsTr("max overflow"))
            Component.onCompleted: {
                maximumLength = editMaxLen
            }
        }
        CLine {
            clip: true
            anchors.bottom: parent.bottom
        }
    }
    Rectangle {
        id: exRect
        clip: true
        width: parent.width
        height: enabledMoreItem?(visible? exrectHeight:0):0
        color: "gray"
        anchors{
            top: modelRect.bottom
            topMargin: exrectTopMargin
        }
        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            height: parent.height
            spacing: 10
            Text {
                id: desc
                text: dropdown?os.i18n.ctr(qsTr("Put more search criteria")):os.i18n.ctr(qsTr("More search criteria"))
                font.pixelSize: UI.FONT_SIZE_16
                font.family: UI.DEFAULT_FONT_FAMILY
                color: "#333333"
                anchors.verticalCenter: parent.verticalCenter
            }
            Image {
                source: "qrc:/icon/icon_08.png"
                anchors.verticalCenter: parent.verticalCenter
                rotation:dropdown?180:0
                Behavior on rotation {
                    RotationAnimation { duration: 200;}
                }
            }
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                dropdown = !dropdown;
            }
        }
    }
    CommonButton {
        id: searchButton
        text: os.i18n.ctr(qsTr("Serach"))
        anchors {
            top: exRect.bottom
            topMargin: buttonTopMargin
        }
        //enabled: nameEdit.text != "" || codeEdit.text != "" || (modelEdit.text != "" && dropdown)
        enabled: nameEdit.text != "" || codeEdit.text != "" || modelEdit.text != ""
        onClicked: {
            clickSearchButton(nameEdit.text, codeEdit.text, modelEdit.text)
//            if(dropdown) {
//                clickSearchButton(nameEdit.text, codeEdit.text, modelEdit.text)
//            } else {
//                clickSearchButton(nameEdit.text, codeEdit.text, "")
//            }
        }
    }
}
