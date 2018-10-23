/*!
* \file nfcmanager.h
* \brief 负责RFID卡扫描功能类的头文件
*
*此类提供了针对RFID卡的ID扫描功能
*
*\author 陈冬 chendong@syberos.com
*\version 1.0
*\date 2015/9/22
*/
#ifndef NFCMANAGER_H
#define NFCMANAGER_H

#include <QObject>
#include <qnearfieldmanager.h>
#include <qnearfieldtarget.h>

/*!
  \class NfcManager
  \inModule 公共模块
  \brief 此类负责扫描RFID卡
  \ingroup common
  NfcManager类实现了扫描RFID卡的相关操作
*/
class NfcManager : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief 析构函数
     *
     */
    ~NfcManager();

    /**
     * @brief getInstance:获取网络管理对象
     * @return  NfcManager*, 非空 获取成功, NULL 获取失败
     */
    static NfcManager* getInstance();

public:
    /**
      * @brief startDetection: 开始扫描RFID
      *
      */
    Q_INVOKABLE void startDetection();

    /**
      * @brief stopDetection: 停止扫描RFID
      *
      */
    Q_INVOKABLE void stopDetection();

private:
    /**
     * @brief 构造函数
     *
     * @param parent : 父对象
     */
    explicit NfcManager(QObject *parent = 0);

signals:
    /**
      * @brief done: 扫描结果信号
      * @param uid    : const QString& RFID的UID码
      */
    void done(const QString& uid);

    /**
      * @brief error: 扫描错误信号
      * @param error    : const QString& 错误信息
      */
    void error(const QString& error);

public slots:

private slots:
    /**
      * @brief targetDetected: 内部槽函数，接收扫描设备信息
      * @param target    : QNearFieldTarget* RFID卡设备描述
      */
    void targetDetected(QNearFieldTarget* target);

    /**
      * @brief targetLost: 内部槽函数，设备离开信息
      * @param target    : QNearFieldTarget* RFID卡设备描述
      */
    void targetLost(QNearFieldTarget* target);

private:
    QNearFieldManager *m_pNearFieldManager;    ///< QT的NFC通讯接口类对象

private:
    static NfcManager* m_pInstance;    ///< 对应单例模型的对象

};

#endif // NFCMANAGER_H
