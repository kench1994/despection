/****************************************************************************
** Meta object code from reading C++ file 'decodeworkspace.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/imageProcess/decodeworkspace.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'decodeworkspace.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_DecodeWorkSpace_t {
    QByteArrayData data[19];
    char stringdata[256];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DecodeWorkSpace_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DecodeWorkSpace_t qt_meta_stringdata_DecodeWorkSpace = {
    {
QT_MOC_LITERAL(0, 0, 15),
QT_MOC_LITERAL(1, 16, 20),
QT_MOC_LITERAL(2, 37, 0),
QT_MOC_LITERAL(3, 38, 13),
QT_MOC_LITERAL(4, 52, 14),
QT_MOC_LITERAL(5, 67, 15),
QT_MOC_LITERAL(6, 83, 13),
QT_MOC_LITERAL(7, 97, 7),
QT_MOC_LITERAL(8, 105, 14),
QT_MOC_LITERAL(9, 120, 15),
QT_MOC_LITERAL(10, 136, 19),
QT_MOC_LITERAL(11, 156, 6),
QT_MOC_LITERAL(12, 163, 19),
QT_MOC_LITERAL(13, 183, 15),
QT_MOC_LITERAL(14, 199, 12),
QT_MOC_LITERAL(15, 212, 11),
QT_MOC_LITERAL(16, 224, 5),
QT_MOC_LITERAL(17, 230, 14),
QT_MOC_LITERAL(18, 245, 10)
    },
    "DecodeWorkSpace\0encodeContentChanged\0"
    "\0encodeContent\0decodeFinished\0"
    "decodeImagePath\0decodeContent\0success\0"
    "encodeFinished\0encodeImagePath\0"
    "changedCameraStatus\0status\0"
    "cameraStatusChanged\0QCamera::Status\0"
    "processFrame\0QVideoFrame\0frame\0"
    "finishedDecode\0encodeText"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DecodeWorkSpace[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       1,   82, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   49,    2, 0x06 /* Public */,
       4,    3,   52,    2, 0x06 /* Public */,
       8,    3,   59,    2, 0x06 /* Public */,
      10,    1,   66,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      12,    1,   69,    2, 0x0a /* Public */,
      14,    1,   72,    2, 0x0a /* Public */,
      17,    3,   75,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Bool,    5,    6,    7,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Bool,    9,    3,    7,
    QMetaType::Void, QMetaType::Int,   11,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 13,   11,
    QMetaType::Void, 0x80000000 | 15,   16,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Bool,    5,    6,    7,

 // properties: name, type, flags
      18, QMetaType::QString, 0x00495003,

 // properties: notify_signal_id
       0,

       0        // eod
};

void DecodeWorkSpace::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DecodeWorkSpace *_t = static_cast<DecodeWorkSpace *>(_o);
        switch (_id) {
        case 0: _t->encodeContentChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->decodeFinished((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const bool(*)>(_a[3]))); break;
        case 2: _t->encodeFinished((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const bool(*)>(_a[3]))); break;
        case 3: _t->changedCameraStatus((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->cameraStatusChanged((*reinterpret_cast< QCamera::Status(*)>(_a[1]))); break;
        case 5: _t->processFrame((*reinterpret_cast< QVideoFrame(*)>(_a[1]))); break;
        case 6: _t->finishedDecode((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const bool(*)>(_a[3]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QCamera::Status >(); break;
            }
            break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVideoFrame >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (DecodeWorkSpace::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DecodeWorkSpace::encodeContentChanged)) {
                *result = 0;
            }
        }
        {
            typedef void (DecodeWorkSpace::*_t)(const QString & , const QString & , const bool & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DecodeWorkSpace::decodeFinished)) {
                *result = 1;
            }
        }
        {
            typedef void (DecodeWorkSpace::*_t)(const QString & , const QString & , const bool & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DecodeWorkSpace::encodeFinished)) {
                *result = 2;
            }
        }
        {
            typedef void (DecodeWorkSpace::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DecodeWorkSpace::changedCameraStatus)) {
                *result = 3;
            }
        }
    }
}

const QMetaObject DecodeWorkSpace::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_DecodeWorkSpace.data,
      qt_meta_data_DecodeWorkSpace,  qt_static_metacall, 0, 0}
};


const QMetaObject *DecodeWorkSpace::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DecodeWorkSpace::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DecodeWorkSpace.stringdata))
        return static_cast<void*>(const_cast< DecodeWorkSpace*>(this));
    return QObject::qt_metacast(_clname);
}

int DecodeWorkSpace::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = getEncodeContent(); break;
        }
        _id -= 1;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setEncodeContent(*reinterpret_cast< QString*>(_v)); break;
        }
        _id -= 1;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 1;
    } else if (_c == QMetaObject::RegisterPropertyMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void DecodeWorkSpace::encodeContentChanged(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DecodeWorkSpace::decodeFinished(const QString & _t1, const QString & _t2, const bool & _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void DecodeWorkSpace::encodeFinished(const QString & _t1, const QString & _t2, const bool & _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void DecodeWorkSpace::changedCameraStatus(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
