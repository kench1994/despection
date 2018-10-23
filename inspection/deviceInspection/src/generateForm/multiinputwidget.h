#ifndef MULTIINPUTWIDGET_H
#define MULTIINPUTWIDGET_H
#include "inputwidget.h"
class MultiInputWidget: public InputWidget
{
public:
    MultiInputWidget(QQmlContext *rootContext, QQuickItem *contentColItem,
                baseWidgetType widgetType, const QString &formType, QVariantMap formValue);
    virtual ~MultiInputWidget();
    //virtual int showWidget();
};

#endif // MULTIINPUTWIDGET_H
