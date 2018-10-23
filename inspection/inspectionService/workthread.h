#ifndef WORKTHREAD_H
#define WORKTHREAD_H

#include <QThread>
#include "inspectioninterface.h"

class WorkThread : public QThread
{
    Q_OBJECT
public:
    explicit WorkThread(QObject *parent = 0);

signals:

public slots:

protected:
    virtual void run();

private:
    InspectionInterface *m_pInspectionInterface;
};

#endif // WORKTHREAD_H
