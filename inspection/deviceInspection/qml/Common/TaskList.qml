import QtQuick 2.0
import com.syberos.basewidgets 2.0
CommonPage {
    id:inspectionTaskList
    /** type:string  检查整改使用： 0，获取检查整改列表；1，获取检查整改确认列表*/
    property string falg: "0"

    /** type:string  页面标题 */
    property alias  titleText: title.titleText

    /** type:string   当前从服务器取数据的动作标志*/
    property string action

    /** type:alias listview的数据源（listmode）  */
    property alias  model :taskList.model

    /** type:string   listview的代理qml路径  */
    property string delegateFile

    /** type:string   从服务器获取到的业务的ID*/
    property string keyID: "0"

    /** type:string   是否允许手动操作*/
    property string limitMode: "0"


    /** type:var  代理生成的component */
    property var    delegateCom

    /** type:alias   页面为空时，提示字*/
    property alias  emptyText: empty.text

    /** type:alias   页面为空时，提示图片*/
    property alias  emptyIcon: empty.iconSource
    property alias  emptyVisible :empty.visible
    contentAreaItem:  Item {
        id: content
        CommonTitlebar{
            id: title
            width: parent.width
            leftItemEnabled: true
            titleItemEnabled: true
            rightItemEnabled: false
            leftItemText:""
            onLeftItemTriggered: pageStack.pop()
            anchors.top: parent.top
            onLeftItemChanged:leftItem.backgroundEnabled = false
            leftItemBackgroundEnabled: false
        }

        ListView {
            id: taskList
            width: parent.width
            clip:true
            anchors{
                top: title.bottom
                bottom: parent.bottom
            }
            delegate: {
                if(!inspectionTaskList.delegateCom){
                    inspectionTaskList.delegateCom = Qt.createComponent(delegateFile);
                }
                return inspectionTaskList.delegateCom;
            }
        }
        CIconLabel {
            id: empty
            visible: taskList.count === 0 ? true : false
            width: parent.width
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.left: parent.left
            z: taskList.z + 1
            iconSource: "qrc:/icon/empty.png"
            text: os.i18n.ctr(qsTr("No result"))
            spacing: 40
        }
    }
}
