/****************************************************************************
** Meta object code from reading C++ file 'networkcommandthread.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../framework/network/command/networkcommandthread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'networkcommandthread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_NetworkCommandThread_t {
    QByteArrayData data[14];
    char stringdata[105];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_NetworkCommandThread_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_NetworkCommandThread_t qt_meta_stringdata_NetworkCommandThread = {
    {
QT_MOC_LITERAL(0, 0, 20),
QT_MOC_LITERAL(1, 21, 8),
QT_MOC_LITERAL(2, 30, 0),
QT_MOC_LITERAL(3, 31, 2),
QT_MOC_LITERAL(4, 34, 7),
QT_MOC_LITERAL(5, 42, 4),
QT_MOC_LITERAL(6, 47, 6),
QT_MOC_LITERAL(7, 54, 4),
QT_MOC_LITERAL(8, 59, 12),
QT_MOC_LITERAL(9, 72, 4),
QT_MOC_LITERAL(10, 77, 5),
QT_MOC_LITERAL(11, 83, 7),
QT_MOC_LITERAL(12, 91, 6),
QT_MOC_LITERAL(13, 98, 6)
    },
    "NetworkCommandThread\0response\0\0id\0"
    "objects\0type\0action\0data\0ProtocolHead\0"
    "head\0error\0errCode\0errMsg\0doTask"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NetworkCommandThread[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    6,   29,    2, 0x06 /* Public */,
      10,    5,   42,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      13,    0,   53,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QByteArray, 0x80000000 | 8,    3,    4,    5,    6,    7,    9,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::Int, QMetaType::QString,    3,    4,    6,   11,   12,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void NetworkCommandThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        NetworkCommandThread *_t = static_cast<NetworkCommandThread *>(_o);
        switch (_id) {
        case 0: _t->response((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4])),(*reinterpret_cast< const QByteArray(*)>(_a[5])),(*reinterpret_cast< const ProtocolHead(*)>(_a[6]))); break;
        case 1: _t->error((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const int(*)>(_a[4])),(*reinterpret_cast< const QString(*)>(_a[5]))); break;
        case 2: _t->doTask(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (NetworkCommandThread::*_t)(const QString , const QString , const QString , const QString , const QByteArray , const ProtocolHead );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&NetworkCommandThread::response)) {
                *result = 0;
            }
        }
        {
            typedef void (NetworkCommandThread::*_t)(const QString , const QString , const QString , const int , const QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&NetworkCommandThread::error)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject NetworkCommandThread::staticMetaObject = {
    { &NetworkThreadBase::staticMetaObject, qt_meta_stringdata_NetworkCommandThread.data,
      qt_meta_data_NetworkCommandThread,  qt_static_metacall, 0, 0}
};


const QMetaObject *NetworkCommandThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NetworkCommandThread::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_NetworkCommandThread.stringdata))
        return static_cast<void*>(const_cast< NetworkCommandThread*>(this));
    return NetworkThreadBase::qt_metacast(_clname);
}

int NetworkCommandThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = NetworkThreadBase::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void NetworkCommandThread::response(const QString _t1, const QString _t2, const QString _t3, const QString _t4, const QByteArray _t5, const ProtocolHead _t6)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)), const_cast<void*>(reinterpret_cast<const void*>(&_t6)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void NetworkCommandThread::error(const QString _t1, const QString _t2, const QString _t3, const int _t4, const QString _t5)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
