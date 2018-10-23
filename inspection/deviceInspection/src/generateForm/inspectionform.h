#ifndef INSPECTIONFORM_H
#define INSPECTIONFORM_H
#include "baseform.h"
class InspectionForm:public BaseForm
{
public:
    InspectionForm(const QVariantMap &deviceValue, const QVariantMap &dataPackage,
                   QQmlContext *rootContext, QQuickItem *contentColItem, QQuickItem *choiceDialogItem);
     ~InspectionForm();
    virtual int createWidget();
    virtual void getDataRow(QMap<QString, QString> &dataRow, const QVariantMap &otherParam);
};

#endif // INSPECTIONFORM_H
