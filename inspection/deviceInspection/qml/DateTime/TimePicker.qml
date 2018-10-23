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
import com.syberos.basewidgets 2.0
/*!
    \qmltype TimePicker
    \brief 时间选择器控件。

    TimePicker：基于Rectangle，其提供时间的选择。时间的小时域是有12小时或者24小时制的区别。
    此会根据系统的时间格式确定与动态变换时间格式以及显示方式。

    \image

    示例代码:
    \code
import QtQuick 2.3
import com.syberos.basewidgets 2.0

Rectangle {
    anchors.fill: parent
    color: "#EEEEEE"

    TimePicker{
        id: timePicker
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
    }
}
    \endcode
 */
Rectangle { //使用Rectangle为了GradientRectangle的color属性的值与之对应
    implicitWidth: 580
    implicitHeight: 450
    clip: true // 防止显示越界

    /*!  设置是否使用24小时格式，默认是系统时间格式*/
    property bool is24HourFormat: gSystemUtils.is24HourFormat

    /*! 小时*/
    property int hour: privateData.systemHours()

    /*!
        \qmlproperty bool CTimePicker::hourEnabled
        设置是否显示小时区域，默认为true。
    */
    property alias hourEnabled: selectHour.visible

    /*!
        \qmlproperty string CTimePicker::hourUnitText
        设置小时的单位，默认是’时‘。
    */
    property alias hourUnitText: selectHour.unitText

    /*! 分钟*/
    property int minute: privateData.systemMinutes()

    /*!
        \qmlproperty string CTimePicker::minuteUnitText
        设置分钟的单位，默认是’分‘
    */
    property alias minuteUnitText: selectMin.unitText

    /*! 秒钟*/
    property int second: privateData.systemSeconds()

    /*!
        \qmlproperty bool CTimePicker::secondEnabled
        设置是否显示秒数区域，默认为false
    */
    property alias secondEnabled: selectSec.visible

    /*!
        \qmlproperty string CTimePicker::secondUnitText
        设置秒的单位，默认是’秒‘
    */
    property alias secondUnitText: selectSec.unitText

    /*! 设置是否显示分隔线*/
    property bool divideLineVisible: true

    /*! 设置非高亮的数字体大小*/
    property int textSize: 36

    /*! 设置高亮的数字体大小*/
    property int focusTextSize: 50

    /*! 设置数字和月日之间的间距*/
    property int textHorizontalCenterOffset: 50

    /*!
        \qmlmethod CTimePicker::time(separate)
        获取时间，使用分隔符separate时间格式。
    */
    function time(separate){
        var time = ""
        if(selectHour.visible){
            time += privateData.addZero(hour)
            time += separate.toString()
        }
        time += privateData.addZero(minute)
        if(selectSec.visible){
            time += separate.toString()
            time += privateData.addZero(second)
        }
        return time
    }

    /*!
        \qmlmethod CTimePicker::setTime(int hh,int mm,int ss)
        设置时分秒的数值
    */
    function setTime(hh,mm,ss){
        hour = hh
        minute = mm
        second = ss
    }

    Rectangle{
        implicitWidth: parent.width; implicitHeight: 50
        anchors.verticalCenter: parent.verticalCenter
        CLine{ anchors.top: parent.top; color: gUiConst.getValue("CL2") }
        CLine{ anchors.bottom: parent.bottom; color: gUiConst.getValue("CL2") }
    }

    Row{
        anchors.fill: parent
        spacing: 0

        PartitionSelector{
            id: partitionSelector
            height: parent.height
            visible: !is24HourFormat
            divLineVisible: divideLineVisible
            onCurrentIndexChanged: {
                if(!is24HourFormat && (selectHour.count === 12)){
                        hour = ((selectHour.currentIndex + 1)% 12) + ((partitionSelector.currentIndex === 0) ? 0 : 12)
                }
            }
        }
        Spinner{
            id: selectHour
            height: parent.height
            model: hourModel
            unitText:"时"//os.i18n.ctr( qsTr("h","时") )
            onVisibleChanged: privateData.doLayout()
            onCurrentIndexChanged: if(dragging || flicking) privateData.updateHour()
            divLineVisible: divideLineVisible
            unitTextSize: textSize
            unitFocusTextSize: focusTextSize
            unitTextHorizontalCenterOffset: textHorizontalCenterOffset
        }
        Spinner{
            id: selectMin
            height: parent.height
            model: minuteModel
            unitText:"分"//os.i18n.ctr( qsTr("m","分") )
            divLineVisible: selectSec.visible && divideLineVisible
            onCurrentIndexChanged: if(dragging || flicking) privateData.updateMinute()
            unitTextSize: textSize
            unitFocusTextSize: focusTextSize
            unitTextHorizontalCenterOffset: textHorizontalCenterOffset
        }
        Spinner{
            id: selectSec
            height: parent.height
            model: secendModel
            unitText: "秒"//os.i18n.ctr( qsTr("s","秒") )
            visible: false
            divLineVisible: false
            onVisibleChanged: privateData.doLayout()
            onCurrentIndexChanged: if(dragging || flicking) privateData.updateSecond()
            unitTextSize: textSize
            unitFocusTextSize: focusTextSize
            unitTextHorizontalCenterOffset: textHorizontalCenterOffset
        }
    }

    ListModel {
        id: hourModel
    }
    ListModel {
        id: minuteModel
    }
    ListModel {
        id: secendModel
    }

    GradientRect{
        isTopGradient: true
        width: parent.width
        anchors.top: parent.top
        gradientBaseColor: parent.color
    }

    GradientRect{
        isTopGradient: false
        width: parent.width
        anchors.bottom: parent.bottom
        gradientBaseColor: parent.color
    }

    Component.onCompleted: {
        privateData.updateTimeHour()
        privateData.updateTimeMinute()
        privateData.updateTimeSecond()
        privateData.doLayout()
    }
    onHourChanged: {
        if(!privateData.userOperated){
            privateData.setHours(hour)
        }
    }
    onIs24HourFormatChanged: {
        privateData.doLayout()
        privateData.updateTimeHour()
    }
    onMinuteChanged: {
        if(!privateData.userOperated){
            privateData.setMinutes(minute)
        }
    }
    onSecondChanged: {
        if(!privateData.userOperated){
            privateData.setSeconds(second)
        }
    }
    onVisibleChanged: if(visible) privateData.doLayout()
    onWidthChanged: privateData.doLayout()

    QtObject{
        id: privateData
        property bool userOperated: false

        function updateTimeHour() {
            var index = hour
            if(!is24HourFormat){
                if(hourModel.count > 12) {
                    partitionSelector.currentIndex = (selectHour.currentIndex > 11) ? 1 : 0
                    index = (index + 11) % 12
                }

                if(hourModel.count > 0 && hourModel.count <= 12 ) {
                    if(partitionSelector.currentIndex === 0){
                        index = (index + 1) % 12
                    } else {
                        index = (index + 1) % 12 + 12
                    }
                }
            }

            if(hourModel.count > 0){
                hourModel.clear()
            }

            var i = 0

            if(is24HourFormat) {
                for(i=0; i<=23; ++i) {
                    hourModel.append({"content": addZero(i)})
                }
            } else {
                for(i=1; i<=12; ++i) {
                    hourModel.append({"content": addZero(i)})
                }
            }
            selectHour.setCurrentIndex(index)
        }

        function updateTimeMinute(){
            minuteModel.clear()
            for(var i=0; i<=59; ++i) {
                minuteModel.append({"content": addZero(i)})
            }
            selectMin.setCurrentIndex( bound(0, minute, 59) )
        }

        function updateTimeSecond(){
            secendModel.clear()
            for(var i=0; i<=59; ++i) {
                secendModel.append({"content": addZero(i)})
            }
            selectSec.setCurrentIndex( bound(0, second, 59) )
        }

        function doLayout(){
            var n = 0
            if(!is24HourFormat){
                n += 1
            }
            if(selectHour.visible){
                n += 1
            }
            if(selectMin.visible){
                n += 1
            }
            if(selectSec.visible){
                n += 1
            }
            if(n > 0){
                var w = width/n
                if(!is24HourFormat){
                    partitionSelector.width = w
                }
                if(selectHour.visible){
                    selectHour.width = w
                }
                if(selectMin.visible){
                    selectMin.width = w
                }
                if(selectSec.visible){
                    selectSec.width = w
                }
            }
        }

        function bound(min,value,max){
            return Math.max(min,Math.min(value,max))
        }

        function addZero(num) {
            if( num <= 9 ){
                num = "0" + num
            }
            return num.toString()
        }

        /*! 获取时间*/
        function updateHour(){
            userOperated = true
            if(!is24HourFormat){
                hour = ((selectHour.currentIndex + 1)% 12) + ((partitionSelector.currentIndex === 0) ? 0 : 12)
            }else{
                hour = selectHour.currentIndex
            }
            userOperated = false
        }
        /*! 获取分钟*/
        function updateMinute(){
            userOperated = true
            minute = selectMin.currentIndex
            userOperated = false
        }
        /*! 获取秒数*/
        function updateSecond(){
            userOperated = true
            second = selectSec.currentIndex
            userOperated = false
        }
        /*! 设置小时*/
        function setHours(hours){
            var h = bound(0, hours, 23)
            if(!is24HourFormat){
                partitionSelector.currentIndex = (h > 11 ? 1: 0)
            }
            if(!is24HourFormat){
                h = (h + 11) % 12
            }
            selectHour.setCurrentIndex( h )
        }
        /*! 设置分钟*/
        function setMinutes(minutes){
            selectMin.setCurrentIndex( bound(0, minutes, 59) )
        }
        /*! 设置秒数*/
        function setSeconds(seconds){
            selectSec.setCurrentIndex( bound(0, seconds, 59) )
        }

        property var systemCurrentDate: new Date()
        function systemHours(){
            return systemCurrentDate.getHours()
        }
        function systemMinutes(){
            return systemCurrentDate.getMinutes()
        }
        function systemSeconds(){
            return systemCurrentDate.getSeconds()
        }

    }

}
