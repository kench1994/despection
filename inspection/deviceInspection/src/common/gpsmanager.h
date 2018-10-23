#ifndef GPSMANAGER_H
#define GPSMANAGER_H

#include <QObject>
#include <QTimer>
#include <QDateTime>
#include <QDBusInterface>
#include <QDBusReply>
#include <QGeoPositionInfoSource>
#include <QGeoSatelliteInfoSource>

class GpsManager : public QObject
{
    Q_OBJECT
public:
    explicit GpsManager(QObject *parent = 0);

public:
    void initDBusLocate();
    void initPosSource();
    void initSatSource();

signals:
    void sglGpsPowerChanged(const bool infoFlag);

    void sglPositionError(const QString &errStr);
    void sglPositionChanged(const QString &infoLon, const QString &infoLat);
    void sglTimeChanged(const QString &infoTmStr);
    void sglStatusChanged(const QString &infoStatus);

    void sglSatError(const QString &errStr);
    void sglSatNumInViewChanged(const QString &infoNum);
    void sglSatNumInUseChanged(const QString &infoNum);
    void sglSatValuesChanged(QList<int> infoIDList, QList<int> infoValList);

public slots:
    void slotSwitchChanged(bool flagLocate, bool flagGps, bool flagBase);
    bool slotGetGpsPowerFlag();

    void slotSyncStart();
    void slotSyncStop();

    void slotPositionUpdated(const QGeoPositionInfo &posInfo);
    void slotPositionError(QGeoPositionInfoSource::Error posError);

    void slotSatInUseUpdated(const QList<QGeoSatelliteInfo> &satList);
    void slotSatInViewUpdated(const QList<QGeoSatelliteInfo> &satList);
    void slotSatError(QGeoSatelliteInfoSource::Error satError);

private:
    void resetSatValList();
    int degreeOfSatSglStrength(const int sglStrength);

private:
    QDBusInterface *m_dbusLocate;
    bool m_flagLocate;
    bool m_flagGps;
    bool m_flagBase;

    QGeoPositionInfoSource *m_posSrc;
    bool m_bLocateFlag;

    QGeoSatelliteInfoSource *m_satSrc;
    QList<int> m_satIDList;
    QList<int> m_satValList;

    QDateTime m_tmBegin;
    QDateTime m_tmEnd;
};

#endif // GPSMANAGER_H
