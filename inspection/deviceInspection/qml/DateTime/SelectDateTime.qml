/*!
* \file SelectDateTime.qml
* \brief 公共页面，通用时间日期选择控件
*
*公共页面，通用选择时间日期功能的控件
*
*\author 张焕林 zhanghuanlin@syberos.com
*\version 1.0
*\date 2016/3/9
*/
import QtQuick 2.0
import com.syberos.basewidgets 2.0

CDialog {
    titleText: {
        if(type == "date"){
            return "选择日期";
        }else if(type == "time"){
            return "选择时间";
        }else{
            return "选择日期和时间";
        }
    }
    spacingBetweenTitleAreaAndMessageArea: 30
    spacingBetweenMessageAreaAndButtonArea: 30
    messageAreaLeftMargin: {
        if(type == "date"){
            return 20;
        }else if(type == "time"){
            return 40;
        }else{
            return 0;
        }
    }
    messageAreaRightMargin: {
        if(type == "date"){
            return 20;
        }else if(type == "time"){
            return 40;
        }else{
            return 0;
        }
    }

    // "date":  只有日期,  "time": 只有时间，"dateTime": 时间和日期
    property string type: "dateTime"
    signal selectedDateTime(string dateTime)
    onAccepted:{
        var dateTime = dateTimeString();
        selectedDateTime(dateTime);
    }

    function dateTimeString(){
        if(type == "date"){
            return messageAreaItem.dateItem.date("-");
        }else if(type == "time"){
            return messageAreaItem.timeItem.time(":") + ":00";
        }else{
            return messageAreaItem.dateItem.date("-") + " " + messageAreaItem.timeItem.time(":") + ":00";
        }
    }

    function setDateTime(date){
        var year = date.getFullYear()
        var month = date.getMonth()+1
        var day = date.getDate()
        var hour = date.getHours()
        var minute = date.getMinutes()

        if(type == "date"){
            messageAreaItem.dateItem.setDate(year, month, day)
        }else if(type == "time"){
            messageAreaItem.timeItem.setTime(hour, minute, 0)
        }else{
            messageAreaItem.dateItem.setDate(year, month, day)
            messageAreaItem.timeItem.setTime(hour, minute, 0)
        }
    }

    messageAreaComponent: Rectangle {
            id: dateTimeRect
            width: 480
            height: 225
            color: "black"

            property alias minimumYear: datePicker.minimumYear
            property alias maximumYear: datePicker.maximumYear
            property alias yearEnabled:  datePicker.yearEnabled
            property alias dayEnabled: datePicker.dayEnabled
            property alias dateDivideLineVisible: datePicker.divideLineVisible

            property alias is24HourFormat: timePicker.is24HourFormat
            property alias timeDivideLineVisible: timePicker.divideLineVisible
            property alias hourEnabled: timePicker.hourEnabled
            property alias hourUnitText: timePicker.hourUnitText
            property alias minuteUnitText: timePicker.minuteUnitText
            property alias  secondEnabled: timePicker.secondEnabled
            property alias secondUnitText: timePicker.secondUnitText
            property int unitTextSize: 18
            property int unitFocusTextSize: 24
            property int unittextHorizontalCenterOffset: 25
            //property string  dateStr: datePicker.date("-")
            //property string timeStr: timePicker.time(":")
            property alias  dateItem: datePicker
            property alias timeItem: timePicker


            Row {
                width: parent.width
                height: parent.height

                DatePicker{
                    id: datePicker
                    width: {
                        if(type == "date"){
                            return parent.width;
                        }else if(type == "time"){
                            return 0;
                        }else{
                        return parent.width*25/40;
                        }
                    }
                    height: parent.height
                    textSize: unitTextSize
                    focusTextSize: unitFocusTextSize
                    textHorizontalCenterOffset: unittextHorizontalCenterOffset
                }

                TimePicker{
                    id: timePicker
                    width:{
                        if(type == "date"){
                            return 0;
                        }else if(type == "time"){
                            return parent.width;
                        }else{
                        return parent.width*15/40;
                        }
                    }
                    height: parent.height
                    textSize: unitTextSize
                    focusTextSize: unitFocusTextSize
                    textHorizontalCenterOffset: unittextHorizontalCenterOffset
                }
            }
        }
}
