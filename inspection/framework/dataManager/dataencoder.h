/*!
* \file dataencoder.h
* \brief 数据的编码类头文件
*
*此类提供了对于网络数据包的编码
*
*\author 陈冬 chendong@syberos.com
*\version 1.0
*\date 2015/9/6
*/
#ifndef DATAENCODER_H
#define DATAENCODER_H

#include <QObject>
#include <QMap>
#include <QDomDocument>
#include <QDomElement>
#include <QStringList>
#include <QList>
#include <QString>
#include "../common/utils.h"
/*!
  \class DataEncoder
  \inModule 数据处理模块
  \brief 负责对数据流进行XML协议化的模块
  \ingroup dataManager
  DataEncoder类为上层提供了XML协议化的接口
*/
class DataEncoder : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief 构造函数
     *
     * @param parent : 父对象
     */
    explicit DataEncoder(QObject *parent = 0);

    bool parsePackage2xmlStr(const DataPackage &inDataPackage, QString &outXmlStr);
   /**
     * @brief begin:开始格式化数据
     */
    void begin();

   /**
     * @brief setHead:设置Head头
     * @param key    : const QString& Key节点
     * @param type    : const QString& type节点
     * @param objects    : const QString&  objects节点
     * @return  bool, true 成功, false 失败
     */
    bool setHead(const QString& key, const QString& type, const QString& objects);

   /**
     * @brief setPara:设置Para节点
     * @param values    :  const QMap<QString, QString>& Para参数的Map对象
     * @return  bool, true 成功, false 失败
     */
    bool setPara(const QMap<QString, QString>& values);

   /**
     * @brief setSummary:设置Summary回复
     * @param value    :  Summary& 回复信息
     * @return  bool, true 成功, false 失败
     */
    bool setSummary(const Summary& value);

   /**
     * @brief setPagerTotalCount:设置Data中Pager的总数
     * @param value    :  QString& Pager的总数
     * @return  bool, true 成功, false 失败
     */
    bool setPagerTotalCount(const QString& value);

   /**
     * @brief addDataTable:增加一个Table
     * @param name    :  QString& name属性
     * @param key    : QString&  key属性
     * @param value    : QList<DataRow>&  Table中的数据
     * @return  bool, true 成功, false 失败
     */
    bool addDataTable(const QString& name, const QString& key, const QList<DataRow> value);

   /**
     * @brief addAttachment:增加一个附件
     * @param attachment    :  Attachment& 附件信息
     * @return  bool, true 成功, false 失败
     */
    bool addAttachment(const Attachment& attachment);

   /**
     * @brief end:解析Head
     * @param output    :  格式化后的XML字符串
     */
    void end(QString& output);
    
signals:
    
public slots:
private:
    QDomDocument m_nDocument;  ///< XML文档生成类
    QDomElement m_nRoot;  ///< Root节点
    QDomElement m_nHead;  ///< Head节点
    QDomElement m_nPara;  ///< Para节点
    QDomElement m_nData;  ///< Data节点
    QDomElement m_nAttachments; ///< 附件节点
};

#endif // DATAENCODER_H
