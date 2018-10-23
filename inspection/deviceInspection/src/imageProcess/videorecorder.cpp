#include "videorecorder.h"
#include <QDateTime>
#include <QFile>
VideoRecorder::VideoRecorder()
{

}

VideoRecorder::~VideoRecorder()
{

}

QString VideoRecorder::createVideoSavePath()
{
    QString path =  MultimediaUtils::getInstance()->getMultimediaSavePath();
    QString fileName = QDateTime::currentDateTime().toString("yyyy-MM-dd_hhmmss") + ".mp4";
    m_nVideoPath = path + "VIDEO_" + fileName;
    return m_nVideoPath;
}

void VideoRecorder::deleteVideoFile()
{
    QFile fl(m_nVideoPath);
    if(fl.exists())
        fl.remove();
}

