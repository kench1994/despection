#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include "datahandlerbase.h"

class MessageHandler : public DataHandlerBase
{
public:
    MessageHandler();
    ~MessageHandler();
    virtual void onLine(const QString &action, const DataPackage &dataPackage);
    virtual void offLine(const QString &action, const DataPackage &dataPackage);
    virtual void pureOnLine(const QString &action, const  DataPackage &dataPackage);

private:
    /**
    * 从本地数据库中获取消息列表
    * @param requestDataPackage : requestDataPackage
    * @param resultDataPackage : resultDataPackage
    */
    bool getLocalMessageList(const DataPackage &requestDataPackage, DataPackage &resultDataPackage);

    /**
    * @brief getLocalUnreadMessageCount : 从本地数据库中获取未读消息个数
    * @param requestDataPackage : requestDataPackage
    * @param resultDataPackage : resultDataPackage
    * @return
    */
    bool getLocalUnreadMessageCount(const DataPackage &requestDataPackage, DataPackage &resultDataPackage);

    /**
    * 从本地数据库中获取消息列表
    * @param dataPackage dataPackage
    * @return QList<QMap<QString, QString> >
    */
    bool getLocalMessageList(const QString &operType, const QString &userID,  const int &maxCount, QList<DataRow > &valueList);

    /**
    * @brief deleteLocalMessages : 删除本地消息
    * @param requestDataPackage : requestDataPackage
    * @return
    */
    bool deleteLocalMessages(const DataPackage &requestDataPackage);

    /**
    * @brief updateLocalMessages : 更新本地消息
    * @param requestDataPackage : requestDataPackage
    * @return
    */
    bool updateLocalMessages(const DataPackage &requestDataPackage);

    /**
    * @brief sendNotification : 发送到通知栏
    * @param msgCount : 通知个数
    * @return
    */
    QString sendNotification(const int msgCount);

    /**
    * @brief savePushMessageToDB : 保存消息到本地数据库
    * @param values : values
    */
    void savePushMessageToDB(QList<DataRow> &values);

    QList<QString> m_nSystemNotifyKeyQueue;    ///< 已经添加到通知栏的系统消息的notifyKey列表
    QString m_nNotifyKey;
    int m_nMessageCount;

};

#endif // MESSAGEHANDLER_H
