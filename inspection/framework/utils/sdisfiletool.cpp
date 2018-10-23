#include "sdisfiletool.h"
#include <QFileInfo>
#include <QMimeDatabase>
#include <QDir>


int SDISFileTool::getSize(QString path)
{
    QFileInfo info(path);
    int fileLen = 0;
    if(info.exists()){
        fileLen = info.size();
    }
    return fileLen;
}

QString SDISFileTool::getMimeType(QString path)
{
    QMimeDatabase mimeDB;
    QFileInfo fileInfo(path);
    if(!fileInfo.exists() && fileInfo.size() <= 0){
        return "";
    }
    QMimeType mime = mimeDB.mimeTypeForFile(fileInfo);
    return mime.name();
}

QString SDISFileTool::getDirPath(QString path) {
    QFileInfo info(path);
    return info.absolutePath();
}

QString SDISFileTool::getFileName(QString path) {
    QFileInfo info(path);
    return info.fileName();
}

bool SDISFileTool::mkdir(QString path) {
    QDir dir(path);
    if (!dir.exists()){
        return dir.mkpath(path);
    }
    return true;
}

void SDISFileTool::move(QString src, QString target) {
    QFile::remove(target);
    QFile::copy(src, target);
    QFile::remove(src);
    sync();
}
