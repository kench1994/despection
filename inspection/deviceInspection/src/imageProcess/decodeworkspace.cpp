#include "decodeworkspace.h"
#include <QDebug>
#include <QBitmap>
#include <QPixmap>
#include "qtcamera.h"
#include <QVideoProbe>
#include <errno.h>
#include "qandroidmultimediautils.h"
DecodeWorkSpace::DecodeWorkSpace(QObject *parent):QObject(parent)
{
    m_pDThread = new DecodeThread();
    connect(m_pDThread, SIGNAL(decodeFinished(QString,QString,bool)), this, SLOT(finishedDecode(QString,QString,bool)));
    m_nImageProcessing = false;
    qDebug() << "DecodeWorkSpace::DecodeWorkSpace QtCamera::getInstance ";
    m_pCamera = QtCamera::getInstance();
    qDebug() << "DecodeWorkSpace::DecodeWorkSpace QtCamera::getInstance" << m_pCamera;
    m_pProbe = new QVideoProbe();
    qDebug() << "DecodeWorkSpace::DecodeWorkSpace  new QVideoProbe()";
    connect(m_pProbe, SIGNAL(videoFrameProbed(QVideoFrame)), this, SLOT(processFrame(QVideoFrame)));
    connect(m_pCamera,SIGNAL(statusChanged(QCamera::Status)),this, SLOT(cameraStatusChanged(QCamera::Status)));
    m_pProbe->setSource(m_pCamera);
    qDebug() << "DecodeWorkSpace::DecodeWorkSpace  m_pProbe->setSource(m_pCamera) m_pProbe " << m_pProbe << "  m_pCamera " << m_pCamera;
    //m_pCamera->setCameraFlashMode("flashOff");
    QCameraExposure *cameraExposure = m_pCamera->exposure();
    cameraExposure->setFlashMode(QCameraExposure::FlashOff);
}

DecodeWorkSpace::~DecodeWorkSpace(){
    qDebug() << "444444444444444444444444DecodeWorkSpace::~DecodeWorkSpace()";
    disconnect(m_pDThread, SIGNAL(decodeFinished(QString,QString,bool)), this, SLOT(finishedDecode(QString,QString,bool)));
    disconnect(m_pProbe, SIGNAL(videoFrameProbed(QVideoFrame)), this, SLOT(processFrame(QVideoFrame)));
    disconnect(m_pCamera,SIGNAL(statusChanged(QCamera::Status)),this, SLOT(cameraStatusChanged(QCamera::Status)));
    qDebug() << "DecodeWorkSpace::~DecodeWorkSpace  m_pDThread->quit() ";
    m_pDThread->quit();
    m_pDThread->wait();
    qDebug() << "DecodeWorkSpace::~DecodeWorkSpace  delete m_pDThread";
    delete m_pDThread;
    qDebug() << "m_pCamera->status() -" << m_pCamera->status();
    if(m_pCamera->status() == QtCamera::ActiveStatus){
        qDebug() << "m_pCamera->status() == QtCamera::ActiveStatus--------------- stopCamera ";
        m_pCamera->stopCamera();
    }
    qDebug() << "delete m_pProbe  ---- " << m_pProbe;
    delete m_pProbe;
    m_pProbe = NULL;
    m_pDThread = NULL;
}

void DecodeWorkSpace::finishedDecode(const QString &decodeImagePath, const QString &decodeContent, const bool &success){
    qDebug() << "finishedDecode decodeImagePath "<< decodeImagePath << " decodeContent "<< decodeContent << "  success " << success;
    if(success == false){
        m_nImageProcessing = false;
    }else{
        emit decodeFinished(decodeImagePath, decodeContent, success);
    }
}

void DecodeWorkSpace::cameraStatusChanged(QCamera::Status status){
    qDebug() << " DecodeWorkSpace::cameraStatusChanged " << status;
    emit changedCameraStatus((int)status);
}

void DecodeWorkSpace::processFrame(QVideoFrame frame)
{
    if(m_nImageProcessing == true){
        //qDebug() << Q_FUNC_INFO <<"  m_nImageProcessing == true return";
        return;
    }

    if (!frame.isValid()) {
        qWarning() << Q_FUNC_INFO << "This frame is not valid.";
        return;
    }
    m_nImageProcessing = true;
    QVideoFrame clonedFrame(frame);
    clonedFrame.map(QAbstractVideoBuffer::ReadOnly);

    qDebug() << Q_FUNC_INFO << clonedFrame.size() << clonedFrame.pixelFormat();
    m_nImg = QImage(frame.size(), QImage::Format_ARGB32);
    //qDebug() << "111111111111  frame bits is  " << clonedFrame.bits() ;
    qt_convert_NV21_to_ARGB32(clonedFrame.bits(),
                              (quint32 *)m_nImg.bits(),
                              clonedFrame.width(),
                              clonedFrame.height());
//    qDebug() << "#################################   fullScreen  " <<m_nImg.width( )
//             << "  height   " <<m_nImg.height() << "format: " <<  m_nImg.format() << strerror(errno);
    QMatrix matrix;
    matrix.rotate(90);
    m_nImg = m_nImg.transformed(matrix);
    clonedFrame.unmap();
    beginDecode();
}

void DecodeWorkSpace::setEncodeContent(const QString encodeContent){
    m_nEncodeContent = encodeContent;
    emit encodeContentChanged(encodeContent);
    beginEncode(m_nEncodeContent, "/tmp/1.png");
}

QString DecodeWorkSpace::getEncodeContent() const{
    return m_nEncodeContent;
}

//void DecodeWorkSpace::setDecodeImagePath(const QString decodeImagePath){

//    QQuickView *m_currentView;
//    QWindowList wl = QGuiApplication::topLevelWindows();
//    qDebug() <<"1111111111111111   QGuiApplication::topLevelWindows()  ";
//    if(!wl.isEmpty()) {
//        m_currentView = qobject_cast<QQuickView *>(wl.first());
//    }
//    m_nImg = m_currentView->grabWindow();
//    qDebug() <<"88888888888888888888888888888888888888888888  fullScreen  " <<m_nImg.width()
//            << "  height   " <<m_nImg.height();
//    m_nDecodeImagePath = decodeImagePath;
//    beginDecode();
//    qDebug() << "99999999999999999999999999999999   fullScreen  " <<m_nImg.width()
//             << "  height   " <<m_nImg.height();
//}

//QString DecodeWorkSpace::getDecodeImagePath() const{
//    return m_nDecodeImagePath;
//}

void DecodeWorkSpace::beginDecode(){
    qDebug() <<"44444444 44444444444444444444444444444    m_pDThread  " << m_pDThread  ;
    m_pDThread->setDecodeFlag(true);
    m_pDThread->setImage(&m_nImg);
    m_pDThread->start();
}

void DecodeWorkSpace::beginEncode(const QString &encodeContent, const QString &encodeImgPath){
    m_pDThread->setDecodeFlag(false);
    m_pDThread->setEncodeContent(encodeContent, encodeImgPath);
    m_pDThread->start();
}



