/****************************************************************************
** Meta object code from reading C++ file 'localcachemanager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "localcache/localcachemanager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'localcachemanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_LocalCacheManager_t {
    QByteArrayData data[21];
    char stringdata[246];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LocalCacheManager_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LocalCacheManager_t qt_meta_stringdata_LocalCacheManager = {
    {
QT_MOC_LITERAL(0, 0, 17),
QT_MOC_LITERAL(1, 18, 18),
QT_MOC_LITERAL(2, 37, 0),
QT_MOC_LITERAL(3, 38, 7),
QT_MOC_LITERAL(4, 46, 6),
QT_MOC_LITERAL(5, 53, 6),
QT_MOC_LITERAL(6, 60, 5),
QT_MOC_LITERAL(7, 66, 11),
QT_MOC_LITERAL(8, 78, 3),
QT_MOC_LITERAL(9, 82, 21),
QT_MOC_LITERAL(10, 104, 14),
QT_MOC_LITERAL(11, 119, 17),
QT_MOC_LITERAL(12, 137, 4),
QT_MOC_LITERAL(13, 142, 8),
QT_MOC_LITERAL(14, 151, 11),
QT_MOC_LITERAL(15, 163, 10),
QT_MOC_LITERAL(16, 174, 14),
QT_MOC_LITERAL(17, 189, 10),
QT_MOC_LITERAL(18, 200, 11),
QT_MOC_LITERAL(19, 212, 21),
QT_MOC_LITERAL(20, 234, 11)
    },
    "LocalCacheManager\0onResultOfResponse\0"
    "\0objects\0action\0result\0error\0DbusPackage\0"
    "map\0sendNetworkResultSlot\0submitProgress\0"
    "DataOperationType\0type\0progress\0"
    "description\0binaryType\0submitFinished\0"
    "submitStop\0submitStart\0networkConnectChanged\0"
    "networkType"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LocalCacheManager[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    5,   49,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    5,   60,    2, 0x0a /* Public */,
      10,    4,   71,    2, 0x0a /* Public */,
      16,    3,   80,    2, 0x0a /* Public */,
      17,    2,   87,    2, 0x0a /* Public */,
      18,    2,   92,    2, 0x0a /* Public */,
      19,    1,   97,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Bool, QMetaType::Int, 0x80000000 | 7,    3,    4,    5,    6,    8,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Bool, QMetaType::Int, 0x80000000 | 7,    3,    4,    5,    6,    8,
    QMetaType::Void, 0x80000000 | 11, QMetaType::Int, QMetaType::QString, QMetaType::Bool,   12,   13,   14,   15,
    QMetaType::Void, 0x80000000 | 11, QMetaType::Bool, QMetaType::Bool,   12,    5,   15,
    QMetaType::Void, 0x80000000 | 11, QMetaType::Bool,   12,   15,
    QMetaType::Void, 0x80000000 | 11, QMetaType::Bool,   12,   15,
    QMetaType::Void, QMetaType::QString,   20,

       0        // eod
};

void LocalCacheManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        LocalCacheManager *_t = static_cast<LocalCacheManager *>(_o);
        switch (_id) {
        case 0: _t->onResultOfResponse((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< DbusPackage(*)>(_a[5]))); break;
        case 1: _t->sendNetworkResultSlot((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< DbusPackage(*)>(_a[5]))); break;
        case 2: _t->submitProgress((*reinterpret_cast< DataOperationType(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4]))); break;
        case 3: _t->submitFinished((*reinterpret_cast< DataOperationType(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 4: _t->submitStop((*reinterpret_cast< DataOperationType(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 5: _t->submitStart((*reinterpret_cast< DataOperationType(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 6: _t->networkConnectChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 4:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< DbusPackage >(); break;
            }
            break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 4:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< DbusPackage >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (LocalCacheManager::*_t)(QString , QString , bool , int , DbusPackage );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&LocalCacheManager::onResultOfResponse)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject LocalCacheManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_LocalCacheManager.data,
      qt_meta_data_LocalCacheManager,  qt_static_metacall, 0, 0}
};


const QMetaObject *LocalCacheManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LocalCacheManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_LocalCacheManager.stringdata))
        return static_cast<void*>(const_cast< LocalCacheManager*>(this));
    return QObject::qt_metacast(_clname);
}

int LocalCacheManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void LocalCacheManager::onResultOfResponse(QString _t1, QString _t2, bool _t3, int _t4, DbusPackage _t5)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
