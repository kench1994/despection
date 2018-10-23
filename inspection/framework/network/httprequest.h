#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H
#include <QNetworkAccessManager>
#include <QUrl>
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
    bool downloadFile(const QStringList &imgPathList);
signals:
    /**
      * 下载文件返回槽函数
      */
    void responseDownLoadFile(const QStringList& downLoadFileList, const int &errorType, const QString &errorText);
private slots:
    void startRequest();
    //void cancelDownload();
    void httpFinished();
    void httpReadyRead();
    //void updateDataReadProgress(qint64 bytesRead, qint64 totalBytes);
    //认证请求,巡检不需要
    //void slotAuthenticationRequired(QNetworkReply*,QAuthenticator *);
//巡检不需要ssl
//#ifndef QT_NO_SSL
//    void sslErrors(QNetworkReply*,const QList<QSslError> &errors);
//#endif
private:
    QUrl url;
    QNetworkAccessManager qnam;
    QNetworkReply *reply;
    QFile *file;
    int downLoadedCount;
    QStringList downLoadImgPath;
    QString m_nImgSavePath;
    QStringList savedImgPathList;
    QString errorText;
    int errorType;  //0 正确; 1 下载出错; 2 重定向; 3 下载出错 + 重定向
};

#endif // HTTPREQUEST_H
