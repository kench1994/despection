#ifndef HEARTBEATMANAGER_H
#define HEARTBEATMANAGER_H

#include <QObject>
#include "calarmmanager.h"

class HeartbeatManager : public QObject
{
    Q_OBJECT
public:

    /**
     * 构造函数
     */
    explicit HeartbeatManager(QObject *parent = 0);

    /**
     * 析构函数
     */
    ~HeartbeatManager();

    /**
     * 开始心跳处理
     */
    void start();

    /**
     * 停止心跳处理
     */
    void stop();

    /**
     * 获取时间间隔
     * @return 时间间隔
     */
    int getInterval();

    /**
     * 设置时间间隔
     * @param interval 时间间隔
     */
    void setInterval(int interval);

    /**
     * 重置心跳时间间隔
     */
    void resetInterval();

    /**
     * 调节时间间隔
     * @param beIncrease true 增大间隔 false 减少间隔
     */
    void adjust(bool beIncrease);

    /**
     * 启动定时器
     *
     */
    void startAlarm();

    /**
     * 停止定时器
     *
     */
    void stopAlarm();

signals:

public slots:

private:
    /**
        * 运行状态
        */
       bool running;

       /**
        * 心跳时间间隔
        */
       int interval;

       /**
        * 时间间隔调整完成标志
        */
       bool adjustIntervalComplete;

       /**
        * 定时器类指针
        */
       CAlarmManager *alarmManager;

};

#endif // HEARTBEATMANAGER_H
