/****************************************************************************
** Meta object code from reading C++ file 'pushmanager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "pushmanager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'pushmanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_PushManager_t {
    QByteArrayData data[20];
    char stringdata[252];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PushManager_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PushManager_t qt_meta_stringdata_PushManager = {
    {
QT_MOC_LITERAL(0, 0, 11),
QT_MOC_LITERAL(1, 12, 14),
QT_MOC_LITERAL(2, 27, 0),
QT_MOC_LITERAL(3, 28, 11),
QT_MOC_LITERAL(4, 40, 11),
QT_MOC_LITERAL(5, 52, 9),
QT_MOC_LITERAL(6, 62, 11),
QT_MOC_LITERAL(7, 74, 8),
QT_MOC_LITERAL(8, 83, 17),
QT_MOC_LITERAL(9, 101, 20),
QT_MOC_LITERAL(10, 122, 6),
QT_MOC_LITERAL(11, 129, 20),
QT_MOC_LITERAL(12, 150, 12),
QT_MOC_LITERAL(13, 163, 4),
QT_MOC_LITERAL(14, 168, 11),
QT_MOC_LITERAL(15, 180, 19),
QT_MOC_LITERAL(16, 200, 11),
QT_MOC_LITERAL(17, 212, 11),
QT_MOC_LITERAL(18, 224, 20),
QT_MOC_LITERAL(19, 245, 6)
    },
    "PushManager\0handlePushData\0\0DbusPackage\0"
    "dbusPackage\0startPush\0restartPush\0"
    "stopPush\0initCompleteInner\0"
    "serverConnectedInner\0result\0"
    "heartbeatResultInner\0onErrorInner\0"
    "type\0description\0handlePushDataInner\0"
    "DataPackage\0dataPackage\0networkStatusChanged\0"
    "status"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PushManager[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   64,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   67,    2, 0x0a /* Public */,
       6,    0,   68,    2, 0x0a /* Public */,
       7,    0,   69,    2, 0x0a /* Public */,
       8,    0,   70,    2, 0x0a /* Public */,
       9,    1,   71,    2, 0x0a /* Public */,
      11,    1,   74,    2, 0x0a /* Public */,
      12,    2,   77,    2, 0x0a /* Public */,
      15,    1,   82,    2, 0x0a /* Public */,
      18,    1,   85,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   10,
    QMetaType::Void, QMetaType::Bool,   10,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,   13,   14,
    QMetaType::Void, 0x80000000 | 16,   17,
    QMetaType::Void, QMetaType::QString,   19,

       0        // eod
};

void PushManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        PushManager *_t = static_cast<PushManager *>(_o);
        switch (_id) {
        case 0: _t->handlePushData((*reinterpret_cast< const DbusPackage(*)>(_a[1]))); break;
        case 1: _t->startPush(); break;
        case 2: _t->restartPush(); break;
        case 3: _t->stopPush(); break;
        case 4: _t->initCompleteInner(); break;
        case 5: _t->serverConnectedInner((*reinterpret_cast< const bool(*)>(_a[1]))); break;
        case 6: _t->heartbeatResultInner((*reinterpret_cast< const bool(*)>(_a[1]))); break;
        case 7: _t->onErrorInner((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 8: _t->handlePushDataInner((*reinterpret_cast< const DataPackage(*)>(_a[1]))); break;
        case 9: _t->networkStatusChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (PushManager::*_t)(const DbusPackage & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&PushManager::handlePushData)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject PushManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_PushManager.data,
      qt_meta_data_PushManager,  qt_static_metacall, 0, 0}
};


const QMetaObject *PushManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PushManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PushManager.stringdata))
        return static_cast<void*>(const_cast< PushManager*>(this));
    return QObject::qt_metacast(_clname);
}

int PushManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void PushManager::handlePushData(const DbusPackage & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
