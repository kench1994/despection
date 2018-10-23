#ifndef INPUTWIDGET_H
#define INPUTWIDGET_H
#include "basewidget.h"
class InputWidget:virtual public BaseWidget
{
public:
    InputWidget(QQmlContext *rootContext, QQuickItem *contentColItem,
                baseWidgetType widgetType, const QString &formType, QVariantMap formValue);
    virtual ~InputWidget();
    virtual int showWidget();
    virtual QString getResult();
    int checkInputLegality(const int &runStatus, QVariantMap &errorMap);
private:
    /**
     * @brief checkInputIsNormal:检测输入值是否正常
     * @return :1 ，正常，0，不正常
     */
    QString checkInputIsNormal();
};

#endif // INPUTWIDGET_H
