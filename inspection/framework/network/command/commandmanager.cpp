#include "commandmanager.h"
#include "../../common/utils.h"
#include <QDebug>
#include <QUuid>

#define DBG_TAG "CommandManager : "

CommandManager::CommandManager(QObject *parent) :
    QObject(parent)
{
    initialize();
}

CommandManager::~CommandManager()
{
    for(int i = 0; i < executorQueue.size(); i++) {
        NetworkCommandThread* executor = executorQueue.at(i);
        executor->quit();
    }
    executorQueue.clear();
}

void CommandManager::initialize()
{
    for(int i = 0; i < NETWORK_COMMAND_THREAD_LIMIT; i++){
        NetworkCommandThread* executor = new NetworkCommandThread(NULL);
        connect(executor,
                SIGNAL(error(const QString, const QString, const QString, const int, const QString)),
                this,
                SLOT(errorInner(const QString, const QString, const QString, const int, const QString)));
        connect(executor,
                SIGNAL(response(const QString, const QString, const QString, const QString, const QByteArray, const ProtocolHead)),
                this,
                SLOT(responseInner(const QString, const QString, const QString, const QString, const QByteArray, const ProtocolHead)));
        connect(executor, SIGNAL(finished()), executor, SLOT(deleteLater()));
        executor->start();
        executorQueue.append(executor);
    }
}

void CommandManager::addTask(CommandTask task)
{
    task.id = QUuid::createUuid().toString();
    commandTaskQueue.add(task);
    handleTasks();
}

void CommandManager::handleTasks()
{
    if(commandTaskQueue.getUndisposedTaskCount() <= 0) {
        qDebug() << DBG_TAG << "Handle tasks - no task more";
        return;
    }

    foreach (NetworkCommandThread* executor, executorQueue) {
        if(executor->isRunning()){
            continue;
        }
        CommandTask task;
        if(!commandTaskQueue.getUndisposedTask(task)){
            qDebug() << DBG_TAG << "Handle tasks - no task more";
            return;
        }
        executor->addTask(task);
    }
}

void CommandManager::responseInner(const QString id, const QString objects, const QString type, const QString action, const QByteArray data, const ProtocolHead head)
{
    commandTaskQueue.remove(id);
    emit response(objects, type, action, data, head);
    handleTasks();
}

void CommandManager::errorInner(const QString id, const QString objects, const QString action, const int errCode, const QString errMsg)
{
    for(int i = 0; i < commandTaskQueue.count(); i++) {
        CommandTask task  = commandTaskQueue.at(i);
        emit error(task.objects, task.action, errCode, 0, errMsg);
    }
    commandTaskQueue.clear();
}

