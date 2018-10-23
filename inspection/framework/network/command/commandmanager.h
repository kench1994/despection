#ifndef COMMANDMANAGER_H
#define COMMANDMANAGER_H
#include "commandtaskqueue.h"
#include "networkcommandthread.h"
#include <QList>

class CommandManager : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief BinaryManager:类初始化函数
     */
    explicit CommandManager(QObject *parent = 0);
    ~CommandManager();

    void addTask(CommandTask task);

protected:
    void initialize();

protected slots:

    void responseInner(const QString id, const QString objects, const QString type, const QString action, const QByteArray data, const ProtocolHead head);
    void errorInner(const QString id, const QString objects, const QString action, const int errCode, const QString errMsg);

signals:
    /**
     * @brief response:服务器对于客户端指令的回应
     * @param objects    : QString  发生错误包头中的Objects
     * @param type    : QString  请求数据包头中的类型，详情见接口文档
     * @param action  : QString  上层自定义的动作名，在SendPackage中指定
     * @param taskData  : QByteArray  服务器返回的数据流
     */
    void response(const QString objects, const QString type, const QString action, const QByteArray taskData, const ProtocolHead protocolHead);
    /**
     * @brief error:错误信号，当错误发生的时候会触发
     * @param objects    : QString  发生错误包头中的Objects
     * @param action  : QString    上层自定义的动作名，在SendPackage中指定
     * @param error  : int      错误编号
     * @param serverCode : int 服务器错误号
     * @param description : QString 服务器错误信息
     */
    void error(const QString objects, const QString action, const int error, const int serverCode, const QString description);
private:
    void handleTasks();

private:
    CommandTaskQueue commandTaskQueue;
    QList<NetworkCommandThread*> executorQueue;
};

#endif // COMMANDMANAGER_H
