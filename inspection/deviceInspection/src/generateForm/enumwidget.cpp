#include "enumwidget.h"
#include <QQuickItem>
EnumWidget::EnumWidget(QQmlContext *rootContext, QQuickItem *contentColItem, QQuickItem *choiceDialogItem,
                       baseWidgetType widgetType, const QString &formType, QVariantMap formValue,
                       QList < QVariantMap > formEnumMapList):
    BaseWidget(rootContext, contentColItem, widgetType, formType, formValue),
    m_pChoiceDialogItem(choiceDialogItem),
    m_nFormEnumMapList(formEnumMapList),
    m_nSelectedEnumIndex(-1)
{

    qDebug() << "EnumWidget::EnumWidget constructor   " << this;
}

EnumWidget::~EnumWidget(){
    qDebug() << "EnumWidget::~EnumWidget()";
}

int EnumWidget::showWidget(){
    qDebug() << "EnumWidget::showWidget() enumMaplist length is " << m_nFormEnumMapList.length();
    if(m_pItem == NULL)
        BaseWidget::showWidget();
    QString defaultValue;
    if(m_nFormEnumMapList.length() > 0){
        defaultValue = m_nFormEnumMapList.at(0)[ENUM_ENUMVALUE].toString();
        setSelectedEnumIndex(0);
    }
    m_pItem->setProperty("endTitle", defaultValue);
    qDebug() << "EnumWidget::EnumWidget showWidget   " << this;
    QObject::connect(m_pItem, SIGNAL(clicked(QString)), this, SLOT(showChoiceDialogItem(QString)));
    return 0;
}

QString EnumWidget::getResult(){
    if(m_nFormEnumMapList.length() == 0 || getSelectedEnumindex() >= m_nFormEnumMapList.length()){
        qDebug() << " retrun   because m_nFormEnumMapList.length() is " << m_nFormEnumMapList.length()
                 << "  selected index is " << getSelectedEnumindex();
        return "";
    }
    QString elementText = BaseWidget::getResult();
    if(elementText.isEmpty())
        return "";
    QDomDocument document;
    document.setContent(elementText, true);
    QDomElement element = document.documentElement();
    QVariantMap selectedEnumMap = m_nFormEnumMapList.at(getSelectedEnumindex());
    element.setAttribute(SUBMIT_NODE_ATTR_VALUE, selectedEnumMap[ENUM_ENUMVALUE].toString());
    element.setAttribute(SUBMIT_NODE_ATTR_ENUMID, selectedEnumMap[ENUM_ENUMID].toString());
    element.setAttribute(SUBMIT_NODE_ATTR_SCORE, selectedEnumMap[ENUM_SCORE].toString());
    //巡检保养提交字段
    if(getFormType() == PAGE_FORM_TYPE_INSPECTION
            || getFormType() == PAGE_FORM_TYPE_DAILYTASK
            || getFormType() == PAGE_FORM_TYPE_MAINTAIN
            || getFormType() == PAGE_FORM_TYPE_DATAREPORT){
        element.setAttribute(SUBMIT_NODE_ATTR_ISNORMAL, selectedEnumMap[ENUM_ISNORMAL].toString());
        //保存对外接口参数
        setExternalParameters(SUBMIT_NODE_ATTR_ISNORMAL, selectedEnumMap[ENUM_ISNORMAL]);
    } else if(getFormType() == PAGE_FORM_TYPE_STANDARD){
        //标准化检查
        //保存对外接口参数
        setExternalParameters(SUBMIT_NODE_ATTR_SCORE, selectedEnumMap[ENUM_SCORE]);
    }
    qDebug() <<"QDomElement EnumWidget::getResult " << document.toString();
    return document.toString();
}

void EnumWidget::showChoiceDialogItem(QString title){
    QStringList showList;
    int selectedIndex = -1;
    for(int i = 0; i < m_nFormEnumMapList.size(); i++){
        showList.append(m_nFormEnumMapList.at(i)[ENUM_ENUMVALUE].toString());

    }
    selectedIndex = getSelectedEnumindex();
    m_pChoiceDialogItem->setProperty("titleText", title);
    QStringList tmpList;
    m_pChoiceDialogItem->setProperty("model", QVariant(tmpList));
    m_pChoiceDialogItem->setProperty("model", QVariant(showList));
    QMetaObject::invokeMethod(m_pChoiceDialogItem, "setSelectedItem" , Qt::DirectConnection, Q_ARG(QVariant, selectedIndex));
    m_pChoiceDialogItem->setProperty("visible", QVariant(true));
    //Disconnect everything connected to an object's signals
    QObject::disconnect(m_pChoiceDialogItem, 0, 0, 0);
    QObject::connect(m_pChoiceDialogItem, SIGNAL(delegateItemTriggered(int)), this, SLOT(popTipItemClicked(int)));
}

void EnumWidget::popTipItemClicked(int index){
    qDebug() << "QmlWidget::popTipItemClicked " << index;
    if(m_pChoiceDialogItem){
        QString selectedStr = m_nFormEnumMapList.at(index)[ENUM_ENUMVALUE].toString();
        m_pItem->setProperty("endTitle", selectedStr);
        setSelectedEnumIndex(index);
        QObject::disconnect(m_pChoiceDialogItem, SIGNAL(delegateItemTriggered(int)), this, SLOT(popTipItemClicked(int)));
    }
}
void EnumWidget::setSelectedEnumIndex(const int &index){
    m_nSelectedEnumIndex = index;
}

int EnumWidget::getSelectedEnumindex()const{
    return m_nSelectedEnumIndex;
}
