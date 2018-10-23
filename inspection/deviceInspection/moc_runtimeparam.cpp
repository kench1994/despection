/****************************************************************************
** Meta object code from reading C++ file 'runtimeparam.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/common/runtimeparam.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'runtimeparam.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_RuntimeParam_t {
    QByteArrayData data[15];
    char stringdata[196];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RuntimeParam_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RuntimeParam_t qt_meta_stringdata_RuntimeParam = {
    {
QT_MOC_LITERAL(0, 0, 12),
QT_MOC_LITERAL(1, 13, 8),
QT_MOC_LITERAL(2, 22, 0),
QT_MOC_LITERAL(3, 23, 16),
QT_MOC_LITERAL(4, 40, 7),
QT_MOC_LITERAL(5, 48, 18),
QT_MOC_LITERAL(6, 67, 10),
QT_MOC_LITERAL(7, 78, 9),
QT_MOC_LITERAL(8, 88, 18),
QT_MOC_LITERAL(9, 107, 20),
QT_MOC_LITERAL(10, 128, 3),
QT_MOC_LITERAL(11, 132, 14),
QT_MOC_LITERAL(12, 147, 19),
QT_MOC_LITERAL(13, 167, 6),
QT_MOC_LITERAL(14, 174, 21)
    },
    "RuntimeParam\0getPadId\0\0setLoginUserInfo\0"
    "userMap\0clearLoginUserInfo\0getUserMap\0"
    "getUserId\0getOrganiseUnitIDs\0"
    "getUserPropertyByKey\0key\0getLoginStatus\0"
    "checkUserPermission\0module\0"
    "getUserPermissionData"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RuntimeParam[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // methods: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x02 /* Public */,
       3,    1,   65,    2, 0x02 /* Public */,
       5,    0,   68,    2, 0x02 /* Public */,
       6,    0,   69,    2, 0x02 /* Public */,
       7,    0,   70,    2, 0x02 /* Public */,
       8,    0,   71,    2, 0x02 /* Public */,
       9,    1,   72,    2, 0x02 /* Public */,
      11,    0,   75,    2, 0x02 /* Public */,
      12,    1,   76,    2, 0x02 /* Public */,
      14,    2,   79,    2, 0x02 /* Public */,

 // methods: parameters
    QMetaType::QString,
    QMetaType::Void, QMetaType::QVariantMap,    4,
    QMetaType::Void,
    QMetaType::QVariantMap,
    QMetaType::QString,
    QMetaType::QString,
    QMetaType::QString, QMetaType::QString,   10,
    QMetaType::Bool,
    QMetaType::Bool, QMetaType::QString,   13,
    QMetaType::QString, QMetaType::QString, QMetaType::QString,   13,   10,

       0        // eod
};

void RuntimeParam::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RuntimeParam *_t = static_cast<RuntimeParam *>(_o);
        switch (_id) {
        case 0: { QString _r = _t->getPadId();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 1: _t->setLoginUserInfo((*reinterpret_cast< const QVariantMap(*)>(_a[1]))); break;
        case 2: _t->clearLoginUserInfo(); break;
        case 3: { QVariantMap _r = _t->getUserMap();
            if (_a[0]) *reinterpret_cast< QVariantMap*>(_a[0]) = _r; }  break;
        case 4: { QString _r = _t->getUserId();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 5: { QString _r = _t->getOrganiseUnitIDs();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 6: { QString _r = _t->getUserPropertyByKey((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 7: { bool _r = _t->getLoginStatus();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 8: { bool _r = _t->checkUserPermission((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 9: { QString _r = _t->getUserPermissionData((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObject RuntimeParam::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_RuntimeParam.data,
      qt_meta_data_RuntimeParam,  qt_static_metacall, 0, 0}
};


const QMetaObject *RuntimeParam::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RuntimeParam::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RuntimeParam.stringdata))
        return static_cast<void*>(const_cast< RuntimeParam*>(this));
    return QObject::qt_metacast(_clname);
}

int RuntimeParam::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
