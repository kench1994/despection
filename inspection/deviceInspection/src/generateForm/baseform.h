#ifndef BASEFORM_H
#define BASEFORM_H
#include "basewidget.h"
#include <QList>
#include <QVariantMap>
#include <QQuickItem>
#include <QQmlContext>
#include <QDomDocument>
#include <QDomNode>
#include <QObject>
#include "../../../framework/common/utils.h"
class BaseForm:public QObject
{
    Q_OBJECT
public:
    BaseForm(const QVariantMap &deviceValue, const QVariantMap &dataPackage, const QString &pageFormType,
             QQmlContext *rootContext, QQuickItem *contentColItem, QQuickItem *choiceDialogItem);
    ~BaseForm();
    /**
     * @brief createWidget:创建控件函数
     * @return:0,成功，否则失败
     */
    virtual int createWidget() = 0;
    /**
     * @brief getResult:获取控件需要提交的结果
     * @return：控件需要提交的结果
     */
    virtual QString getResult();

    /**
     * @brief getDataRow : 获取该表单的dataRow
     * @param otherParam : 构造dataRow时需要的其他参数
     * @param map : dataRow
     */
    virtual void getDataRow(QMap<QString, QString> &dataRow, const QVariantMap &otherParam);
    /**
     * @brief parseXml：解析xml
     * @return：0，成功；否则失败
     */
    int parseXml();
    /**
     * @brief checkInputLegality:对控件提交值的合法性验证
     * @param runStatus：设备运行状态
     * @return ：0，合法；否则失败
     */
    int checkInputLegality(const int &runStatus = 1);
    /**
     * @brief getPageFormType:获取表单类型
     * @return ：返回表单类型
     */
    QString getPageFormType()const;
signals:
    /**
     * @brief errorOccurredWhenCheckLegality: 对控件进行验证发现错误的信号
     * @param label：控件主标题
     * @param errorType：错误类型
     * @param dataType：数据类型
     * @param precision：数据精度
     */
    void errorOccurredWhenCheckLegality(const QString &label, const int &errorType,
                                         const QString &dataType, const QString &precision);
protected:
    QQuickItem *m_pChoiceDialogItem; ///< 弹出对话框的指针
    QQuickItem *m_pContentColItem; ///< 控件填充列的指针
    QQmlContext *m_pRootContext; ///< 根context的指针
    QList < BaseWidget * > m_pBaseWidgetList; ///< 控件列表
    QVariantMap m_nDeviceValue; ///< 设备的一些参数
    QVariantMap m_nDataPackage; ///< 巡检或保养项详情

    QList <QVariantMap> m_nFormValueList;  ///< 对m_nDataPackage解析后生成创建表单的数据
    QList <QVariantMap> m_nFormEnumList; ///< 对m_nDataPackage解析后生成创建表单的枚举数据
private:
    QString m_nPageFormType;        ///< 页面类型

};

#endif // BASEFORM_H
