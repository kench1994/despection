/****************************************************************************
** Meta object code from reading C++ file 'multimediautils.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/imageProcess/multimediautils.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'multimediautils.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MultimediaUtils_t {
    QByteArrayData data[8];
    char stringdata[103];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MultimediaUtils_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MultimediaUtils_t qt_meta_stringdata_MultimediaUtils = {
    {
QT_MOC_LITERAL(0, 0, 15),
QT_MOC_LITERAL(1, 16, 21),
QT_MOC_LITERAL(2, 38, 0),
QT_MOC_LITERAL(3, 39, 20),
QT_MOC_LITERAL(4, 60, 18),
QT_MOC_LITERAL(5, 79, 3),
QT_MOC_LITERAL(6, 83, 11),
QT_MOC_LITERAL(7, 95, 7)
    },
    "MultimediaUtils\0getMultimediaSavePath\0"
    "\0removeMultimediaFile\0multimediaFlieList\0"
    "all\0scaledImage\0imgPath"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MultimediaUtils[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // methods: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x02 /* Public */,
       3,    2,   35,    2, 0x02 /* Public */,
       3,    1,   40,    2, 0x22 /* Public | MethodCloned */,
       6,    1,   43,    2, 0x02 /* Public */,

 // methods: parameters
    QMetaType::QString,
    QMetaType::Void, QMetaType::QStringList, QMetaType::Bool,    4,    5,
    QMetaType::Void, QMetaType::QStringList,    4,
    QMetaType::QString, QMetaType::QString,    7,

       0        // eod
};

void MultimediaUtils::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MultimediaUtils *_t = static_cast<MultimediaUtils *>(_o);
        switch (_id) {
        case 0: { QString _r = _t->getMultimediaSavePath();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 1: _t->removeMultimediaFile((*reinterpret_cast< const QStringList(*)>(_a[1])),(*reinterpret_cast< const bool(*)>(_a[2]))); break;
        case 2: _t->removeMultimediaFile((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 3: { QString _r = _t->scaledImage((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObject MultimediaUtils::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_MultimediaUtils.data,
      qt_meta_data_MultimediaUtils,  qt_static_metacall, 0, 0}
};


const QMetaObject *MultimediaUtils::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MultimediaUtils::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MultimediaUtils.stringdata))
        return static_cast<void*>(const_cast< MultimediaUtils*>(this));
    return QObject::qt_metacast(_clname);
}

int MultimediaUtils::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
QT_END_MOC_NAMESPACE
