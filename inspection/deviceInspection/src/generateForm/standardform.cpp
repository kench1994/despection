#include "standardform.h"
#include "formConfigType.h"
#include "inputwidget.h"
#include "enumwidget.h"
#include "inputenumwidget.h"
#include "../common/runtimeparam.h"
#include <QDebug>

StandardForm::StandardForm(const QVariantMap &deviceValue, const QVariantMap &dataPackage,
                               QQmlContext *rootContext, QQuickItem *contentColItem, QQuickItem *choiceDialogItem)
    :BaseForm(deviceValue, dataPackage, PAGE_FORM_TYPE_STANDARD, rootContext, contentColItem, choiceDialogItem)
{
    createWidget();
}

StandardForm::~StandardForm(){
    qDebug() << "StandardForm::~StandardForm()";
}

int StandardForm::createWidget(){
    qDebug() << "StandardForm::createWidget::";
    for(int i = 0; i < m_nFormValueList.count(); i++){
        QVariantMap indicatorMap = m_nFormValueList.at(i);
        QString indicatorType = indicatorMap[WIDGET_INDICATORTYPE].toString();
        QString iscounter = indicatorMap[STD_INDICATORTYPE_ISCOUNTER].toString();
        //创建控件
        BaseWidget *widget;
        if (indicatorType == STD_INDICATORTYPE_FIXEDTYPE && iscounter == STD_INDICATORTYPE_ISCOUNTER_NO){
            //单选类型
            widget = new BaseWidget(m_pRootContext, m_pContentColItem, STANDARD_WIDGET_CHECK,
                                    PAGE_FORM_TYPE_STANDARD, indicatorMap);
        } else if(indicatorType == STD_INDICATORTYPE_FIXEDTYPE && iscounter == STD_INDICATORTYPE_ISCOUNTER_YES){
            //输入类型
            widget = new InputWidget(m_pRootContext, m_pContentColItem, STANDARD_WIDGET_INPUT,
                                     PAGE_FORM_TYPE_STANDARD, indicatorMap);
        }else if(indicatorType == STD_INDICATORTYPE_ENUMTYPE && iscounter == STD_INDICATORTYPE_ISCOUNTER_NO){
            //枚举类型
            QList< QVariantMap > enumMapList;
            for(int j = 0; j < m_nFormEnumList.count(); j++) {
                QVariantMap enumMap = m_nFormEnumList.at(j);
                if(indicatorMap[NODE_ATTR_INDICATORID].toString() == enumMap[NODE_ATTR_INDICATORID].toString())
                    enumMapList.append(enumMap);
            }
            widget = new EnumWidget(m_pRootContext, m_pContentColItem, m_pChoiceDialogItem, STANDARD_WIDGET_ENUM,
                                     PAGE_FORM_TYPE_STANDARD, indicatorMap, enumMapList);
        } else if(indicatorType == STD_INDICATORTYPE_ENUMTYPE && iscounter == STD_INDICATORTYPE_ISCOUNTER_YES){
            //枚举加输入类型
            QList< QVariantMap > enumMapList;
            for(int j = 0; j < m_nFormEnumList.count(); j++) {
                QVariantMap enumMap = m_nFormEnumList.at(j);
                if(indicatorMap[NODE_ATTR_INDICATORID].toString()
                        == enumMap[NODE_ATTR_INDICATORID].toString())
                    enumMapList.append(enumMap);
            }
            widget = new InputEnumWidget(m_pRootContext, m_pContentColItem, m_pChoiceDialogItem,
                                         STANDARD_WIDGET_INPUT_ENUM, PAGE_FORM_TYPE_STANDARD, indicatorMap, enumMapList);
        }
        widget->showWidget();
        m_pBaseWidgetList.append(widget);
    }
    return 0;
}

void StandardForm::getDataRow(QMap<QString, QString> &dataRow, const QVariantMap &otherParam)
{

    QVariantMap dataTableMap;
    RuntimeParam *runParam = RuntimeParam::getInstance();
    dataTableMap = m_nDataPackage["tables"].toList().at(0).toMap();
    dataRow["TaskItemDataKey"] = dataTableMap["value"].toList().at(0).toMap()["TaskItemDataKey"].toString();
    dataRow["TaskObjectDataKey"] = m_nDeviceValue["TaskObjectDataKey"].toString();
    dataRow["ItemData"] = getResult();
    dataRow[SUBMIT_PDAID] = runParam->getPadId();
    dataRow[SUBMIT_MODIFIEDBY] = runParam->getUserId();
    dataRow["ExamUser"] = runParam->getUserId();
    double sumScore = 0.0;
    for(int i = 0; i < m_pBaseWidgetList.length(); i++){
        double score = m_pBaseWidgetList.at(i)->getExternalParameters()["Score"].toDouble();
        sumScore += score;
    }
    dataRow["Score"] = QString("%1").arg(sumScore, 0, 'f', 2);
    dataRow["TotalIndicatorCount"] = QString::number(m_pBaseWidgetList.length());
    dataRow["NormalIndicatorCount"] = QString::number(m_pBaseWidgetList.length());
    dataRow["SerialKey"] = otherParam["serialKey"].toString();
}
