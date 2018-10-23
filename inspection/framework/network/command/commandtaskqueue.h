#ifndef COMMANDTASKQUEUE_H
#define COMMANDTASKQUEUE_H

#include <QList>
#include "../../common/utils.h"
#include <QMutex>

class CommandTaskQueue
{
public:
    CommandTaskQueue();

   void add(CommandTask task);

   void remove(int pos);

   void clear();

   int count();

   CommandTask at(int pos);

   bool fetch(int pos, CommandTask task);

   bool getUndisposedTask(CommandTask& task);

   int getUndisposedTaskCount();

   void remove(QString id);

private:
    QList<CommandTask> lstTasks;

    QMutex mutex;
};

#endif // COMMANDTASKQUEUE_H
