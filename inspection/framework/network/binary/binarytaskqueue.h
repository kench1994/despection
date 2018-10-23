#ifndef BINARYTASKQUEUE_H
#define BINARYTASKQUEUE_H

#include <QList>
#include <QMutex>

#include "../../common/utils.h"
class BinaryTaskQueue
{
public:
    BinaryTaskQueue();
    void add(BinaryTask task);
    void remove(int pos);
    void remove(QString uuid);
    void clear();
    int count();
    BinaryTask at(int pos);
    bool get(QString uuid, BinaryTask& task);
    bool fetch(int pos, BinaryTask& task);
    bool getUndisposedTask(BinaryTask& task);
    int getUndisposedTaskCount();
private:
    QList<BinaryTask> lstTasks;
    QMutex mutex;
};

#endif // BINARYTASKQUEUE_H
