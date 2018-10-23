#include "dailyTaskform.h"
#include "formConfigType.h"
#include "inputwidget.h"
#include "enumwidget.h"
#include "timewidget.h"
#include "../common/runtimeparam.h"
#include <QDebug>
#include <QDateTime>
DailyTaskForm::DailyTaskForm(const QVariantMap &deviceValue, const QVariantMap &dataPackage,
                               QQmlContext *rootContext, QQuickItem *contentColItem, QQuickItem *choiceDialogItem)
    :BaseForm(deviceValue, dataPackage, PAGE_FORM_TYPE_DAILYTASK, rootContext, contentColItem, choiceDialogItem)
{
    createWidget();
}

DailyTaskForm::~DailyTaskForm(){
    qDebug() << "DailyTaskForm::~DailyTaskForm()";
}

int DailyTaskForm::createWidget(){
    //qDebug() << "DailyTaskForm::createWidget:::   " << m_nFormValueList;
    for(int i = 0; i < m_nFormValueList.count(); i++){
        QVariantMap indicatorMap = m_nFormValueList.at(i);
        QString paramType = indicatorMap[WIDGET_INDICATORTYPE].toString();
        QString dataType = indicatorMap[WIDGET_DATATYPE].toString();
        //创建控件
        BaseWidget *widget;
        qDebug() << "paramType is " << paramType << "   dataType is  " << dataType;
        if (paramType == WIDGET_INDICATORTYPE_ENUMTYPE){
            QList< QVariantMap > enumMapList;
            for(int j = 0; j < m_nFormEnumList.count(); j++) {
                QVariantMap enumMap = m_nFormEnumList.at(j);
                if(indicatorMap[NODE_ATTR_INDICATORID].toString()
                        == enumMap[NODE_ATTR_INDICATORID].toString())
                    enumMapList.append(enumMap);
            }
            widget = new EnumWidget(m_pRootContext, m_pContentColItem, m_pChoiceDialogItem,
                                    INSPECTION_WIDGET_ENUM, PAGE_FORM_TYPE_DAILYTASK, indicatorMap, enumMapList);

        } else if(paramType == WIDGET_INDICATORTYPE_INPUT &&
                  (dataType == WIDGET_DATATYPE_INTTYPE ||
                   dataType == WIDGET_DATATYPE_STRINGTYPE ||
                   dataType == WIDGET_DATATYPE_DECIMALTYPE)){
            widget = new InputWidget(m_pRootContext, m_pContentColItem, INSPECTION_WIDGET_INPUT,
                                     PAGE_FORM_TYPE_DAILYTASK, indicatorMap);
        } else if(paramType == WIDGET_INDICATORTYPE_INPUT && dataType == WIDGET_DATATYPE_DATETIME){
            widget = new TimeWidget(m_pRootContext, m_pContentColItem, INSPECTION_WIDGET_DATETIME,
                                    PAGE_FORM_TYPE_DAILYTASK, indicatorMap);
        } else if(paramType == WIDGET_INDICATORTYPE_INPUT && dataType == WIDGET_DATATYPE_DATE){
            widget = new TimeWidget(m_pRootContext, m_pContentColItem, INSPECTION_WIDGET_DATE,
                                    PAGE_FORM_TYPE_DAILYTASK, indicatorMap);
        } else if(paramType == WIDGET_INDICATORTYPE_INPUT && dataType == WIDGET_DATATYPE_TIME){
            widget = new TimeWidget(m_pRootContext, m_pContentColItem, INSPECTION_WIDGET_TIME,
                                    PAGE_FORM_TYPE_DAILYTASK, indicatorMap);
        }
        widget->showWidget();
        m_pBaseWidgetList.append(widget);
    }
    return 0;
}

void DailyTaskForm::getDataRow(QMap<QString, QString> &dataRow, const QVariantMap &otherParam)
{
    QVariantMap dataTableMap;
    dataTableMap = m_nDataPackage["tables"].toList().at(0).toMap();
    dataRow["PlanObjectID"] = dataTableMap["value"].toList().at(0).toMap()["PlanObjectID"].toString();
    dataRow["PlanGroupID"] = dataTableMap["value"].toList().at(0).toMap()["PlanGroupID"].toString();
    dataRow["TaskDataKey"] = m_nDeviceValue["TaskDataKey"].toString();
    dataRow["ItemData"] = getResult();
    dataRow["Score"] = "0";
    dataRow["PDAID"] = RuntimeParam::getInstance()->getPadId();
    dataRow["IsRFIDScan"] = m_nDeviceValue["IsRFIDScan"].toString();
    dataRow["IsBarCodeScan"] = m_nDeviceValue["IsBarCodeScan"].toString();
    dataRow["TotalIndicatorCount"] = QString::number(m_pBaseWidgetList.length());
    dataRow["ObjectStatus"] = otherParam["runStatus"].toString();
    dataRow["SerialKey"] = otherParam["serialKey"].toString();

    int normalCount = 0;
    for(int i = 0; i < m_pBaseWidgetList.length(); i++){
        QString isNormal = m_pBaseWidgetList.at(i)->getExternalParameters()["IsNormal"].toString();
        if(isNormal == "1")
            normalCount++;
    }
    dataRow["NormalIndicatorCount"] = QString::number(normalCount);
    dataRow["CreatedBy"] =  RuntimeParam::getInstance()->getUserId();
}
