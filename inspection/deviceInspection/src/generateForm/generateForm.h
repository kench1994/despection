/*!
* \file generateForm.h
* \brief 巡检和保养动态生成表单模块
*
*此类导入到qml使用，根据服务器内容动态生成qml页面，并且把qml页面填报的内容提交到服务器
*
*\author 桓乐乐 huanlele@syberos.com
*\version 1.0
*\date 2015/9/7
*/
#ifndef GENERATEFORM_H
#define GENERATEFORM_H
#include <QVariantMap>
#include <QObject>
#include <QString>
#include <QList>
#include <QMap>
#include "../../../framework/common/utils.h"
#include "../../../framework/common/convertdata.h"
#include "baseform.h"
#include "formConfigType.h"
#include "inspectionform.h"
#include "maintainform.h"
#include "standardform.h"
#include "formConfigType.h"
#include "datareportform.h"
#include "dailyTaskform.h"
/*!
  \class GenerateForm
  \inModule 巡检和保养动态生成表单模块
  \brief 导入到qml使用，根据服务器内容动态生成qml页面，并且把qml页面填报的内容提交到服务器
  \ingroup generateForm
  GenerateForm 巡检和保养动态生成表单模块
*/
class QQuickView;
class QQmlContext;
class TransmitManager;
class GenerateForm:public QObject
{
    Q_OBJECT
public:
    GenerateForm(QObject *parent = 0);
    ~GenerateForm();
    Q_PROPERTY(QString rootObjName READ getRootObjName WRITE setRootObjName)
    Q_PROPERTY(QString contentColObjName READ getContentColObjName WRITE setContentColObjName)
    Q_PROPERTY(QString choiceDialogItemObjName READ getChoiceDialogObjName WRITE setChoiceDialogObjName)

    Q_PROPERTY(QVariantMap deviceValue READ getDeviceValue WRITE setDeviceValue)
    Q_PROPERTY(QVariantMap dataPackage READ getDataPackage WRITE setDataPackage)
    Q_PROPERTY(QString formType READ getFormType WRITE setFormType)

    /**
     * @brief getResponse: 对服务器得到的数据进行判断并进行转码，然后开始生成qml控件
     */
    Q_INVOKABLE void initializeForm();

    /**
     * @brief setRootObjName: 设置qml的根控件的objectName
     * @param parentObjName: 根控件的objectName
     */
    void setRootObjName(const QString &parentObjName);

    /**
     * @brief getRootObjName: 获取qml的根控件的objectName
     * @return : 根控件的objectName
     */
    QString getRootObjName() const;

    /**
     * @brief setContentColObjName: 设置控件填充列的objectName
     * @param contentColObjName: 控件填充列的objectName
     */
    void setContentColObjName(const QString &contentColObjName);

    /**
     * @brief getContentColObjName: 获取控件填充列的objectName
     * @return : 控件填充列的objectName
     */
    QString getContentColObjName() const;

    /**
     * @brief setFormType: 设置界面类型
     * @param formType: inspection, 巡检； maintain，保养
     */
    void setFormType(const QString &formType);

    /**
     * @brief getFormType: 获取界面类型
     * @return : 界面类型
     */
    QString getFormType() const;

    /**
     * @brief setChoiceDialogObjName: 设置弹出对话框的objectName
     * @param choiceDialogObjName: 弹出对话框的objectName
     */
    void setChoiceDialogObjName(const QString &choiceDialogObjName);

    /**
     * @brief getChoiceDialogObjName: 获取弹出对话框的objectName
     * @return : 弹出对话框的objectName
     */
    QString getChoiceDialogObjName() const;

    /**
     * @brief getDeviceValue: 获取设备的一些参数(界面显示需要，但是又没包含在datapackage中)
     * @return : 设备参数的map结构体
     */
    QVariantMap getDeviceValue();

    /**
     * @brief setDeviceValue: 设置设备的一些参数(界面显示需要，但是又没包含在datapackage中)，通过qml页面进行设置
     * @param deviceValue: 设备参数信息
     */
    void setDeviceValue(QVariantMap deviceValue);

    /**
     * @brief getDataPackage: 获取从服务器获取的设备故障申报详情信息
     * @return : 设备故障申报详情信息
     */
    QVariantMap getDataPackage();

    /**
     * @brief setDataPackage: 设置从服务器获取的设备故障申报详情信息
     * @param dataPackage: 从服务器获取的设备故障申报详情信息
     */
    void setDataPackage(QVariantMap dataPackage);

    /**
     * @brief getItemData : 获取动态表单数据
     * @return : 动态表单数据(ItemData)
     */
    Q_INVOKABLE QVariantMap getItemData(const QVariantMap &otherParam);
    /**
     * @brief checkFormInputLegality : 检验表单中输入值的合法性
     * @return : 0，合法；否则失败
     */
    Q_INVOKABLE int checkFormInputLegality();

signals:
    /**
     * @brief dataError:数据解析出错，发送信号到qml
     * @param error:错误信息
     */
    void dataError(QString error);
    /**
     * @brief createComponentFinished:解析数据后创建qml控件完成
     */
    void createComponentFinished();

    /**
     * @brief checkLegalityErrorOccur:检查到不合法输入发送给qml的信号
     * @param lable:不合法字段名
     * @param errorType:不合法类型
     */
    void checkLegalityErrorOccurred(const QString &label, const int &errorType,
                                    const QString &dataType, const QString &precision);
private:
    /**
     * @brief getItemPtr:获取qml中各个控件的指针
     */
    void getItemPtr();
    /**
     * @brief parseFormXml:解析xml中的表单字段
     */
    void parseFormXml();
private:
    QString m_nFormType; ///< 界面类型, inspection,巡检;maitain 保养
    QQuickView *m_pViewer; ///< Qquickviewer指针
    QDomNodeList m_nIndicatorEnumList; ///< xml中巡检或保养项枚举值列表
    QVariantMap m_nDeviceValue; ///< 设备的一些参数
    QVariantMap m_nDataPackage; ///< 巡检或保养项详情
    QVariant otherParam;        ///< 其他参数

    QString m_nRootObjName; ///< 根控件的objectName
    QString m_nContentColObjName; ///< 控件填充列的objectName
    QString m_nChoiceDialogObjName; ///< 弹出对话框的objectName
    QString m_nSubmitBtnObjName; ///< 提交按钮的objectName

    QQuickItem *m_pChoiceDialogItem; ///< 弹出对话框的指针
    QQuickItem *m_pContentColItem; ///< 控件填充列的指针
    QQmlContext *m_pRootContext; ///< 根context的指针

    TransmitManager *m_pTransMng;  ///< 想服务器发送数据类的指针
    QList<BaseForm * > m_pBaseFormList;
    BaseForm *m_pBaseForm;
};

#endif //GENERATEFORM_H
