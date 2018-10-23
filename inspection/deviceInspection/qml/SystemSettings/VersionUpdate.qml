/*!
* \file VersionUpdate.qml
* \brief 升级包下载提升框控件
*
*
*
*\author 何昆 hekun@syberos.com
*\version 1.0
*\date 2015/9/9
*/

import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.1
import com.syberos.basewidgets 2.0
/*!
  \class VersionUpdate
  \inModule 版本升级模块
  \brief 升级包下载提升框控件
  \ingroup SystemSettings
  升级包下载提升框控件
*/

CDialog {
    id:downloadNotification
    anchors.bottom: parent.bottom;
    width: parent.width;
    height: 100;
    visible : false
    titleText: os.i18n.ctr(qsTr("Version update"));   //版本更新
    rejectButtonText: os.i18n.ctr(qsTr("Cancel"));   //取消
    acceptedButtonText: os.i18n.ctr(qsTr("Download"));          //下载
    canceledOnBackKeyReleased:true
    canceledOnOutareaClicked: true
    onAccepted: {
        rootWindow.closeKeyboard()
        //进入升级包下载安装界面。
        pageStack.push("qrc:/qml/SystemSettings/UpgradeProcess.qml");
    }
}







