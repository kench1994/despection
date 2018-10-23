#include "gpsmanager.h"

#include <QDebug>
GpsManager::GpsManager(QObject *parent) :
    QObject(parent),
    m_dbusLocate(NULL),
    m_flagLocate(false),
    m_flagGps(false),
    m_flagBase(false),
    m_posSrc(NULL),
    m_bLocateFlag(false),
    m_satSrc(NULL) {
    m_tmBegin = QDateTime::currentDateTime();
    m_tmEnd = m_tmBegin;

    resetSatValList();

    initDBusLocate();
    initPosSource();
    initSatSource();
}

void GpsManager::resetSatValList()
{
    m_satIDList.clear();
    for(int i=0; i<12; i++){
        m_satIDList << 0;
    }

    m_satValList.clear();
    for(int i=0; i<12; i++){
        m_satValList << 0;
    }
}

void GpsManager::initDBusLocate()
{
    m_dbusLocate = new QDBusInterface("org.freedesktop.Geoclue.Master",
                                       "/org/freedesktop/Geoclue/Master",
                                       "org.freedesktop.Geoclue.Master",
                                       QDBusConnection::sessionBus(),
                                       this);
    if(m_dbusLocate->isValid() != true) {
        qDebug() << "@@@ @@@ @@@ GpsManager::initDBusLocate : m_dbusLocate is not valid.";
        qDebug() << "@@@ @@@ @@@ GpsManager::initDBusLocate :try systembus.";
        m_dbusLocate = new QDBusInterface("org.freedesktop.Geoclue.Master",
                                       "/org/freedesktop/Geoclue/Master",
                                       "org.freedesktop.Geoclue.Master",
                                       QDBusConnection::systemBus(),
                                       this);
    }
    if(m_dbusLocate->isValid() != true) {
        qDebug() << "@@@ @@@ @@@ GpsManager::initDBusLocate : m_dbusLocate is not valid on systembus.";
        return;
    }
    QObject::connect(m_dbusLocate,
                     SIGNAL(SwitchChanged(bool, bool, bool)),
                     this,
                     SLOT(slotSwitchChanged(bool, bool, bool)));

    QDBusMessage reply = m_dbusLocate->call("GetSwitch");
    if(reply.type() == QDBusMessage::ErrorMessage) {
        qDebug() << "@@@ @@@ @@@ GpsManager::initDBusLocate : reply : error : " << reply.errorMessage();
        return;
    }
    if(reply.arguments().length() < 3) {
        qDebug() << "@@@ @@@ @@@ GpsManager::initDBusLocate : reply : param error";
        return;
    }

    m_flagLocate = reply.arguments().at(0).toBool();
    m_flagGps = reply.arguments().at(1).toBool();
    m_flagBase = reply.arguments().at(2).toBool();

    qDebug() << "@@@ @@@ @@@ GpsManager::initDBusLocate : successful";
}

void GpsManager::slotSwitchChanged(bool flagLocate, bool flagGps, bool flagBase)
{
    m_flagLocate = flagLocate;
    m_flagGps = flagGps;
    m_flagBase = flagBase;

    bool flag = m_flagGps;
    emit sglGpsPowerChanged(flag);
}

bool GpsManager::slotGetGpsPowerFlag()
{
    bool flag = m_flagGps;
    return flag;
}

int GpsManager::degreeOfSatSglStrength(const int sglStrength)
{
    int degree = sglStrength;

    if(degree < 0)
        degree = 0;
    else if(degree > 100)
        degree = 100;

    return degree;
}

void GpsManager::slotSyncStart()
{
    QString infoStatus = QString("定位中");// 未定位|定位中|已定位
    emit sglStatusChanged(infoStatus);

    QString infoTmStr = "";
    emit sglTimeChanged(infoTmStr);

    QString infoLon = QString("");// 经度
    QString infoLat = QString("");// 纬度
    emit sglPositionChanged(infoLon, infoLat);

    QString infoNumInView = "0";
    emit sglSatNumInViewChanged(infoNumInView);
    QString infoNumInUse = "0";
    emit sglSatNumInUseChanged(infoNumInUse);

    resetSatValList();
    emit sglSatValuesChanged(m_satIDList, m_satValList);

    m_tmBegin = QDateTime::currentDateTime();
    m_tmEnd = m_tmBegin;
    /////////////////////////////////////////////////////////////////////////////////
    m_bLocateFlag = false;
    m_posSrc->startUpdates();
    m_satSrc->startUpdates();
}

void GpsManager::slotSyncStop()
{
    QString infoStatus = QString("未定位");// 未定位|定位中|已定位
    emit sglStatusChanged(infoStatus);

    QString infoTmStr = "";
    emit sglTimeChanged(infoTmStr);

    QString infoLon = QString("");// 经度
    QString infoLat = QString("");// 纬度
    emit sglPositionChanged(infoLon, infoLat);

    QString infoNumInView = "";
    emit sglSatNumInViewChanged(infoNumInView);
    QString infoNumInUse = "";
    emit sglSatNumInUseChanged(infoNumInUse);

    resetSatValList();
    emit sglSatValuesChanged(m_satIDList, m_satValList);

    /////////////////////////////////////////////////////////////////////////////////
    m_posSrc->stopUpdates();
    m_satSrc->stopUpdates();
    m_bLocateFlag = false;
}

void GpsManager::initPosSource()
{
    QStringList srcList = QGeoPositionInfoSource::availableSources();
    qDebug() << "@@@ @@@ @@@ GpsManager: QGeoPositionInfoSource : " << srcList;

    m_posSrc = QGeoPositionInfoSource::createDefaultSource(this);
    if(!m_posSrc)
    {
        qDebug() << "@@@ @@@ @@@ GpsManager::initPosSource : error";
        return;
    }

    m_posSrc->setPreferredPositioningMethods(QGeoPositionInfoSource::AllPositioningMethods);
//    m_posSrc->setUpdateInterval(1000);
    connect(m_posSrc,
            SIGNAL(positionUpdated(QGeoPositionInfo)),
            this,
            SLOT(slotPositionUpdated(QGeoPositionInfo)));
    connect(m_posSrc,
            SIGNAL(error(QGeoPositionInfoSource::Error)),
            this,
            SLOT(slotPositionError(QGeoPositionInfoSource::Error)));

    qDebug() << "@@@ @@@ @@@ GpsManager::initPosSource : successful";
}

void GpsManager::slotPositionUpdated(const QGeoPositionInfo &posInfo)
{
    QString strLon = QString("%1").arg(posInfo.coordinate().longitude());
    QString strLat = QString("%1").arg(posInfo.coordinate().latitude());
    emit sglPositionChanged(strLon, strLat);

    QDateTime tmStamp = posInfo.timestamp();
    QString infoTmStr = "";
    infoTmStr = tmStamp.toString("yyyy-MM-dd  hh:mm:ss");
    emit sglTimeChanged(infoTmStr);

    if(m_bLocateFlag != true)
    {
//        QString infoStatus = QString("已定位");// 未定位|定位中|已定位
//        emit sglStatusChanged(infoStatus);
//        m_bLocateFlag = true;

        m_tmEnd = QDateTime::currentDateTime();
        qint64 secElapse = m_tmBegin.secsTo(m_tmEnd);
        int mmNum = secElapse/60;
        QString mmNumStr = QString::number(mmNum).rightJustified(2, '0');
        int ssNum = secElapse%60;
        QString ssNumStr = QString::number(ssNum).rightJustified(2, '0');
        if(mmNum > 99) {
            mmNumStr = "EE";
            ssNumStr = "EE";
        }
        QString infoStatus = QString("已定位  （%1分%2秒）").arg(mmNumStr).arg(ssNumStr);// 未定位|定位中|已定位
        emit sglStatusChanged(infoStatus);
        m_bLocateFlag = true;
    }
}

void GpsManager::slotPositionError(QGeoPositionInfoSource::Error posError)
{
    QString errStr = QString("PosErr:%1").arg(posError);
    qDebug() << "@@@ @@@ @@@ GpsManager::slotPositionError : " << errStr;

    emit sglPositionError(errStr);
}

void GpsManager::initSatSource()
{
    QStringList srcList = QGeoSatelliteInfoSource::availableSources();
    qDebug() << "@@@ @@@ @@@ GpsManager: QGeoSatelliteInfoSource : " << srcList;

    m_satSrc = QGeoSatelliteInfoSource::createDefaultSource(this);
    if(!m_satSrc)
    {
        qDebug() << "@@@ @@@ @@@ GpsManager::initSatSource : error";
        return;
    }

//    m_satSrc->setUpdateInterval(1000);
    connect(m_satSrc,
            SIGNAL(satellitesInViewUpdated(QList<QGeoSatelliteInfo>)),
            this,
            SLOT(slotSatInViewUpdated(QList<QGeoSatelliteInfo>)));
    connect(m_satSrc,
            SIGNAL(satellitesInUseUpdated(QList<QGeoSatelliteInfo>)),
            this,
            SLOT(slotSatInUseUpdated(QList<QGeoSatelliteInfo>)));
    connect(m_satSrc,
            SIGNAL(error(QGeoSatelliteInfoSource::Error)),
            this,
            SLOT(slotSatError(QGeoSatelliteInfoSource::Error)));

    qDebug() << "@@@ @@@ @@@ GpsManager::initSatSource : successful";
}

void GpsManager::slotSatInViewUpdated(const QList<QGeoSatelliteInfo> &satList)
{
    qDebug() << "@@@ @@@ @@@ GpsManager::slotSatInViewUpdated : satList :" << satList.count();
    QString strNum = QString("%1").arg(satList.count());
    emit sglSatNumInViewChanged(strNum);

    resetSatValList();

    int valCnt = m_satValList.count();
    int satCnt = satList.count();
    int syncNum = satCnt<valCnt?satCnt:valCnt;
    for(int i=0; i<syncNum; i++)
    {
        m_satIDList[i] = satList[i].satelliteIdentifier();
        m_satValList[i] = degreeOfSatSglStrength(satList[i].signalStrength());
    }
    emit sglSatValuesChanged(m_satIDList, m_satValList);
}

void GpsManager::slotSatInUseUpdated(const QList<QGeoSatelliteInfo> &satList)
{
    qDebug() << "@@@ @@@ @@@ GpsManager::slotSatInUseUpdated : satList : " << satList.count();
    QString strNum = QString("%1").arg(satList.count());
    emit sglSatNumInUseChanged(strNum);
}

void GpsManager::slotSatError(QGeoSatelliteInfoSource::Error satError)
{
    QString errStr = QString("SatErr:%1").arg(satError);
    qDebug() << "@@@ @@@ @@@ GpsManager::slotSatError : " << errStr;

    emit sglSatError(errStr);
}
