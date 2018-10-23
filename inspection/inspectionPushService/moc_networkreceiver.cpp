/****************************************************************************
** Meta object code from reading C++ file 'networkreceiver.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "networkreceiver.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'networkreceiver.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_NetworkReceiver_t {
    QByteArrayData data[12];
    char stringdata[135];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_NetworkReceiver_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_NetworkReceiver_t qt_meta_stringdata_NetworkReceiver = {
    {
QT_MOC_LITERAL(0, 0, 15),
QT_MOC_LITERAL(1, 16, 12),
QT_MOC_LITERAL(2, 29, 0),
QT_MOC_LITERAL(3, 30, 5),
QT_MOC_LITERAL(4, 36, 4),
QT_MOC_LITERAL(5, 41, 11),
QT_MOC_LITERAL(6, 53, 15),
QT_MOC_LITERAL(7, 69, 6),
QT_MOC_LITERAL(8, 76, 14),
QT_MOC_LITERAL(9, 91, 11),
QT_MOC_LITERAL(10, 103, 11),
QT_MOC_LITERAL(11, 115, 19)
    },
    "NetworkReceiver\0initComplete\0\0error\0"
    "type\0description\0heartbeatResult\0"
    "result\0handlePushData\0DataPackage\0"
    "dataPackage\0startReceptionInner"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NetworkReceiver[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x06 /* Public */,
       3,    2,   40,    2, 0x06 /* Public */,
       6,    1,   45,    2, 0x06 /* Public */,
       8,    1,   48,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      11,    0,   51,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    4,    5,
    QMetaType::Void, QMetaType::Bool,    7,
    QMetaType::Void, 0x80000000 | 9,   10,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void NetworkReceiver::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        NetworkReceiver *_t = static_cast<NetworkReceiver *>(_o);
        switch (_id) {
        case 0: _t->initComplete(); break;
        case 1: _t->error((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 2: _t->heartbeatResult((*reinterpret_cast< const bool(*)>(_a[1]))); break;
        case 3: _t->handlePushData((*reinterpret_cast< const DataPackage(*)>(_a[1]))); break;
        case 4: _t->startReceptionInner(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (NetworkReceiver::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&NetworkReceiver::initComplete)) {
                *result = 0;
            }
        }
        {
            typedef void (NetworkReceiver::*_t)(const int & , const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&NetworkReceiver::error)) {
                *result = 1;
            }
        }
        {
            typedef void (NetworkReceiver::*_t)(const bool & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&NetworkReceiver::heartbeatResult)) {
                *result = 2;
            }
        }
        {
            typedef void (NetworkReceiver::*_t)(const DataPackage & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&NetworkReceiver::handlePushData)) {
                *result = 3;
            }
        }
    }
}

const QMetaObject NetworkReceiver::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_NetworkReceiver.data,
      qt_meta_data_NetworkReceiver,  qt_static_metacall, 0, 0}
};


const QMetaObject *NetworkReceiver::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NetworkReceiver::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_NetworkReceiver.stringdata))
        return static_cast<void*>(const_cast< NetworkReceiver*>(this));
    return QThread::qt_metacast(_clname);
}

int NetworkReceiver::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void NetworkReceiver::initComplete()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void NetworkReceiver::error(const int & _t1, const QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void NetworkReceiver::heartbeatResult(const bool & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void NetworkReceiver::handlePushData(const DataPackage & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
