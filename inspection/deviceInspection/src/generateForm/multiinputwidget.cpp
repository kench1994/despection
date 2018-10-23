#include "multiinputwidget.h"
#include <QQuickItem>
#include <QDomElement>
MultiInputWidget::MultiInputWidget(QQmlContext *rootContext, QQuickItem *contentColItem,
                         baseWidgetType widgetType, const QString &formType, QVariantMap formValue):
    BaseWidget(rootContext, contentColItem, widgetType, formType, formValue),
    InputWidget(rootContext, contentColItem, widgetType, formType, formValue)
{
    qDebug() << "MultiInputWidget::MultiInputWidget constructor";
}

MultiInputWidget::~MultiInputWidget(){
    qDebug() << "MultiInputWidget::~MultiInputWidget()";
}

//int MultiInputWidget::showWidget(){
//    qDebug() << "MultiInputWidget::showWidget()";
//    if(m_pItem == NULL)
//        BaseWidget::showWidget();
//    QString formType = this->getFormType();
//    if(formType == PAGE_FORM_TYPE_STANDARD)
//        m_pItem->setProperty("unit", m_nFormValueMap[SUBMIT_NODE_ATTR_COUNTUNITNAME].toString());
//    else if(formType == PAGE_FORM_TYPE_INSPECTION || formType == PAGE_FORM_TYPE_DATAREPORT)
//        m_pItem->setProperty("inputText", m_nFormValueMap[SUBMIT_NODE_ATTR_DEFAULTVALUE].toString());
//    return 0;
//}
