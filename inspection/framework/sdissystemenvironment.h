#ifndef SDISSYSTEMENVIRONMENT_H
#define SDISSYSTEMENVIRONMENT_H

#include <QObject>

class SDISSystemEnvironment : public QObject
{
    Q_OBJECT

public:
    /**
     * 获取设备ID
     */
    static QString getDeviceID();
signals:

public slots:

private:
    /**
     * 设备唯一ID
     */
    static QString deviceID;
};

#endif // SDISSYSTEMENVIRONMENT_H
