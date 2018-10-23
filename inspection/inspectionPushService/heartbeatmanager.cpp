#include "heartbeatmanager.h"
#include "push_define.h"
#include <QDebug>

/**
 * 构造函数
 */
HeartbeatManager::HeartbeatManager(QObject *parent) :QObject(parent),
    running(false), interval(HEARTBEA_INTERVAL_MIN),adjustIntervalComplete(false)
{
    qDebug() << "------------HeartbeatManager::HeartbeatManager--------------------";
    alarmManager = new CAlarmManager(this);
    alarmManager->cancelAllAlarmsBySopid();
}

/**
 * 析构函数
 */
HeartbeatManager::~HeartbeatManager()
{
    delete alarmManager;
}


/**
 * 获取时间间隔
 * @return 时间间隔
 */
int HeartbeatManager::getInterval() {
    return interval;
}

/**
 * 设置时间间隔
 * @param interval 时间间隔
 */
void HeartbeatManager::setInterval(int interval) {
    if(interval == this->interval) {
        return;
    }
    this->interval = interval;
    /**
     * restart alarm notification when running
     */
    if(running) {
        stop();
        start();
    }
}


/**
 * 重置心跳时间间隔
 */
void HeartbeatManager::resetInterval() {
    this->interval = HEARTBEA_INTERVAL_MIN;
    adjustIntervalComplete = false;
}


/**
 * 调节时间间隔
 * @param beIncrease true 增大间隔 false 减少间隔
 */
void HeartbeatManager::adjust(bool beIncrease) {
    /**
     * 增加时间间隔的时候调已经完成则忽略
     */
    if(adjustIntervalComplete && beIncrease) {
        return;
    }
    /**
     * 当降低时间间隔时，认为间隔已经调整完成，无法再增加新的时间间隔了
     */
    if(!beIncrease) {
        adjustIntervalComplete = true;
    }
    int tmp;
    if(beIncrease) {
        tmp = interval + 2;
        tmp = tmp > HEARTBEA_INTERVAL_MAX ? HEARTBEA_INTERVAL_MAX : tmp;
    }else{
        tmp = interval - 1;
        tmp = tmp < HEARTBEA_INTERVAL_MIN ? HEARTBEA_INTERVAL_MIN : tmp;
    }
    setInterval(tmp);
    qDebug() << "-------------HeartbeatManager::adjust----------adjust finished:  " <<  tmp;
}

void HeartbeatManager::start() {
    qDebug() << Q_FUNC_INFO <<"   -start alarm:  " <<  interval;
    /* 由于Alarm是单次的，所以每次又要重新定时
    if(running) {
        return;
    }
    */
    startAlarm();
    running = true;
}

void HeartbeatManager::stop(){
    qDebug() << "-------------HeartbeatManager::stop----------stop alarm:  " <<  interval;
    if(!running) {
        return;
    }
    stopAlarm();
    running = false;
}

void HeartbeatManager::startAlarm()
{
    alarmManager->cancelAllAlarmsBySopid();
    QDateTime firstTime = QDateTime::currentDateTime();
    alarmManager->setAlarm(firstTime, getInterval(), false, "inspectionPushService");
}

void HeartbeatManager::stopAlarm()
{
    alarmManager->cancelAllAlarmsBySopid();
}

