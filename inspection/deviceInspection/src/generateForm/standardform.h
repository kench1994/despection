#ifndef STANDARDFORM_H
#define STANDARDFORM_H
#include "baseform.h"
class StandardForm:public BaseForm
{
public:
    StandardForm(const QVariantMap &deviceValue, const QVariantMap &dataPackage,
                 QQmlContext *rootContext, QQuickItem *contentColItem, QQuickItem *choiceDialogItem);
    ~StandardForm();
    virtual int createWidget();
    virtual void getDataRow(QMap<QString, QString> &dataRow, const QVariantMap &otherParam);
};

#endif // STANDARDFORM_H
