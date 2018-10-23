/****************************************************************************
** Meta object code from reading C++ file 'upgradeThread.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/upgrade/upgradeThread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'upgradeThread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_UpgradeThread_t {
    QByteArrayData data[27];
    char stringdata[283];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_UpgradeThread_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_UpgradeThread_t qt_meta_stringdata_UpgradeThread = {
    {
QT_MOC_LITERAL(0, 0, 13),
QT_MOC_LITERAL(1, 14, 7),
QT_MOC_LITERAL(2, 22, 0),
QT_MOC_LITERAL(3, 23, 11),
QT_MOC_LITERAL(4, 35, 13),
QT_MOC_LITERAL(5, 49, 3),
QT_MOC_LITERAL(6, 53, 8),
QT_MOC_LITERAL(7, 62, 12),
QT_MOC_LITERAL(8, 75, 7),
QT_MOC_LITERAL(9, 83, 6),
QT_MOC_LITERAL(10, 90, 10),
QT_MOC_LITERAL(11, 101, 16),
QT_MOC_LITERAL(12, 118, 6),
QT_MOC_LITERAL(13, 125, 7),
QT_MOC_LITERAL(14, 133, 6),
QT_MOC_LITERAL(15, 140, 18),
QT_MOC_LITERAL(16, 159, 5),
QT_MOC_LITERAL(17, 165, 7),
QT_MOC_LITERAL(18, 173, 27),
QT_MOC_LITERAL(19, 201, 6),
QT_MOC_LITERAL(20, 208, 26),
QT_MOC_LITERAL(21, 235, 5),
QT_MOC_LITERAL(22, 241, 7),
QT_MOC_LITERAL(23, 249, 10),
QT_MOC_LITERAL(24, 260, 7),
QT_MOC_LITERAL(25, 268, 6),
QT_MOC_LITERAL(26, 275, 7)
    },
    "UpgradeThread\0process\0\0pkgFullPath\0"
    "installStatus\0ret\0response\0occuredError\0"
    "objects\0errMsg\0restartApp\0sendInstallError\0"
    "reason\0errCode\0doTask\0recvInstallProcess\0"
    "sopid\0pkgPath\0CPackageInfo::PackageStatus\0"
    "status\0CPackageInfo::PackageError\0"
    "error\0percent\0doTaskOnce\0addTask\0"
    "pkgMd5\0pkgSha1"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_UpgradeThread[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   59,    2, 0x06 /* Public */,
       6,    1,   66,    2, 0x06 /* Public */,
       7,    3,   69,    2, 0x06 /* Public */,
      10,    0,   76,    2, 0x06 /* Public */,
      11,    2,   77,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      14,    0,   82,    2, 0x08 /* Private */,
      15,    5,   83,    2, 0x08 /* Private */,
      23,    0,   94,    2, 0x09 /* Protected */,

 // methods: name, argc, parameters, tag, flags
      24,    3,   95,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::Int,    3,    4,    5,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Int,    8,    9,    5,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   12,   13,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, 0x80000000 | 18, 0x80000000 | 20, QMetaType::Int,   16,   17,   19,   21,   22,
    QMetaType::Void,

 // methods: parameters
    QMetaType::Bool, QMetaType::QString, QMetaType::QString, QMetaType::QString,    3,   25,   26,

       0        // eod
};

void UpgradeThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        UpgradeThread *_t = static_cast<UpgradeThread *>(_o);
        switch (_id) {
        case 0: _t->process((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 1: _t->response((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->occuredError((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 3: _t->restartApp(); break;
        case 4: _t->sendInstallError((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: _t->doTask(); break;
        case 6: _t->recvInstallProcess((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< CPackageInfo::PackageStatus(*)>(_a[3])),(*reinterpret_cast< CPackageInfo::PackageError(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5]))); break;
        case 7: _t->doTaskOnce(); break;
        case 8: { bool _r = _t->addTask((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 2:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< CPackageInfo::PackageStatus >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (UpgradeThread::*_t)(const QString , int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&UpgradeThread::process)) {
                *result = 0;
            }
        }
        {
            typedef void (UpgradeThread::*_t)(const QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&UpgradeThread::response)) {
                *result = 1;
            }
        }
        {
            typedef void (UpgradeThread::*_t)(const QString & , const QString & , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&UpgradeThread::occuredError)) {
                *result = 2;
            }
        }
        {
            typedef void (UpgradeThread::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&UpgradeThread::restartApp)) {
                *result = 3;
            }
        }
        {
            typedef void (UpgradeThread::*_t)(const QString , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&UpgradeThread::sendInstallError)) {
                *result = 4;
            }
        }
    }
}

const QMetaObject UpgradeThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_UpgradeThread.data,
      qt_meta_data_UpgradeThread,  qt_static_metacall, 0, 0}
};


const QMetaObject *UpgradeThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UpgradeThread::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_UpgradeThread.stringdata))
        return static_cast<void*>(const_cast< UpgradeThread*>(this));
    return QThread::qt_metacast(_clname);
}

int UpgradeThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void UpgradeThread::process(const QString _t1, int _t2, int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void UpgradeThread::response(const QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void UpgradeThread::occuredError(const QString & _t1, const QString & _t2, int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void UpgradeThread::restartApp()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void UpgradeThread::sendInstallError(const QString _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_END_MOC_NAMESPACE
