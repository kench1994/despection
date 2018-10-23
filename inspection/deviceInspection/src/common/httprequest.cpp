#include "httprequest.h"
#include <QtNetwork>
#include <QFile>
#include <QFileInfo>
#include <QTimer>
#include <errno.h>
#include <QDir>
#include "../../../framework/common/systemconfig.h"
#define MULTIMEDIA_DOWNLOAD_FILE "/home/user/inspectionDownload/"
#define DOWNLOADING_FILE_SUFFIX "_temp"
HttpRequest::HttpRequest(QObject *parent):QObject(parent),reply(NULL), m_nDownLoading(false)
{
    m_nMultimediaSavePath = MULTIMEDIA_DOWNLOAD_FILE;
    QDir dir(m_nMultimediaSavePath);
    if(!dir.exists())
        dir.mkdir(m_nMultimediaSavePath);
    QFileInfoList fileList = dir.entryInfoList();
    for(int i = 0; i < fileList.length(); i++){
        QFileInfo fileInfo = fileList.at(i);
        qDebug() << "  " << fileInfo.absoluteFilePath() <<"  filename is " << fileInfo.fileName();
        if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
            continue;
        // if(fileInfo.fileName().contains(DOWNLOADING_FILE_SUFFIX))
        QFile::remove(fileInfo.absoluteFilePath());
        //m_nDownLoadedFiles.append(fileInfo.fileName());
    }
}

HttpRequest::~HttpRequest(){
}

void HttpRequest::downloadFile(const QStringList &multimediaFiles)
{
    qDebug() << Q_FUNC_INFO << multimediaFiles;
    for(int i = 0; i < multimediaFiles.count(); i ++){
        QString httpPath = multimediaFiles.at(i);
        QUrl tmpUrl(httpPath);
        qDebug() << "m_nDownLoadedFiles  " << m_nDownLoadedFiles;
        qDebug() << "tmpUrl.fileName()  " << tmpUrl.fileName();
        if(m_nDownLoadedFiles.contains(tmpUrl.fileName())){
            qDebug() << tmpUrl.fileName() << " exist";
            emit responseDownLoadFile(httpPath, m_nMultimediaSavePath + tmpUrl.fileName(), true, "");
        } else if(!m_nUrlList.contains(httpPath)){
            m_nUrlList.append(httpPath);
        }
    }
    startRequest();
}

void HttpRequest::startRequest()
{
    if(m_nUrlList.count() > 0 && !m_nDownLoading){
        QUrl tmpUrl(m_nUrlList.at(0));
        QString tmpFileName = tmpUrl.fileName() + DOWNLOADING_FILE_SUFFIX;
        QString tmpFilePath = m_nMultimediaSavePath + tmpFileName;
        qDebug() << "tmpFileName is---------------------  " << tmpFilePath  ;
        if (QFile::exists(tmpFilePath)) {
            QFile::remove(tmpFilePath);
        }
        m_nFile = new QFile(tmpFilePath);
        if (!m_nFile->open(QIODevice::WriteOnly)) {
            delete m_nFile;
            qDebug() << "file open error " << m_nFile->errorString() << QString(strerror(errno));
            emit responseDownLoadFile(m_nUrlList.at(0), "", false, m_nFile->errorString());
            m_nUrlList.removeAt(0);
             QTimer::singleShot(100, this, SLOT(startRequest()));
        } else {
            m_nDownLoading = true;
            reply = qnam.get(QNetworkRequest(tmpUrl));
            connect(reply, SIGNAL(finished()),
                    this, SLOT(httpFinished()));
            connect(reply, SIGNAL(readyRead()),
                    this, SLOT(httpReadyRead()));
        }
    }
}

void HttpRequest::httpFinished()
{
    m_nFile->flush();
    m_nFile->close();
    m_nDownLoading = false;
    QVariant redirectionTarget = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if (reply->error()) {
        m_nFile->remove();
        qDebug() << "download error  " << reply->errorString();
        emit responseDownLoadFile(m_nUrlList.at(0), "", false, reply->errorString());
    } else if (!redirectionTarget.isNull()) {
        //QUrl newUrl = url.resolved(redirectionTarget.toUrl());
        qDebug() << "HTTP  Redirect ,  return";
        m_nFile->remove();
        emit responseDownLoadFile(m_nUrlList.at(0), "", false, "url redirect");
    } else {
        qDebug() << "HTTP  request  ok ";
        QString filePath = m_nFile->fileName().replace(DOWNLOADING_FILE_SUFFIX, "");
        bool rv =  m_nFile->rename(filePath);
        int pos = filePath.lastIndexOf("/");
        QString fileName = filePath.right(filePath.length() - pos - 1);
        qDebug() << "rename  return " << rv << "   fileName " << fileName;
        m_nDownLoadedFiles.append(fileName);
        emit responseDownLoadFile(m_nUrlList.at(0), filePath, true, "");
    }
    delete m_nFile;
    reply->deleteLater();
    m_nUrlList.removeAt(0);
     QTimer::singleShot(100, this, SLOT(startRequest()));
}

void HttpRequest::httpReadyRead()
{
    // this slot gets called every time the QNetworkReply has new data.
    // We read all of its new data and write it into the file.
    // That way we use less RAM than when reading it at the finished()
    // signal of the QNetworkReply
        qDebug() << Q_FUNC_INFO;
        m_nFile->write(reply->readAll());
}

//void HttpRequest::updateDataReadProgress(qint64 bytesRead, qint64 totalBytes)
//{
//    if (httpRequestAborted)
//        return;

//    progressDialog->setMaximum(totalBytes);
//    progressDialog->setValue(bytesRead);
//}

//void HttpRequest::slotAuthenticationRequired(QNetworkReply*,QAuthenticator *authenticator)
//{
//    QDialog dlg;
//    Ui::Dialog ui;
//    ui.setupUi(&dlg);
//    dlg.adjustSize();
//    ui.siteDescription->setText(tr("%1 at %2").arg(authenticator->realm()).arg(url.host()));

//    // Did the URL have information? Fill the UI
//    // This is only relevant if the URL-supplied credentials were wrong
//    ui.userEdit->setText(url.userName());
//    ui.passwordEdit->setText(url.password());

//    if (dlg.exec() == QDialog::Accepted) {
//        authenticator->setUser(ui.userEdit->text());
//        authenticator->setPassword(ui.passwordEdit->text());
//    }
//}
