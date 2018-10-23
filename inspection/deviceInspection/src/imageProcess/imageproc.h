/*!
* \file imageproc.h
* \brief 图片编辑控件
*
*此类导入到qml使用，进行图片编辑
*
*\author 桓乐乐 huanlele@syberos.com
*\version 1.0
*\date 2015/9/7
*/
#ifndef IMAGEPROC_H
#define IMAGEPROC_H
#include <QQuickPaintedItem>
#include <QPainter>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QColor>
/*!
  \class ImageProc
  \inModule 图片编辑控件
  \brief 导入到qml使用，图片编辑
  \ingroup generateForm
  ImageProc 图片编辑控件
*/
class ImageProc:public QQuickPaintedItem
{
    Q_OBJECT

public:
    ImageProc(QQuickItem *parent = 0);
    ~ImageProc();
    Q_PROPERTY(QString imagePath READ  getImagePath WRITE setImagePath)
    Q_PROPERTY(QColor editColor READ  getEditColor WRITE setEditColor)
    /**
         * @brief paint： 画笔函数
         * @param painter： 画笔指针
         */
    void paint(QPainter *painter);

    /**
     * @brief saveEditImage： 保存编辑后的图片
     * @param imgPath： 保存编辑后的图片的路径
     * @return ：保存编辑后的图片的路径
     */
    Q_INVOKABLE QString saveEditImage( QString imgPath = "");

    /**
     * @brief removeImage： 删除被编辑后的图片
     * @param imgPath： 图片路径
     */
    Q_INVOKABLE void removeImage(QString imgPath = "");

    /**
     * @brief setImagePath： 被编辑图片的路径
     * @param imagePath： 图片的路径
     */
    void setImagePath(const QString &imagePath);

    /**
     * @brief getImagePath： 获取被编辑图片的路径
     * @return ： 图片的路径
     */
    QString getImagePath() const;

    /**
     * @brief setEditColor： 设置画笔颜色
     * @param editColor： 画笔颜色
     */
    void setEditColor(const QColor &editColor);

    /**
     * @brief getEditColor： 获取画笔颜色
     * @return ： 画笔颜色
     */
    QColor getEditColor() const;
protected:

    /**
     * @brief mousePressEvent：鼠标按下事件
     * @param event： 鼠标事件
     */
    void mousePressEvent(QMouseEvent *event);

    /**
     * @brief mouseMoveEvent： 鼠标移动事件
     * @param event： 鼠标事件
     */
    void mouseMoveEvent(QMouseEvent *event);

    /**
     * @brief mouseReleaseEvent： 鼠标松开事件
     * @param event： 鼠标事件
     */
    void mouseReleaseEvent(QMouseEvent *event);
private:

    /**
     * @brief initialize： 初始化函数
     */
    void initialize();
private:
    QPoint m_nStartPnt;   ///< 鼠标开始位置
    QPoint m_nEndPnt;     ///< 鼠标结束位置
    bool m_nIsPressed;    ///< 鼠标是否按下
    bool m_nErase;        ///< 是否擦处编辑过的痕迹
    QImage *m_pCoverImg;  ///< 蒙版图片
    QImage *m_pBgImg;     ///< 背景图片
    int m_nItemHeight;    ///< 图片编辑控件的高度
    int m_nItemWidth;     ///< 图片编辑控件的宽度
    QString m_nImagePath; ///< 图片路径
    QColor m_nColor;      ///< 编辑图片的画笔颜色
    float m_nScaled;      ///< 图片缩放比例
    int m_nSavedImageWidth; ///< 保存图片的宽度
    int m_nSavedImageHeight; ///< 保存图片的高度
};

#endif // IMAGEPROC_H
