/****************************************************************************
** Meta object code from reading C++ file 'themeadaptor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/theme/themeadaptor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'themeadaptor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ThemeAdaptor_t {
    QByteArrayData data[18];
    char stringdata[179];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ThemeAdaptor_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ThemeAdaptor_t qt_meta_stringdata_ThemeAdaptor = {
    {
QT_MOC_LITERAL(0, 0, 12),
QT_MOC_LITERAL(1, 13, 7),
QT_MOC_LITERAL(2, 21, 0),
QT_MOC_LITERAL(3, 22, 5),
QT_MOC_LITERAL(4, 28, 12),
QT_MOC_LITERAL(5, 41, 15),
QT_MOC_LITERAL(6, 57, 5),
QT_MOC_LITERAL(7, 63, 9),
QT_MOC_LITERAL(8, 73, 9),
QT_MOC_LITERAL(9, 83, 12),
QT_MOC_LITERAL(10, 96, 8),
QT_MOC_LITERAL(11, 105, 12),
QT_MOC_LITERAL(12, 118, 14),
QT_MOC_LITERAL(13, 133, 12),
QT_MOC_LITERAL(14, 146, 2),
QT_MOC_LITERAL(15, 149, 8),
QT_MOC_LITERAL(16, 158, 5),
QT_MOC_LITERAL(17, 164, 14)
    },
    "ThemeAdaptor\0reflesh\0\0theme\0getThemePath\0"
    "getThemeResPath\0getID\0getNameCN\0"
    "getNameEN\0getImagePath\0getStyle\0"
    "getIconCount\0getStaticCount\0getNodeParas\0"
    "id\0parentId\0index\0getStaticParas"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ThemeAdaptor[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // methods: name, argc, parameters, tag, flags
       1,    1,   79,    2, 0x02 /* Public */,
       4,    0,   82,    2, 0x02 /* Public */,
       5,    0,   83,    2, 0x02 /* Public */,
       6,    0,   84,    2, 0x02 /* Public */,
       7,    0,   85,    2, 0x02 /* Public */,
       8,    0,   86,    2, 0x02 /* Public */,
       9,    0,   87,    2, 0x02 /* Public */,
      10,    0,   88,    2, 0x02 /* Public */,
      11,    0,   89,    2, 0x02 /* Public */,
      12,    0,   90,    2, 0x02 /* Public */,
      13,    2,   91,    2, 0x02 /* Public */,
      13,    2,   96,    2, 0x02 /* Public */,
      17,    1,  101,    2, 0x02 /* Public */,

 // methods: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::QString,
    QMetaType::QString,
    QMetaType::QString,
    QMetaType::QString,
    QMetaType::QString,
    QMetaType::QString,
    QMetaType::QString,
    QMetaType::Int,
    QMetaType::Int,
    QMetaType::QVariantMap, QMetaType::QString, QMetaType::QString,   14,   15,
    QMetaType::QVariantMap, QMetaType::Int, QMetaType::QString,   16,   15,
    QMetaType::QVariantMap, QMetaType::QString,   14,

       0        // eod
};

void ThemeAdaptor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ThemeAdaptor *_t = static_cast<ThemeAdaptor *>(_o);
        switch (_id) {
        case 0: _t->reflesh((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: { QString _r = _t->getThemePath();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 2: { QString _r = _t->getThemeResPath();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 3: { QString _r = _t->getID();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 4: { QString _r = _t->getNameCN();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 5: { QString _r = _t->getNameEN();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 6: { QString _r = _t->getImagePath();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 7: { QString _r = _t->getStyle();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 8: { int _r = _t->getIconCount();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 9: { int _r = _t->getStaticCount();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 10: { QMap<QString,QVariant> _r = _t->getNodeParas((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QMap<QString,QVariant>*>(_a[0]) = _r; }  break;
        case 11: { QMap<QString,QVariant> _r = _t->getNodeParas((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QMap<QString,QVariant>*>(_a[0]) = _r; }  break;
        case 12: { QMap<QString,QVariant> _r = _t->getStaticParas((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QMap<QString,QVariant>*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObject ThemeAdaptor::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ThemeAdaptor.data,
      qt_meta_data_ThemeAdaptor,  qt_static_metacall, 0, 0}
};


const QMetaObject *ThemeAdaptor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ThemeAdaptor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ThemeAdaptor.stringdata))
        return static_cast<void*>(const_cast< ThemeAdaptor*>(this));
    return QObject::qt_metacast(_clname);
}

int ThemeAdaptor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
