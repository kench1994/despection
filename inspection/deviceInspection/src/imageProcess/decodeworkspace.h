/*!
* \file decodeworkspace.h
* \brief 从相机取帧，并进行一二维码解析；根据输入内容生成一二维码
*
*此类从相机取帧，并进行一二维码解析；根据输入内容生成一二维码
*
*\author 桓乐乐 huanlele@syberos.com
*\version 1.0
*\date 2015/9/7
*/
#ifndef DECODEWORKSPACE_H
#define DECODEWORKSPACE_H
#include <QObject>
#include "decodethread.h"
#include <QImage>
#include <QVideoFrame>
#include <QCamera>

/*!
  \class DecodeWorkSpace
  \inModule 一二维码解析，生成模块
  \brief 根据相机获取的帧进行一二维码解析，并且可以根据文字内容生成一二维码
  \ingroup generateForm
  DecodeWorkSpace 为用户提供一二维解析和生成的接口
*/
class QString;
class QImage;
class QVideoProbe;
class QtCamera;
class DecodeWorkSpace:public QObject
{
    Q_OBJECT
    //Q_PROPERTY(QString decodeImagePath READ getDecodeImagePath WRITE setDecodeImagePath NOTIFY decodeImagePathChanged)
    Q_PROPERTY(QString encodeText READ getEncodeContent WRITE setEncodeContent NOTIFY encodeContentChanged)

public:
    DecodeWorkSpace(QObject *parent = 0);
    ~DecodeWorkSpace();

    //    void setDecodeImagePath(const QString decodeImagePath);
    //    QString getDecodeImagePath() const;

    /**
     * @brief getEncodeContent： 获取生成二维码图片的内容
     * @return ： 返回要生成二维码图片的内容
     */
    QString getEncodeContent() const;

    /**
     * @brief setEncodeContent： 设置生成二维码图片的内容
     * @param encodeContent： 内容
     */
    void setEncodeContent(const QString encodeContent);
signals:

    /**
     * @brief encodeContentChanged： 生成二维码图片的内容改变信号
     * @param encodeContent：生成二维码图片的内容
     */
    void encodeContentChanged(const QString &encodeContent);

    /**
     * @brief decodeFinished： 解析一二维码完成的信号，发送到qml
     * @param decodeImagePath：一二维码图片路径
     * @param decodeContent： 解析到的内容
     * @param success： true，成功，false，失败
     */
    void decodeFinished(const QString &decodeImagePath, const QString &decodeContent, const bool &success);

    /**
     * @brief encodeFinished：生成一二维码图片完成信号，发送到qml
     * @param encodeImagePath： 图片保存路径
     * @param encodeContent： 生成一二维码的内容
     * @param success： true，成功； false，失败
     */
    void encodeFinished(const QString &encodeImagePath, const QString &encodeContent, const bool &success);

    /**
     * @brief changedCameraStatus：相机状态变化信号
     * @param status： 相机状态值
     */
    void changedCameraStatus(int status);

public slots:

    /**
    * @brief cameraStatusChanged
    * @param status
    */
    void cameraStatusChanged(QCamera::Status status);

    /**
     * @brief processFrame： 连接QVideoProbe的videoFrameProbed信号的槽
     * @param frame ：获取到的帧
     */
    void processFrame(QVideoFrame frame);

    /**
     * @brief finishedDecode： 接受线程解码完成的槽函数
     * @param decodeImagePath： 解码的图片的路径
     * @param decodeContent： 解码获取到的内容
     * @param success： true，成功； false，失败
     */
    void finishedDecode(const QString &decodeImagePath, const QString &decodeContent, const bool &success);
private:
    /**
     * @brief beginDecode：开始解码
     */
    void beginDecode();
    /**
     * @brief beginEncode ：开始生成一二维码
     * @param encodeContent：生成一二维码图片的内容
     * @param encodeImgPath： 保存生成的一二维码图片的路径
     */
    void beginEncode(const QString &encodeContent, const QString &encodeImgPath);
private:
    QImage m_nImg; ///< 要进行解码的图片对象
    QString m_nEncodeContent; ///< 要生成一二维码图片的内容
    DecodeThread *m_pDThread; ///< 进行解码和生成一二维码图片的工作线程
    QtCamera *m_pCamera; ///< 相机指针
    QVideoProbe *m_pProbe; ///< QVideoProbe对象指针，主要用于获取相机的帧
    bool m_nImageProcessing; ///< 图片是否正在解析，true，正在解析，等待，false，解析完成，可以继续获取帧进行解析

};
#endif // DECODEWORKSPACE_H
