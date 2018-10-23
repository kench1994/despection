#include "binarytaskqueue.h"

BinaryTaskQueue::BinaryTaskQueue()
{
}

void BinaryTaskQueue::add(BinaryTask task)
{
    QMutexLocker locker(&mutex);
    lstTasks.append(task);
}

void BinaryTaskQueue::remove(int pos)
{
    QMutexLocker locker(&mutex);
    lstTasks.removeAt(pos);
}

void BinaryTaskQueue::remove(QString uuid)
{
    QMutexLocker locker(&mutex);
    for(int i = lstTasks.size(); i > 0; i--) {
        BinaryTask task = lstTasks.at(i-1);
        if(task.uuid == uuid) {
            lstTasks.removeAt(i-1);
        }
    }
}

void BinaryTaskQueue::clear()
{
    QMutexLocker locker(&mutex);
    lstTasks.clear();
}

int BinaryTaskQueue::count()
{
    return lstTasks.size();
}

BinaryTask BinaryTaskQueue::at(int pos)
{
    QMutexLocker locker(&mutex);
    return lstTasks.at(pos);
}

bool BinaryTaskQueue::get(QString uuid, BinaryTask& task)
{
    QMutexLocker locker(&mutex);
    for(int i = lstTasks.size(); i > 0; i--) {
        BinaryTask tmp = lstTasks.at(i-1);
        if(tmp.uuid == uuid) {
            task = tmp;
            return true;
        }
    }
    return false;
}

bool BinaryTaskQueue::fetch(int pos, BinaryTask& task)
{
    QMutexLocker locker(&mutex);
    if(pos >= lstTasks.size()) {
        return false;
    }
    BinaryTask tmp = lstTasks.at(pos);
    lstTasks.removeAt(pos);
    task = tmp;
    return true;
}

bool BinaryTaskQueue::getUndisposedTask(BinaryTask& task)
{
    QMutexLocker locker(&mutex);
    for(int i = 0; i < lstTasks.size(); i++) {
        BinaryTask tmp = lstTasks.at(i);
        if(!tmp.disposing) {
            task = tmp;
            task.disposing = true;
            lstTasks.replace(i, task);
            return true;
        }
    }
    return false;
}

int BinaryTaskQueue::getUndisposedTaskCount()
{
    QMutexLocker locker(&mutex);
    int count = 0;
    for(int i = 0; i < lstTasks.size(); i++) {
        BinaryTask tmp = lstTasks.at(i);
        if(!tmp.disposing) {
            count++;
        }
    }
    return count;
}
