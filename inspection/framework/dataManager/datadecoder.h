/*!
* \file datadecoder.h
* \brief 数据的解码类头文件
*
*此类提供了对于网络数据包的解码
*
*\author 陈冬 chendong@syberos.com
*\version 1.0
*\date 2015/9/6
*/
#ifndef DATADECODER_H
#define DATADECODER_H

#include <QObject>
#include <QDomElement>
#include "../common/utils.h"

/*!
  \class DataDecoder
  \inModule 数据处理模块
  \brief 负责对数据流进行协议化解析的模块
  \ingroup dataManager
  DataDecoder类为上层提供了解析的接口
*/
class DataDecoder : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief 构造函数
     *
     * @param parent : 父对象
     */
    explicit DataDecoder(QObject *parent = 0);
   /**
     * @brief parsePackage:解析数据
     * @param content    :  QString& 解析前的XML数据
     * @param output    : DataPackage&  解析后的数据
     * @return  bool, true 成功, false 失败
     */
    bool parsePackage(const QString& content, DataPackage& output);

private:
   /**
     * @brief parseHead:解析Head
     * @param headTag    :  QDomElement& Head节点
     * @param head    : DataHead&  解析后的数据
     * @return  bool, true 成功, false 失败
     */
    bool parseHead(const QDomElement& headTag, DataHead& head);
   /**
     * @brief parsePara:解析Para参数
     * @param paraTag    :  QDomElement& Para数据节点
     * @param para    : QMap<QString, QString>&  解析后的数据
     * @return  bool, true 成功, false 失败
     */
    bool parsePara(const QDomElement& paraTag, QMap<QString, QString>& para);
   /**
     * @brief parseData:解析Data区
     * @param dataTag    :  QDomElement& Data数据节点
     * @param tables    : QList<DataTable>&  Table列表数据
     * @param pagerTotalCount    : QString&  Page总数
     * @return  bool, true 成功, false 失败
     */
    bool parseData(const QDomElement& dataTag, QList<DataTable>& tables, QString& pagerTotalCount);
   /**
     * @brief parseTable:解析Table区
     * @param tableTag    :  QDomElement& Table数据节点
     * @param table    : DataPackage&  解析后的数据
     * @return  bool, true 成功, false 失败
     */
    bool parseTable(const QDomElement& tableTag, DataTable& table);
   /**
     * @brief parseSummary:解析Summary区
     * @param summaryTag    :  QDomElement& Summary数据节点
     * @param summary    : Summary&  解析后的数据
     * @return  bool, true 成功, false 失败
     */
    bool parseSummary(const QDomElement& summaryTag, Summary& summary);
   /**
     * @brief parseAttachments:解析附件
     * @param attachmentsTag    :  QDomElement& 附件数据节点
     * @param attachments    : QList<Attachment>&  解析后的数据
     * @return  bool, true 成功, false 失败
     */
    bool parseAttachments(const QDomElement& attachmentsTag, QList<Attachment>& attachments);
   /**
     * @brief parseAttachment:解析单个附件
     * @param attachmentTag    :  QDomElement& Attachment数据节点
     * @param attachment    : Attachment&  解析后的数据
     * @return  bool, true 成功, false 失败
     */
    bool parseAttachment(const QDomElement& attachmentTag, Attachment& attachment);

signals:
    
public slots:

};

#endif // DATADECODER_H
