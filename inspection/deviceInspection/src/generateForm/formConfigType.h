/*!
* \file formConfigType.h
* \brief 动态表单配置类
*
*在此类中配置动态表单所需的参数
*
*\author 桓乐乐 huanlele@syberos.com
*\version 1.0
*\date 2015/9/7
*/
#ifndef FORMCONFIGTYPE_H
#define FORMCONFIGTYPE_H
#include <QString>
#include <QStringList>
#include <QVariantMap>
//动态表单生成以及提交需要的枚举以及宏定义
enum baseWidgetType{
    WIDGET_TYPE_UNKNOWN,
    INSPECTION_WIDGET_INPUT,
    INSPECTION_WIDGET_ENUM,
    INSPECTION_WIDGET_DATETIME,
    INSPECTION_WIDGET_DATE,
    INSPECTION_WIDGET_TIME,
    MAINTAIN_WIDGET_INPUT,
    MAINTAIN_WIDGET_ENUM,
    MAINTAIN_WIDGET_DATETIME,
    MAINTAIN_WIDGET_DATE,
    MAINTAIN_WIDGET_TIME,
    STANDARD_WIDGET_INPUT,
    STANDARD_WIDGET_ENUM,
    STANDARD_WIDGET_INPUT_ENUM,
    STANDARD_WIDGET_CHECK,
    DATAREPORT_WIDGET_INPUT,
    DATAREPORT_WIDGET_ENUM,
    DATAREPORT_WIDGET_DATETIME,
    DATAREPORT_WIDGET_DATE,
    DATAREPORT_WIDGET_TIME,
    DATAREPORT_WIDGET_MULTIINPUT
};

//表单类型
#define PAGE_FORM_TYPE_INSPECTION   "inspection"   //巡检
#define PAGE_FORM_TYPE_MAINTAIN     "maintain"      //保养
#define PAGE_FORM_TYPE_STANDARD     "standard"      //标准化检查
#define PAGE_FORM_TYPE_DATAREPORT     "dataReport"      //数据上报
#define PAGE_FORM_TYPE_DAILYTASK     "dailyTask"      //日常任务

//控件类型字段
//控件是输入还是枚举
#define WIDGET_INDICATORTYPE "IndicatorType"
#define WIDGET_INDICATORTYPE_ENUMTYPE "2"
#define WIDGET_INDICATORTYPE_INPUT   "1"

//输入型控件的输入数值类型
#define WIDGET_DATATYPE "DataType"   //巡检和保养使用
#define WIDGET_DATATYPE_STRINGTYPE    "StringType"
#define WIDGET_DATATYPE_INTTYPE    "IntType"
#define WIDGET_DATATYPE_DECIMALTYPE    "DecimalType"

#define WIDGET_DISPLAYTYPE "DisplayType"   //数据上报使用，区分多行，单行输入
#define WIDGET_DISPLAYTYPE_MULITTEXT    "MultiTextType"
#define WIDGET_DISPLAYTYPE_SINGLETEXT    "SingleTextType"


#define WIDGET_DATATYPE_DATETIME    "DateTimeType"
#define WIDGET_DATATYPE_DATE    "DateType"
#define WIDGET_DATATYPE_TIME    "TimeType"
    //标准化检查控件类型
#define STD_INDICATORTYPE_FIXEDTYPE "1"  //固定类型；
#define STD_INDICATORTYPE_ENUMTYPE "2" //枚举类型
#define STD_INDICATORTYPE_ISCOUNTER "IsCounter" //标准化检查使用
#define STD_INDICATORTYPE_ISCOUNTER_YES "1"
#define STD_INDICATORTYPE_ISCOUNTER_NO "0"

#define WIDGET_MAINTITLE "DisplayName"

//界面展现需要字段   begain
//枚举值，标准化检查，巡检，保养通用,
//EnumCode，IsNormal:标准化检查没有这两个字段
//Score:保养没有该字段
#define ENUM_ENUMID "EnumID"
#define ENUM_ENUMCODE "EnumCode"
#define ENUM_ENUMVALUE "EnumValue"
#define ENUM_SCORE "Score"
#define ENUM_INDICATORID "IndicatorID"
#define ENUM_ISNORMAL "IsNormal"
#define ENUM_RISKLEVEL "RiskLevel"

//检查指标和枚举类表关联字段
#define NODE_ATTR_INDICATORID "IndicatorID"
//界面展现需要字段   end





//向服务器提交需要字段
#define SUBMIT_ID       "ID"
#define SUBMIT_ITEMSTATUS "ItemStatus"
#define SUBMIT_MODIFIEDDATE "ModifiedDate"
#define SUBMIT_EXCEPTIONTYPE "ExceptionType"
#define SUBMIT_TASKITEMID "TaskItemID"
#define SUBMIT_TASKITEMAPID "TaskItemMapID"
#define SUBMIT_TASKITEMAPTYPE "TaskItemMapType"
#define SUBMIT_MAINTENANCETYPE "MaintenanceType"
#define SUBMIT_NORMALCOUNT "NormalCount"
#define SUBMIT_ISRFIDSCAN "IsRFIDScan"
#define SUBMIT_ISBARCODESCAN "IsBarCodeScan"
#define SUBMIT_PDAID "PDAID"
#define SUBMIT_MODIFIEDBY "ModifiedBy"
#define SUBMIT_EXAMUSER "ExamUser"
#define SUBMIT_ITEMDATAS "ItemDatas"
#define SUBMIT_ITEMDATA "ItemData"
#define SUBMIT_CREATEDBY "CreatedBy"
#define SUBMIT_CREATEDDATE "CreatedDate"
#define SUBMIT_STANDARDSCORE "StandardScore"


//指标项
#define SUBMIT_NODE_ENGLISHNAME "EnglishName"
#define SUBMIT_NODE_INDICATORNAME "IndicatorName"
#define SUBMIT_NODE_PARENTINDICATORID "ParentIndicatorID"
#define SUBMIT_NODE_ATTR_INDICATORID "IndicatorID"
#define SUBMIT_NODE_ATTR_INDICATORCODE "IndicatorCode"
#define SUBMIT_NODE_ATTR_MODELITEMID "ModelItemID"
#define SUBMIT_NODE_ATTR_VALUE "Value"
#define SUBMIT_NODE_ATTR_ISNORMAL "IsNormal"
#define SUBMIT_NODE_ATTR_RISKLEVEL "RiskLevel"
#define SUBMIT_NODE_ATTR_ENUMID "EnumID"
#define SUBMIT_NODE_ATTR_DISPLAYNAME "DisplayName"
#define SUBMIT_NODE_ATTR_ISCOUNTER "IsCounter"
#define SUBMIT_NODE_ATTR_PARAMETERTYPE "ParameterType"
#define SUBMIT_NODE_ATTR_INDICATORTYPE "IndicatorType"
#define SUBMIT_NODE_ATTR_SCORE "Score"
#define SUBMIT_NODE_ATTR_COUNTER "Counter"
#define SUBMIT_NODE_ATTR_COUNTUNITNAME "CountUnitName"
#define SUBMIT_NODE_ATTR_DEFAULTVALUE "DefaultValue"

//此类为初始化全局变量而创建
class FormConfigType
{
public:
    /**
     * @brief getInstance：获取单例
     * @return: 返回单例指针
     */
    static FormConfigType *getInstance();
    /**
     * @brief getWidgetPagePathMap：获取控件的路径
     * @return： 控件的路径
     */
    QMap<int, QString> getWidgetPagePathMap();

    /**
     * @brief getWidgetSubmitKeyList:获取控件需要提交的属性字段
     * @param widgetType：控件类型
     * @return:属性字段列表
     */
    QStringList getWidgetSubmitKeyList(baseWidgetType widgetType);
private:
    /**
     * @brief FormConfigType:构造函数
     */
    FormConfigType();
    static FormConfigType *m_pFormConfigType;///<  本地单例对象
private:
    //控件的路径
    QMap<int, QString> widgetPagePath_map; ///<控件路径

};
#endif // FORMCONFIGTYPE_H
