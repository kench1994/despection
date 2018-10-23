#ifndef VIDEORECORDER_H
#define VIDEORECORDER_H
#include "multimediautils.h"
#include <QObject>
/**
 * @brief The VideoRecorder class : 视频录制播放管理类
 */
class VideoRecorder : public QObject
{
    Q_OBJECT
public:
    VideoRecorder();
    ~VideoRecorder();
    /**
     * @brief getVideoSavePath
     * @return
     */
    Q_INVOKABLE QString createVideoSavePath();
    /**
     * @brief deleteVideoFile
     */
    Q_INVOKABLE void deleteVideoFile();
private:
    QString m_nVideoPath;
};

#endif // VIDEORECORDER_H
