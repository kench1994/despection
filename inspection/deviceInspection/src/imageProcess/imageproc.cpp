#include "imageproc.h"
#include <QPainter>
#include <QPixmap>
#include <QBitmap>
#include <QImage>
#include <QDebug>
#include <QRectF>
#include <QRgb>
int num = 0;
ImageProc::ImageProc(QQuickItem *parent):QQuickPaintedItem(parent),
    m_pCoverImg(NULL),
    m_pBgImg(NULL)

{

    this->m_nIsPressed = false;
    this->m_nErase = false;
    this->m_nScaled = 1.0;
    this->m_nSavedImageHeight = 640;
    this->m_nSavedImageWidth = 480;
    this->m_nItemHeight = 0;
    this->m_nItemWidth = 0;
    setAcceptedMouseButtons(Qt::LeftButton);
}

ImageProc::~ImageProc(){
    qDebug() << "ImageProc::~ImageProc m_pBgImg " << m_pBgImg<< "   m_pCoverImg  " << m_pCoverImg;
    if(m_pBgImg != NULL)
        delete m_pBgImg;
    if(m_pCoverImg != NULL)
        delete m_pCoverImg;
}

void ImageProc::initialize(){
    m_nItemWidth = boundingRect().width();
    m_nItemHeight = boundingRect().height();
    m_nScaled = (float)m_nSavedImageHeight/m_nItemHeight;
    qDebug() << "boundingRect height   " << m_nItemHeight << "  width  " << m_nItemWidth << "  scaled is  " << m_nScaled;
    *m_pBgImg = m_pBgImg->scaled(m_nSavedImageWidth,m_nSavedImageHeight);
    m_pCoverImg = new QImage(m_nSavedImageWidth, m_nSavedImageHeight, QImage::Format_ARGB32);
    QRgb value;
    value = qRgba(0, 0, 0, 0);
    for(int i = 0; i < m_nItemWidth; i++){
        for(int j = 0; j < m_nItemHeight; j++)
        m_pCoverImg->setPixel(QPoint(i, j), value);
    }
}

void ImageProc::paint(QPainter *painter)
{
    if(m_pBgImg == NULL)
        return;
    qDebug() << "paint  000000000000000000";
    QRect target(0, 0, m_nItemWidth, m_nItemHeight);
    QRect source(0, 0, m_nSavedImageWidth, m_nSavedImageHeight);
    painter->drawImage(target, *m_pBgImg, source);
    painter->drawImage(target, *m_pCoverImg, source);
}

void ImageProc::setImagePath(const QString &imagePath){
    m_nImagePath = imagePath;
    m_pBgImg = new QImage(m_nImagePath);
    qDebug() << "m_pBgImg is  " << m_pBgImg << "  path  is " << m_nImagePath;
    initialize();
    update();
}

QString ImageProc::getImagePath() const{
    return m_nImagePath;
}

void ImageProc::setEditColor(const QColor &editColor){
    m_nColor = editColor;
}

QColor ImageProc::getEditColor() const{
    return m_nColor;
}

QString ImageProc::saveEditImage( QString imgPath){
    QString path = imgPath;
    if(path.isEmpty())
        path = m_nImagePath;
    QFile file(path);
    file.remove();
    QPainter paint(m_pBgImg);
    QRect target(0, 0, m_pBgImg->width(), m_pBgImg->height());
    paint.drawImage(target, *m_pCoverImg, target);
    m_pBgImg->save(path, 0, 50);
    sync();
    qDebug() <<"save to  " << path;
    return path;
}

void ImageProc::removeImage(QString imgPath){
    QString path = imgPath;
    if(path.isEmpty())
        path = m_nImagePath;
    QFile file(path);
    qDebug() << "ImageProc::removeImage " << path;
    file.remove();
    sync();
}


void ImageProc::mousePressEvent(QMouseEvent *event){

    int x = event->pos().x();
    int y = event->pos().y();
    m_nStartPnt = QPoint(x * m_nScaled, y * m_nScaled);
    m_nEndPnt = QPoint(x * m_nScaled, y * m_nScaled);
    this->m_nIsPressed = true;
}

void ImageProc::mouseReleaseEvent(QMouseEvent *event){
    this->m_nIsPressed = false;
}

void ImageProc::mouseMoveEvent(QMouseEvent *event){
    if(num != 2){
        num++;
        return;
    }
    num = 0;
    if(m_nErase == false){
        if(this->m_nIsPressed){
            int x = event->pos().x();
            int y = event->pos().y();
            m_nEndPnt = QPoint(x * m_nScaled, y * m_nScaled);
            //m_nEndPnt = event->pos();
            QPainter imgPaint(m_pCoverImg);
            imgPaint.setPen(QPen(m_nColor, 4));
            imgPaint.drawLine(m_nStartPnt, m_nEndPnt);
            update();
            m_nStartPnt = m_nEndPnt;
        }
    }else{
        m_nEndPnt = event->pos();
        int startX = m_nEndPnt.x();
        int startY = m_nEndPnt.y();
        QRgb value;
        value = qRgba(0, 0, 0, 0);
        for(int i = startX - 30; i < startX +30; i++)
            for(int j = startY - 30; j < startY + 30; j++)
                m_pCoverImg->setPixel(QPoint(i, j), value);
        update();
    }
}
