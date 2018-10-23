#ifndef MAINTAINFORM_H
#define MAINTAINFORM_H
#include "baseform.h"
class MaintainForm:public BaseForm
{
public:
    MaintainForm(const QVariantMap &deviceValue, const QVariantMap &dataPackage,
                 QQmlContext *rootContext, QQuickItem *contentColItem, QQuickItem *choiceDialogItem);
    ~MaintainForm();
    virtual int createWidget();
    virtual void getDataRow(QMap<QString, QString> &dataRow, const QVariantMap &otherParam);
};

#endif // MAINTAINFORM_H
