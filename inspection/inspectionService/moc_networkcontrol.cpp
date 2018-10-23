/****************************************************************************
** Meta object code from reading C++ file 'networkcontrol.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "networkcontrol.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'networkcontrol.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_NetworkControl_t {
    QByteArrayData data[25];
    char stringdata[260];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_NetworkControl_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_NetworkControl_t qt_meta_stringdata_NetworkControl = {
    {
QT_MOC_LITERAL(0, 0, 14),
QT_MOC_LITERAL(1, 15, 23),
QT_MOC_LITERAL(2, 39, 0),
QT_MOC_LITERAL(3, 40, 7),
QT_MOC_LITERAL(4, 48, 6),
QT_MOC_LITERAL(5, 55, 6),
QT_MOC_LITERAL(6, 62, 5),
QT_MOC_LITERAL(7, 68, 11),
QT_MOC_LITERAL(8, 80, 3),
QT_MOC_LITERAL(9, 84, 21),
QT_MOC_LITERAL(10, 106, 11),
QT_MOC_LITERAL(11, 118, 12),
QT_MOC_LITERAL(12, 131, 10),
QT_MOC_LITERAL(13, 142, 11),
QT_MOC_LITERAL(14, 154, 8),
QT_MOC_LITERAL(15, 163, 4),
QT_MOC_LITERAL(16, 168, 11),
QT_MOC_LITERAL(17, 180, 7),
QT_MOC_LITERAL(18, 188, 14),
QT_MOC_LITERAL(19, 203, 6),
QT_MOC_LITERAL(20, 210, 8),
QT_MOC_LITERAL(21, 219, 6),
QT_MOC_LITERAL(22, 226, 14),
QT_MOC_LITERAL(23, 241, 6),
QT_MOC_LITERAL(24, 248, 11)
    },
    "NetworkControl\0sendNetworkResultSignal\0"
    "\0objects\0action\0result\0error\0DbusPackage\0"
    "map\0networkConnectChanged\0networkType\0"
    "errorOccured\0serverCode\0description\0"
    "response\0type\0DataPackage\0package\0"
    "binaryProgress\0length\0progress\0status\0"
    "binaryResponse\0target\0binaryError"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NetworkControl[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    5,   54,    2, 0x06 /* Public */,
       9,    1,   65,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      11,    5,   68,    2, 0x0a /* Public */,
      14,    4,   79,    2, 0x0a /* Public */,
       5,    4,   88,    2, 0x0a /* Public */,
      18,    4,   97,    2, 0x0a /* Public */,
      22,    2,  106,    2, 0x0a /* Public */,
      24,    3,  111,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Bool, QMetaType::Int, 0x80000000 | 7,    3,    4,    5,    6,    8,
    QMetaType::Void, QMetaType::QString,   10,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Int, QMetaType::Int, QMetaType::QString,    3,    4,    6,   12,   13,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, 0x80000000 | 16,    3,   15,    4,   17,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,    3,   15,    4,    5,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::Int, QMetaType::Int,    4,   19,   20,   21,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    4,   23,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::QString,    4,    6,   13,

       0        // eod
};

void NetworkControl::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        NetworkControl *_t = static_cast<NetworkControl *>(_o);
        switch (_id) {
        case 0: _t->sendNetworkResultSignal((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< DbusPackage(*)>(_a[5]))); break;
        case 1: _t->networkConnectChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->errorOccured((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3])),(*reinterpret_cast< const int(*)>(_a[4])),(*reinterpret_cast< const QString(*)>(_a[5]))); break;
        case 3: _t->response((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const DataPackage(*)>(_a[4]))); break;
        case 4: _t->result((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4]))); break;
        case 5: _t->binaryProgress((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3])),(*reinterpret_cast< const int(*)>(_a[4]))); break;
        case 6: _t->binaryResponse((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 7: _t->binaryError((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (NetworkControl::*_t)(QString , QString , bool , int , DbusPackage );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&NetworkControl::sendNetworkResultSignal)) {
                *result = 0;
            }
        }
        {
            typedef void (NetworkControl::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&NetworkControl::networkConnectChanged)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject NetworkControl::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_NetworkControl.data,
      qt_meta_data_NetworkControl,  qt_static_metacall, 0, 0}
};


const QMetaObject *NetworkControl::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NetworkControl::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_NetworkControl.stringdata))
        return static_cast<void*>(const_cast< NetworkControl*>(this));
    return QObject::qt_metacast(_clname);
}

int NetworkControl::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
void NetworkControl::sendNetworkResultSignal(QString _t1, QString _t2, bool _t3, int _t4, DbusPackage _t5)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void NetworkControl::networkConnectChanged(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
