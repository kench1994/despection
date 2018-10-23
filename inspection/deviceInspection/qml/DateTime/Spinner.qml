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

PathView{
    id: spinner
    // public:
    /*! 单位信息的文本*/
    property alias unitText: unitTxt.text
    /*! 单位信息的水平位移量*/
    property int unitTextHorizontalCenterOffset: 50
    /*! 分割线的有效性*/
    property alias divLineVisible: div.visible

    property int unitTextSize: 36
    property int unitFocusTextSize: 50

    /*! 设定当前index值*/
    function setCurrentIndex(index) {
        positionViewAtIndex(index, PathView.Center)
    }

    // setting:
    implicitWidth: 240
    implicitHeight: parent.height
    pathItemCount: 5
    preferredHighlightBegin: 0.5
    preferredHighlightEnd: 0.5
    highlightRangeMode: PathView.StrictlyEnforceRange
    snapMode: PathView.SnapToItem
    //flickDeceleration: 500
    maximumFlickVelocity: 500

    path: Path{
        startX: spinner.width/2; startY: 0;
        PathLine { x: spinner.width/2; y: spinner.height }
    }

    delegate: Text {
        width: parent.width
        height: parent.height
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text: content
        font.pixelSize: PathView.isCurrentItem ? unitFocusTextSize : unitTextSize
        color: PathView.isCurrentItem ? "#333333" : "#999999"
        MouseArea{
            anchors.fill: parent
            anchors.margins: -18
            onReleased: {
                if(containsMouse){
                    spinner.currentIndex = index
                }
            }
        }
    }

    Text{
        id: unitTxt
        anchors.centerIn: parent
        anchors.horizontalCenterOffset: unitTextHorizontalCenterOffset + paintedWidth/4
        font.pixelSize: unitTextSize 
        color: "#bbbbbb"
    }

    BorderImage {
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

