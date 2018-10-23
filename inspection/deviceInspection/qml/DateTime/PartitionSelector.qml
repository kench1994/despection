/*
 * This file is part of syberos-toolkit-core
 * Copyright (C) 2015 Beijing Yuan Xin Technology Co.,Ltd. All rights reserved.
 * Authors:
 *       Dongshuang Zhao <zhaodongshuang@syberos.com>
 *
 * This software, including documentation, is protected by copyright controlled
 * by Beijing Yuan Xin Technology Co.,Ltd. All rights are reserved.
 */
import QtQuick 2.3

Item{
    id: rootItem
    width: 193
    height: 240

    property alias divLineVisible: div.visible
    property alias selectorWidth: listView.width
    property alias selectorHeight: listView.height
    property alias currentIndex: listView.currentIndex

    ListView{
        id: listView
        width: parent.width
        height: 240
        anchors.verticalCenter: parent.verticalCenter

        model: ListModel {
            ListElement { type: 0 }
            ListElement { type: 1 }
        }
        delegate: Text {
            id: delegateItem
            width: parent.width
            height: parent.height/2
            text: (type === 0) ? os.i18n.ctr(qsTr("AM","上午")) : os.i18n.ctr(qsTr("PM","下午"))
            font.pixelSize: delegateItem.ListView.isCurrentItem ? 50 : 36
            color: delegateItem.ListView.isCurrentItem ? "#333333" : "#999999"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            MouseArea{
                anchors.fill: parent
                onReleased: {
                    if(containsMouse)
                        listView.currentIndex = index
                }
            }
        }

        onCurrentIndexChanged:{
            if(listView.currentIndex === 1){
                amAnimation.stop()
                pmAnimation.start()
            }else{
                pmAnimation.stop()
                amAnimation.start()
            }
        }

        NumberAnimation{ id: pmAnimation; target: listView; property: "anchors.verticalCenterOffset"; easing.type: Easing.OutQuart; to: - 30 }
        NumberAnimation{ id: amAnimation; target: listView; property: "anchors.verticalCenterOffset"; easing.type: Easing.OutQuart; to: 70}

        onDragEnded: {
            if(contentY > 0){
                if(listView.currentIndex !== 1){
                    listView.currentIndex = 1
                }
            }else{
                if(listView.currentIndex !== 0){
                    listView.currentIndex = 0
                }
            }
        }
    }

    Image {
        id: div
        source: "qrc:/icon/line.png"
        width: 1
        height: 225
        anchors.right: parent.right
        Rectangle{
            width: 3
            color: "#FFFFFF"
            opacity: 0.8
            height: 56
            anchors.centerIn: parent
        }
    }
}
