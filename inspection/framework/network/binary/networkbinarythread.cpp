#include "networkbinarythread.h"

#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <QFile>
#include <QDebug>
#include "../../utils/sdisfiletool.h"
#include "../../config/sdisruntimecontext.h"
#include "../../network/tools/networkencryptionutil.h"
#include "../../network/networkheadtool.h"
#include "../../common/systemconfig.h"
#define DBG_TAG "NetworkBinaryThread : "
#define NETWORK_BINARY_TMP_EXTENSION ".tmp"

const int NETWORK_RESPONSE = -10;

NetworkBinaryThread::NetworkBinaryThread(QObject *parent) :
    NetworkThreadBase(parent)
{
    mDataHandler.setDeviceID(SDISRuntimeContext::getInstance()->getDeviceID());
}

NetworkBinaryThread::~NetworkBinaryThread(){
    qDebug() << DBG_TAG << "~NetworkBinaryThread";
    cleanup();
    if(!wait(3000)) //Wait until it actually has terminated (max. 3 sec)
    {
        terminate(); //Thread didn't exit in time, probably deadlocked, terminate it!
        wait(); //We have to wait again here!
    }
}

QString NetworkBinaryThread::getUuid()
{
    return mId;
}

void NetworkBinaryThread::setTask(BinaryTask task)
{
    mAction = task.action;
    mRemotePath = task.remotePath;
    mLocalPath = task.localPath;
    mId = task.uuid;
    mName = task.name;
    mTitle = task.title;
    mDescription = task.description;
    mSeriesKey = task.seriesKey;
    bDownload = task.download;
}

QString NetworkBinaryThread::getTmpFilePath()
{
    return mLocalPath + NETWORK_BINARY_TMP_EXTENSION;
}

bool NetworkBinaryThread::generateData(QByteArray& data) {
    DataPackage dataPackage;
    if(bDownload) {
        int size = SDISFileTool::getSize(getTmpFilePath());
        dataPackage.head.key = SDISRuntimeContext::getInstance()->getDeviceID();
        dataPackage.head.type = "0";
        dataPackage.head.objects = "DownloadFileStream";
        dataPackage.para.insert("FileUrl", mRemotePath);
        dataPackage.para.insert("StartIndex", QString::number(size));
        dataPackage.para.insert("PartStartIndex", QString::number(size));
        dataPackage.para.insert("PartLength", "0");
    }else{
        int size = SDISFileTool::getSize(mLocalPath);
        if(size <= 0) {
            return false;
        }
        QString mime = SDISFileTool::getMimeType(mLocalPath);
        qDebug() << Q_FUNC_INFO << " original mime is " << mime;
        if(mime == "audio/mp4" || mime == "audio/m4a")
            mime = "audio/x-m4a";
        dataPackage.head.key = SDISRuntimeContext::getInstance()->getDeviceID();
        dataPackage.head.type = "0";
        dataPackage.head.objects = "UploadFileStream";
        DataTable table;
        DataRow row;
        row.insert("FileName", mName);
        row.insert("Title", mTitle);
        row.insert("Descript", mDescription);
        row.insert("FileUrl", mRemotePath);
        row.insert("MIMEType", mime);
        row.insert("PartNumber", "0");
        row.insert("StartIndex", "0");
        row.insert("PartStartIndex", "0");
        row.insert("SerialKey", mSeriesKey);
        row.insert("FileLength", QString::number(size));
        row.insert("PartLength", QString::number(size));
        table.value.append(row);
        table.name = "table1";
        table.key = mName;
        dataPackage.tables.append(table);
    }
    return mDataHandler.encodeData(dataPackage, data);
}

void NetworkBinaryThread::cleanup() {
    NetworkThreadBase::cleanup();
}

QString NetworkBinaryThread::getProtocolVersion()
{
    QString xmlVersion = SystemConfig::getInstance()->getXmlVersion();
    QString protocolVersion = SystemConfig::getInstance()->getProtocolVersion();
    if(xmlVersion == "" || protocolVersion == ""){
        return "1.0";
    }else{
        return "1.6";
    }
}

int NetworkBinaryThread::sendBinary(int pos){
    int encryptType = SDISRuntimeContext::getInstance()->getDataEncryptionType();
    int fileSize = SDISFileTool::getSize(mLocalPath);
    QFile file(mLocalPath);
    if (!file.open(QIODevice::ReadOnly)){
        return NETWORK_ERROR_READ_FILE;
    }

    if(pos > 0) {
        if(!file.seek(pos)) {
            file.close();
            return NETWORK_ERROR_READ_FILE;
        }
    }

    int progressTmp = pos / (fileSize / 100);
    int bufferSize = 0;
    QString version = getProtocolVersion();
    if(version > "1.0"){
        bufferSize = NETWORK_BINARY_BLOCK_LENGTH_8K;
    }else{
        bufferSize = NETWORK_BINARY_BLOCK_LENGTH_4K;
    }
    char *buffer = (char *)malloc(bufferSize);
    while (pos < fileSize) {
        memset((void *) buffer, 0, bufferSize);
        int size = fileSize - pos;
        int read;
        int ret = -1;

        size = size > bufferSize ? bufferSize : size;
        read = file.read(buffer, size);
        if(encryptType == 1) {
            QByteArray srcData(buffer, size);
            QByteArray sentData;
            NetworkEncryptionUtil::getFinalSentData(srcData, sentData);
            ret = sendData(sentData.data(), sentData.size());
        }else{
            ret = sendData(buffer, read);
        }
        if(ret < 0) {
            file.close();
            free(buffer);
            return NETWORK_ERROR_SEND_FAIL;
        }
        int tmp;
        if(version <= "1.0"){
            ret = recvData((char*)&tmp, sizeof(tmp));
            if(ret < 0) {
                file.close();
                free(buffer);
                return NETWORK_ERROR_SEND_FAIL;
            }
        }
        pos += read;

        if(progressTmp <= 100 && pos / (fileSize / 100) >= progressTmp) {
            progressTmp = pos / (fileSize / 100);
            emit progress(mId, mAction, fileSize,
                          pos,
                          NETWORK_BINARY_STAUS_UPLOADING);
        }
    }
    file.close();
    free(buffer);
    emit progress(mId, mAction, fileSize, fileSize, NETWORK_BINARY_STAUS_UPLOADING);
    return NETWORK_NO_ERROR;
}

int NetworkBinaryThread::recvBinary(int totalLen)
{
    int encryptType = SDISRuntimeContext::getInstance()->getDataEncryptionType();
    int fileSize = SDISFileTool::getSize(getTmpFilePath());
    QFile file(getTmpFilePath());
    QString fileDirPath = SDISFileTool::getDirPath(getTmpFilePath());
    SDISFileTool::mkdir(fileDirPath);
    if(fileSize > 0) {
        if (!file.open(QIODevice::Append|QIODevice::WriteOnly)){
            return NETWORK_ERROR_STORE_ATTACHMENT;
        }
    }else{
        if (!file.open(QIODevice::WriteOnly)){
            return NETWORK_ERROR_STORE_ATTACHMENT;
        }
    }
    emit progress(mId, mAction, totalLen, fileSize, NETWORK_BINARY_STAUS_DOWNLOADING);
    int bufferSize = 0;
    QString version = getProtocolVersion();
    if(version > "1.0"){
        bufferSize = NETWORK_BINARY_BLOCK_LENGTH_8K;
    }else{
        bufferSize = NETWORK_BINARY_BLOCK_LENGTH_4K;
    }
    if(encryptType == 1){
        bufferSize += BODY_HEAD_LENGTH;
    }
    char *buffer = (char *)malloc(bufferSize);
    int progressTmp = fileSize / (totalLen / 100);
    while(fileSize < totalLen){
        memset((void *) buffer, 0, bufferSize);
        int ret;
        int size = totalLen - fileSize;
        if(encryptType == 1){
            size += BODY_HEAD_LENGTH;
        }
        int bufLen;
        size = size > bufferSize ? bufferSize : size;
        ret = recvData(buffer, size);
        if(ret < 0) {
            qDebug() << DBG_TAG << "Receive data fail";
            file.flush();
            file.close();
            free(buffer);
            sync();
            return NETWORK_ERROR_RECV_FAIL;
        }
        bufLen = ret;
        if(version <= "1.0"){
            ret = sendData((char*)&NETWORK_RESPONSE, sizeof(NETWORK_RESPONSE));
            if(ret < 0) {
                qDebug() << DBG_TAG << "Send data fail";
                file.flush();
                file.close();
                free(buffer);
                sync();
                return NETWORK_ERROR_SEND_FAIL;
            }
        }
        if(encryptType == 1){
            QByteArray src((const char *)buffer, bufLen);
            QByteArray dst;
            NetworkEncryptionUtil::getFinalReceivedData(src, dst);
            file.write(dst.constData(), dst.size());
            fileSize += dst.size();
        }else{
            file.write(buffer, bufLen);
            fileSize += bufLen;
        }
        if(progressTmp <= 100 && fileSize / (totalLen / 100) > progressTmp) {
            progressTmp = fileSize / (totalLen / 100);
            emit progress(mId, mAction, totalLen, fileSize, NETWORK_BINARY_STAUS_DOWNLOADING);
        }
    }
    emit progress(mId, mAction, totalLen, totalLen, NETWORK_BINARY_STAUS_DOWNLOADING);
    file.flush();
    file.close();
    free(buffer);
    sync();
    return NETWORK_NO_ERROR;
}

void NetworkBinaryThread::run()
{
    qDebug() << DBG_TAG << "run - Thread ID : " << QThread::currentThreadId();
    emit progress(mId, mAction, 0, 0, NETWORK_BINARY_STAUS_BEGIN);
    QByteArray data;
    int ret;
    if(!generateData(data)) {
        emit error(mId, mAction, NETWORK_ERROR_ENCODE_FAIL, "Generate package fail");
        cleanup();
        return;
    }
    QByteArray sentData;
    if(!NetworkEncryptionUtil::getFinalSentData(data, sentData)){
        emit error(mId, mAction, NETWORK_ERROR_ENCODE_FAIL, "Encrypt data fail");
        cleanup();
        return;
    }

    ProtocolHead head;
    memcpy(head.mark, NETWORK_PROTOCOL_MARK, sizeof(head.mark));
    head.charset = SDISRuntimeContext::getInstance()->getProtocolCharset();
    head.format = SDISRuntimeContext::getInstance()->getProtocolFormat();
    head.mainVersion = NETWORK_PROTOCOL_MAIN_VERSION;
    head.subVersion = NETWORK_PROTOCOL_SUB_VERSION;
    head.result = 0;
    head.type = QString(TYPE_BINARY).toInt();
    head.statusCode = 0;
    head.length = sentData.size();
    char protocolBuffer[48] = {0};
    int authType = SDISRuntimeContext::getInstance()->getAuthenticationType();
    bool encrypt = false;
    if(authType == 1){
        encrypt = true;
    }
    int len = NetworkHeadTool::generateProtocolHead(head, encrypt, false, protocolBuffer);
    QByteArray headBuf;
    headBuf.append(protocolBuffer, len);

    ret = connectToServer();
    if(ret != NETWORK_NO_ERROR) {
        emit error(mId, mAction, ret, "Generate package fail");
        cleanup();
        return;
    }

    emit progress(mId, mAction, 0, 0, NETWORK_BINARY_STAUS_REQUEST);

    ret = sendData(headBuf);
    if(ret < 0) {
        qDebug() << DBG_TAG << "Send head fail";
        emit error(mId, mAction, NETWORK_ERROR_SEND_FAIL, "Send head fail");
        cleanup();
        return;
    }
    ret = sendData(sentData);
    if(ret < 0) {
        qDebug() << DBG_TAG << "Sent data fail";
        emit error(mId, mAction, NETWORK_ERROR_SEND_FAIL, "Generate package fail");
        cleanup();
        return;
    }

    ProtocolHead protocolHead;
    memset(&protocolHead, 0, sizeof(ProtocolHead));
    int headLen = 4;

    if(authType == 0){
        headLen = 16;
    }else if(authType == 1){
        headLen = 48;
    }
    memset((void *)protocolBuffer, 0, sizeof(protocolBuffer));
    ret = recvData(protocolBuffer, headLen);
    NetworkHeadTool::decodeProtocolHead(protocolHead, protocolBuffer);
    if(strncmp(protocolHead.mark, NETWORK_PROTOCOL_MARK, strlen(NETWORK_PROTOCOL_MARK)) != 0){
        qDebug() << DBG_TAG << "Protocol head error";
        emit error(mId, mAction, NETWORK_ERROR_DECODE_FAIL, "Protocol head error");
        cleanup();
        return;
    }
    len = protocolHead.length;

    if(ret < 0) {
        qDebug() << DBG_TAG << "Fetch head fail";
        qDebug() << DBG_TAG << errno << " : " << strerror(errno);
        emit error(mId, mAction, NETWORK_ERROR_RECV_FAIL, "Fetch head fail");
        cleanup();
        return;
    }

    QByteArray dataRecv;
    ret = recvData(dataRecv, len);
    if(ret < len){
        qDebug() << DBG_TAG << "Protocol DataPackage error";
        emit error(mId, mAction, NETWORK_ERROR_RECV_FAIL, "recv data fail");
        cleanup();
        return;
    }
    /**
     * Decrypt data if it's necessary
     */
    QByteArray receivedData;
    if(!NetworkEncryptionUtil::getFinalReceivedData(dataRecv, receivedData)){
        qDebug() << "Decrypt data fail!";
        emit error(mId, mAction, NETWORK_ERROR_DECODE_FAIL, "Decrypt error");
        cleanup();
        return;
    }

    DataPackage packageRecv;
    if(!mDataHandler.decodeData(receivedData, packageRecv, protocolHead.format, protocolHead.charset)) {
        qDebug() << "Decode data fail!";
        emit error(mId, mAction, NETWORK_ERROR_DECODE_FAIL, "Decode error");
        cleanup();
        return;
    }

    /**
     * Handle token error
     */
    if (packageRecv.summary.result != NETWORK_SERVER_RESULT_HANDLING &&
            (packageRecv.summary.code == NETWORK_SERVER_TOKEN_INVALID ||
             packageRecv.summary.code == NETWORK_SERVER_TOKEN_TIMEOUT)) {
        emit error(mId, mAction, NETWORK_ERROR_TOKEN_INVALID_OR_TIMEOUT, packageRecv.summary.description);
        cleanup();
        return;
    }

    if(bDownload){
        if(packageRecv.summary.result != NETWORK_SERVER_RESULT_HANDLING){
            emit error(mId, mAction, NETWORK_ERROR_NO_ATTACHMENT, packageRecv.summary.description);
            cleanup();
            return;
        }
        if(packageRecv.tables.size() <= 0 || packageRecv.tables.at(0).value.size() <= 0) {
            emit error(mId, mAction, NETWORK_ERROR_NO_ATTACHMENT, "Table is empty");
            cleanup();
            return;
        }
    }else{
        if(packageRecv.summary.result != NETWORK_SERVER_RESULT_HANDLING || packageRecv.summary.code != 30){
            emit error(mId, mAction, NETWORK_ERROR_PROTOCOL, packageRecv.summary.description);
            cleanup();
            return;
        }
        if(packageRecv.tables.size() <= 0 || packageRecv.tables.at(0).value.size() <= 0) {
            emit error(mId, mAction, NETWORK_ERROR_NO_ATTACHMENT, "Table is empty");
            cleanup();
            return;
        }
    }

    emit progress(mId, mAction, 0, 0, NETWORK_BINARY_STAUS_INFORMATION);

    if(bDownload) {
        ret = recvBinary(packageRecv.tables.at(0).value.at(0).value("PartLength", "0").toInt());
        if(ret != NETWORK_NO_ERROR){
            emit error(mId, mAction, ret, "recvBinary error");
            cleanup();
            return;
        }
    }else{
       ret =  sendBinary(packageRecv.tables.at(0).value.at(0).value("StartIndex", "0").toInt());
        if(ret != NETWORK_NO_ERROR){
            emit error(mId, mAction, ret, "sendBinary error");
            cleanup();
            return;
        }
    }

    if(bDownload) {
        SDISFileTool::move(getTmpFilePath(), mLocalPath);
    }else{
        // todo - delete file after upload ?
    }
    emit progress(mId, mAction, 0, 0, NETWORK_BINARY_STAUS_FINISHED);

    cleanup();
    emit response(mId, mAction, mLocalPath);
}
