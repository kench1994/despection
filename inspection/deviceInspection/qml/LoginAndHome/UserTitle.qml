/*!
* \file UserTitle.qml
* \brief 用户信息显示控件
*
*
*
*\author 何昆 hekun@syberos.com
*\version 1.0
*\date 2015/9/9
*/
import QtQuick 2.0
/*!
  \class UserTitle
  \inModule 用户卡片模块
  \brief 用户信息显示控件
  \ingroup LoginAndHome
  用户信息显示控件, 包括：单位，部门，角色
*/
Column {
    id: userInfoCol;

    /** type:string  单位 */
    property string name1: ""
    /** type:string  单位名称 */
    property string value1: ""

    /** type:string  部门 */
    property string name2: ""
    /** type:string  部门名称 */
    property string value2: ""

    /** type:string  角色 */
    property string name3: ""
    /** type:string  角色值*/
    property string value3: ""
    width: parent.width;
    spacing: 50
    Rectangle {
        height: infoValue1.height;
        width: parent.width;
        Text {
            id: infoName1;
            anchors.left: parent.left;
            anchors.leftMargin: 50;
            color: "#999999"
            visible: text != ""
            text: userInfoCol.name1;
            font.pixelSize: 18;
        }
        Text {
            id: infoValue1;
            anchors.left: infoName1.right;
            anchors.leftMargin: 20;
            anchors.right: parent.right;
            anchors.rightMargin: 20
            font.pixelSize: 18;
            visible: text != ""
            text: userInfoCol.value1;
            wrapMode: Text.WordWrap;
            color: "#333333"
        }

    }

    //        Rectangle {
    //            width: parent.width;
    //            height: 50;
    //        }
    Rectangle {
        height: infoValue2.height;
        width: parent.width;
        Text {
            id: infoName2;
            anchors.left: parent.left;
            anchors.leftMargin: 50;
            color: "#999999"
            visible: text != ""
            text: userInfoCol.name2;
            font.pixelSize: 18;
        }
        Text {
            id: infoValue2;
            anchors.left: infoName2.right;
            anchors.leftMargin: 20;
            anchors.right: parent.right;
            anchors.rightMargin: 20
            font.pixelSize: 18;
            visible: text != ""
            text: userInfoCol.value2;
            wrapMode: Text.WordWrap;
            color: "#333333"
        }

    }

    //        Rectangle {
    //            width: parent.width;
    //            height: 50;
    //        }
    Rectangle {
        height: infoValue3.height;
        width: parent.width;
        Text {
            id: infoName3;
            anchors.left: parent.left;
            anchors.leftMargin: 50;
            color: "#999999"
            visible: text != ""
            text: userInfoCol.name3;
            font.pixelSize: 18;
        }
        Text {
            id: infoValue3;
            anchors.left: infoName3.right;
            anchors.leftMargin: 20;
            anchors.right: parent.right;
            anchors.rightMargin: 20
            font.pixelSize: 18;
            visible: text != ""
            text: userInfoCol.value3;
            wrapMode: Text.WordWrap;
            color: "#333333"
        }
    }
}
