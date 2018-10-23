#include "datadecoder.h"
#include <QDomDocument>
#include <QDomElement>
#include <QDebug>
#include <QFile>
#include <QUuid>

DataDecoder::DataDecoder(QObject *parent) :
    QObject(parent)
{
}

bool DataDecoder::parsePackage(const QString& content, DataPackage& output)
{
    QDomDocument    document;
    QString         strError;
    int  errLin = 0, errCol = 0;
    if( !document.setContent(content, false, &strError, &errLin, &errCol) ) {
        qDebug()<<"parse package failed";
        return false;
    }
    if( document.isNull() ) {
        qDebug()<<"document is null !" ;
        return false;
    }
    QDomElement rootTag = document.documentElement();
    QDomElement headTag = rootTag.firstChildElement(DEXML_TAG_HEAD);
    QDomElement paraTag = rootTag.firstChildElement(DEXML_TAG_PARA);
    QDomElement dataTag = rootTag.firstChildElement(DEXML_TAG_DATA);
    QDomElement summaryTag = rootTag.firstChildElement(DEXML_TAG_SUMMARY);
    QDomElement attachmentsTag = rootTag.firstChildElement(DEXML_TAG_ATTACHMENTS);
    parseHead(headTag, output.head);
    parsePara(paraTag, output.para);
    if(!summaryTag.isNull()){
        parseSummary(summaryTag, output.summary);
    }
    parseData(dataTag, output.tables, output.pagerTotalCount);
    if(!attachmentsTag.isNull()){
        parseAttachments(attachmentsTag, output.attachments);
    }
    return true;
}

bool DataDecoder::parseHead(const QDomElement& headTag, DataHead& head)
{
    if(headTag.isNull()) {
        return false;
    }
    head.key = headTag.attribute(DEXML_ATT_HEAD_KEY);
    head.type = headTag.attribute(DEXML_ATT_HEAD_TYPE);
    head.objects = headTag.attribute(DEXML_ATT_HEAD_OBJECTS);
    head.date = headTag.attribute(DEXML_ATT_HEAD_DEXTIME);
    return true;
}

bool DataDecoder::parsePara(const QDomElement& paraTag, QMap<QString, QString>& para)
{
    if(paraTag.isNull()){
        return false;
    }
    QDomNamedNodeMap attrMap =  paraTag.attributes();
    for(int i = 0; i < attrMap.length(); i++){
        QDomNode node = attrMap.item(i);
        QDomAttr attrNode = node.toAttr();
        QString key = attrNode.name();
        QString value = attrNode.value();
        para.insert(key, value);
    }
    return true;
}

bool DataDecoder::parseSummary(const QDomElement& summaryTag, Summary& summary)
{
    summary.code = summaryTag.attribute(DEXML_ATT_SUMMARY_CODE).toInt();
    summary.result = summaryTag.attribute(DEXML_ATT_SUMMARY_RESULT).toInt();
    summary.description = summaryTag.text();
    return true;
}

bool DataDecoder::parseData(const QDomElement& data, QList<DataTable>& tables, QString& pagerTotalCount)
{
    if(data.isNull()) {
        return false;
    }
    pagerTotalCount = data.attribute(DEXML_ATT_DATA_SOURCE_PAGE_TOTAL_COUNT);
    QDomNode nodeTable = data.firstChild();
    while (!nodeTable.isNull()) {
        if (nodeTable.isElement()) {
            QDomElement table = nodeTable.toElement();
            DataTable dataTable;
            if(parseTable(table, dataTable)){
                tables.append(dataTable);
            }
        }
        nodeTable = nodeTable.nextSibling();
    }
    return true;
}

bool DataDecoder::parseTable(const QDomElement& tableTag, DataTable& table)
{
    QString tmp;
    QStringList tmpFields;

    if(tableTag.isNull()){
        return false;
    }

    // parse key & name
    table.key = tableTag.attribute(DEXML_ATT_DATA_KEY);
    table.name = tableTag.attribute(DEXML_ATT_DATA_NAME);

    // parse fields
    QDomElement fieldTag = tableTag.firstChildElement(DEXML_TAG_DATA_TABLE_FIELD);
    if(!fieldTag.isNull()){
        tmp = fieldTag.attribute(DEXML_ATT_DATA_TABLE_FIDLES_VALUE);
        tmpFields = tmp.split(DEXML_SEPARATOR);
    }

    // parse datas
    QDomNode nodeTable = tableTag.firstChild();
    while (!nodeTable.isNull()) {
        if (nodeTable.isElement()) {
            QDomElement value = nodeTable.toElement();
            if(value.tagName() == DEXML_TAG_DATA_TABLE_VALUE){
                DataRow data;
                tmp = value.attribute(DEXML_ATT_DATA_TABLE_VALUE_VALUE);
                if(!tmp.isEmpty()){
                    QStringList valList = tmp.split(DEXML_SEPARATOR);
                    for (int i = 0; i < valList.size(); ++i){
                        if(i >= tmpFields.size() || i >= valList.size()) {
                            // data wrong
                            qDebug() << "Parse Data Wrong:" << i;
                            break;
                        }
                        data.insert(tmpFields.at(i), valList.at(i));
                    }
                }
                table.value.append(data);
            }
        }
        nodeTable = nodeTable.nextSibling();
    }
    return true;
}

bool DataDecoder::parseAttachments(const QDomElement& attachmentsTag, QList<Attachment>& attachments)
{
    QDomNode nodeTable = attachmentsTag.firstChild();
    while (!nodeTable.isNull()) {
        if (nodeTable.isElement()) {
            QDomElement attachmentTag = nodeTable.toElement();
            Attachment attachment;
            if(parseAttachment(attachmentTag, attachment)){
                attachments.append(attachment);
            }
        }
        nodeTable = nodeTable.nextSibling();
    }
    return true;
}

bool DataDecoder::parseAttachment(const QDomElement& attachmentTag, Attachment& attachment)
{
    QString tmp;
    int length;

    if(attachmentTag.isNull()){
        return false;
    }

    // parse name, mime & length
    attachment.name = attachmentTag.attribute(DEXML_ATT_ATTACHMENT_NAME);
    attachment.mime = attachmentTag.attribute(DEXML_ATT_ATTACHMENT_MIME);
    attachment.serialKey = attachmentTag.attribute(DEXML_ATT_ATTACHMENT_SERIAL);
    length = attachmentTag.attribute(DEXML_ATT_ATTACHMENT_LENGTH).toUInt();
    attachment.length = length;
    attachment.title = attachmentTag.attribute(DEXML_ATT_ATTACHMENT_TITLE);
    attachment.description = attachmentTag.attribute(DEXML_ATT_ATTACHMENT_DESCRIPT);

    QString originalData = attachmentTag.text();
    QByteArray textData = originalData.toLocal8Bit();
    QByteArray binaryData = QByteArray::fromBase64(textData);

    tmp = ATTACHMENT_TMP_PATH;
    tmp += QUuid::createUuid().toString();
    QFile file(tmp);
    if (!file.open(QIODevice::ReadWrite)){
        qDebug() << "Couldn't save attachment to " << tmp;
        return false;
    }
    attachment.path = tmp;
    file.write(binaryData);
    file.flush();
    file.close();
    /**
     * @todo : Verify Image file
     */
    return true;
}

