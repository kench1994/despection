/*!
* \file datastorage.h
* \brief 本地数据存储模块
*
*此类是本地数据存储模块
*
*\author 桓乐乐 huanlele@syberos.com
*\version 1.0
*\date 2015/9/7
*/
#ifndef DATASTORAGE_H
#define DATASTORAGE_H

#include <QString>
#include <QList>
#include <QMap>
#include <QQueue>
#include <QtSql/QSqlDatabase>
#include "../../../framework/common/utils.h"
#include "../../../framework/common/systemconfig.h"
#include <csystemnotificationmanager.h>
#include <csystemnotificationwatcher.h>
#include <QTimer>
/*!
  \class DataStorage
  \inModule 本地数据存储模块
  \brief 负责对本地业务数据存储的模块
  \ingroup dataManager
  DataStorage类为上层提供消息和登陆用户信息的存储和读取接口
*/
class DataStorage : public QObject
{
    Q_OBJECT
public:
   /**
     * @brief removeMessage:移除指定系统消息
     * @param uuid    : const QString& 消息的UUID
     * @return  bool, true 成功, false 失败
     */
   Q_INVOKABLE bool removeMessage(const QString &messageType, QString uuid);

    /**
     * @brief removeMessageFromList : 从数组中删除消息
     * @param messageType:消息类型 1:系统消息 2：业务消息
     * @param uuid:消息uuid
     * @return :true 成功, false 失败
     */
    Q_INVOKABLE bool removeMessageFromList(const QString &messageType, const QString uuid);

   /**
     * @brief getInstance:获取单例对象
     * @return  DataStorage*, 非NULL 成功, NULL 失败
     */
    static DataStorage* getInstance();

    /**
    * @brief getMessageCount:获取当前消息总数
     * @return int & 消息的个数
    */
    Q_INVOKABLE int getMessageCount(const QString &messageType);

    /**
    * @brief getMessageTitle:获取消息头
    * @param index    : 指定消息的索引
    * @return  存储消息头, "" 失败,否则成功
    */
    Q_INVOKABLE QVariantMap getMessageByIndex(const QString &messageType, const int &index);

    /**
    * @brief getMessage:获取特定的消息
    * @param uuid    : const QString& 消息的UUID
    * @return   消息输出结果
    */
    Q_INVOKABLE QVariantMap getMessageByUuid(const QString &messageType, const QString &uuid);

    /**
    * @brief setMessageStatus:获取特定的消息
    * @param uuid    : 指定消息的uuid
    * @param status    : 指定消息的状态，0--未读； 1--已读
    * @return  设置是否成功。true--成功；false--失败
    */
    Q_INVOKABLE bool setMessageStatus(const QString &messageType, const QString uuid, int status);

    /**
    * @brief removeMessageNotifyByKey : 根据KEY移除通知
    * @param notifyKey    : 通知的key
    * @return  是否成功移除。true--成功；false--失败
    */
    Q_INVOKABLE bool removeMessageNotifyByKey(const QString& notifyKey);//add by chen ruihua

    /**
    * @brief removeAllMessageNotify : 移除所有通知
    * @param sopId    : sopId
    * @return  是否成功移除。true--成功；false--失败
    */
    Q_INVOKABLE bool removeAllMessageNotify(const QString &sopId);//add by chen ruihua
private slots:
    /**
    * @brief initialize:类初始化函数
    */
    void initialize();
private:
    explicit DataStorage();

    /**
    * @brief addMessage:添加一个系统消息
    * @param msg    : Message& 消息的结构体
    * @return  bool, true 成功, false 失败
    */
    bool addMessage(const Message& msg);
    /**
    * @brief getMessageList:获取本地系统消息列表
    * @param values    : QList<Message>& 消息输出结果
    * @return  bool, true 成功, false 失败
    */
    bool setMessageList();
    /**
      * @brief updateMessage:更新系统消息到数据库
      * @param list: 系统消息
      * @return  int: 更新到数据库的记录数
      */
    int updateMessage(const QList< QVariant > &list);

    /**
     * @brief updateNotification:更新通知栏消息
     * @param msg:需要更新到通知栏上的消息
     * @return : 返回通知栏的notifyKey
     */
    QString updateNotification(Message msg);
    /**
     * @brief updateLocalNofiyCacheAndNotification:更新本地缓存并调用updateNotification
     * @param manual: true 手动出发，false 等待通知栏回调
     */
    void updateLocalNofiyCacheAndNotification();


signals:
    /**
      * @brief sendUpdateFlage:发送消息更新信号
      * @param count: 新消息个数。-1--获取新消息失败；>=0--新消息个数
      */
    void sendUpdateFlage(int count);

public slots:
    /**
     * @brief recvMessage:接受从服务器取回来的消息
     * @param mapList:消息列表
     * @param isRecved:从服务器接受是否成功
     */
    void recvMessage(QVariant mapList, bool isRecved);
    /**
     * @brief removeMsgFromNotification:监听通知栏消息变化信号
     * @param sopid: 进程sopid
     * @param count:通知栏消息变化数量
     */
    void removeMsgFromNotification();
private:
    QSqlDatabase m_nDB;               ///< 本地数据库存储类
    QList <Message> m_nSystemMessageList;   ///<  系统消息列表
    QList <Message> m_nBusinessMessageList; ///< 业务消息列表
    QList <Message> m_nPendingSystemMessageList;  ///< 待添加到通知栏的消息的列表
    QList <Message> m_nPendingBusinessMessageList;  ///< 待添加到通知栏的消息的列表
    QList<QString> m_nSystemNotifyKeyQueue;    ///< 已经添加到通知栏的系统消息的notifyKey列表
    QList<QString> m_nBusinessNotifyKeyQueue;  ///< 已经添加到通知栏的业务消息的notifyKey列表
    static DataStorage* m_pInstance;  ///<  本地单例对象

    int m_nMsgCountForShowInList;     ///<  列表中显示消息数量
    int m_nSystemMsgCountShowInNotify;  ///<  通知栏中显示系统消息数量
    int m_nBusinessMsgCountShowInNotify;  ///<  通知栏中显示业务消息数量

    QTimer m_nTimer;                     ///< 定时器指针


    QString m_nUserFullPath;           ///<  用户名文件路径
    QString m_nDbFullPath;             ///<  数据库存储路径

    CSystemNotificationWatcher  m_nSystemNotificationWatcher; ///<  操作通知栏的类的对象
    CSystemNotificationManager m_nNotificationManager; ///<  操作通知栏的类的对象
};

#endif // DATASTORAGE_H
