/*!
* \file enumwidget.h
* \brief 动态表单的枚举类型控件
*   动态表单的枚举类型控件
*\author 桓乐乐 huanlele@syberos.com
*\version 1.0
*\date 2015/9/7
*/
#ifndef ENUMWIDGET_H
#define ENUMWIDGET_H
#include "basewidget.h"
/*!
  \class EnumWidget
  \inModule 动态表单的枚举类型控件
  \brief 动态表单的枚举类型控件
  \ingroup EnumWidget
  EnumWidget 动态表单的枚举类型控件
*/
class EnumWidget:virtual public BaseWidget
{
public:
    EnumWidget(QQmlContext *rootContext, QQuickItem *contentColItem, QQuickItem *choiceDialogItem,
               baseWidgetType widgetType, const QString &formType, QVariantMap formValue,
               QList < QVariantMap > formEnumMapList);
    virtual ~EnumWidget();
    virtual int showWidget();
    virtual QString getResult();
    /**
     * @brief setSelectedEnumIndex:设置选中枚举值的索引
     * @param index:索引
     */
    void setSelectedEnumIndex(const int &index);

    /**
     * @brief getSelectedEnumindex:获取选中枚举值的索引
     * @return ： 索引
     */
    int getSelectedEnumindex()const;

public slots:
    /**
     * @brief popTipItemClicked:下拉列表控件点击弹出的浮层的点击事件
     * @param index:所选值的索引
     */
    virtual void popTipItemClicked(int index);

    virtual void showChoiceDialogItem(QString title);
protected:
    QQuickItem *m_pChoiceDialogItem; ///<下拉列表的指针
    QList < QVariantMap > m_nFormEnumMapList; ///<枚举值列表
private:
    int m_nSelectedEnumIndex;  ///< 选中的枚举值的索引
};

#endif // ENUMWIDGET_H
