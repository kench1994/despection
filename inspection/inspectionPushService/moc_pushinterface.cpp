/****************************************************************************
** Meta object code from reading C++ file 'pushinterface.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "pushinterface.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'pushinterface.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_PushInterface_t {
    QByteArrayData data[14];
    char stringdata[187];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PushInterface_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PushInterface_t qt_meta_stringdata_PushInterface = {
    {
QT_MOC_LITERAL(0, 0, 13),
QT_MOC_LITERAL(1, 14, 15),
QT_MOC_LITERAL(2, 30, 43),
QT_MOC_LITERAL(3, 74, 14),
QT_MOC_LITERAL(4, 89, 0),
QT_MOC_LITERAL(5, 90, 11),
QT_MOC_LITERAL(6, 102, 11),
QT_MOC_LITERAL(7, 114, 17),
QT_MOC_LITERAL(8, 132, 6),
QT_MOC_LITERAL(9, 139, 13),
QT_MOC_LITERAL(10, 153, 6),
QT_MOC_LITERAL(11, 160, 9),
QT_MOC_LITERAL(12, 170, 12),
QT_MOC_LITERAL(13, 183, 3)
    },
    "PushInterface\0D-Bus Interface\0"
    "com.syberos.inspectionPushService.interface\0"
    "handlePushData\0\0DbusPackage\0dbusPackage\0"
    "setRegisterStatus\0varMap\0networkStatus\0"
    "status\0setStatus\0functionType\0map"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PushInterface[] = {

 // content:
       7,       // revision
       0,       // classname
       1,   14, // classinfo
       4,   16, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // classinfo: key, value
       1,    2,

 // signals: name, argc, parameters, tag, flags
       3,    1,   36,    4, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    1,   39,    4, 0x0a /* Public */,
       9,    1,   42,    4, 0x0a /* Public */,
      11,    2,   45,    4, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 5,    6,

 // slots: parameters
    QMetaType::Void, QMetaType::QVariantMap,    8,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void, QMetaType::QString, QMetaType::QVariantMap,   12,   13,

       0        // eod
};

void PushInterface::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        PushInterface *_t = static_cast<PushInterface *>(_o);
        switch (_id) {
        case 0: _t->handlePushData((*reinterpret_cast< const DbusPackage(*)>(_a[1]))); break;
        case 1: _t->setRegisterStatus((*reinterpret_cast< const QVariantMap(*)>(_a[1]))); break;
        case 2: _t->networkStatus((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->setStatus((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QVariantMap(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (PushInterface::*_t)(const DbusPackage & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&PushInterface::handlePushData)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject PushInterface::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_PushInterface.data,
      qt_meta_data_PushInterface,  qt_static_metacall, 0, 0}
};


const QMetaObject *PushInterface::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PushInterface::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PushInterface.stringdata))
        return static_cast<void*>(const_cast< PushInterface*>(this));
    return QObject::qt_metacast(_clname);
}

int PushInterface::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void PushInterface::handlePushData(const DbusPackage & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
