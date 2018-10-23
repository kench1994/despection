#include "inspectionform.h"
#include "formConfigType.h"
#include "inputwidget.h"
#include "enumwidget.h"
#include "timewidget.h"
#include "../common/runtimeparam.h"
#include <QDebug>
#include <QDateTime>
InspectionForm::InspectionForm(const QVariantMap &deviceValue, const QVariantMap &dataPackage,
                               QQmlContext *rootContext, QQuickItem *contentColItem, QQuickItem *choiceDialogItem)
    :BaseForm(deviceValue, dataPackage, PAGE_FORM_TYPE_INSPECTION, rootContext, contentColItem, choiceDialogItem)
{
    createWidget();
}

InspectionForm::~InspectionForm(){
    qDebug() << "InspectionForm::~InspectionForm()";
}

int InspectionForm::createWidget(){
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
                                    INSPECTION_WIDGET_ENUM, PAGE_FORM_TYPE_INSPECTION, indicatorMap, enumMapList);

        } else if(paramType == WIDGET_INDICATORTYPE_INPUT &&
                  (dataType == WIDGET_DATATYPE_INTTYPE ||
                   dataType == WIDGET_DATATYPE_STRINGTYPE ||
                   dataType == WIDGET_DATATYPE_DECIMALTYPE)){
            widget = new InputWidget(m_pRootContext, m_pContentColItem, INSPECTION_WIDGET_INPUT,
                                     PAGE_FORM_TYPE_INSPECTION, indicatorMap);
        } else if(paramType == WIDGET_INDICATORTYPE_INPUT && dataType == WIDGET_DATATYPE_DATETIME){
            widget = new TimeWidget(m_pRootContext, m_pContentColItem, INSPECTION_WIDGET_DATETIME,
                                    PAGE_FORM_TYPE_INSPECTION, indicatorMap);
        } else if(paramType == WIDGET_INDICATORTYPE_INPUT && dataType == WIDGET_DATATYPE_DATE){
            widget = new TimeWidget(m_pRootContext, m_pContentColItem, INSPECTION_WIDGET_DATE,
                                    PAGE_FORM_TYPE_INSPECTION, indicatorMap);
        } else if(paramType == WIDGET_INDICATORTYPE_INPUT && dataType == WIDGET_DATATYPE_TIME){
            widget = new TimeWidget(m_pRootContext, m_pContentColItem, INSPECTION_WIDGET_TIME,
                                    PAGE_FORM_TYPE_INSPECTION, indicatorMap);
        }
        widget->showWidget();
        m_pBaseWidgetList.append(widget);
    }
    return 0;
}

void InspectionForm::getDataRow(QMap<QString, QString> &dataRow, const QVariantMap &otherParam)
{
    QVariantMap dataTableMap;
    dataTableMap = m_nDataPackage["tables"].toList().at(0).toMap();
    dataRow["TaskObjectDataKey"] = dataTableMap["value"].toList().at(0).toMap()["TaskObjectDataKey"].toString();
    dataRow["ItemData"] = getResult();
    dataRow["ObjectStatus"] = otherParam["runStatus"].toString();
    dataRow["SerialKey"] = otherParam["serialKey"].toString();
    dataRow["IsDelegate"] = otherParam["isDelegate"].toString();
    dataRow["PDAID"] = RuntimeParam::getInstance()->getPadId();
    dataRow["IsRFIDScan"] = m_nDeviceValue["IsRFIDScan"].toString();
    dataRow["IsBarCodeScan"] = m_nDeviceValue["IsBarCodeScan"].toString();
    int normalCount = 0;
    for(int i = 0; i < m_pBaseWidgetList.length(); i++){
        QString isNormal = m_pBaseWidgetList.at(i)->getExternalParameters()["IsNormal"].toString();
        if(isNormal == "1")
            normalCount++;
    }
    dataRow["TotalIndicatorCount"] = QString::number(m_pBaseWidgetList.length());
    dataRow["NormalIndicatorCount"] = QString::number(normalCount);
    dataRow["ModifiedBy"] =  RuntimeParam::getInstance()->getUserId();
}
