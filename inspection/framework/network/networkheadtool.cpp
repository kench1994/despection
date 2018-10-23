#include "networkheadtool.h"
#include <QCryptographicHash>
#include <QDebug>
#include "../config/sdisruntimecontext.h"

NetworkHeadTool::NetworkHeadTool()
{
}

void NetworkHeadTool::decodeProtocolHead(ProtocolHead& head, char* data)
{
    char* p = data;
    memcpy((void*)head.mark, p, strlen(NETWORK_PROTOCOL_MARK));
    p += sizeof(head.mark);
    head.mainVersion = (unsigned char)*p; p++;
    head.subVersion = (unsigned char)*p; p++;
    head.type = (unsigned char)*p; p++;
    switch((unsigned char)*p){
    case SingleAction:
        head.format = SingleAction;
    case JSON:
        head.format = JSON;
        break;
    case DEXML:
        head.format = DEXML;
        break;
    default:
        head.format = DEXML;
        break;
    }
    p++;
    switch((unsigned char)*p){
    case ASCII:
        head.charset = ASCII;
        break;
    case UTF7:
        head.charset = UTF7;
        break;
    case UTF8:
        head.charset = UTF8;
        break;
    case UTF16:
        head.charset = UTF16;
        break;
    case UTF32:
        head.charset = UTF32;
        break;
    case GB2312:
        head.charset = GB2312;
        break;
    default:
        head.charset = UTF8;
        break;
    }
    p++;
    head.result = (unsigned char)*p; p++;
    memcpy((void*)&head.statusCode, p, sizeof(head.statusCode)); p += sizeof(head.statusCode);
    memcpy((void*)&head.length, p, sizeof(head.length));
}

//升序排列
static bool cmp(const QString &s1, const QString &s2){
    return s1 < s2;
}

/**
* 生成支持基本加密的包头 - 48字节
*
* @param emptyToken true token空 false不空
*
* @return 包头数组
*/
int NetworkHeadTool::generateProtocolHead(ProtocolHead& head,
                                             bool encrypt,
                                             bool emptyToken,
                                             char* out) {
    int len = 16, i = 0;
    for (i = 0; i < 4; i++){
        out[i] = head.mark[i];
    }
    out[4] = head.mainVersion;
    out[5] = head.subVersion;
    out[6] = head.type;
    out[7] = head.format;
    out[8] = head.charset;
    out[9] = head.result;
    memcpy((void *)(&out[10]), (void*)(&head.statusCode), sizeof(short));
    memcpy((void *)(&out[12]), (void*)(&head.length), sizeof(int));

    if(encrypt){
        len = 48;
        long long time = QDateTime::currentMSecsSinceEpoch() / 1000;
        memcpy((void *)(&out[16]), (void*)(&time), sizeof(long long));
        AccessToken token = SDISRuntimeContext::getInstance()->getCurrentToken();
        if(emptyToken || token.accessKey == 0){
            for(i = 0; i < 24; i++) {
                out[24+i] = 0;
            }
        } else {
            long long accessKey = token.accessKey;
            long long accessToken = token.accessToken;

            /**
            * Fill in accessKey
            */
            memcpy((void *)(&out[24]), (void*)(&accessKey), sizeof(long long));

            /**
            * Generate encryptToken
            */
            QStringList values;
            values.append(QString::number(accessKey));
            values.append(QString::number(accessToken));
            values.append(QString::number(time));
            qSort(values.begin(), values.end(), cmp);
            QByteArray md5str = values.join("").toLocal8Bit();
            QByteArray md5sum = QCryptographicHash::hash(md5str, QCryptographicHash::Md5);
            /**
            * Fill in encryptToken
            */
            qDebug() << "------------------------md5sum.size(): "<< md5sum.size();
            for (i = 0; i < md5sum.size(); i++){
                out[32 + i] = md5sum.at(i);
            }
        }
    }
    return len;
}


