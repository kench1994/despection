/****************************************************************************
** Meta object code from reading C++ file 'datahandlermanager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "dataHandler/datahandlermanager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'datahandlermanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_DataHandlerManager_t {
    QByteArrayData data[13];
    char stringdata[155];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DataHandlerManager_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DataHandlerManager_t qt_meta_stringdata_DataHandlerManager = {
    {
QT_MOC_LITERAL(0, 0, 18),
QT_MOC_LITERAL(1, 19, 18),
QT_MOC_LITERAL(2, 38, 0),
QT_MOC_LITERAL(3, 39, 7),
QT_MOC_LITERAL(4, 47, 6),
QT_MOC_LITERAL(5, 54, 6),
QT_MOC_LITERAL(6, 61, 5),
QT_MOC_LITERAL(7, 67, 11),
QT_MOC_LITERAL(8, 79, 3),
QT_MOC_LITERAL(9, 83, 17),
QT_MOC_LITERAL(10, 101, 19),
QT_MOC_LITERAL(11, 121, 11),
QT_MOC_LITERAL(12, 133, 21)
    },
    "DataHandlerManager\0onResultOfResponse\0"
    "\0objects\0action\0result\0error\0DbusPackage\0"
    "map\0onBeginSyncSignal\0BusinessMessageType\0"
    "messageType\0sendNetworkResultSlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DataHandlerManager[] = {

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
       1,    5,   29,    2, 0x06 /* Public */,
       9,    4,   40,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      12,    5,   49,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Bool, QMetaType::Int, 0x80000000 | 7,    3,    4,    5,    6,    8,
    QMetaType::Void, 0x80000000 | 10, QMetaType::QString, QMetaType::QString, QMetaType::QVariantMap,   11,    4,    3,    8,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Bool, QMetaType::Int, 0x80000000 | 7,    3,    4,    5,    6,    8,

       0        // eod
};

void DataHandlerManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DataHandlerManager *_t = static_cast<DataHandlerManager *>(_o);
        switch (_id) {
        case 0: _t->onResultOfResponse((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< DbusPackage(*)>(_a[5]))); break;
        case 1: _t->onBeginSyncSignal((*reinterpret_cast< BusinessMessageType(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QVariantMap(*)>(_a[4]))); break;
        case 2: _t->sendNetworkResultSlot((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< DbusPackage(*)>(_a[5]))); break;
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
        case 2:
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
            typedef void (DataHandlerManager::*_t)(QString , QString , bool , int , DbusPackage );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DataHandlerManager::onResultOfResponse)) {
                *result = 0;
            }
        }
        {
            typedef void (DataHandlerManager::*_t)(BusinessMessageType , QString , QString , QVariantMap );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DataHandlerManager::onBeginSyncSignal)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject DataHandlerManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_DataHandlerManager.data,
      qt_meta_data_DataHandlerManager,  qt_static_metacall, 0, 0}
};


const QMetaObject *DataHandlerManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DataHandlerManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DataHandlerManager.stringdata))
        return static_cast<void*>(const_cast< DataHandlerManager*>(this));
    return QObject::qt_metacast(_clname);
}

int DataHandlerManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void DataHandlerManager::onResultOfResponse(QString _t1, QString _t2, bool _t3, int _t4, DbusPackage _t5)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DataHandlerManager::onBeginSyncSignal(BusinessMessageType _t1, QString _t2, QString _t3, QVariantMap _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
