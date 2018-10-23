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
\class DatePicker
\brief 日期选择器控件。

*/
/*!
    \qmltype DatePicker
    \inqmlmodule com.syberos.basewidgets
    \since 2.0
    \ingroup controls
    \brief 日期选择器控件。

    DatePicker：基于Rectangle，其主要实现一个日期选择器。需要注意的是月份改变的时候
    天数会发生变化，需要根据进行矫正。用户根据实际需求可以确定年、月、日是否加载的情况。

    关键技术：使用PathView完成循环显示控件，并使用此展示年月日的循环选择，需要注意的是
    如果年份和月份发生改变的时候，其对应的天数需要根据实际情况进行变动。

    示例代码:
    \code
import QtQuick 2.3
import com.syberos.basewidgets 2.0

Rectangle {
    anchors.fill: parent
    color: "#EEEEEE"

    DatePicker{
        id: datePicker
        anchors.top: parent.top
        anchors.topMargin: 100
        anchors.horizontalCenter: parent.horizontalCenter
        year: 2013
        month: 2
        day: 5
    }
}
    \endcode
 */

Rectangle {//使用Rectangle为了GradientRectangle的color属性的值与之对应
    implicitWidth: 580
    implicitHeight: 450
    clip: true // 防止显示越界

    /*! 日*/
    property int day: privateData.systemDay()

    /*!
        \qmlproperty bool CDatePicker::dayEnabled
        设置是否启用天
    */
    property alias dayEnabled: selectDay.visible

    /*! 年的最大值，默认值2038 */
    property int maximumYear: 2038

    /*! 年的最小值，默认值1970 */
    property int minimumYear: 1970

    /*! 月的最大值，默认值12 */
    property int maximumMonth: 12

    /*! 月的最小值，默认值1 */
    property int minimumMonth: 1

    /*! 日的最小值，默认值1*/
    property int minimumDay: 1

    /*! 月*/
    property int month: privateData.systemMonth()

    /*! 年*/
    property int year: privateData.systemYear()

    /*!
        \qmlproperty bool CDatePicker::yearEnabled
        设置是否使用年份
    */
    property alias yearEnabled: selectYear.visible

    /*! 设置是否显示分隔线*/
    property bool divideLineVisible: true

    /*! 设置非高亮的数字体大小*/
    property int textSize: 36

    /*! 设置高亮的数字体大小*/
    property int focusTextSize: 50

    /*! 设置数字和年月日之间的间距*/
    property int textHorizontalCenterOffset: 50


    /*!
        \qmlmethod CDatePicker::date(string separate)
        以separate作为分隔符获取日期
    */
    function date(separate){
        var date = ""
        if(selectYear.visible){
            date += year.toString()
            date += separate.toString()
        }
        date += privateData.addZero(month)
        if(selectDay.visible){
            date += separate.toString()
            date += privateData.addZero(day)
        }
        return date
    }

    /*!
        \qmlmethod CDatePicker::setDate(int newYear, int newMonth, int newDay)
        设置年月日的数组
    */
    function setDate(newYear, newMonth, newDay){
        year = newYear
        month = newMonth
        day = newDay
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

        Spinner{
            id: selectYear
            model: yearModel
            onCurrentIndexChanged: {
                if(dragging || flicking){
                    privateData.updateCurrentYear()
                    privateData.updateDays()
                }
            }
            unitText:"年"//os.i18n.ctr( qsTr("Y","年") )
            unitTextHorizontalCenterOffset: 35
            onVisibleChanged: {
                privateData.doLayout()
                privateData.updateDateYears()
            }
            divLineVisible: divideLineVisible
            unitTextSize: textSize
            unitFocusTextSize: focusTextSize
        }
        Spinner{
            id: selectMonth
            model: monthModel
            onCurrentIndexChanged:{
                if(dragging || flicking){
                    privateData.updateCurrentMonth()
                    privateData.updateDays()
                }
            }
            unitText: "月"//os.i18n.ctr( qsTr("M","月") )
            divLineVisible: selectDay.visible && divideLineVisible
            unitTextSize: textSize
            unitFocusTextSize: focusTextSize
            unitTextHorizontalCenterOffset: textHorizontalCenterOffset
        }
        Spinner{
            id: selectDay
            model: dayModel
            unitText:"日"//os.i18n.ctr( qsTr("D","日") )
            divLineVisible: false
            onVisibleChanged: privateData.doLayout()
            onCurrentIndexChanged: privateData.updateCurrentDay()
            unitTextSize: textSize
            unitFocusTextSize: focusTextSize
            unitTextHorizontalCenterOffset: textHorizontalCenterOffset
        }
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

    ListModel {
        id: yearModel
    }
    ListModel {
        id: monthModel
    }
    ListModel {
        id: dayModel
    }

    Component.onCompleted: {
        privateData.updateDateYears()
        privateData.updateDateMonths()
        privateData.updateDays()
        privateData.doLayout()
    }
    onDayChanged: {
        if(!privateData.userOperated){
            privateData.setDay(day)
        }
    }
    onMaximumYearChanged: {
        privateData.updateYearsRange()
    }
    onMinimumYearChanged: {
        privateData.updateYearsRange()
    }
    onMonthChanged: {
        if(!privateData.userOperated){
            privateData.setMonth(month)
        }
    }
    onVisibleChanged: {
        if(visible) {
            privateData.doLayout()
        }
    }
    onWidthChanged: privateData.doLayout()
    onYearChanged: {
        if(!privateData.userOperated){
            privateData.setYear(year)
        }
    }

    QtObject{
        id: privateData
        property bool userOperated: false

        function updateDateYears(){
            yearModel.clear()
            for(var i=minimumYear; i<=maximumYear; ++i) {
                yearModel.append({"content": i})
            }
            selectYear.setCurrentIndex( bound(minimumYear, year, maximumYear) - minimumYear )
        }
        function updateDateMonths(){
            monthModel.clear()
            for(var i=minimumMonth; i<=maximumMonth; ++i) {
                monthModel.append({"content": addZero(i)})
            }
            selectMonth.setCurrentIndex( bound(minimumMonth, month, maximumMonth) - minimumMonth )
        }
        function updateDateDays(){
            var days = getDays(year,month)
            dayModel.clear()
            for(var i=minimumDay; i<=days; ++i) {
                dayModel.append({"content": addZero(i)})
            }
            return days
        }
        function updateDays(){
            var tmpDay = day
            var days = updateDateDays()
            selectDay.setCurrentIndex(Math.min(tmpDay, days) - minimumDay)
        }

        function doLayout(){
            var n = 0
            if(selectYear.visible){
                n += 1
            }
            if(selectMonth.visible){
                n += 1
            }
            if(selectDay.visible){
                n += 1
            }
            if(n > 0){
                var w = width/n
                if(selectYear.visible){
                    selectYear.width = w
                }
                if(selectMonth.visible){
                    selectMonth.width = w
                }
                if(selectDay.visible){
                    selectDay.width = w
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
            return num.toString();
        }

        function getDays(year,month){
            var day = new Date(year,month,0)
            return day.getDate()
        }

        property var systemCurrentDate: new Date()
        function systemYear(){
            return systemCurrentDate.getFullYear()
        }

        function systemMonth(){
            return systemCurrentDate.getMonth() + 1
        }

        function systemDay(){
            return systemCurrentDate.getDate()
        }

        /*! 获取当前设置的年份*/
        function updateCurrentYear() {
            userOperated = true
            if(selectYear.visible){
                year = selectYear.currentIndex + minimumYear
            }else{
                year = systemYear()
            }
            userOperated = false
        }
        /*! 获取当前设定的月份*/
        function updateCurrentMonth() {
            userOperated = true
            month = selectMonth.currentIndex + 1
            userOperated = false
        }
        /*! 获取当前设定的天*/
        function updateCurrentDay() {
            userOperated = true
            day = selectDay.currentIndex + 1
            userOperated = false
        }
        /*! 设置年份*/
        function setYear( yearValue ){
            selectYear.setCurrentIndex( bound(minimumYear, yearValue, maximumYear) - minimumYear )
            updateDays()
        }
        /*! 设置月份*/
        function setMonth( monthValue ){
            selectMonth.setCurrentIndex( bound(minimumMonth, monthValue, maximumMonth) - minimumMonth )
            updateDays()
        }
        /*!  设置天*/
        function setDay( dayValue ){
            updateDays()
            selectDay.setCurrentIndex( bound(minimumDay, dayValue, getDays(year,month)) - minimumDay )
        }

        /*! 设置年份的区间*/
        function updateYearsRange() {
            var tmpYear = year
            updateDateYears()
            setYear(tmpYear)
            //如果tmpYear不在minYear和maxYear之间，那么可能出现闰年调整到其他年份，日期改变的情况
            updateDays()
        }

    }
}
