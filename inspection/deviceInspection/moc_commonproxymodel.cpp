/****************************************************************************
** Meta object code from reading C++ file 'commonproxymodel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/generallistmodel/commonproxymodel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'commonproxymodel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CommonProxyModel_t {
    QByteArrayData data[13];
    char stringdata[127];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CommonProxyModel_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CommonProxyModel_t qt_meta_stringdata_CommonProxyModel = {
    {
QT_MOC_LITERAL(0, 0, 16),
QT_MOC_LITERAL(1, 17, 12),
QT_MOC_LITERAL(2, 30, 0),
QT_MOC_LITERAL(3, 31, 5),
QT_MOC_LITERAL(4, 37, 9),
QT_MOC_LITERAL(5, 47, 8),
QT_MOC_LITERAL(6, 56, 14),
QT_MOC_LITERAL(7, 71, 9),
QT_MOC_LITERAL(8, 81, 7),
QT_MOC_LITERAL(9, 89, 5),
QT_MOC_LITERAL(10, 95, 14),
QT_MOC_LITERAL(11, 110, 11),
QT_MOC_LITERAL(12, 122, 4)
    },
    "CommonProxyModel\0modelChanged\0\0model\0"
    "itemCount\0setModel\0setModelFilter\0"
    "filterStr\0getItem\0index\0getSourceIndex\0"
    "setListData\0data"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CommonProxyModel[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x06 /* Public */,

 // methods: name, argc, parameters, tag, flags
       3,    0,   55,    2, 0x02 /* Public */,
       4,    0,   56,    2, 0x02 /* Public */,
       5,    1,   57,    2, 0x02 /* Public */,
       6,    1,   60,    2, 0x02 /* Public */,
       8,    1,   63,    2, 0x02 /* Public */,
      10,    1,   66,    2, 0x02 /* Public */,
      11,    1,   69,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void,

 // methods: parameters
    QMetaType::QObjectStar,
    QMetaType::Int,
    QMetaType::Void, QMetaType::QObjectStar,    3,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::QVariant, QMetaType::Int,    9,
    QMetaType::Int, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::QVariantList,   12,

       0        // eod
};

void CommonProxyModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CommonProxyModel *_t = static_cast<CommonProxyModel *>(_o);
        switch (_id) {
        case 0: _t->modelChanged(); break;
        case 1: { QObject* _r = _t->model();
            if (_a[0]) *reinterpret_cast< QObject**>(_a[0]) = _r; }  break;
        case 2: { int _r = _t->itemCount();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 3: _t->setModel((*reinterpret_cast< QObject*(*)>(_a[1]))); break;
        case 4: _t->setModelFilter((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: { QVariant _r = _t->getItem((*reinterpret_cast< const int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QVariant*>(_a[0]) = _r; }  break;
        case 6: { int _r = _t->getSourceIndex((*reinterpret_cast< const int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 7: _t->setListData((*reinterpret_cast< const QList<QVariant>(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CommonProxyModel::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CommonProxyModel::modelChanged)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject CommonProxyModel::staticMetaObject = {
    { &QSortFilterProxyModel::staticMetaObject, qt_meta_stringdata_CommonProxyModel.data,
      qt_meta_data_CommonProxyModel,  qt_static_metacall, 0, 0}
};


const QMetaObject *CommonProxyModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CommonProxyModel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CommonProxyModel.stringdata))
        return static_cast<void*>(const_cast< CommonProxyModel*>(this));
    return QSortFilterProxyModel::qt_metacast(_clname);
}

int CommonProxyModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QSortFilterProxyModel::qt_metacall(_c, _id, _a);
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
void CommonProxyModel::modelChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
