#include "commandtaskqueue.h"

CommandTaskQueue::CommandTaskQueue()
{
}

void CommandTaskQueue::add(CommandTask task)
{
    QMutexLocker locker(&mutex);
    if(task.type == "1") {
        lstTasks.insert(0, task);
    }else{
        lstTasks.append(task);
    }
}

void CommandTaskQueue::remove(int pos)
{
    QMutexLocker locker(&mutex);
    lstTasks.removeAt(pos);
}

void CommandTaskQueue::clear()
{
    QMutexLocker locker(&mutex);
    lstTasks.clear();
}

int CommandTaskQueue::count()
{
    return lstTasks.size();
}

CommandTask CommandTaskQueue::at(int pos)
{

    QMutexLocker locker(&mutex);
    return lstTasks.at(pos);
}

bool CommandTaskQueue::fetch(int pos, CommandTask task)
{
    QMutexLocker locker(&mutex);
    if(pos >= lstTasks.size()) {
        return false;
    }
    CommandTask tmp = lstTasks.at(pos);
    lstTasks.removeAt(pos);
    task = tmp;
    return true;
}

bool CommandTaskQueue::getUndisposedTask(CommandTask& task)
{
    QMutexLocker locker(&mutex);
    for(int i = 0; i < lstTasks.size(); i++) {
        CommandTask tmp = lstTasks.at(i);
        if(!tmp.disposing) {
            task = tmp;
            task.disposing = true;
            lstTasks.replace(i, task);
            return true;
        }
    }
    return false;
}

int CommandTaskQueue::getUndisposedTaskCount()
{
    QMutexLocker locker(&mutex);
    int count = 0;
    for(int i = 0; i < lstTasks.size(); i++) {
        CommandTask tmp = lstTasks.at(i);
        if(!tmp.disposing) {
            count++;
        }
    }
    return count;
}

void CommandTaskQueue::remove(QString id)
{
    QMutexLocker locker(&mutex);
    for(int i = lstTasks.size(); i > 0; i--) {
        CommandTask task = lstTasks.at(i-1);
        if(task.id == id) {
            lstTasks.removeAt(i-1);
        }
    }
}
