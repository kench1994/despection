/*!
* \file DeviceInfo.qml
* \brief 系统设置-->设备信息页面
*
*设备信息页面: 用于显示终端设备的设备编号及所属单位等信息
*
*\author 陈瑞华 chenruihua@syberos.com
*\version 1.0
*\date 2015/9/8
*/
import QtQuick 2.0
import com.syberos.basewidgets 2.0
import com.syberos.generateForm 1.0
import "../Common"
import "../Common/constants.js" as UI
/*!
  \class DeviceInfo
  \inModule 系统设置
  \brief 系统设置，设备信息页面
  \ingroup SystemSettings
  系统设置，设备信息页面: 显示设备编号及设备归属单位
*/
CommonPage {
        id: deivceInfoPage
        //        objectName: "deviceInfoPage"
        statusBarHoldEnabled: true
        statusBarHoldItemColor: "white"
        orientationPolicy: CPageOrientation.LockPortrait
        clip: true
        property int clickCount: 0

        contentAreaItem:  Rectangle {
            id: contentRect

            CommonTitlebar {
                id: title
                titleText: os.i18n.ctr( qsTr("DeviceInfo") )//设备信息
                onLeftItemTriggered: pageStack.pop()
            }

            Rectangle {
                id: titleSeparateLine
                width: parent.width; height: 1
                color: "#000000"
                opacity: 0.1
                z: parent.z + 1
                anchors.top: title.bottom
            }

            Rectangle {
                id: backGroundRect
                color: "#eeeff1"
                anchors.top: title.bottom
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right

                Flickable {
                    id: detailInfo
                    anchors.fill: parent
                    flickDeceleration: Flickable.VerticalFlick
                    clip: true
                    contentHeight: totalCol.height

                    Item {
                        id: topToHeaderSeparatorItem
                        width: parent.width; height: 10
                    }

                    Column {
                        id: totalCol
                        anchors.top: topToHeaderSeparatorItem.bottom
                        width: parent.width
                        spacing: 1

                        ListItem {
                            id: deviceIDItem
                            mainTitle: os.i18n.ctr( qsTr("DeviceID") )
                            endTitle: inspectionSystemParam.getPadId()
                            type:""
                            onClicked: {
                                deivceInfoPage.clickCount++;
                                if(deivceInfoPage.clickCount == 5){
                                    rootWindow.showDeveloperModel = !rootWindow.showDeveloperModel;
                                    deivceInfoPage.clickCount = 0;
                                }
                            }
                        }
                        ListItem {
                            id: enterpriseItem
                            visible: false
                            type:""
                            mainTitle: os.i18n.ctr( qsTr("Enterprises Institutions") )// "所属单位"
                            endTitle: os.i18n.ctr( qsTr("XX Dep XX Co") )//"某公司某部门"
                        }
                        ListItem {
                            id: developerModel
                            visible: rootWindow.showDeveloperModel
                            type:""
                            mainTitle: os.i18n.ctr( qsTr("System version"))
                            subTitle: inspectionSystemConfig.getOsVersion()
                        }
                        ListItem {
                            id: removeCache
                            type:""
                            visible: rootWindow.showDeveloperModel
                            mainTitle: os.i18n.ctr( qsTr("Clear cache"))
                            onClicked: {
                                removeCacheDlg.show();
                            }
                        }
                    }
                } //end of  Flickable{ id:detailInfo
            } //end of  Rectangle{ id: backGroundRect
        } // end of    contentAreaItem:  Rectangle { id: contentRect
        CDialog{
            id: removeCacheDlg
            titleText: os.i18n.ctr( qsTr("Clear cache"))
            messageText : os.i18n.ctr( qsTr("Clear Cache and app will reboot")) //"清除缓存后，程序将会自动重启，是否清除？"
            acceptedButtonText: os.i18n.ctr( qsTr("Ok"))
            rejectButtonText: os.i18n.ctr( qsTr("Cancel"))
            onAccepted:{
                var values = {};
                transmitManager.setStatus("clearDB", values);
                CTOAST.requestToast(os.i18n.ctr(qsTr( "Clear Success" )), "", "" );//清除成功
                Qt.quit();
            }
        }
    }
