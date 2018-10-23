#include "dataencoder.h"
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QMimeDatabase>
#include "../common/systemconfig.h"

DataEncoder::DataEncoder(QObject *parent) :
    QObject(parent)
{
}

bool DataEncoder::parsePackage2xmlStr(const DataPackage &package, QString &outXmlStr){
    int i = 0;
        begin();
        /**
         * @todo : fetch SN from device & fill it in header
         */
        qDebug() << "1111111parsePackage2xmlStr  head key " << package.head.key << "  type " << package.head.type << "  objects " << package.head.objects;
        if(!setHead(package.head.key, package.head.type, package.head.objects)){
            qDebug() << "setHead  error";
            return false;
        }
        if(!setPara(package.para)) {
            qDebug() << "setPara  error";
            return false;
        }
        if(!package.summary.description.isEmpty()){
            if(!setSummary(package.summary)){
                qDebug() << "setSummary  error";
                return false;
            }
        }
        for (i = 0; i < package.tables.size(); ++i) {
            DataTable table = package.tables.at(i);
            if(!addDataTable(table.name, table.key, table.value)){
                qDebug() << "addDataTable  error";
                return false;
            }
        }
        for (i = 0; i < package.attachments.size(); ++i) {
            Attachment attachment = package.attachments.at(i);
            if(!addAttachment(attachment)){
                qDebug() << "addAttachment  error";
                return false;
            }
        }
        end(outXmlStr);
        //qDebug() << "request:" << outXmlStr;
        return true;
}

void DataEncoder::begin()
{
    m_nDocument.clear();
    m_nRoot = m_nDocument.createElement(DEXML_TAG_ROOT);
    m_nDocument.appendChild(m_nRoot);
    m_nHead = m_nDocument.createElement(DEXML_TAG_HEAD);
    m_nRoot.appendChild(m_nHead);
    m_nPara = m_nDocument.createElement(DEXML_TAG_PARA);
    m_nRoot.appendChild(m_nPara);
    m_nData = m_nDocument.createElement(DEXML_TAG_DATA);
    m_nRoot.appendChild(m_nData);
    m_nAttachments = m_nDocument.createElement(DEXML_TAG_ATTACHMENTS);
    m_nRoot.appendChild(m_nAttachments);
}

bool DataEncoder::setHead(const QString& key, const QString& type, const QString& objects)
{
    QDomAttr attr;
    qDebug() << " key " << key << "  type " << type << "  objects " << objects;
    if(key.isEmpty() || type.isEmpty() || objects.isEmpty()){
        qDebug() << "  key  type  object empty";
        return false;
    }
    attr = m_nDocument.createAttribute(DEXML_ATT_HEAD_KEY);
    attr.setValue(key);
    m_nHead.setAttributeNode(attr);
    attr = m_nDocument.createAttribute(DEXML_ATT_HEAD_TYPE);
    attr.setValue(type);
    m_nHead.setAttributeNode(attr);
    attr = m_nDocument.createAttribute(DEXML_ATT_HEAD_OBJECTS);
    attr.setValue(objects);
    m_nHead.setAttributeNode(attr);
    QString xmlVersion = SystemConfig::getInstance()->getXmlVersion();
    QString protocolVersion = SystemConfig::getInstance()->getProtocolVersion();
    if(xmlVersion != "" && protocolVersion != ""){
        attr = m_nDocument.createAttribute(DEXML_ATT_HEAD_DEX_VERSION);
        attr.setValue(xmlVersion);
        m_nHead.setAttributeNode(attr);
        attr = m_nDocument.createAttribute(DEXML_ATT_HEAD_VERSION);
        attr.setValue(protocolVersion);
        m_nHead.setAttributeNode(attr);
    }
    attr = m_nDocument.createAttribute(DEXML_ATT_HEAD_DEXTIME);
    QDateTime date = QDateTime::currentDateTime();
    attr.setValue(date.toString(SERVER_DATE_FORMAT));
    m_nHead.setAttributeNode(attr);

    return true;
}

bool DataEncoder::setPara(const QMap<QString, QString>& values)
{
    QMap<QString, QString>::const_iterator i = values.constBegin();
    while (i != values.constEnd()) {
        QDomAttr attr;
        attr = m_nDocument.createAttribute(i.key());
        attr.setValue(i.value());
        m_nPara.setAttributeNode(attr);
        ++i;
    }
    return true;
}

bool DataEncoder::setPagerTotalCount(const QString& value)
{
    if(value.isEmpty()){
        return false;
    }
    QDomAttr attr;
    attr = m_nDocument.createAttribute(DEXML_ATT_DATA_SOURCE_PAGE_TOTAL_COUNT);
    attr.setValue(value);
    m_nData.setAttributeNode(attr);
    return true;
}

bool DataEncoder::setSummary(const Summary& value)
{
    QString tmp;
    /// Create Summary Tag
    QDomElement tagSummary = m_nDocument.createElement(DEXML_TAG_SUMMARY);
    m_nRoot.appendChild(tagSummary);

    /// Fill Code in
    QDomAttr attr;
    attr = m_nDocument.createAttribute(DEXML_ATT_SUMMARY_CODE);
    tmp = QString("%1").arg(value.code);
    attr.setValue(tmp);
    tagSummary.setAttributeNode(attr);

    /// Fill Result in
    attr = m_nDocument.createAttribute(DEXML_ATT_SUMMARY_RESULT);
    tmp = QString("%1").arg(value.result);
    attr.setValue(tmp);
    tagSummary.setAttributeNode(attr);

    /// Fill Description in
    //tagSummary.setNodeValue(value.description);
    QDomText valueText = m_nDocument.createTextNode(value.description);
    tagSummary.appendChild(valueText);
    return true;
}

bool DataEncoder::addDataTable(const QString& name,
                               const QString& key,
                               const QList<DataRow> value){
    int i = 0;
    QString tmp;
    QDomAttr attr;
    /// fill in table tag
    QDomElement table = m_nDocument.createElement(DEXML_TAG_DATA_TABLE);
    if(name.isEmpty() || key.isEmpty()) {
        return false;
    }
    m_nData.appendChild(table);

    /// begin to fill in attributes
    attr = m_nDocument.createAttribute(DEXML_ATT_DATA_NAME);
    attr.setValue(name);
    table.setAttributeNode(attr);
    attr = m_nDocument.createAttribute(DEXML_ATT_DATA_KEY);
    attr.setValue(key);
    table.setAttributeNode(attr);
    /// fill in fields
    if(!value.isEmpty()){
        DataRow dataRow = value.at(0);
        if(!dataRow.isEmpty()){
            QDomElement fields = m_nDocument.createElement(DEXML_TAG_DATA_TABLE_FIELD);
            table.appendChild(fields);
            attr = m_nDocument.createAttribute(DEXML_ATT_DATA_TABLE_FIDLES_VALUE);
            tmp = "";
            QMap<QString, QString>::const_iterator i = dataRow.constBegin();
            while (i != dataRow.constEnd()) {
                if(!tmp.isEmpty()){
                    tmp += DEXML_SEPARATOR;
                }
                tmp += i.key();
                ++i;
            }
            attr.setValue(tmp);
            fields.setAttributeNode(attr);
        }
    }
    tmp = "";

    /// fill in values
    if(!value.isEmpty()){
        for (i = 0; i < value.size(); ++i) {
            DataRow dataRow = value.at(i);
            tmp = "";
            if(!dataRow.isEmpty()){
                bool first = true;
                QMap<QString, QString>::const_iterator j = dataRow.constBegin();
                while (j != dataRow.constEnd()) {
                    if(!first){
                        tmp += DEXML_SEPARATOR;
                    }else{
                        first = false;
                    }
                    tmp += j.value();
                    ++j;
                }
            }
            QDomElement values = m_nDocument.createElement(DEXML_TAG_DATA_TABLE_VALUE);
            table.appendChild(values);
            attr = m_nDocument.createAttribute(DEXML_ATT_DATA_TABLE_VALUE_VALUE);
            attr.setValue(tmp);
            values.setAttributeNode(attr);
        }
    }
    return true;
}

bool DataEncoder::addAttachment(const Attachment& attachment)
{
    QMimeDatabase mimeDB;
    QString tmp;
    QDomAttr attr;
    /// fill in table tag
    QDomElement tagAttachment = m_nDocument.createElement(DEXML_TAG_ATTACHMENTS_ATTACHMENT);
    if(attachment.name.isEmpty() || attachment.path.isEmpty()) {
        return false;
    }
    QFileInfo fileInfo(attachment.path);
    if(!fileInfo.exists() && fileInfo.size() <= 0){
        qDebug() << "Add Attachment Fail! " << " Couldn't' found : " << attachment.path;
        return false;
    }
    QFile file(attachment.path);
    if (!file.open(QIODevice::ReadOnly)){
        qDebug() << "Add Attachment Fail! " << " Couldn't' found : " << attachment.path;
        return false;
    }

    m_nAttachments.appendChild(tagAttachment);

    /// begin to fill in attributes
    attr = m_nDocument.createAttribute(DEXML_ATT_ATTACHMENT_NAME);
    attr.setValue(attachment.name);
    tagAttachment.setAttributeNode(attr);
    attr = m_nDocument.createAttribute(DEXML_ATT_ATTACHMENT_MIME);
    QMimeType mime = mimeDB.mimeTypeForFile(fileInfo);
    attr.setValue(mime.name());
    tagAttachment.setAttributeNode(attr);
    attr = m_nDocument.createAttribute(DEXML_ATT_ATTACHMENT_LENGTH);
    tmp = QString("%1").arg(fileInfo.size());
    attr.setValue(tmp);
    tagAttachment.setAttributeNode(attr);
    attr = m_nDocument.createAttribute(DEXML_ATT_ATTACHMENT_SERIAL);
    attr.setValue(attachment.serialKey);
    tagAttachment.setAttributeNode(attr);
    attr = m_nDocument.createAttribute(DEXML_ATT_ATTACHMENT_TITLE);
    if(attachment.title.isEmpty()){
        attr.setValue(attachment.name);
    }else{
        attr.setValue(attachment.title);
    }
    tagAttachment.setAttributeNode(attr);
    attr = m_nDocument.createAttribute(DEXML_ATT_ATTACHMENT_DESCRIPT);
    attr.setValue(attachment.description);
    tagAttachment.setAttributeNode(attr);
    /// Fill in data
    /// Convert original binary to text
    QByteArray data = file.readAll();
    QByteArray text = data.toBase64();
    tmp = text;
    /// Fill text data in
    //tagAttachment.setNodeValue(tmp);
    QDomText valueText = m_nDocument.createTextNode(tmp);
    tagAttachment.appendChild(valueText);
    return true;
}

void DataEncoder::end(QString& output)
{
    QString tmp = m_nDocument.toString();
    output = tmp;
}
