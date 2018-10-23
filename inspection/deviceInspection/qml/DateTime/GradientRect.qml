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
import QtGraphicalEffects 1.0

Item {
    implicitWidth: 720
    implicitHeight: 40
    property bool isTopGradient: true
    property alias gradientBaseColor: bug.color

    Rectangle {
        id: bug
        anchors.fill: parent
        smooth: true
        visible: false
    }

    Image {
        id: mask
        source: isTopGradient ? "qrc:/icon/GradientTop.png" : "qrc:/icon/GradientBottom.png"
        sourceSize: Qt.size(parent.width, parent.height)
        smooth: true
        visible: false
    }

    OpacityMask {
        anchors.fill: bug
        source: bug
        maskSource: mask
    }
}
