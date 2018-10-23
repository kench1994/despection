#include "audiorecorder.h"
#include <QAudioFormat>
#include <QUrl>
#include <QDateTime>
#include <QVector>
#include <QAudioFormat>
#include "../../../framework/common/systemconfig.h"
#include "multimediautils.h"
static qreal getPeakValue(const QAudioFormat &format);
static QVector<qreal> getBufferLevels(const QAudioBuffer &buffer);

template <class T>
static QVector<qreal> getBufferLevels(const T *buffer, int frames, int channels);
AudioRecorder::AudioRecorder()
{
    m_nAudio = new QAudioRecorder(this);
    m_pAudioProbe = new QAudioProbe();
    m_pAudioPlayProbe = new QAudioProbe();
    connect(m_pAudioProbe, SIGNAL(audioBufferProbed(QAudioBuffer)), this, SLOT(processBuffer(QAudioBuffer)));
    connect(m_pAudioPlayProbe, SIGNAL(audioBufferProbed(QAudioBuffer)), this, SLOT(processBuffer(QAudioBuffer)));
    m_pAudioProbe->setSource(m_nAudio);
    m_pAudioPlayProbe->setSource(&m_nMediaPlayer);
    connect(&m_nMediaPlayer, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(soundPlayState(QMediaPlayer::State)));
}

AudioRecorder::~AudioRecorder()
{
}

bool AudioRecorder::startAudioRecorder()
{
    QString multimediaSavepath = MultimediaUtils::getInstance()->getMultimediaSavePath();
    QString fileName = QDateTime::currentDateTime().toString("yyyy-MM-dd_hhmmss") + ".m4a";
    m_nAudioFilePath = multimediaSavepath + "SOUND_" + fileName;
    QAudioEncoderSettings audioSettings;
    audioSettings.setCodec("audio/AAC");
    audioSettings.setQuality(QMultimedia::HighQuality);
    //m_nAudio->setEncodingSettings(audioSettings);
     m_nAudio->setEncodingSettings(audioSettings, QVideoEncoderSettings(), "ipod");
    qDebug() << __PRETTY_FUNCTION__<< "supported audio codecs: " << m_nAudio->supportedAudioCodecs();
    qDebug() << __PRETTY_FUNCTION__<< "supported audio sample rates: " << m_nAudio->supportedAudioSampleRates();
    qDebug() << __PRETTY_FUNCTION__<< "bit rate: " << audioSettings.bitRate();
    qDebug() << __PRETTY_FUNCTION__<< "sample rate: " << audioSettings.sampleRate();

    QFile file(m_nAudioFilePath);
    if(!file.open(QIODevice::WriteOnly)) {
        qDebug() << " WARNING , can not open file " << m_nAudioFilePath ;
        return false;
    }
    if (!file.setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner
                             | QFileDevice::ReadGroup| QFileDevice::ReadOther )) {
        qDebug() << " WARNING , can not change permissions " << m_nAudioFilePath ;
    }
    file.close();
    qDebug() << __PRETTY_FUNCTION__ << m_nAudioFilePath;

    m_nAudio->setOutputLocation(QUrl(m_nAudioFilePath));
    qDebug() << "m_nAudio->record()";
    m_nRecording = true;
    m_nAudio->record();
    return true;
}

void AudioRecorder::stopAudioRecorder()
{
    qDebug() << "m_nAudio->stop()";
    m_nRecording = false;
    m_nAudio->stop();
    qDebug() << Q_FUNC_INFO;
    emit audioRecorderFinished(m_nAudioFilePath, true, "ok");
}

void AudioRecorder::cancelAudioRecorder()
{
    m_nRecording = false;
    m_nAudio->stop();
    QFile fl(m_nAudioFilePath);
    if(fl.exists())
        fl.remove();
    qDebug() << Q_FUNC_INFO << " remove " << m_nAudioFilePath;
}

bool AudioRecorder::getAudioRecord()
{
    return m_nRecording;
}

void AudioRecorder::startPlayAudio(const QString &filePath)
{
    if(m_nMediaPlayer.state() == QMediaPlayer::PlayingState){
        qDebug() << " filePath  " << filePath << "  is playing  return";
        return;
    }
    qDebug() << "AudioRecorder::startPlayAudio  " << filePath;
    m_nMediaPlayer.setMedia(QUrl::fromLocalFile(filePath));
    m_nMediaPlayer.setVolume(100);
    m_nMediaPlayer.play();
}

void AudioRecorder::stopPlayAudio()
{
    if(m_nMediaPlayer.state() == QMediaPlayer::StoppedState)
        return;
    m_nMediaPlayer.stop();
    qDebug() << Q_FUNC_INFO;
}

QString AudioRecorder::getPlayState()
{
    QString playState = "";
    switch (m_nMediaPlayer.state()) {
    case QMediaPlayer::PlayingState :
        playState = "playing";
        break;
    case QMediaPlayer::StoppedState:
        playState = "stopped";
    default:
        playState = "stopped";
        break;
    }
    return playState;
}

void AudioRecorder::processBuffer(const QAudioBuffer&buffer){
    //qDebug() << __PRETTY_FUNCTION__<< "t2 ====== "<<QDateTime::currentDateTime();
    QVector<qreal> levels = getBufferLevels(buffer);
    for (int i = 0; i < levels.count(); ++i) {
       //qDebug() << "LEVELS: 1111111111111   " << levels.at(i);
        emit volumeChanged(levels.at(i));
    }
}

void AudioRecorder::soundPlayState(QMediaPlayer::State state)
{
    QString playState = "";
    switch (state) {
    case QMediaPlayer::PlayingState :
        playState = "playing";
        break;
    case QMediaPlayer::StoppedState:
        playState = "stopped";
    default:
        playState = "stopped";
        break;
    }
    emit soundPlayStateChanged(playState);
}



// This function returns the maximum possible sample value for a given audio format
qreal getPeakValue(const QAudioFormat& format)
{
    // Note: Only the most common sample formats are supported
    if (!format.isValid()) {
        return qreal(0);
    }

    if (format.codec() != "audio/pcm") {
        return qreal(0);
    }

    switch (format.sampleType()) {
    case QAudioFormat::Unknown:
        break;
    case QAudioFormat::Float:
        if (format.sampleSize() != 32) // other sample formats are not supported
            return qreal(0);
        return qreal(1.00003);
    case QAudioFormat::SignedInt:
        if (format.sampleSize() == 32)
            return qreal(INT_MAX);
        if (format.sampleSize() == 16)
            return qreal(SHRT_MAX);
        if (format.sampleSize() == 8)
            return qreal(CHAR_MAX);
        break;
    case QAudioFormat::UnSignedInt:
        if (format.sampleSize() == 32)
            return qreal(UINT_MAX);
        if (format.sampleSize() == 16)
            return qreal(USHRT_MAX);
        if (format.sampleSize() == 8)
            return qreal(UCHAR_MAX);
        break;
    }

    return qreal(0);
}

// returns the audio level for each channel
QVector<qreal> getBufferLevels(const QAudioBuffer& buffer)
{
    QVector<qreal> values;

    if (!buffer.format().isValid() || buffer.format().byteOrder() != QAudioFormat::LittleEndian)
        return values;

    //qDebug() << Q_FUNC_INFO <<"buffer.format().codec()  " << buffer.format().codec();
    if (buffer.format().codec() != "audio/pcm")
        return values;

    int channelCount = buffer.format().channelCount();
    //qDebug() << Q_FUNC_INFO <<"channelCount  " << channelCount;
    values.fill(0, channelCount);
    qreal peak_value = getPeakValue(buffer.format());
    //qDebug() << Q_FUNC_INFO <<"peak_value  " << peak_value;
    if (qFuzzyCompare(peak_value, qreal(0)))
        return values;

    switch (buffer.format().sampleType()) {
    case QAudioFormat::Unknown:
    case QAudioFormat::UnSignedInt:
        if (buffer.format().sampleSize() == 32)
            values = getBufferLevels(buffer.constData<quint32>(), buffer.frameCount(), channelCount);
        if (buffer.format().sampleSize() == 16)
            values = getBufferLevels(buffer.constData<quint16>(), buffer.frameCount(), channelCount);
        if (buffer.format().sampleSize() == 8)
            values = getBufferLevels(buffer.constData<quint8>(), buffer.frameCount(), channelCount);
        for (int i = 0; i < values.size(); ++i)
            values[i] = qAbs(values.at(i) - peak_value / 2) / (peak_value / 2);
        break;
    case QAudioFormat::Float:
        if (buffer.format().sampleSize() == 32) {
            values = getBufferLevels(buffer.constData<float>(), buffer.frameCount(), channelCount);
            for (int i = 0; i < values.size(); ++i)
                values[i] /= peak_value;
        }
        break;
    case QAudioFormat::SignedInt:
        if (buffer.format().sampleSize() == 32)
            values = getBufferLevels(buffer.constData<qint32>(), buffer.frameCount(), channelCount);
        if (buffer.format().sampleSize() == 16)
            values = getBufferLevels(buffer.constData<qint16>(), buffer.frameCount(), channelCount);
        if (buffer.format().sampleSize() == 8)
            values = getBufferLevels(buffer.constData<qint8>(), buffer.frameCount(), channelCount);
        for (int i = 0; i < values.size(); ++i)
            values[i] /= peak_value;
        break;
    }

    return values;
}

template <class T>
QVector<qreal> getBufferLevels(const T *buffer, int frames, int channels)
{
    QVector<qreal> max_values;
    max_values.fill(0, channels);

    for (int i = 0; i < frames; ++i) {
        for (int j = 0; j < channels; ++j) {
            qreal value = qAbs(qreal(buffer[i * channels + j]));
            if (value > max_values.at(j))
                max_values.replace(j, value);
        }
    }

    return max_values;
}
