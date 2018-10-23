#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H
#include <QNetworkAccessManager>
#include <QUrl>
#include <QFile>
#include <QObject>
#include <QStringList>
class QAuthenticator;
class QNetworkReply;
class QFile;
class HttpRequest:public QObject
{
    Q_OBJECT
public:
    HttpRequest(QObject *parent = 0);
    ~HttpRequest();
    Q_INVOKABLE void downloadFile(const QStringList &multimediaFiles);
signals:
    /**
      * 下载文件返回槽函数
      */
    void responseDownLoadFile(const QString& downLoadUrl, const QString &filePath, const bool result, const QString &errorText);
private slots:
    void startRequest();
    void httpFinished();
    void httpReadyRead();
    //void updateDataReadProgress(qint64 bytesRead, qint64 totalBytes);
private:
    QNetworkAccessManager qnam;
    QNetworkReply *reply;
    QFile *m_nFile;
    bool m_nDownLoading;
    QString m_nMultimediaSavePath;///<保存下载文件的路径
    QStringList m_nUrlList;///<要下载的Url路径列表
    QStringList m_nDownLoadedFiles; ///<已经下载的文件列表
};

#endif // HTTPREQUEST_H
