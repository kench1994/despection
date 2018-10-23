#include "networkencryptionutil.h"
#include "../../../framework/config/sdisruntimecontext.h"
#include "../../../framework/dataManager/datacrypto.h"
#include <QDebug>

NetworkEncryptionUtil::NetworkEncryptionUtil(QObject *parent) :
    QObject(parent)
{
}

/**
* 转换最终发送的数据
* @param src 源数据
* @return 结果
*/
bool NetworkEncryptionUtil::getFinalSentData(const QByteArray& src,
                                             QByteArray& dst,
                                             bool system) {
   bool ret = false;
   int encryptType= SDISRuntimeContext::getInstance()->getDataEncryptionType();
   if(encryptType == 0) {
       dst = src;
       ret = true;
   }else if(encryptType == 1) {
       /**
        * Encrypt data if it's necessary
        */
       QString key;
       QString iv;
       if(system){
           key = QString(APP_SYSTEM_KEY);
           iv = QString(APP_SYSTEM_IV);
       }else{
           key = SDISRuntimeContext::getInstance()->getAppSecretKey();
           iv = SDISRuntimeContext::getInstance()->getAppSecretIV();
       }
       ret = encryptDataInStandardMode(src, dst, key, iv);
   }else{
       /**
        * SSL 'data equal source data
        */
       dst = src;
       ret = true;
   }
   return ret;
}

bool NetworkEncryptionUtil::getFinalReceivedData(const QByteArray& src,
                                                 const int& length,
                                                 QByteArray& dst,
                                                 bool system)
{
    QByteArray data(src.data(), length);
    return getFinalReceivedData(data, dst, system);
}

bool NetworkEncryptionUtil::getFinalReceivedData(const QByteArray& src,
                                                 QByteArray& dst,
                                                 bool system)
{
    bool ret = false;
    int encryptType = SDISRuntimeContext::getInstance()->getDataEncryptionType();
    if(encryptType == 0) {
        dst = src;
        ret = true;
    }else if(encryptType == 1) {
        /**
         * Decrypt data if it's necessary
         */
        QString key;
        QString iv;
        if(system){
            key = QString(APP_SYSTEM_KEY);
            iv = QString(APP_SYSTEM_IV);
        }else{
            key = SDISRuntimeContext::getInstance()->getAppSecretKey();
            iv = SDISRuntimeContext::getInstance()->getAppSecretIV();
        }
        ret = decryptDataInStandardMode(src, dst, key, iv);
    }else{
        /**
         * SSL 'data equal source data
         */
        dst = src;
        ret = true;
    }
    return ret;
}
/**
 * 加密数据（标准加密模式）
 * @param src 源数据
 * @return  加密后的数据
 */
bool NetworkEncryptionUtil::encryptDataInStandardMode(const QByteArray& src,
                                                      QByteArray& dst,
                                                      const QString &key,
                                                      const QString &iv) {
    /**
     * 若数据长度小于16，StartIndex、Count均为0，即不加密
     */
    int length = src.length();
    int count = 0;
    int startIndex = 0;
    char bodyHead[BODY_HEAD_LENGTH] = {0};
    if(length < 16) {
        count = 0;
        startIndex = 0;
    }else if(length >= 16 && length < 32) {
        count = 16;
    }else if(length >= 32 && length < 48) {
        count = generateCount(2);
    }else if(length >= 48 && length < 64) {
        count = generateCount(3);
    }else if(length >= 64) {
        count = generateCount(4);
    }

    if(length >= 16){
        if(length - count == 0) {
            startIndex = 0;
        }else{
            startIndex = qrand()%(length - count);
        }
    }

    qDebug() << "------------encryptDataInStandardMode--------startIndex: "<< startIndex;
    qDebug() << "------------encryptDataInStandardMode-------------count: "<< count;
    bool ret = generateDataBodyHead(bodyHead, startIndex, count, key, iv);
    if(!ret){
        return false;
    }
    dst = QByteArray(bodyHead, BODY_HEAD_LENGTH);

    const char *srcData = src.data();
    int bodyLen = src.length();
    char *bodyData = (char *)malloc(bodyLen);
    memset((void *)bodyData, 0, bodyLen);
    if(length >= 16){
        ret = generateEncryptData(bodyData, srcData, bodyLen, startIndex, count, key, iv);
        if(!ret){
            free(bodyData);
            return false;
        }
    }else{
        memcpy((void *)bodyData, (void *)srcData, bodyLen);
    }

    dst += QByteArray(bodyData, bodyLen);
    qDebug() << "------------encryptDataInStandardMode------------dst: "<< dst.size();
    free(bodyData);
    return true;
}
/**
 * 解密数据（标准加密格式）
 * @param src 源数据
 * @return 解密后数据
 */
bool NetworkEncryptionUtil::decryptDataInStandardMode(const QByteArray& src,
                                                      QByteArray& dst,
                                                      const QString &key,
                                                      const QString &iv) {
    /**
     * Parse body head
     */

    /**
     * init encrypt head
     */
    const char *srcData = src.data();
    int srcLen = src.length();
    unsigned char *bodyHeadBeforeEncryption = (unsigned char *)malloc(BODY_HEAD_INFO_LENGTH);
    memset((void *) bodyHeadBeforeEncryption, 0, BODY_HEAD_INFO_LENGTH);
    memcpy((void *) bodyHeadBeforeEncryption, (void *)(srcData+BODY_HEAD_OFFSET_START_INDEX), BODY_HEAD_INFO_LENGTH);

    /**
     * decrypt head
     */
    unsigned char *bodyHead = (unsigned char *)malloc(BODY_HEAD_INFO_LENGTH);
    memset((void *) bodyHead, 0, BODY_HEAD_INFO_LENGTH);
    int outLen = 0;

    QByteArray keyBytes = key.toLocal8Bit();
    QByteArray ivBytes = iv.toLocal8Bit();
    unsigned char *decKey = (unsigned char *)keyBytes.data();
    unsigned char *decIV = (unsigned char *)ivBytes.data();
    bool ret = DataCrypto::decrypt(bodyHeadBeforeEncryption,
                                   bodyHead,
                                   BODY_HEAD_INFO_LENGTH,
                                   outLen,
                                   decKey,
                                   decIV,
                                   AES_CBC_128_MODE, false);
    if(!ret){
        free(bodyHeadBeforeEncryption);
        free(bodyHead);
        return false;
    }

    int startIndex = getStartIndex(bodyHead);
    int count = getCount(bodyHead);
    qDebug() << "----------NetworkEncryptionUtil::decryptDataInStandardMode---------startIndex: " << startIndex;
    qDebug() << "----------NetworkEncryptionUtil::decryptDataInStandardMode--------------count: " << count;
    if(startIndex < 0 || count >= srcLen || startIndex+count >= srcLen){
        return false;
    }
    /**
     * Parse data
     */
    QByteArray bodyDataBeforeEncryption(srcData+BODY_HEAD_LENGTH, srcLen - BODY_HEAD_LENGTH);
    return decryptBodyData(bodyDataBeforeEncryption, startIndex, count, dst, key, iv);
}
/**
 * 获取开始索引
 * @param bodyHead 包体头数据
 * @return 开始索引
 */
int NetworkEncryptionUtil::getStartIndex(const unsigned char *bodyHead) {
    int index = *((int *)(bodyHead+(BODY_HEAD_OFFSET_START_INDEX - BODY_HEAD_APP_ID_LENGTH)));
    return index;
}

/**
 * 获取范围值
 * @param bodyHead 数据体包头
 * @return 范围值（count）
 */
int NetworkEncryptionUtil::getCount(const unsigned char *bodyHead) {
    int count = *((int *)(bodyHead+(BODY_HEAD_OFFSET_COUNT - BODY_HEAD_APP_ID_LENGTH)));
    return count;
}

/**
 * 解密包体数据
 * @param src  源数据
 * @param startIndex  开始索引
 * @param count  count值（数据范围）
 * @return 包体数据（解密后）
 */
bool NetworkEncryptionUtil::decryptBodyData(const QByteArray &src,
                                            const int &startIndex,
                                            const int &count,
                                            QByteArray &dst,
                                            const QString& key,
                                            const QString &iv) {
    if(count <= 0) {// No encryption data
        dst = src;
        return true;
    }
    int bodyLen = src.length();
    const char *srcData = src.data();

    /**
     * init encrypt body
     */
    unsigned char *encryptionData = (unsigned char *)malloc(count);
    memset((void *) encryptionData, 0, count);
    memcpy((void *) encryptionData, (void *)(srcData+startIndex), count);

    /**
     * decrypt body
     */
    unsigned char *decryptionData = (unsigned char *)malloc(count);
    memset((void *) decryptionData, 0, count);
    int outLen = 0;
    QByteArray keyBytes = key.toLocal8Bit();
    QByteArray ivBytes = iv.toLocal8Bit();
    unsigned char *decKey = (unsigned char *)keyBytes.data();
    unsigned char *decIV = (unsigned char *)ivBytes.data();
    bool ret = DataCrypto::decrypt(encryptionData,
                                   decryptionData,
                                   count,
                                   outLen,
                                   decKey,
                                   decIV,
                                   AES_CBC_128_MODE, false);

    if(!ret){
        free(encryptionData);
        free(decryptionData);
        return false;
    }

    char *bodyData = (char *)malloc(bodyLen);
    memset((void *) bodyData, 0, bodyLen);

    /**
     * Copy head data
     */
    memcpy((void *) bodyData, (void *)srcData, startIndex);

    /**
     * Copy decrypted data
     */
    memcpy((void *) (bodyData+startIndex), (void *)(decryptionData), count);

    /**
     * Copy tail data
     */
    memcpy((void *) (bodyData+startIndex+count), (void *)(srcData+startIndex+count), bodyLen-startIndex-count);

    /**
     *
     */
    dst = QByteArray(bodyData, bodyLen);

    /**
     * free memory
     */
    free(encryptionData);
    free(decryptionData);
    free(bodyData);

    return true;
}


/**
* 生成数据体头
* @param head 数据体头
* @param startIndex  开始索引
* @param count  数据范围
*/
bool NetworkEncryptionUtil::generateDataBodyHead(char *head,
                                                 const int &startIndex,
                                                 const int &count,
                                                 const QString &key,
                                                 const QString &iv) {
   /**
    * 1 ~ 8 bytes are 0
    */
    memset((void * )head, 0, 8);

    /**
    * 8 ~ 16 bytes are app ID
    */
   long long appID = SDISRuntimeContext::getInstance()->getAppId().toLongLong();
   memcpy((void * )(head+BODY_HEAD_OFFSET_APP_ID), &appID, 8);


   char dataInfo[BODY_HEAD_INFO_LENGTH] = {0};

   /**
    * 16 ~ 20 bytes are data start index
    */
   memcpy((void * )dataInfo, (void *)(&startIndex), 4);

   /**
    * 20 ~ 24 bytes are encrypted data length
    */
   memcpy((void * )(dataInfo+4), (void *)(&count), 4);

   /**
    * 24 ~ 32 bytes are 0
    */

   unsigned char encryptedBodyHead[16] = {0};
   int outLen = 0;
   QByteArray keyBytes = key.toLocal8Bit();
   QByteArray ivBytes = iv.toLocal8Bit();
   unsigned char *encKey = (unsigned char *)keyBytes.data();
   unsigned char *encIV = (unsigned char *)ivBytes.data();
   bool ret = DataCrypto::encrypt(dataInfo,
                                  BODY_HEAD_INFO_LENGTH,
                                  encryptedBodyHead,
                                  &outLen,
                                  encKey,
                                  encIV,
                                  AES_CBC_128_MODE, false);
   if(!ret){
       return false;
   }
   memcpy((void * )(head+BODY_HEAD_OFFSET_START_INDEX), encryptedBodyHead, BODY_HEAD_INFO_LENGTH);
   return true;
}

/**
 * 生成加密数据
 * @param src  源数据
 * @param startIndex  开始索引
 * @param count count值（范围）
 * @return  加密数据
 */
bool NetworkEncryptionUtil::generateEncryptData(char *body,
                                                const char *srcData,
                                                const int &bodyLen,
                                                const int &startIndex,
                                                const int &count,
                                                const QString& key,
                                                const QString& iv) {

    /**
     * Source data before encrypted
     */
    char *srcEncryptionData = (char *)malloc(count);
    memset((void *)srcEncryptionData, 0, count);
    memcpy((void *)srcEncryptionData, (void *)(srcData+startIndex), count);

    /**
     * Destination data after encrypted.
     * Use AES 128 to encrypted
     */
    unsigned char *dstEncryptionData = (unsigned char *)malloc(count);
    memset((void *)dstEncryptionData, 0, count);
    int outLen = 0;
    QByteArray keyBytes = key.toLocal8Bit();
    QByteArray ivBytes = iv.toLocal8Bit();
    unsigned char *encKey = (unsigned char *)keyBytes.data();
    unsigned char *encIV = (unsigned char *)ivBytes.data();
    bool ret = DataCrypto::encrypt(srcEncryptionData,
                                   count,
                                   dstEncryptionData,
                                   &outLen,
                                   encKey,
                                   encIV,
                                   AES_CBC_128_MODE, false);

    if(!ret){
        free(srcEncryptionData);
        free(dstEncryptionData);
        return false;
    }

    /**
     * Copy head data
     */
    memcpy((void *)body, (void *)srcData, startIndex);

    /**
     * Copy encrypted data
     */
    memcpy((void *)(body+startIndex), (void *)dstEncryptionData, count);

    /**
     * Copy tail data
     */
    memcpy((void *)(body+(startIndex+count)), (void *)(srcData+(startIndex+count)), bodyLen-startIndex-count);

    free(srcEncryptionData);
    free(dstEncryptionData);

    return true;
}


/**
 * 生成随机Count值
 * @param maxCount 最大值
 * @return 随机Count值
 */
int NetworkEncryptionUtil::generateCount(const int &maxCount) {
    int type = qrand()%maxCount;
    int ret;
    switch (type) {
        case 0:
            ret = 16;
            break;
        case 1:
            ret = 32;
            break;
        case 2:
            ret = 48;
            break;
        case 3:
            ret = 64;
            break;
        default:
            ret = 16;
            break;
    }
    return ret;
}


