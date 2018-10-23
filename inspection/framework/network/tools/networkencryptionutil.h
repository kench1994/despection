#ifndef NETWORKENCRYPTIONUTIL_H
#define NETWORKENCRYPTIONUTIL_H

#include <QObject>

class NetworkEncryptionUtil : public QObject
{
    Q_OBJECT
public:
    NetworkEncryptionUtil(QObject *parent = 0);
    /**
     * 转换最终发送的数据
     * @param src 源数据
     * @param dst 结果
     * @param system true 系统参数相关指令（用于更新安全参数） false 其他指令
     * @return bool ture:成功 false:失败
     */
    static bool getFinalSentData(const QByteArray& src,
                                 QByteArray& dst,
                                 bool system = false);

    /**
     * 转换最终接收的数据
     * @param src 源数据
     * @param length 数据长度
     * @param dst 结果
     * @return bool ture:成功 false:失败
     */
    static bool getFinalReceivedData(const QByteArray& src,
                                     const int& length,
                                     QByteArray& dst,
                                     bool system = false);
    /**
     * 转换最终接收的数据
     * @param src 源数据
     * @param dst 结果
     * @return bool ture:成功 false:失败
     */
    static bool getFinalReceivedData(const QByteArray& src,
                                     QByteArray& dst,
                                     bool system = false);
signals:

public slots:

private:
    static bool encryptDataInStandardMode(const QByteArray& src,
                                          QByteArray& dst,
                                          const QString &key,
                                          const QString &iv);

    static bool decryptDataInStandardMode(const QByteArray& src,
                                          QByteArray& dst,
                                          const QString &key,
                                          const QString &iv);

    static int getStartIndex(const unsigned char *bodyHead);
    static int getCount(const unsigned char *bodyHead);

    static bool decryptBodyData(const QByteArray &src,
                                const int &startIndex,
                                const int &count,
                                QByteArray &dst,
                                const QString &key,
                                const QString &iv);

    static bool generateDataBodyHead(char *head,
                                     const int &startIndex,
                                     const int &count,
                                     const QString &key,
                                     const QString &iv);

    static bool generateEncryptData(char *body,
                                    const char *srcData,
                                    const int &bodyLen,
                                    const int &startIndex,
                                    const int &count,
                                    const QString &key,
                                    const QString &iv);

    static int generateCount(const int &maxCount);

};

#endif // NETWORKENCRYPTIONUTIL_H
