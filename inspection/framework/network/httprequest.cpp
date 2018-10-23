#include "httprequest.h"
#include <QtNetwork>
#include <QFile>
#include <QFileInfo>
#include <QTimer>
#include <errno.h>
#include <QDir>
//#include "../common/systemconfig.h"
#define DOWNLOAD_IMAGE_FILE "downlaod_image"
HttpRequest::HttpRequest(QObject *parent):QObject(parent),reply(NULL), file(NULL), downLoadedCount(0)
{
//    SystemConfig *sysConf = SystemConfig::getInstance();
//    m_nImgSavePath = sysConf->getAppDataPath() + QString("/") + DOWNLOAD_IMAGE_FILE;
    QDir dir(m_nImgSavePath);
    if(!dir.exists())
        dir.mkdir(m_nImgSavePath);
}
HttpRequest::~HttpRequest(){
}

void HttpRequest::startRequest()
{
    qDebug() << "downLoadImgPath.count() is  " << downLoadImgPath.count();
    if(downLoadImgPath.count() == 0 || downLoadedCount > downLoadImgPath.count() - 1){
        qDebug() << " down all image ok";
        emit responseDownLoadFile(savedImgPathList, errorType, errorText);
    } else {
        url = downLoadImgPath.at(downLoadedCount);
        downLoadedCount++;
        QFileInfo fileInfo(url.path());
        QString fileName = fileInfo.fileName();
        qDebug() << "fileName is---------------------  " << fileName  ;
        fileName = m_nImgSavePath + "/" + fileName;
        qDebug() << "fileName is-1111111111--------------------  " << fileName  ;
        if (QFile::exists(fileName)) {
            QFile::remove(fileName);
        }
        file = new QFile(fileName);
        if (!file->open(QIODevice::WriteOnly)) {
            delete file;
            file = 0;
            qDebug() << "file open error " << file->errorString() << QString(strerror(errno));
        } else{
            reply = qnam.get(QNetworkRequest(url));
            connect(reply, SIGNAL(finished()),
                    this, SLOT(httpFinished()));
            connect(reply, SIGNAL(readyRead()),
                    this, SLOT(httpReadyRead()));
        }
    }
}

bool HttpRequest::downloadFile(const QStringList &imgPathList)
{
    qDebug() << "downLoadFile-----------   " << imgPathList;
    errorText = "";
    errorType = 0;
    downLoadedCount = 0;
    downLoadImgPath = imgPathList;
    savedImgPathList.clear();
    startRequest();
    return true;
}

void HttpRequest::httpFinished()
{
    qDebug() << "void HttpRequest::httpFinished  -----";
    file->flush();
    file->close();
    QVariant redirectionTarget = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if (reply->error()) {
        file->remove();
        qDebug() << "HTTP  Download failed " << reply->errorString() << QString(strerror(errno));
        errorText += reply->errorString();
        errorText += "\r";
        if(errorType != 0)
            errorType = 3;
        else
            errorType = 1;
    } else if (!redirectionTarget.isNull()) {
        QUrl newUrl = url.resolved(redirectionTarget.toUrl());
        qDebug() << "HTTP  Redirect to " << newUrl.toString() << "  return";
        file->remove();
        errorText += " HTTP  Redirect\r";
        if(errorType != 0)
            errorType = 3;
        else
            errorType = 2;
        return;
    } else {
        qDebug() << "HTTP  request  ok ";
        savedImgPathList.append(file->fileName());
    }
    reply->deleteLater();
    reply = 0;
    delete file;
    file = 0;
     QTimer::singleShot(100, this, SLOT(startRequest()));
}

void HttpRequest::httpReadyRead()
{
    if (file)
        file->write(reply->readAll());
}
