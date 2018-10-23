/****************************************************************************
** Meta object code from reading C++ file 'syncmanager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "sync/syncmanager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'syncmanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_SyncManager_t {
    QByteArrayData data[16];
    char stringdata[200];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SyncManager_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SyncManager_t qt_meta_stringdata_SyncManager = {
    {
QT_MOC_LITERAL(0, 0, 11),
QT_MOC_LITERAL(1, 12, 20),
QT_MOC_LITERAL(2, 33, 0),
QT_MOC_LITERAL(3, 34, 19),
QT_MOC_LITERAL(4, 54, 15),
QT_MOC_LITERAL(5, 70, 6),
QT_MOC_LITERAL(6, 77, 7),
QT_MOC_LITERAL(7, 85, 3),
QT_MOC_LITERAL(8, 89, 18),
QT_MOC_LITERAL(9, 108, 6),
QT_MOC_LITERAL(10, 115, 5),
QT_MOC_LITERAL(11, 121, 11),
QT_MOC_LITERAL(12, 133, 21),
QT_MOC_LITERAL(13, 155, 12),
QT_MOC_LITERAL(14, 168, 17),
QT_MOC_LITERAL(15, 186, 13)
    },
    "SyncManager\0onSyncFinishedSignal\0\0"
    "BusinessMessageType\0businessMsgType\0"
    "action\0objects\0map\0onResultOfResponse\0"
    "result\0error\0DbusPackage\0sendNetworkResultSlot\0"
    "syncFinished\0DataOperationType\0"
    "operationType"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SyncManager[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    4,   34,    2, 0x06 /* Public */,
       8,    5,   43,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      12,    5,   54,    2, 0x0a /* Public */,
      13,    5,   65,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::QString, QMetaType::QString, QMetaType::QVariantMap,    4,    5,    6,    7,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Bool, QMetaType::Int, 0x80000000 | 11,    6,    5,    9,   10,    7,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Bool, QMetaType::Int, 0x80000000 | 11,    6,    5,    9,   10,    7,
    QMetaType::Void, 0x80000000 | 14, 0x80000000 | 3, QMetaType::QString, QMetaType::QString, QMetaType::QVariantMap,   15,    4,    5,    6,    7,

       0        // eod
};

void SyncManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SyncManager *_t = static_cast<SyncManager *>(_o);
        switch (_id) {
        case 0: _t->onSyncFinishedSignal((*reinterpret_cast< const BusinessMessageType(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< QVariantMap(*)>(_a[4]))); break;
        case 1: _t->onResultOfResponse((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< DbusPackage(*)>(_a[5]))); break;
        case 2: _t->sendNetworkResultSlot((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< DbusPackage(*)>(_a[5]))); break;
        case 3: _t->syncFinished((*reinterpret_cast< const DataOperationType(*)>(_a[1])),(*reinterpret_cast< const BusinessMessageType(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4])),(*reinterpret_cast< const QVariantMap(*)>(_a[5]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (SyncManager::*_t)(const BusinessMessageType , const QString , const QString , QVariantMap );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SyncManager::onSyncFinishedSignal)) {
                *result = 0;
            }
        }
        {
            typedef void (SyncManager::*_t)(QString , QString , bool , int , DbusPackage );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SyncManager::onResultOfResponse)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject SyncManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_SyncManager.data,
      qt_meta_data_SyncManager,  qt_static_metacall, 0, 0}
};


const QMetaObject *SyncManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SyncManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SyncManager.stringdata))
        return static_cast<void*>(const_cast< SyncManager*>(this));
    return QObject::qt_metacast(_clname);
}

int SyncManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void SyncManager::onSyncFinishedSignal(const BusinessMessageType _t1, const QString _t2, const QString _t3, QVariantMap _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SyncManager::onResultOfResponse(QString _t1, QString _t2, bool _t3, int _t4, DbusPackage _t5)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
