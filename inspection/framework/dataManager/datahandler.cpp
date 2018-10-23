#include "datahandler.h"
#include <QDebug>
#include "../config/sdisruntimecontext.h"
DataHandler::DataHandler(QObject *parent) :
    QObject(parent)
{
}

bool DataHandler::encodeData(const DataPackage& package, QByteArray& output)
{
    DEXCharset charset = SDISRuntimeContext::getInstance()->getProtocolCharset();
    DEXDataFormat format = SDISRuntimeContext::getInstance()->getProtocolFormat();
    QString content;
    int i = 0;
    m_nEncoder.begin();
    //fetch SN from device & fill it in header
    if(!m_nEncoder.setHead(SDISRuntimeContext::getInstance()->getDeviceID(), package.head.type, package.head.objects)){
        return false;
    }
    if(!m_nEncoder.setPara(package.para)) {
        return false;
    }
    if(!package.summary.description.isEmpty()){
        if(!m_nEncoder.setSummary(package.summary)){
            return false;
        }
    }
    for (i = 0; i < package.tables.size(); ++i) {
        DataTable table = package.tables.at(i);
        if(!m_nEncoder.addDataTable(table.name, table.key, table.value)){
            return false;
        }
    }
    for (i = 0; i < package.attachments.size(); ++i) {
        Attachment attachment = package.attachments.at(i);
        if(!m_nEncoder.addAttachment(attachment)){
            return false;
        }
    }
    m_nEncoder.end(content);
    qDebug() << "request:" << content;
    if(content.isEmpty()){
        return false;
    }
    QByteArray data;
    switch (charset) {
    case ASCII:
        data = content.toLocal8Bit();
        break;
    case UTF8:
        data = content.toUtf8();
        break;
    default:
        data = content.toUtf8();
        break;
    }
    if(!m_nCompress.gzipCompress(data, output)) {
        return false;
    }
    if(output.isEmpty()){
        return false;
    }
    return true;
}

bool DataHandler::decodeData(const QByteArray& data, DataPackage& output, DEXDataFormat format, DEXCharset charset)
{
    QByteArray content;
    if(data.isEmpty()) {
        return false;
    }
    if(!m_nCompress.gzipDecompress(data, content)){
        return false;
    }
    if(content.isEmpty()){
        return false;
    }
    QString tmp;
    switch (charset) {
    case ASCII:
        tmp = QString::fromLocal8Bit(content);
        break;
    case UTF8:
        tmp = QString::fromUtf8(content);
        break;
    case UTF16:
        tmp = QString::fromUtf16((ushort*)content.data());
        break;
    default:
        tmp = QString::fromUtf8(content);
        break;
    }
    //QString tmp = QString::fromUtf8(content);
    qDebug() <<"DataHandler::decodeData       00000000000   "<< tmp;
    if(!m_nDecoder.parsePackage(tmp, output)){
        return false;
    }
    return true;
}

void DataHandler::setDeviceID(const QString& value)
{
    m_nDevideID = value;
}
