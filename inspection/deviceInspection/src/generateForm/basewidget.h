/*!
* \file basewidget.h
* \brief 动态表单的控件
* 动态表单的控件的基类
*
*\author 桓乐乐 huanlele@syberos.com
*\version 1.0
*\date 2015/9/7
*/
#ifndef BASEWIDGET_H
#define BASEWIDGET_H
#include <QObject>
#include <QList>
#include <QQuickItem>
#include <QQmlContext>
#include <QVariantMap>
#include <QDomElement>
#include "formConfigType.h"
/*!
  \class BaseWidget
  \inModule 动态表单的控件
  \brief 动态表单的控件的基类
  \ingroup BaseWidget
  BaseWidget 动态表单的控件的基类
*/
class BaseWidget:public QObject
{
    Q_OBJECT
public:
    BaseWidget(QQmlContext *rootContext, QQuickItem *contentColItem,
               baseWidgetType widgetType, const QString &formType, QVariantMap formValue);
    virtual ~BaseWidget();
    /**
     * @brief showWidget:显示控件
     * @return:0,成功
     */
    virtual int showWidget();
    /**
     * @brief getResult:获取控件需要提交的结果
     * @return：控件需要提交的结果
     */
    virtual QString getResult();
    /**
     * @brief checkInputLegality：验证控件提交值的合法性
     * @param runStatus:设备运行状态
     * @param errorMap：出参，错误参数
     * @return:错误类型值
     */
    virtual int checkInputLegality(const int &runStatus, QVariantMap &errorMap);

    /**
     * @brief createQmlWidget:创建控件
     * @param qmlPath:控件的qml路径
     * @return：控件指针
     */
    QQuickItem *createQmlWidget(const QString &qmlPath);
    /**
     * @brief getWidgetType:获取控件类型
     * @return ：控件类型
     */
    baseWidgetType getWidgetType();
    /**
     * @brief setWigetType:设置控件类型
     * @param type：控件类型
     */
    void setWigetType(baseWidgetType type);
    /**
     * @brief getFormType:获取控件所属表单类型
     * @return ：表单类型
     */
    QString getFormType();
    /**
     * @brief setExternalParameters:设置该控件对外提供的参数
     * @param key：参数的属性字段
     * @param value：参数的属性值
     */
    void setExternalParameters(const QString &key, const QVariant &value);
    /**
     * @brief getExternalParameters:获取该控件对外提供的参数
     * @return : 对外提供的参数
     */
    QVariantMap getExternalParameters();
    /**
     * @brief getChecked:控件是否被选中
     * @return:选中状态
     */
    bool getChecked();

public slots:
    /**
     * @brief popTipItemClicked:下拉列表控件点击弹出的浮层的点击事件
     * @param index:所选值的索引
     */
    virtual void popTipItemClicked(int index);
    /**
     * @brief showChoiceDialogItem：显示下拉列表框
     * @param title:下拉列表框标题
     */
    virtual void showChoiceDialogItem(QString title);

    /**
     * @brief dateTimeSelected:时间选择框函数
     * @param dateTime:选择的时间
     */
    virtual void dateTimeSelected(QString dateTime);

    /**
     * @brief setChecked:设置控件选中状态
     * @param checked：状态
     */
    virtual void setChecked(bool checked);
protected:
    QQuickItem *m_pItem; ///< 控件指针
    QQuickItem *m_pContentColItem;  ///< 控件所属列表的指针
    QQmlContext *m_pRootContext;    ///< 控件所属页面的根指针
    QVariantMap m_nFormValueMap;    ///< 控件的属性字段
private:
    baseWidgetType m_nWidgetType;   ///< 控件的类型
    QString m_nbaseFormType;        ///<控件所属表单类型
    QVariantMap m_nExternalParam;   ///<对外使用参数，表单统计使用，比如score，isnormal等
    bool m_nChecked;                ///<控件是否选中,默认选中，只有标准化检查修改次值
};

#endif // BASEWIDGET_H
