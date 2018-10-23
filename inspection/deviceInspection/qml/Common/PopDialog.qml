import QtQuick 2.0
import com.syberos.basewidgets 2.0
FocusScope {
    id: root
    parent: gAppUtils.pageStackWindow.contentRoot
    z: 1000
    anchors.fill: parent
    visible: false
    property string bgColor: "#000000"
    property double bgOpacity:  0.50
    Rectangle{
        id:background
        anchors.fill: parent
        color: root.bgColor
        opacity: bgOpacity
        z:-1
    } /*! end of Rectangle */
    /*! 用户需要显示弹层时，调用该信号。 */
    function show() {
        root.visible = true;
    }

    /*! 用户需要隐藏弹层时，调用该信号。 */
    function hide() {
        root.visible = false;
    }

    signal canceled()
    /*! 当点击空白区域时候退出。*/
    MouseArea {
        id: popMenuLayerStyleClick
        anchors.fill: parent
        onClicked: {
            root.canceled()
            hide();
        }
    }

    /*! 点击返回键触发hide()请求。 */
    Keys.onReleased:{
        if (event.key == Qt.Key_Back || event.key == Qt.Key_Escape) {
            event.accepted = true
            canceled()
            hide();
            console.log("aaaaaaaaa    onReleased")
        }/*! end of if ( event) */
    }
    onVisibleChanged: {
        if(visible){
            gAppUtils.pushDialog(root)
        }else{
            gAppUtils.popDialog(root)
        }
        focus = visible;
    }

    Component.onDestruction: {
        root.visible = false
        gAppUtils.popDialog(root)
    }
}


