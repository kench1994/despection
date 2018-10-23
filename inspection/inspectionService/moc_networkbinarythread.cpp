/****************************************************************************
** Meta object code from reading C++ file 'networkbinarythread.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../framework/network/binary/networkbinarythread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'networkbinarythread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_NetworkBinaryThread_t {
    QByteArrayData data[11];
    char stringdata[93];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_NetworkBinaryThread_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_NetworkBinaryThread_t qt_meta_stringdata_NetworkBinaryThread = {
    {
QT_MOC_LITERAL(0, 0, 19),
QT_MOC_LITERAL(1, 20, 8),
QT_MOC_LITERAL(2, 29, 0),
QT_MOC_LITERAL(3, 30, 4),
QT_MOC_LITERAL(4, 35, 6),
QT_MOC_LITERAL(5, 42, 9),
QT_MOC_LITERAL(6, 52, 5),
QT_MOC_LITERAL(7, 58, 6),
QT_MOC_LITERAL(8, 65, 8),
QT_MOC_LITERAL(9, 74, 11),
QT_MOC_LITERAL(10, 86, 6)
    },
    "NetworkBinaryThread\0response\0\0uuid\0"
    "action\0localPath\0error\0errMsg\0progress\0"
    "totalLength\0status"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NetworkBinaryThread[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   29,    2, 0x06 /* Public */,
       6,    4,   36,    2, 0x06 /* Public */,
       8,    5,   45,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,    3,    4,    5,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Int, QMetaType::QString,    3,    4,    6,    7,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Int, QMetaType::Int, QMetaType::Int,    3,    4,    9,    8,   10,

       0        // eod
};

void NetworkBinaryThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        NetworkBinaryThread *_t = static_cast<NetworkBinaryThread *>(_o);
        switch (_id) {
        case 0: _t->response((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 1: _t->error((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4]))); break;
        case 2: _t->progress((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3])),(*reinterpret_cast< const int(*)>(_a[4])),(*reinterpret_cast< const int(*)>(_a[5]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (NetworkBinaryThread::*_t)(const QString , const QString , const QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&NetworkBinaryThread::response)) {
                *result = 0;
            }
        }
        {
            typedef void (NetworkBinaryThread::*_t)(const QString , const QString , const int , const QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&NetworkBinaryThread::error)) {
                *result = 1;
            }
        }
        {
            typedef void (NetworkBinaryThread::*_t)(const QString , const QString , const int , const int , const int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&NetworkBinaryThread::progress)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject NetworkBinaryThread::staticMetaObject = {
    { &NetworkThreadBase::staticMetaObject, qt_meta_stringdata_NetworkBinaryThread.data,
      qt_meta_data_NetworkBinaryThread,  qt_static_metacall, 0, 0}
};


const QMetaObject *NetworkBinaryThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NetworkBinaryThread::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_NetworkBinaryThread.stringdata))
        return static_cast<void*>(const_cast< NetworkBinaryThread*>(this));
    return NetworkThreadBase::qt_metacast(_clname);
}

int NetworkBinaryThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void NetworkBinaryThread::response(const QString _t1, const QString _t2, const QString _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void NetworkBinaryThread::error(const QString _t1, const QString _t2, const int _t3, const QString _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void NetworkBinaryThread::progress(const QString _t1, const QString _t2, const int _t3, const int _t4, const int _t5)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
