/*!
* \file comparetool.h
* \brief 比较工具类，主要用于比较网路数据和本地数据
*
*\author 桓乐乐 huanlele@syberos.com
*\version 1.0
*\date 2017/2/15
*/
#ifndef COMPARETOOL_H
#define COMPARETOOL_H
#include <QMap>
#include <QStringList>
#include "../../framework/common/utils.h"
/*!
  \class CompareTool
  \inModule 比较工具类，主要用于比较网路数据和本地数据
  \brief 比较网路数据和本地数据,获取新增数据，删除数据等信息
  \ingroup common
*/
class CompareTool
{
public:
    CompareTool();
    ~CompareTool();
    /**
     * @brief compare : 根据关键字返回本地有而服务器没有的列表
     * @param localValues : 本地数据列表
     * @param netValues : 服务器数据列表
     * @param keywords : 比较字段
     * @return : 本地有而服务器没有的列表
     */
    QStringList compare(const QList<QMap<QString, QString> > &localValues, const QList<QMap<QString, QString> > &netValues,
                           const QString &keywords);
    /**
     * @brief compare : 对服务器返回任务列表数据进行处理，获取该任务列表中新增列表，更新任务列表，删除列表
     * @param data : 服务器返回任务列表数据
     * @param keyWords : 比较字段
     */
    void compare(const DataPackage &data,const QString &keyWords);
    /**
     * @brief getAddList : 获取新增列表
     * @return : 新增列表
     */
    const QStringList & getAddList();
    /**
     * @brief getUpdateList : 获取更新列表
     * @return : 更新列表
     */
    const QStringList & getUpdateList();
    /**
     * @brief getDeleteList : 获取删除列表
     * @return : 删除列表
     */
    const QStringList & getDeleteList();

    /**
     * @brief getTimeList : 获取删除列表
     * @return : 删除列表
     */
    const QStringList & getTimeList();

private:
    QStringList m_nAddList;
    QStringList m_nUpdateList;
    QStringList m_nDeleteList;
    QStringList m_nTimeList;

};

#endif // COMPARETOOL_H
