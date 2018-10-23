/****************************************************************************
** Meta object code from reading C++ file 'clientinterface.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/common/clientinterface.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'clientinterface.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ClientInterface_t {
    QByteArrayData data[19];
    char stringdata[240];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ClientInterface_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ClientInterface_t qt_meta_stringdata_ClientInterface = {
    {
QT_MOC_LITERAL(0, 0, 15),
QT_MOC_LITERAL(1, 16, 15),
QT_MOC_LITERAL(2, 32, 38),
QT_MOC_LITERAL(3, 71, 8),
QT_MOC_LITERAL(4, 80, 0),
QT_MOC_LITERAL(5, 81, 7),
QT_MOC_LITERAL(6, 89, 6),
QT_MOC_LITERAL(7, 96, 6),
QT_MOC_LITERAL(8, 103, 5),
QT_MOC_LITERAL(9, 109, 11),
QT_MOC_LITERAL(10, 121, 3),
QT_MOC_LITERAL(11, 125, 14),
QT_MOC_LITERAL(12, 140, 8),
QT_MOC_LITERAL(13, 149, 17),
QT_MOC_LITERAL(14, 167, 13),
QT_MOC_LITERAL(15, 181, 11),
QT_MOC_LITERAL(16, 193, 16),
QT_MOC_LITERAL(17, 210, 24),
QT_MOC_LITERAL(18, 235, 4)
    },
    "ClientInterface\0D-Bus Interface\0"
    "com.syberos.inspectionClient.interface\0"
    "response\0\0objects\0action\0result\0error\0"
    "DbusPackage\0map\0messageUpdated\0operType\0"
    "resultDbusPackage\0messageUpdate\0"
    "messageType\0callFinishedSlot\0"
    "QDBusPendingCallWatcher*\0call"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ClientInterface[] = {

 // content:
       7,       // revision
       0,       // classname
       1,   14, // classinfo
       6,   16, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // classinfo: key, value
       1,    2,

 // signals: name, argc, parameters, tag, flags
       3,    5,   46,    4, 0x06 /* Public */,
      11,    1,   57,    4, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    5,   60,    4, 0x0a /* Public */,
      13,    5,   71,    4, 0x0a /* Public */,
      14,    1,   82,    4, 0x0a /* Public */,
      16,    1,   85,    4, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Bool, QMetaType::Int, 0x80000000 | 9,    5,    6,    7,    8,   10,
    QMetaType::Void, QMetaType::QString,   12,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Bool, QMetaType::Int, QMetaType::QVariantMap,    5,    6,    7,    8,   10,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Bool, QMetaType::Int, 0x80000000 | 9,    5,    6,    7,    8,   10,
    QMetaType::Void, QMetaType::QString,   15,
    QMetaType::Void, 0x80000000 | 17,   18,

       0        // eod
};

void ClientInterface::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ClientInterface *_t = static_cast<ClientInterface *>(_o);
        switch (_id) {
        case 0: _t->response((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< DbusPackage(*)>(_a[5]))); break;
        case 1: _t->messageUpdated((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->result((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< QVariantMap(*)>(_a[5]))); break;
        case 3: _t->resultDbusPackage((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< DbusPackage(*)>(_a[5]))); break;
        case 4: _t->messageUpdate((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->callFinishedSlot((*reinterpret_cast< QDBusPendingCallWatcher*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ClientInterface::*_t)(QString , QString , bool , int , DbusPackage );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ClientInterface::response)) {
                *result = 0;
            }
        }
        {
            typedef void (ClientInterface::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ClientInterface::messageUpdated)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject ClientInterface::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ClientInterface.data,
      qt_meta_data_ClientInterface,  qt_static_metacall, 0, 0}
};


const QMetaObject *ClientInterface::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ClientInterface::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ClientInterface.stringdata))
        return static_cast<void*>(const_cast< ClientInterface*>(this));
    return QObject::qt_metacast(_clname);
}

int ClientInterface::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void ClientInterface::response(QString _t1, QString _t2, bool _t3, int _t4, DbusPackage _t5)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ClientInterface::messageUpdated(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
