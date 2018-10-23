#include "inputenumwidget.h"

InputEnumWidget::InputEnumWidget(QQmlContext *rootContext, QQuickItem *contentColItem,
                                 QQuickItem *choiceDialogItem, baseWidgetType widgetType, const QString &formType,
                                 QVariantMap formValue, QList < QVariantMap > formEnumMapList):
    BaseWidget(rootContext, contentColItem, widgetType, formType, formValue),
    InputWidget(rootContext, contentColItem, widgetType, formType, formValue),
    EnumWidget(rootContext, contentColItem, choiceDialogItem, widgetType, formType, formValue, formEnumMapList)
{
    qDebug() << "InputEnumWidget::InputEnumWidget constructor";
}

InputEnumWidget::~InputEnumWidget(){
    qDebug() << "InputEnumWidget::~InputEnumWidget()";
}

int InputEnumWidget::showWidget(){
    qDebug() << "InputEnumWidget::showWidget()";
    InputWidget::showWidget();
    EnumWidget::showWidget();
    return 0;
}

QString InputEnumWidget::getResult(){
    if(m_nFormEnumMapList.length() == 0 || getSelectedEnumindex() >= m_nFormEnumMapList.length()){
        qDebug() << " InputEnumWidget retrun   because m_nFormEnumMapList.length() is " << m_nFormEnumMapList.length()
                 << "  selected index is " << getSelectedEnumindex();
        return "";
    }
    QString elementText = BaseWidget::getResult();
    if(elementText.isEmpty())
        return "";
    QDomDocument document;
    document.setContent(elementText, true);
    QDomElement element = document.documentElement();
    QString inputValue = m_pItem->property("inputText").toString();
    if(getFormType() == PAGE_FORM_TYPE_STANDARD){
        QString counter = inputValue;
        if(counter.isEmpty())
            counter = "1";
        QVariantMap selectedEnumMap = m_nFormEnumMapList.at(getSelectedEnumindex());
        element.setAttribute(SUBMIT_NODE_ATTR_VALUE, selectedEnumMap[ENUM_ENUMVALUE].toString());
        element.setAttribute(SUBMIT_NODE_ATTR_ENUMID, selectedEnumMap[ENUM_ENUMID].toString());
        QString enumScore = selectedEnumMap[ENUM_SCORE].toString();
        double score = counter.toInt() * enumScore.toDouble();
        element.setAttribute(SUBMIT_NODE_ATTR_COUNTER, counter);
        element.setAttribute(SUBMIT_NODE_ATTR_SCORE, QString("%1").arg(score, 0, 'f', 2));
        setExternalParameters(SUBMIT_NODE_ATTR_SCORE, QString("%1").arg(score, 0, 'f', 2));
    }
    return document.toString();
}
