/****************************************************************************
** Meta object code from reading C++ file 'gpsmanager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/common/gpsmanager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gpsmanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_GpsManager_t {
    QByteArrayData data[41];
    char stringdata[593];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GpsManager_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GpsManager_t qt_meta_stringdata_GpsManager = {
    {
QT_MOC_LITERAL(0, 0, 10),
QT_MOC_LITERAL(1, 11, 18),
QT_MOC_LITERAL(2, 30, 0),
QT_MOC_LITERAL(3, 31, 8),
QT_MOC_LITERAL(4, 40, 16),
QT_MOC_LITERAL(5, 57, 6),
QT_MOC_LITERAL(6, 64, 18),
QT_MOC_LITERAL(7, 83, 7),
QT_MOC_LITERAL(8, 91, 7),
QT_MOC_LITERAL(9, 99, 14),
QT_MOC_LITERAL(10, 114, 9),
QT_MOC_LITERAL(11, 124, 16),
QT_MOC_LITERAL(12, 141, 10),
QT_MOC_LITERAL(13, 152, 11),
QT_MOC_LITERAL(14, 164, 22),
QT_MOC_LITERAL(15, 187, 7),
QT_MOC_LITERAL(16, 195, 21),
QT_MOC_LITERAL(17, 217, 19),
QT_MOC_LITERAL(18, 237, 10),
QT_MOC_LITERAL(19, 248, 10),
QT_MOC_LITERAL(20, 259, 11),
QT_MOC_LITERAL(21, 271, 17),
QT_MOC_LITERAL(22, 289, 10),
QT_MOC_LITERAL(23, 300, 7),
QT_MOC_LITERAL(24, 308, 8),
QT_MOC_LITERAL(25, 317, 19),
QT_MOC_LITERAL(26, 337, 13),
QT_MOC_LITERAL(27, 351, 12),
QT_MOC_LITERAL(28, 364, 19),
QT_MOC_LITERAL(29, 384, 16),
QT_MOC_LITERAL(30, 401, 7),
QT_MOC_LITERAL(31, 409, 17),
QT_MOC_LITERAL(32, 427, 29),
QT_MOC_LITERAL(33, 457, 8),
QT_MOC_LITERAL(34, 466, 19),
QT_MOC_LITERAL(35, 486, 24),
QT_MOC_LITERAL(36, 511, 7),
QT_MOC_LITERAL(37, 519, 20),
QT_MOC_LITERAL(38, 540, 12),
QT_MOC_LITERAL(39, 553, 30),
QT_MOC_LITERAL(40, 584, 8)
    },
    "GpsManager\0sglGpsPowerChanged\0\0infoFlag\0"
    "sglPositionError\0errStr\0sglPositionChanged\0"
    "infoLon\0infoLat\0sglTimeChanged\0infoTmStr\0"
    "sglStatusChanged\0infoStatus\0sglSatError\0"
    "sglSatNumInViewChanged\0infoNum\0"
    "sglSatNumInUseChanged\0sglSatValuesChanged\0"
    "QList<int>\0infoIDList\0infoValList\0"
    "slotSwitchChanged\0flagLocate\0flagGps\0"
    "flagBase\0slotGetGpsPowerFlag\0slotSyncStart\0"
    "slotSyncStop\0slotPositionUpdated\0"
    "QGeoPositionInfo\0posInfo\0slotPositionError\0"
    "QGeoPositionInfoSource::Error\0posError\0"
    "slotSatInUseUpdated\0QList<QGeoSatelliteInfo>\0"
    "satList\0slotSatInViewUpdated\0slotSatError\0"
    "QGeoSatelliteInfoSource::Error\0satError"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GpsManager[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       9,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  104,    2, 0x06 /* Public */,
       4,    1,  107,    2, 0x06 /* Public */,
       6,    2,  110,    2, 0x06 /* Public */,
       9,    1,  115,    2, 0x06 /* Public */,
      11,    1,  118,    2, 0x06 /* Public */,
      13,    1,  121,    2, 0x06 /* Public */,
      14,    1,  124,    2, 0x06 /* Public */,
      16,    1,  127,    2, 0x06 /* Public */,
      17,    2,  130,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      21,    3,  135,    2, 0x0a /* Public */,
      25,    0,  142,    2, 0x0a /* Public */,
      26,    0,  143,    2, 0x0a /* Public */,
      27,    0,  144,    2, 0x0a /* Public */,
      28,    1,  145,    2, 0x0a /* Public */,
      31,    1,  148,    2, 0x0a /* Public */,
      34,    1,  151,    2, 0x0a /* Public */,
      37,    1,  154,    2, 0x0a /* Public */,
      38,    1,  157,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    7,    8,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void, QMetaType::QString,   12,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QString,   15,
    QMetaType::Void, QMetaType::QString,   15,
    QMetaType::Void, 0x80000000 | 18, 0x80000000 | 18,   19,   20,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool, QMetaType::Bool, QMetaType::Bool,   22,   23,   24,
    QMetaType::Bool,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 29,   30,
    QMetaType::Void, 0x80000000 | 32,   33,
    QMetaType::Void, 0x80000000 | 35,   36,
    QMetaType::Void, 0x80000000 | 35,   36,
    QMetaType::Void, 0x80000000 | 39,   40,

       0        // eod
};

void GpsManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        GpsManager *_t = static_cast<GpsManager *>(_o);
        switch (_id) {
        case 0: _t->sglGpsPowerChanged((*reinterpret_cast< const bool(*)>(_a[1]))); break;
        case 1: _t->sglPositionError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->sglPositionChanged((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 3: _t->sglTimeChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->sglStatusChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->sglSatError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->sglSatNumInViewChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 7: _t->sglSatNumInUseChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 8: _t->sglSatValuesChanged((*reinterpret_cast< QList<int>(*)>(_a[1])),(*reinterpret_cast< QList<int>(*)>(_a[2]))); break;
        case 9: _t->slotSwitchChanged((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 10: { bool _r = _t->slotGetGpsPowerFlag();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 11: _t->slotSyncStart(); break;
        case 12: _t->slotSyncStop(); break;
        case 13: _t->slotPositionUpdated((*reinterpret_cast< const QGeoPositionInfo(*)>(_a[1]))); break;
        case 14: _t->slotPositionError((*reinterpret_cast< QGeoPositionInfoSource::Error(*)>(_a[1]))); break;
        case 15: _t->slotSatInUseUpdated((*reinterpret_cast< const QList<QGeoSatelliteInfo>(*)>(_a[1]))); break;
        case 16: _t->slotSatInViewUpdated((*reinterpret_cast< const QList<QGeoSatelliteInfo>(*)>(_a[1]))); break;
        case 17: _t->slotSatError((*reinterpret_cast< QGeoSatelliteInfoSource::Error(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 8:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<int> >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (GpsManager::*_t)(const bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GpsManager::sglGpsPowerChanged)) {
                *result = 0;
            }
        }
        {
            typedef void (GpsManager::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GpsManager::sglPositionError)) {
                *result = 1;
            }
        }
        {
            typedef void (GpsManager::*_t)(const QString & , const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GpsManager::sglPositionChanged)) {
                *result = 2;
            }
        }
        {
            typedef void (GpsManager::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GpsManager::sglTimeChanged)) {
                *result = 3;
            }
        }
        {
            typedef void (GpsManager::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GpsManager::sglStatusChanged)) {
                *result = 4;
            }
        }
        {
            typedef void (GpsManager::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GpsManager::sglSatError)) {
                *result = 5;
            }
        }
        {
            typedef void (GpsManager::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GpsManager::sglSatNumInViewChanged)) {
                *result = 6;
            }
        }
        {
            typedef void (GpsManager::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GpsManager::sglSatNumInUseChanged)) {
                *result = 7;
            }
        }
        {
            typedef void (GpsManager::*_t)(QList<int> , QList<int> );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GpsManager::sglSatValuesChanged)) {
                *result = 8;
            }
        }
    }
}

const QMetaObject GpsManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_GpsManager.data,
      qt_meta_data_GpsManager,  qt_static_metacall, 0, 0}
};


const QMetaObject *GpsManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GpsManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GpsManager.stringdata))
        return static_cast<void*>(const_cast< GpsManager*>(this));
    return QObject::qt_metacast(_clname);
}

int GpsManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    }
    return _id;
}

// SIGNAL 0
void GpsManager::sglGpsPowerChanged(const bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void GpsManager::sglPositionError(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void GpsManager::sglPositionChanged(const QString & _t1, const QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void GpsManager::sglTimeChanged(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void GpsManager::sglStatusChanged(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void GpsManager::sglSatError(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void GpsManager::sglSatNumInViewChanged(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void GpsManager::sglSatNumInUseChanged(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void GpsManager::sglSatValuesChanged(QList<int> _t1, QList<int> _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}
QT_END_MOC_NAMESPACE
