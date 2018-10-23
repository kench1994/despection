#include "timewidget.h"
#include "../common/runtimeparam.h"
#include <QQuickItem>
#include <QDateTime>
QQuickItem * TimeWidget::m_pDateTimeItem = NULL;
TimeWidget::TimeWidget(QQmlContext *rootContext, QQuickItem *contentColItem,
                       baseWidgetType widgetType, const QString &formType, QVariantMap formValue):
    BaseWidget(rootContext, contentColItem, widgetType, formType, formValue)
{
    qDebug() << "TimeWidget::TimeWidget constructor   " << widgetType;
    switch (widgetType) {
    case INSPECTION_WIDGET_DATETIME:
    case MAINTAIN_WIDGET_DATETIME:
    case DATAREPORT_WIDGET_DATETIME:
        m_nDateType = "dateTime";
        break;
    case INSPECTION_WIDGET_DATE:
    case MAINTAIN_WIDGET_DATE:
    case DATAREPORT_WIDGET_DATE:
        m_nDateType = "date";
        break;
    case INSPECTION_WIDGET_TIME:
    case MAINTAIN_WIDGET_TIME:
    case DATAREPORT_WIDGET_TIME:
        m_nDateType = "time";
        break;
    default:
        break;
    }
    qDebug() << "m_nFormValueMap[SUBMIT_NODE_ATTR_DEFAULTVALUE]  " << m_nFormValueMap[SUBMIT_NODE_ATTR_DEFAULTVALUE];
    if(m_nFormValueMap[SUBMIT_NODE_ATTR_DEFAULTVALUE] == "now"){
        if(m_nDateType == "date"){
            m_nDateTime = QDate::currentDate().toString("yyyy-MM-dd");
        } else if(m_nDateType == "time")
            m_nDateTime = QTime::currentTime().toString("hh:mm:ss");
        else
            m_nDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    } else if(m_nFormValueMap[SUBMIT_NODE_ATTR_DEFAULTVALUE] == ""){
        m_nDateTime = "";
    } else
        m_nDateTime = m_nFormValueMap[SUBMIT_NODE_ATTR_DEFAULTVALUE].toString();
}

TimeWidget::~TimeWidget(){
    qDebug() << "TimeWidget::~TimeWidget()";
    if(m_pDateTimeItem != NULL){
        m_pDateTimeItem = NULL;
    }
}

int TimeWidget::showWidget(){
    if(m_pItem == NULL)
        BaseWidget::showWidget();
    m_pItem->setProperty("endTitle", m_nDateTime);
    qDebug() << "EnumWidget::EnumWidget showWidget   " << this;
    QObject::connect(m_pItem, SIGNAL(clicked(QString)), this, SLOT(showChoiceDialogItem(QString)));
    return 0;
}

QString TimeWidget::getResult(){
    QString elementText = BaseWidget::getResult();
    if(elementText.isEmpty())
        return "";
    QDomDocument document;
    document.setContent(elementText, true);
    QDomElement element = document.documentElement();
    element.setAttribute(SUBMIT_NODE_ATTR_VALUE, m_nDateTime);
    element.setAttribute(SUBMIT_NODE_ATTR_ISNORMAL, "1");
    setExternalParameters(SUBMIT_NODE_ATTR_ISNORMAL, "1");
    qDebug() <<"QDomElement TimeWidget::getResult " << document.toString();
    return document.toString();
}

void TimeWidget::showChoiceDialogItem(QString title){
    Q_UNUSED(title)
    QQuickView *view = RuntimeParam::getInstance()->getViewer();
    QQmlEngine *engine = view->engine();
    qDebug() << "TimeWidget::showChoiceDialogItem  ";
    if(m_pDateTimeItem == NULL){
        QQmlComponent component(engine, QUrl("qrc:/qml/DateTime/SelectDateTime.qml"), QQmlComponent::PreferSynchronous);
        if(component.isError())
            qDebug() << "component error is  " << component.errors();
        QObject *obj = component.create(m_pRootContext);
        m_pDateTimeItem = qobject_cast<QQuickItem *>(obj);
        m_pDateTimeItem->setParent(m_pContentColItem);
    }
    QObject::disconnect(m_pDateTimeItem, 0, 0, 0);
    QObject::connect(m_pDateTimeItem, SIGNAL(selectedDateTime(QString)), this, SLOT(dateTimeSelected(QString)));
    qDebug() << "TimeWidget::m_pDateTimeItem  " << m_pDateTimeItem;
    QDateTime dateTime;
    if(m_nDateTime == ""){
        dateTime = QDateTime::currentDateTime();
    }else {
        if(m_nDateType == "dateTime")
            dateTime = QDateTime::fromString(m_nDateTime, "yyyy-MM-dd hh:mm:ss");
        else if(m_nDateType == "date")
            dateTime = QDateTime::fromString(m_nDateTime, "yyyy-MM-dd");
        else if(m_nDateType == "time")
            dateTime = QDateTime::fromString(m_nDateTime, "hh:mm:ss");
    }
    qDebug() << "showChoiceDialogItem  dateTime " << dateTime;
    m_pDateTimeItem->setProperty("type", m_nDateType);
    QMetaObject::invokeMethod(m_pDateTimeItem, "setDateTime" , Qt::DirectConnection, Q_ARG(QVariant, dateTime));
    QMetaObject::invokeMethod(m_pDateTimeItem, "show" , Qt::DirectConnection);
}

void TimeWidget::dateTimeSelected(QString dateTime){
    m_pItem->setProperty("endTitle", dateTime);
    m_nDateTime = dateTime;
    QObject::disconnect(m_pDateTimeItem, SIGNAL(selectedDateTime(QString)), this, SLOT(dateTimeSelected(QString)));
}
