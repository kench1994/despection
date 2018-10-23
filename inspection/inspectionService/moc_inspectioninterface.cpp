/****************************************************************************
** Meta object code from reading C++ file 'inspectioninterface.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "inspectioninterface.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'inspectioninterface.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_InspectionInterface_t {
    QByteArrayData data[21];
    char stringdata[260];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_InspectionInterface_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_InspectionInterface_t qt_meta_stringdata_InspectionInterface = {
    {
QT_MOC_LITERAL(0, 0, 19),
QT_MOC_LITERAL(1, 20, 15),
QT_MOC_LITERAL(2, 36, 39),
QT_MOC_LITERAL(3, 76, 16),
QT_MOC_LITERAL(4, 93, 0),
QT_MOC_LITERAL(5, 94, 7),
QT_MOC_LITERAL(6, 102, 6),
QT_MOC_LITERAL(7, 109, 6),
QT_MOC_LITERAL(8, 116, 5),
QT_MOC_LITERAL(9, 122, 11),
QT_MOC_LITERAL(10, 134, 3),
QT_MOC_LITERAL(11, 138, 14),
QT_MOC_LITERAL(12, 153, 10),
QT_MOC_LITERAL(13, 164, 9),
QT_MOC_LITERAL(14, 174, 11),
QT_MOC_LITERAL(15, 186, 11),
QT_MOC_LITERAL(16, 198, 11),
QT_MOC_LITERAL(17, 210, 12),
QT_MOC_LITERAL(18, 223, 9),
QT_MOC_LITERAL(19, 233, 14),
QT_MOC_LITERAL(20, 248, 11)
    },
    "InspectionInterface\0D-Bus Interface\0"
    "com.syberos.inspectionService.interface\0"
    "responseToClient\0\0objects\0action\0"
    "result\0error\0DbusPackage\0map\0"
    "messageUpdated\0messgeType\0aaaaaaaaa\0"
    "receiveData\0DataPackage\0dataPackage\0"
    "functionType\0setStatus\0handlePushData\0"
    "dbusPackage"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_InspectionInterface[] = {

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
      13,    5,   60,    4, 0x0a /* Public */,
      14,    3,   71,    4, 0x0a /* Public */,
      18,    2,   78,    4, 0x0a /* Public */,
      19,    1,   83,    4, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Bool, QMetaType::Int, 0x80000000 | 9,    5,    6,    7,    8,   10,
    QMetaType::Void, QMetaType::QString,   12,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Bool, QMetaType::Int, 0x80000000 | 9,    5,    6,    7,    8,   10,
    QMetaType::Void, 0x80000000 | 15, QMetaType::QString, QMetaType::QString,   16,    6,   17,
    QMetaType::Void, QMetaType::QString, QMetaType::QVariantMap,   17,   10,
    QMetaType::Void, 0x80000000 | 9,   20,

       0        // eod
};

void InspectionInterface::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        InspectionInterface *_t = static_cast<InspectionInterface *>(_o);
        switch (_id) {
        case 0: _t->responseToClient((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< DbusPackage(*)>(_a[5]))); break;
        case 1: _t->messageUpdated((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->aaaaaaaaa((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< DbusPackage(*)>(_a[5]))); break;
        case 3: _t->receiveData((*reinterpret_cast< const DataPackage(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 4: _t->setStatus((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QVariantMap(*)>(_a[2]))); break;
        case 5: _t->handlePushData((*reinterpret_cast< const DbusPackage(*)>(_a[1]))); break;
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
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< DataPackage >(); break;
            }
            break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< DbusPackage >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (InspectionInterface::*_t)(QString , QString , bool , int , DbusPackage );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&InspectionInterface::responseToClient)) {
                *result = 0;
            }
        }
        {
            typedef void (InspectionInterface::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&InspectionInterface::messageUpdated)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject InspectionInterface::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_InspectionInterface.data,
      qt_meta_data_InspectionInterface,  qt_static_metacall, 0, 0}
};


const QMetaObject *InspectionInterface::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *InspectionInterface::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_InspectionInterface.stringdata))
        return static_cast<void*>(const_cast< InspectionInterface*>(this));
    return QObject::qt_metacast(_clname);
}

int InspectionInterface::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void InspectionInterface::responseToClient(QString _t1, QString _t2, bool _t3, int _t4, DbusPackage _t5)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void InspectionInterface::messageUpdated(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
