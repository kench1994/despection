#ifndef INPUTENUMWIDGET_H
#define INPUTENUMWIDGET_H
#include "inputwidget.h"
#include "enumwidget.h"
class InputEnumWidget:public InputWidget, public EnumWidget
{
public:
    InputEnumWidget(QQmlContext *rootContext, QQuickItem *contentColItem, QQuickItem *choiceDialogItem,
                    baseWidgetType widgetType, const QString &formType,
                    QVariantMap formValue, QList < QVariantMap > formEnumMapList);
    virtual ~InputEnumWidget();
    virtual int showWidget();
    virtual QString getResult();
};

#endif // INPUTENUMWIDGET_H
