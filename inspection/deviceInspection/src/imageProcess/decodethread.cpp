#include "decodethread.h"
#include <QDebug>
#include <QImage>
#include <QFile>
#include <cqrcoder.h>
#include <errno.h>
#define  BARCODE_IMAGE_WIDTH 450
#define  BARCODE_IMAGE_HEIGHT 450
#define  DECODE_TMP_IMAGE_PATH "/tmp/syberosDecodeImage.jpg"
DecodeThread::DecodeThread(QObject *parent):QThread(parent),
    m_pImg(NULL),
    m_nDecode(false)
{
}

void DecodeThread::setImage(QImage *img){
    if(img){
        qDebug() <<"m_pImg width===========   " << img->width();
        m_pImg = img;
    }
    else
        qDebug() <<"&&&&&&&&&&&&&&&&&&&&&&  m_pImg " << m_pImg;
}

void DecodeThread::setEncodeContent(const QString &content, const QString &encodeImgPath)
{
    qDebug() << "encode content " << content << " encodeImagPath " << encodeImgPath;
    m_nEncodeContent = content;
    m_nEncodeImagePath = encodeImgPath;
}

void DecodeThread::setDecodeFlag(const bool decode){
    m_nDecode = decode;
}

void DecodeThread::run(){
    if(m_nDecode && m_pImg){
        usleep(300000);
        QFile::remove(DECODE_TMP_IMAGE_PATH);
        int decodeImgWidth = BARCODE_IMAGE_WIDTH;
        QImage img = m_pImg->copy((m_pImg->width() - decodeImgWidth)/2,
                                 (m_pImg->height() - decodeImgWidth)/2,
                                 decodeImgWidth, decodeImgWidth);
        bool saveRv = img.save(DECODE_TMP_IMAGE_PATH);
        qDebug() <<"img.save   ready to sync " <<saveRv << strerror(errno);
        CQRCoder zxingInf;
        QString content;
        int type;
        qDebug() << "begine  zxingInf.decodeQR   " << DECODE_TMP_IMAGE_PATH;
        int rv = zxingInf.decodeQR(DECODE_TMP_IMAGE_PATH, content, type);
        qDebug() << "5555555555555  zxingInf.decodeQR decodeFinished ok " << rv << " type is " << type;
        emit decodeFinished(DECODE_TMP_IMAGE_PATH, content, rv);
    } else if(m_nDecode == false) {
        CQRCoder zxingInf;
        bool rv = zxingInf.encodeQR(m_nEncodeContent, m_nEncodeImagePath);
        emit encodeFinished(m_nEncodeImagePath, m_nEncodeContent, rv);
    }
}
