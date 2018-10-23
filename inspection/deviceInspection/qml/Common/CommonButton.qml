/*!
* \file CommonButton.qml
* \brief 公共控件，通用按钮
*
*公共控件，通用按钮
*
*\author 桓乐乐 huanlele@syberos.com
*\version 1.0
*\date 2015/9/7
*/
import QtQuick 2.0
import com.syberos.basewidgets 2.0
/*!
  \class CommonButton
  \inModule 公共控件
  \brief 公共控件，通用按钮
  \ingroup common
  公共控件，通用按钮
*/
CColorButton {
    id: searchButton
    width: 440; height: 60
    Component.onCompleted: {
        backgroundItem.radius = 10;
        anchors.horizontalCenter = parent.horizontalCenter;
    }
}
