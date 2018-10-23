/****************************************************************************
** Meta object code from reading C++ file 'networksender.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "networksender.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'networksender.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_NetworkSender_t {
    QByteArrayData data[13];
    char stringdata[169];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_NetworkSender_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_NetworkSender_t qt_meta_stringdata_NetworkSender = {
    {
QT_MOC_LITERAL(0, 0, 13),
QT_MOC_LITERAL(1, 14, 13),
QT_MOC_LITERAL(2, 28, 0),
QT_MOC_LITERAL(3, 29, 11),
QT_MOC_LITERAL(4, 41, 12),
QT_MOC_LITERAL(5, 54, 5),
QT_MOC_LITERAL(6, 60, 4),
QT_MOC_LITERAL(7, 65, 11),
QT_MOC_LITERAL(8, 77, 15),
QT_MOC_LITERAL(9, 93, 6),
QT_MOC_LITERAL(10, 100, 29),
QT_MOC_LITERAL(11, 130, 18),
QT_MOC_LITERAL(12, 149, 19)
    },
    "NetworkSender\0initKeepAlive\0\0cleanThread\0"
    "initComplete\0error\0type\0description\0"
    "heartbeatResult\0result\0"
    "initPushAuthorityAndKeepAlive\0"
    "sendHeartbeatInner\0unregisterPushInner"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NetworkSender[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x06 /* Public */,
       3,    0,   55,    2, 0x06 /* Public */,
       4,    0,   56,    2, 0x06 /* Public */,
       5,    2,   57,    2, 0x06 /* Public */,
       8,    1,   62,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      10,    0,   65,    2, 0x0a /* Public */,
      11,    0,   66,    2, 0x0a /* Public */,
      12,    0,   67,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    6,    7,
    QMetaType::Void, QMetaType::Bool,    9,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void NetworkSender::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        NetworkSender *_t = static_cast<NetworkSender *>(_o);
        switch (_id) {
        case 0: _t->initKeepAlive(); break;
        case 1: _t->cleanThread(); break;
        case 2: _t->initComplete(); break;
        case 3: _t->error((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 4: _t->heartbeatResult((*reinterpret_cast< const bool(*)>(_a[1]))); break;
        case 5: _t->initPushAuthorityAndKeepAlive(); break;
        case 6: _t->sendHeartbeatInner(); break;
        case 7: _t->unregisterPushInner(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (NetworkSender::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&NetworkSender::initKeepAlive)) {
                *result = 0;
            }
        }
        {
            typedef void (NetworkSender::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&NetworkSender::cleanThread)) {
                *result = 1;
            }
        }
        {
            typedef void (NetworkSender::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&NetworkSender::initComplete)) {
                *result = 2;
            }
        }
        {
            typedef void (NetworkSender::*_t)(const int & , const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&NetworkSender::error)) {
                *result = 3;
            }
        }
        {
            typedef void (NetworkSender::*_t)(const bool & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&NetworkSender::heartbeatResult)) {
                *result = 4;
            }
        }
    }
}

const QMetaObject NetworkSender::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_NetworkSender.data,
      qt_meta_data_NetworkSender,  qt_static_metacall, 0, 0}
};


const QMetaObject *NetworkSender::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NetworkSender::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_NetworkSender.stringdata))
        return static_cast<void*>(const_cast< NetworkSender*>(this));
    return QThread::qt_metacast(_clname);
}

int NetworkSender::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void NetworkSender::initKeepAlive()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void NetworkSender::cleanThread()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void NetworkSender::initComplete()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void NetworkSender::error(const int & _t1, const QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void NetworkSender::heartbeatResult(const bool & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_END_MOC_NAMESPACE
