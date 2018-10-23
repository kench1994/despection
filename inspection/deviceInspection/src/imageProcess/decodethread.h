/*!
* \file decodethread.h
* \brief 一二维码解析，生成的工作线程
*
*此类是一二维码解析，生成的工作线程
*
*\author 桓乐乐 huanlele@syberos.com
*\version 1.0
*\date 2015/9/7
*/
#ifndef DECODETHREAD_H
#define DECODETHREAD_H
#include <QThread>
/*!
  \class DecodeThread
  \inModule 一二维码解析，生成的工作线程
  \brief 根据传入的进行一二维码解析，并且可以根据文字内容生成一二维码
  \ingroup generateForm
  DecodeThread 一二维码解析，生成的工作线程
*/
class QImage;
class DecodeThread: public QThread
{
    Q_OBJECT
public:
    DecodeThread(QObject *parent = 0);
    /**
     * @brief setDecodeFlag: 设置线程状态
     * @param decode: true, 解析一二维码图片；false，生成二维码图片
     */
    void setDecodeFlag(const bool decode);

    /**
     * @brief setImage: 设置要解析的图片的指针
     * @param img: 图片指针
     */
    void setImage(QImage *img);

    /**
     * @brief setEncodeContent: 设置生成一二维码的内容和生成图片的路径
     * @param content: 要生成一二维码的内容
     * @param encodeImgPath: 要生成一二维码图片的保存路径
     */
    void setEncodeContent(const QString &content, const QString &encodeImgPath);
signals:
    /**
     * @brief decodeFinished: 解码完成信号
     * @param decodeImagePath: 被解析的一二维码图片的路径
     * @param decodeContent: 解析一二维码图片获取的内容
     * @param success: true，成功； false，失败
     */
    void decodeFinished(const QString &decodeImagePath, const QString &decodeContent, const bool &success);

    /**
     * @brief encodeFinished: 生成一二维码完成信号
     * @param encodeImagePath: 生成一二维码图片的路径
     * @param encodeContent: 用来生成一二维码图片的内容
     * @param success: true，成功； false，失败
     */
    void encodeFinished(const QString &encodeImagePath, const QString &encodeContent, const bool &success);
protected:
    void run();
private:
    volatile bool m_nDecode; ///< true, 解析一二维码图片；false，生成二维码图片
    QImage *m_pImg; ///< 要解析的图片的指针
    QString m_nEncodeContent; ///< 用来生成二维码图片的内容
    QString m_nEncodeImagePath; ///< 要生成二维码图片的保存路径
};

#endif // DECODETHREAD_H
