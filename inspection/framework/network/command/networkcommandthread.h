#ifndef NETWORKCOMMANDTHREAD_H
#define NETWORKCOMMANDTHREAD_H

#include "../networkthreadbase.h"
#include "../../common/utils.h"
#include <QMutex>

class NetworkCommandThread : public NetworkThreadBase
{
    Q_OBJECT
public:
    NetworkCommandThread(QObject *parent = 0);
    ~NetworkCommandThread();
    bool isRunning();
    bool addTask(CommandTask& task);

protected:
    void cleanup();
    /**
     * @brief run:线程运行函数
     */
    void run();

protected slots:
    /**
     * @brief doTask:执行任务的槽函数
     */
    void doTask();

signals:
    /**
     * @brief response:服务器对于客户端指令的回应
     * @param objects    : QString  发生错误包头中的Objects
     * @param type    : QString  请求数据包头中的类型，详情见接口文档
     * @param action  : QString  上层自定义的动作名，在SendPackage中指定
     * @param data  : DataPackage  服务器返回的数据流
     */
    void response(const QString id, const QString objects, const QString type, const QString action, const QByteArray data, const ProtocolHead head);

    void error(const QString id, const QString objects, const QString action, const int errCode, const QString errMsg);

private:
    QString mCurrentAction;
    QString mCurrentObject;
    QString mCurrentType;
    QString mId;
    CommandTask taskInfo;
    bool running;
    QMutex mutex;

};

#endif // NETWORKCOMMANDTHREAD_H
