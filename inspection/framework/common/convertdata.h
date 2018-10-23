/*!
* \file convertdata.h
* \brief 数据处理模块
*
*数据处理模块
*
*\author 张子健 zhangzijian@syberos.com
*\version 1.0
*\date 2015/9/8
*/
#ifndef CONVERTDATA_H
#define CONVERTDATA_H

#include <QObject>
#include <QStringList>
#include <QVariantMap>
#include "../framework/common/utils.h"

/*!
  \class ConvertData
  \inModule 数据处理模块
  \brief 将原始数据转换成QVariantMap类型数据供qml使用
  \ingroup common
  ConvertData类为qml提供转换后的QVariantMap类型数据
*/

class ConvertData : public QObject
{
    Q_OBJECT

public:

    /**
     * @brief   getInstance:获取单例对象
     * @return  ConvertData*, 非NULL 成功, NULL 失败
     */
    static ConvertData *getInstance();

    /**
     * @brief  convert:获取转换后的QVariantMap类型数据
     * @param  package: DataPackage & 原始数据
     * @return QVariantMap, 转换后的数据
     */
    QVariantMap convert(DataPackage &package);

    /**
     * @brief  convert: 将原始数据转换成QVariantMap类型数据
     * @param  package: DataPackage & 原始数据
     * @param  map : QVariantMap & 返回的结果
     */
    void convert(const DataPackage &package, QVariantMap &map);

    QString convertForSql(const QStringList &list);

    QString convert(const QStringList &list);

signals:

public slots:

private:

    /**
     * @brief 构造函数
     *
     * @param parent : 父对象
     */
    ConvertData(QObject *parent = 0);

    static ConvertData *m_pInstance; ///<  本地单例对象
};

#endif // CONVERTDATA_H
