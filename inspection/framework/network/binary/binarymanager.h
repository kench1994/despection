#ifndef BINARYMANAGER_H
#define BINARYMANAGER_H

#include <QObject>
#include <QMutex>
#include <QString>
#include "../../common/utils.h"
#include "networkbinarythread.h"
#include "binarytaskqueue.h"

/*!
  \class BinaryManager
  \inModule 网络模块
  \brief 底层负责数据收发二进制数据模块
  \ingroup network
  BinaryManager类实现了二进制任务的管理工作
*/
class BinaryManager : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief BinaryManager:类初始化函数
     */
    explicit BinaryManager(QObject *parent = 0);

    void addTask(const QString action, const QString remotePath, const QString localPath, const QString name, const QString title, const QString description, const QString seriesKey, bool download);

private:
    NetworkBinaryThread* getExecutor();

    void cleanupExecutor(const QString& uuid);

    /**
     * @brief startTasks:开始执行任务
     */
    void startTasks();

signals:

    void error(const QString action, const int error, const QString errMsg);

    void response(const QString action, const QString target);

    void progress(const QString action, const int length,
                  const int progress, const int status);

protected slots:


    void errorInner(const QString uuid, const QString action, const int error, const QString errMsg);

    void responseInner(const QString uuid, const QString action, const QString localPath);

    void progressInner(const QString uuid,
                             const QString action,
                             const int totalLength,
                             const int progress,
                             const int status);

private:
    BinaryTaskQueue binaryTaskQueue;
    QList<NetworkBinaryThread*> executorQueue;
};

#endif // BINARYMANAGER_H
