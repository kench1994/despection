#ifndef DAILYTASKFORM_H
#define DAILYTASKFORM_H
#include "baseform.h"
class DailyTaskForm:public BaseForm
{
public:
    DailyTaskForm(const QVariantMap &deviceValue, const QVariantMap &dataPackage,
                   QQmlContext *rootContext, QQuickItem *contentColItem, QQuickItem *choiceDialogItem);
     ~DailyTaskForm();
    virtual int createWidget();
    virtual void getDataRow(QMap<QString, QString> &dataRow, const QVariantMap &otherParam);
};

#endif // DAILYTASKFORM_H
