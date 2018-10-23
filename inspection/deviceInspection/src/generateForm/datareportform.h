#ifndef DATAREPORTFORM_H
#define DATAREPORTFORM_H
#include "baseform.h"
class DataReportForm:public BaseForm
{
public:
    DataReportForm(const QVariantMap &deviceValue, const QVariantMap &dataPackage,
                   QQmlContext *rootContext, QQuickItem *contentColItem, QQuickItem *choiceDialogItem);
     ~DataReportForm();
    virtual int createWidget();
//    int constructDataPackage(DataPackage &dataPkg, const QString &serialKey,
//                                    const QVariant &imgMapListQVariant, const QVariantMap &otherParam);
private:
    void createHeadTitle();
    QQuickItem *m_pheadTitle;
};

#endif // DATAREPORTFORM_H
