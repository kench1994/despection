#ifndef AUDIORECORDER_H
#define AUDIORECORDER_H
#include <QObject>
#include <QFile>
#include <QAudioRecorder>
#include <QMediaPlayer>
#include <QAudioBuffer>
#include <QAudioProbe>
/**
 * @brief The AudioRecorder class : 声音管理类，包括录音，播放
 */
class AudioRecorder : public QObject
{
    Q_OBJECT
public:
    AudioRecorder();
    ~AudioRecorder();
    /**
     * @brief startAudioRecorder : 开始录音
     * @return : 是否成功
     */
    Q_INVOKABLE bool startAudioRecorder();
    /**
     * @brief stopAudioRecorder : 停止录音
     */
    Q_INVOKABLE void stopAudioRecorder();
    /**
     * @brief cancelAudioRecorder : 取消录音
     */
    Q_INVOKABLE void cancelAudioRecorder();

    /**
     * @brief getAudioRecord : 获取录音状态
     * @return : true: 正在录音; false:没有录音
     */
    Q_INVOKABLE bool getAudioRecord();
    /**
     * @brief startPlayAudio : 开始播放声音
     * @param filePath : 声音路径
     */
    Q_INVOKABLE void startPlayAudio(const QString &filePath);
    /**
     * @brief stopPlayAudio : 停止播放声音
     */
    Q_INVOKABLE void stopPlayAudio();

    /**
     * @brief getPlayStat
     * @return
     */
    Q_INVOKABLE QString getPlayState();

signals:
    /**
     * @brief audioRecorderFinished : 录音完成信号
     * @param filePath : 录音文件路径
     * @param result : 录音是否正常
     * @param description : 描述
     */
    void audioRecorderFinished(const QString filePath, bool result, const QString &description);
    /**
     * @brief volumeChanged : 音量变化信号
     * @param volumn : 音量大小
     */
    void volumeChanged(const qreal &volume);

    /**
     * @brief soundPlayStateChanged : 播放状态变化信号
     * @param state : 播放状态
     */
    void soundPlayStateChanged(const QString state);
public slots:
    /**
      * @brief processBuffer : 监听音量的槽函数
      * @param buffer : 音量采样数据
      */
     void processBuffer(const QAudioBuffer&buffer);

     /**
      * @brief soundPlayState : 播放状态槽函数
      */
     void soundPlayState(QMediaPlayer::State state);
private:
    QAudioRecorder* m_nAudio; ///<录音类
    QMediaPlayer m_nMediaPlayer; ///<播放类
    QAudioProbe *m_pAudioProbe;  ///<监听录音类
    QAudioProbe *m_pAudioPlayProbe;  ///<监听播放类
    QString m_nAudioFilePath; ///< 录音文件保存路径
    bool m_nRecording; ///<录音状态
};

#endif // AUDIORECORDER_H
