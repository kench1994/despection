#include "basewidget.h"
#include "../common/runtimeparam.h"
#include <QQuickItem>
#include <QQmlComponent>
#include <QQuickView>
#include <QDomDocument>
BaseWidget::BaseWidget(QQmlContext *rootContext, QQuickItem *contentColItem,
                       baseWidgetType widgetType, const QString &formType, QVariantMap formValue)
    :m_pItem(NULL),
      m_pContentColItem(contentColItem),
      m_pRootContext(rootContext),
      m_nFormValueMap(formValue),
      m_nWidgetType(widgetType),
      m_nbaseFormType(formType),
      m_nChecked(true)

{
    qDebug() << "BaseWidget::BaseWidget constructor";
}

BaseWidget::~BaseWidget(){
    if(m_pItem != NULL){
        delete m_pItem;
        m_pItem = NULL;
        qDebug() << "~BaseWidget delete m_pItem";
    }
    qDebug() << "BaseWidget::~BaseWidget() ";
}

int BaseWidget::showWidget(){
    qDebug() << "BaseWidget::showWidget()  ===============   ";
    baseWidgetType widgetType = this->getWidgetType();
    FormConfigType *formConfType = FormConfigType::getInstance();
    QQuickItem *item = createQmlWidget(formConfType->getWidgetPagePathMap()[widgetType]);
    item->setProperty("mainTitle", m_nFormValueMap[WIDGET_MAINTITLE]);
    //枚举类型时点击显示列表的时候传递参数
    item->setProperty("itemId", m_nFormValueMap[WIDGET_MAINTITLE]);
    return 0;
}

QQuickItem *BaseWidget::createQmlWidget(const QString &qmlPath){
    RuntimeParam *runParam = RuntimeParam::getInstance();
    QQuickView *view = runParam->getViewer();
    QQmlEngine *engine = view->engine();

    QQmlComponent component(engine, QUrl(qmlPath), QQmlComponent::PreferSynchronous);
    if(component.isError())
        qDebug() << "component error is  " << component.errors();
    QObject *obj = component.create(m_pRootContext);
    m_pItem = qobject_cast<QQuickItem *>(obj);
    m_pItem->setParentItem(m_pContentColItem);
    if(getFormType() == PAGE_FORM_TYPE_STANDARD)
        QObject::connect(m_pItem, SIGNAL(changedChecked(bool)), this, SLOT(setChecked(bool)));
    return m_pItem;
}

QString BaseWidget::getResult(){
    if(getChecked() == false)
        return "";
    QDomDocument document;
    baseWidgetType widgetType = this->getWidgetType();
    FormConfigType *formConfType = FormConfigType::getInstance();

    QStringList submitKeyList = formConfType->getWidgetSubmitKeyList(widgetType);
    if(submitKeyList.length() == 0){
        qDebug() << "submitKeyList.length() == 0";
        return "";
    }
    qDebug() << "widgettype is " << widgetType <<  "  submitKeyList is  " << submitKeyList;
    QDomElement element = document.createElement(m_nFormValueMap[submitKeyList[0] ].toString());
    for(int i = 1; i < submitKeyList.count(); i++){
        QString key = submitKeyList.at(i);
        element.setAttribute(key, m_nFormValueMap[key].toString());
    }

    if(getWidgetType() == STANDARD_WIDGET_CHECK){
        double standardScore = m_nFormValueMap[SUBMIT_STANDARDSCORE].toDouble();
        element.setAttribute(SUBMIT_NODE_ATTR_SCORE, QString("%1").arg(standardScore, 0, 'f', 2));
        setExternalParameters(SUBMIT_NODE_ATTR_SCORE, standardScore);
    }

    document.appendChild(element);
    qDebug() << "BaseWidget::getResult  text is  " << document.toString();
    return document.toString();
}

int BaseWidget::checkInputLegality(const int &runStatus, QVariantMap &errorMap){
    Q_UNUSED(runStatus)
    Q_UNUSED(errorMap)
    return 0;
}

void BaseWidget::setWigetType(baseWidgetType type){
    m_nWidgetType = type;
}

QString BaseWidget::getFormType(){
    return m_nbaseFormType;
}

baseWidgetType BaseWidget::getWidgetType(){
    return m_nWidgetType;
}

void BaseWidget::setExternalParameters(const QString &key, const QVariant &value){
    m_nExternalParam[key] = value;
}

QVariantMap BaseWidget::getExternalParameters(){
    return m_nExternalParam;
}

void BaseWidget::popTipItemClicked(int index){
    qDebug() << "BaseWidget::popTipItemClicked  " << index;
}

void BaseWidget::showChoiceDialogItem(QString title){

    qDebug() << "BaseWidget::showChoiceDialogItem   " << title;
}

void BaseWidget::dateTimeSelected(QString dateTime){
    qDebug() << "BaseWidget::dateTimeSelected   " << dateTime;
}

bool BaseWidget::getChecked(){
    return m_nChecked;
}

void BaseWidget::setChecked(bool checked){
    m_nChecked = checked;
}
