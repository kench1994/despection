/*!
* \file UserCard.qml
* \brief 用户卡片界面
*
*
*
*\author 何昆 hekun@syberos.com
*\version 1.0
*\date 2015/9/9
*/
import QtQuick 2.0
import com.syberos.basewidgets 2.0
import "../Common"
import "../Common/constants.js" as UI

/*!
  \class UserCard
  \inModule 用户卡片模块
  \brief 用于显示登录用户的个人信息
  \ingroup LoginAndHome
  用于显示登录用户的个人信息. 包括：用户名，单位，部门，角色。
*/
CommonPage {
    id: userCardPage
    statusBarHoldEnabled: true
    statusBarHoldItemColor: "white"
    orientationPolicy: CPageOrientation.LockPortrait
    clip: true
    property string nameId: ""
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
            //titleText:os.i18n.ctr(qsTr("User Card"))
            titleText:{
                var name;
                var clientPriority = inspectionSystemConfig.getClientPriority();
                if(clientPriority == false){
                    name = inspectionSystemParam.getUserPermissionData(userCardPage.nameId, "ModuleName");
                } else
                    name = inspectionFuncsItemModel.getNodeParas(userCardPage.nameId, "")["NameCN"];
                return name;
            }
        }
        Rectangle {
            id: rectBackGround;
            width: parent.width
            anchors{
                top: title.bottom
                bottom: parent.bottom
            }
            color: "#ededed";
            Rectangle {
                //背景框
                anchors.fill: parent;
                anchors.bottomMargin: 70;
                anchors.rightMargin: 40;
                anchors.topMargin: 30;
                anchors.leftMargin: 40
                radius: 4;
                //border.width: 1;
                //border.color: "#dddddd";
                Flickable{
                    id:contentFlickable
                    anchors.fill: parent
                    boundsBehavior:Flickable.StopAtBounds
                    clip:true
                    contentHeight: contentColumn.height + 50
                    Column{
                        id:contentColumn
                        width: parent.width
                        anchors.top: parent.top;
                        anchors.topMargin: 50;
                        Rectangle {
                            //图片
                            id: rectImage;
                            width: 128;
                            height: 128;
                            anchors.horizontalCenter: parent.horizontalCenter;
                            color: "transparent"
                            border.color: "transparent";
                            Image {
                                source: "file://" + inspectionSystemConfig.getAppInstallPath() +  "/res/userCard.png"
                                fillMode: Image.PreserveAspectFit
                            }
                        }

                        Item{
                            width: parent.width
                            height: textName.height + texJobname.height + 150
                            Text {
                                //姓名
                                id: textName;
                                //anchors.horizontalCenter: parent.horizontalCenter;
                                anchors.top: parent.top
                                anchors.topMargin: 30;
                                anchors.left: parent.left
                                anchors.leftMargin: 10
                                anchors.right: parent.right
                                anchors.rightMargin: 10
                                horizontalAlignment:Text.AlignHCenter
                                wrapMode: Text.WrapAnywhere
                                text: inspectionSystemParam.getUserPropertyByKey("UserName");
                                font.pixelSize: 24;
                                color: "#333333";
                            }
                            Text {
                                //职务
                                id: texJobname
                                anchors.top: textName.bottom;
                                anchors.topMargin: 20;
                                anchors.left: parent.left
                                anchors.leftMargin: 10
                                anchors.right: parent.right
                                anchors.rightMargin: 10
                                horizontalAlignment:Text.AlignHCenter
                                wrapMode: Text.WrapAnywhere
                                text: inspectionSystemParam.getUserPropertyByKey("TechnicalPost")
                                font.pixelSize: 18;
                                color: "#999999";
                            }

                            Rectangle {
                                //分割线
                                id:rectLine;
                                anchors.top: texJobname.bottom;
                                anchors.topMargin: 50;
                                width: 300;
                                height: 1;
                                anchors.horizontalCenter: parent.horizontalCenter;
                                color: "#dddddd"
                            }
                        }
                        UserTitle {
                            id: infoTitle;
                            name1: os.i18n.ctr(qsTr("Organise Unit:"))
                            value1: inspectionSystemParam.getUserPropertyByKey("OrganiseUnitNames")
                            name2: os.i18n.ctr(qsTr("Department:"))
                            value2: inspectionSystemParam.getUserPropertyByKey("DepartmentNames")
                            name3: os.i18n.ctr(qsTr("Role:"))
                            value3: inspectionSystemParam.getUserPropertyByKey("RoleNames")
                        }

                    }
                }


            }
            Item{
                anchors.right: parent.right
                anchors.rightMargin: 175
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 50
                visible: myInspectSysSetting.getOfflineMode() != "2"
                Row{
                    height:80
                    spacing:15
                    Rectangle{
                        width: 40
                        height: 40
                        color: "transparent"
                    Image{
                        id :settingimage
                        width: 40
                        height: 40
                        source: "qrc:/icon/changepwd.png"
                        fillMode: Image.PreserveAspectFit;
                        anchors.fill: parent;

                    }
                    }
                    Item{
                       width: 100
                       height: 40
                        Text{
                            id :pwdchangetext
                            anchors.verticalCenter: parent.verticalCenter
                            font.pixelSize: 20;
                            text:os.i18n.ctr(qsTr("Modify password"))
                            color: "#999999"

                        }
                      MouseArea{
                          anchors.fill: parent
                          onClicked: {
                              pageStack.push("qrc:/qml/SystemSettings/ChangePwd.qml");
                          }
                      }
                    }
                }
            }
        }
    }
}
