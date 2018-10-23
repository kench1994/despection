#ifndef TIMEWIDGET_H
#define TIMEWIDGET_H
#include "basewidget.h"
class TimeWidget:virtual public BaseWidget
{
public:
    TimeWidget(QQmlContext *rootContext, QQuickItem *contentColItem,
               baseWidgetType widgetType, const QString &formType, QVariantMap formValue);
    virtual ~TimeWidget();
    virtual int showWidget();
    virtual QString getResult();
public slots:
    virtual void showChoiceDialogItem(QString title);
    virtual void dateTimeSelected(QString dateTime);
private:
    QString m_nDateType;  ///<时间控件类型，dateTime:日期和时间;date:日期；time：时间
    QString m_nDateTime;  ///< 选中的时间
    static QQuickItem *m_pDateTimeItem;
};

#endif // TIMEWIDGET_H
