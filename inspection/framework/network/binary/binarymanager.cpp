#include "binarymanager.h"
#include <QUuid>
#include <QMutexLocker>
#include <QDebug>

#define DBG_TAG "BinaryManager : "

BinaryManager::BinaryManager(QObject *parent) :
    QObject(parent)
{
}

void BinaryManager::addTask(const QString action, const QString remotePath, const QString localPath, const QString name, const QString title, const QString description, const QString seriesKey, bool download)
{
    if((remotePath.isEmpty() || localPath.isEmpty()) && download) {
        qDebug() << DBG_TAG << "Binary parameters wrong!";
        return;
    }
    BinaryTask task;
    task.action = action;
    task.remotePath = remotePath;
    task.localPath = localPath;
    task.uuid = QUuid::createUuid().toString();
    task.name = name;
    task.title = title;
    task.description = description;
    task.seriesKey = seriesKey;
    task.download = download;
    task.disposing = false;
    binaryTaskQueue.add(task);
    startTasks();
}

void BinaryManager::startTasks()
{
    qDebug() << "startTasks" << binaryTaskQueue.count();
    if(binaryTaskQueue.getUndisposedTaskCount() <= 0) {
        qDebug() << DBG_TAG << "No more tasks! 11";
        return;
    }
    BinaryTask task;
    NetworkBinaryThread* executor = getExecutor();
    if(executor == NULL){
        qDebug() << DBG_TAG << "Executor busy!";
        return;
    }
    bool ret = binaryTaskQueue.getUndisposedTask(task);
    if(!ret) {
        qDebug() << DBG_TAG << "No more tasks!22";
        return;
    }
    connect(executor,
            SIGNAL(error(const QString, const QString, const int, const QString)),
            this,
            SLOT(errorInner(const QString, const QString, const int, const QString)));
    connect(executor,
            SIGNAL(response(const QString, const QString, const QString)),
            this,
            SLOT(responseInner(const QString, const QString, const QString)));
    connect(executor,
            SIGNAL(progress(const QString, const QString, const int, const int, const int)),
            this,
            SLOT(progressInner(const QString, const QString, const int,const int, const int)));
    connect(executor, SIGNAL(finished()), executor, SLOT(deleteLater()));
    executor->setTask(task);
    executor->start();
}

NetworkBinaryThread* BinaryManager::getExecutor()
{
    NetworkBinaryThread* executor = NULL;
    if(executorQueue.size() < NETWORK_BINARY_THREAD_LIMIT) {
        executor = new NetworkBinaryThread(NULL);
        executorQueue.append(executor);
    }
    return executor;
}

void BinaryManager::errorInner(const QString uuid, const QString action, const int error, const QString errMsg)
{
    binaryTaskQueue.remove(uuid);
    emit this->error(action, error, errMsg);
    cleanupExecutor(uuid);
    startTasks();
}

void BinaryManager::responseInner(const QString uuid, const QString action, const QString localPath)
{
    binaryTaskQueue.remove(uuid);
    emit response(action, localPath);
    cleanupExecutor(uuid);
    startTasks();
}

void BinaryManager::progressInner(const QString uuid, const QString action, const int totalLength, const int progress, const int status)
{
    emit this->progress(action, totalLength, progress, status);
}


void BinaryManager::cleanupExecutor(const QString& uuid)
{
    qDebug() << DBG_TAG << "cleanupExecutor " << uuid;
    for(int i = executorQueue.size(); i > 0; i--){
        NetworkBinaryThread* executor = executorQueue.at(i-1);
        if(executor->getUuid() == uuid) {
            executorQueue.removeAt(i-1);
        }
    }
}


