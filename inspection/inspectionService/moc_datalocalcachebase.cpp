/****************************************************************************
** Meta object code from reading C++ file 'datalocalcachebase.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "localcache/datalocalcachebase.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'datalocalcachebase.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_DataLocalCacheBase_t {
    QByteArrayData data[20];
    char stringdata[229];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DataLocalCacheBase_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DataLocalCacheBase_t qt_meta_stringdata_DataLocalCacheBase = {
    {
QT_MOC_LITERAL(0, 0, 18),
QT_MOC_LITERAL(1, 19, 16),
QT_MOC_LITERAL(2, 36, 0),
QT_MOC_LITERAL(3, 37, 17),
QT_MOC_LITERAL(4, 55, 4),
QT_MOC_LITERAL(5, 60, 8),
QT_MOC_LITERAL(6, 69, 11),
QT_MOC_LITERAL(7, 81, 10),
QT_MOC_LITERAL(8, 92, 16),
QT_MOC_LITERAL(9, 109, 6),
QT_MOC_LITERAL(10, 116, 12),
QT_MOC_LITERAL(11, 129, 13),
QT_MOC_LITERAL(12, 143, 18),
QT_MOC_LITERAL(13, 162, 7),
QT_MOC_LITERAL(14, 170, 6),
QT_MOC_LITERAL(15, 177, 5),
QT_MOC_LITERAL(16, 183, 11),
QT_MOC_LITERAL(17, 195, 3),
QT_MOC_LITERAL(18, 199, 15),
QT_MOC_LITERAL(19, 215, 13)
    },
    "DataLocalCacheBase\0onSubmitProgress\0"
    "\0DataOperationType\0type\0progress\0"
    "description\0binaryType\0onSubmitFinished\0"
    "result\0onSubmitStop\0onSubmitStart\0"
    "onResultOfResponse\0objects\0action\0"
    "error\0DbusPackage\0map\0networkResponse\0"
    "networkResult"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DataLocalCacheBase[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    4,   49,    2, 0x06 /* Public */,
       8,    3,   58,    2, 0x06 /* Public */,
      10,    2,   65,    2, 0x06 /* Public */,
      11,    2,   70,    2, 0x06 /* Public */,
      12,    5,   75,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      18,    5,   86,    2, 0x0a /* Public */,
      19,    5,   97,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::Int, QMetaType::QString, QMetaType::Bool,    4,    5,    6,    7,
    QMetaType::Void, 0x80000000 | 3, QMetaType::Bool, QMetaType::Bool,    4,    9,    7,
    QMetaType::Void, 0x80000000 | 3, QMetaType::Bool,    4,    7,
    QMetaType::Void, 0x80000000 | 3, QMetaType::Bool,    4,    7,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Bool, QMetaType::Int, 0x80000000 | 16,   13,   14,    9,   15,   17,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Bool, QMetaType::Int, 0x80000000 | 16,   13,   14,    9,   15,   17,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Bool, QMetaType::Int, 0x80000000 | 16,   13,   14,    9,   15,   17,

       0        // eod
};

void DataLocalCacheBase::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DataLocalCacheBase *_t = static_cast<DataLocalCacheBase *>(_o);
        switch (_id) {
        case 0: _t->onSubmitProgress((*reinterpret_cast< DataOperationType(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4]))); break;
        case 1: _t->onSubmitFinished((*reinterpret_cast< DataOperationType(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 2: _t->onSubmitStop((*reinterpret_cast< DataOperationType(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 3: _t->onSubmitStart((*reinterpret_cast< DataOperationType(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 4: _t->onResultOfResponse((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< DbusPackage(*)>(_a[5]))); break;
        case 5: _t->networkResponse((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< DbusPackage(*)>(_a[5]))); break;
        case 6: _t->networkResult((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const bool(*)>(_a[3])),(*reinterpret_cast< const int(*)>(_a[4])),(*reinterpret_cast< const DbusPackage(*)>(_a[5]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 4:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< DbusPackage >(); break;
            }
            break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 4:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< DbusPackage >(); break;
            }
            break;
        case 6:
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
            typedef void (DataLocalCacheBase::*_t)(DataOperationType , int , const QString & , bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DataLocalCacheBase::onSubmitProgress)) {
                *result = 0;
            }
        }
        {
            typedef void (DataLocalCacheBase::*_t)(DataOperationType , bool , bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DataLocalCacheBase::onSubmitFinished)) {
                *result = 1;
            }
        }
        {
            typedef void (DataLocalCacheBase::*_t)(DataOperationType , bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DataLocalCacheBase::onSubmitStop)) {
                *result = 2;
            }
        }
        {
            typedef void (DataLocalCacheBase::*_t)(DataOperationType , bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DataLocalCacheBase::onSubmitStart)) {
                *result = 3;
            }
        }
        {
            typedef void (DataLocalCacheBase::*_t)(QString , QString , bool , int , DbusPackage );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DataLocalCacheBase::onResultOfResponse)) {
                *result = 4;
            }
        }
    }
}

const QMetaObject DataLocalCacheBase::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_DataLocalCacheBase.data,
      qt_meta_data_DataLocalCacheBase,  qt_static_metacall, 0, 0}
};


const QMetaObject *DataLocalCacheBase::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DataLocalCacheBase::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DataLocalCacheBase.stringdata))
        return static_cast<void*>(const_cast< DataLocalCacheBase*>(this));
    return QObject::qt_metacast(_clname);
}

int DataLocalCacheBase::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void DataLocalCacheBase::onSubmitProgress(DataOperationType _t1, int _t2, const QString & _t3, bool _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DataLocalCacheBase::onSubmitFinished(DataOperationType _t1, bool _t2, bool _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void DataLocalCacheBase::onSubmitStop(DataOperationType _t1, bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void DataLocalCacheBase::onSubmitStart(DataOperationType _t1, bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void DataLocalCacheBase::onResultOfResponse(QString _t1, QString _t2, bool _t3, int _t4, DbusPackage _t5)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_END_MOC_NAMESPACE
