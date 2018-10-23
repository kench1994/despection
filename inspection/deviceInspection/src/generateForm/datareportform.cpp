#include "datareportform.h"
#include "formConfigType.h"
#include "inputwidget.h"
#include "multiinputwidget.h"
#include "enumwidget.h"
#include "timewidget.h"
#include "../common/runtimeparam.h"
#include <QDebug>
#include <QDateTime>
DataReportForm::DataReportForm(const QVariantMap &deviceValue, const QVariantMap &dataPackage,
                               QQmlContext *rootContext, QQuickItem *contentColItem, QQuickItem *choiceDialogItem)
    :BaseForm(deviceValue, dataPackage, PAGE_FORM_TYPE_DATAREPORT, rootContext, contentColItem, choiceDialogItem),
      m_pheadTitle(NULL)
{
    createWidget();
}

DataReportForm::~DataReportForm(){
    qDebug() << "DataReportForm::~DataReportForm()";
    if(m_pheadTitle != NULL){
        delete m_pheadTitle;
        m_pheadTitle = NULL;
    }
}

int DataReportForm::createWidget(){
    //qDebug() << "DataReportForm::createWidget:::   " << m_nFormValueList;
    createHeadTitle();
    for(int i = 0; i < m_nFormValueList.count(); i++){
        QVariantMap indicatorMap = m_nFormValueList.at(i);
        QString paramType = indicatorMap[WIDGET_INDICATORTYPE].toString();
        QString dataType = indicatorMap[WIDGET_DATATYPE].toString();
        //创建控件
        BaseWidget *widget = NULL;
        if (paramType == WIDGET_INDICATORTYPE_ENUMTYPE){
            QList< QVariantMap > enumMapList;
            for(int j = 0; j < m_nFormEnumList.count(); j++) {
                QVariantMap enumMap = m_nFormEnumList.at(j);
                if(indicatorMap[NODE_ATTR_INDICATORID].toString()
                        == enumMap[NODE_ATTR_INDICATORID].toString())
                    enumMapList.append(enumMap);
            }
            widget = new EnumWidget(m_pRootContext, m_pContentColItem, m_pChoiceDialogItem,
                                    DATAREPORT_WIDGET_ENUM, PAGE_FORM_TYPE_DATAREPORT, indicatorMap, enumMapList);
        } else if(paramType == WIDGET_INDICATORTYPE_INPUT &&
                  (dataType == WIDGET_DATATYPE_INTTYPE ||
                   dataType == WIDGET_DATATYPE_STRINGTYPE ||
                   dataType == WIDGET_DATATYPE_DECIMALTYPE)){
            QString displayType = indicatorMap[WIDGET_DISPLAYTYPE].toString();
            if(displayType == WIDGET_DISPLAYTYPE_SINGLETEXT)
                widget = new InputWidget(m_pRootContext, m_pContentColItem, DATAREPORT_WIDGET_INPUT,
                                         PAGE_FORM_TYPE_DATAREPORT, indicatorMap);
            else if(displayType == WIDGET_DISPLAYTYPE_MULITTEXT)
                widget = new MultiInputWidget(m_pRootContext, m_pContentColItem, DATAREPORT_WIDGET_MULTIINPUT,
                                         PAGE_FORM_TYPE_DATAREPORT, indicatorMap);
        } else if(paramType == WIDGET_INDICATORTYPE_INPUT && dataType == WIDGET_DATATYPE_DATETIME){
            widget = new TimeWidget(m_pRootContext, m_pContentColItem, DATAREPORT_WIDGET_DATETIME,
                                    PAGE_FORM_TYPE_DATAREPORT, indicatorMap);
        } else if(paramType == WIDGET_INDICATORTYPE_INPUT && dataType == WIDGET_DATATYPE_DATE){
            widget = new TimeWidget(m_pRootContext, m_pContentColItem, DATAREPORT_WIDGET_DATE,
                                    PAGE_FORM_TYPE_DATAREPORT, indicatorMap);
        } else if(paramType == WIDGET_INDICATORTYPE_INPUT && dataType == WIDGET_DATATYPE_TIME){
            widget = new TimeWidget(m_pRootContext, m_pContentColItem, DATAREPORT_WIDGET_TIME,
                                    PAGE_FORM_TYPE_DATAREPORT, indicatorMap);
        }
        if(widget == NULL){
            qDebug() << "widget == NULL  continue";
            continue;
        }
        widget->showWidget();
        m_pBaseWidgetList.append(widget);
    }
    return 0;
}

void DataReportForm::createHeadTitle(){
    QQuickView *view = RuntimeParam::getInstance()->getViewer();
    QQmlEngine *engine = view->engine();
    QQmlComponent component(engine, QUrl("qrc:/qml/Common/HeadTitle.qml"), QQmlComponent::PreferSynchronous);
    if(component.isError())
        qDebug() << "component error is  " << component.errors();
    QObject *obj = component.create(m_pRootContext);
    m_pheadTitle = qobject_cast<QQuickItem *>(obj);
    m_pheadTitle->setParentItem(m_pContentColItem);
    QVariantMap dataTableMap = m_nDataPackage["tables"].toList().at(0).toMap();
    QString deviceName = dataTableMap["value"].toList().at(0).toMap()["EquipmentName"].toString();
    if(deviceName.isEmpty())
        deviceName = dataTableMap["value"].toList().at(0).toMap()["ItemName"].toString();
    m_pheadTitle->setProperty("name", deviceName);
}
