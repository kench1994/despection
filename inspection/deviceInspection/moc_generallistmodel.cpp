/****************************************************************************
** Meta object code from reading C++ file 'generallistmodel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/generallistmodel/generallistmodel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'generallistmodel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_GeneralListModel_t {
    QByteArrayData data[19];
    char stringdata[188];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GeneralListModel_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GeneralListModel_t qt_meta_stringdata_GeneralListModel = {
    {
QT_MOC_LITERAL(0, 0, 16),
QT_MOC_LITERAL(1, 17, 8),
QT_MOC_LITERAL(2, 26, 0),
QT_MOC_LITERAL(3, 27, 6),
QT_MOC_LITERAL(4, 34, 12),
QT_MOC_LITERAL(5, 47, 14),
QT_MOC_LITERAL(6, 62, 9),
QT_MOC_LITERAL(7, 72, 11),
QT_MOC_LITERAL(8, 84, 4),
QT_MOC_LITERAL(9, 89, 10),
QT_MOC_LITERAL(10, 100, 7),
QT_MOC_LITERAL(11, 108, 5),
QT_MOC_LITERAL(12, 114, 8),
QT_MOC_LITERAL(13, 123, 5),
QT_MOC_LITERAL(14, 129, 10),
QT_MOC_LITERAL(15, 140, 16),
QT_MOC_LITERAL(16, 157, 4),
QT_MOC_LITERAL(17, 162, 17),
QT_MOC_LITERAL(18, 180, 7)
    },
    "GeneralListModel\0rowCount\0\0parent\0"
    "setRolesName\0QList<QString>\0roleNames\0"
    "setListData\0data\0updateData\0setData\0"
    "index\0roleName\0value\0removeData\0"
    "removeDataByKeys\0keys\0getRoleByRoleName\0"
    "getItem"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GeneralListModel[] = {

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
       1,    1,   64,    2, 0x02 /* Public */,
       1,    0,   67,    2, 0x22 /* Public | MethodCloned */,
       4,    1,   68,    2, 0x02 /* Public */,
       7,    1,   71,    2, 0x02 /* Public */,
       9,    0,   74,    2, 0x02 /* Public */,
      10,    3,   75,    2, 0x02 /* Public */,
      14,    1,   82,    2, 0x02 /* Public */,
      15,    2,   85,    2, 0x02 /* Public */,
      17,    1,   90,    2, 0x02 /* Public */,
      18,    1,   93,    2, 0x02 /* Public */,

 // methods: parameters
    QMetaType::Int, QMetaType::QModelIndex,    3,
    QMetaType::Int,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, QMetaType::QVariantList,    8,
    QMetaType::Void,
    QMetaType::Bool, QMetaType::Int, QMetaType::QString, QMetaType::QVariant,   11,   12,   13,
    QMetaType::Bool, QMetaType::Int,   11,
    QMetaType::Bool, QMetaType::QString, QMetaType::QString,   12,   16,
    QMetaType::Int, QMetaType::QString,   12,
    QMetaType::QVariant, QMetaType::Int,   11,

       0        // eod
};

void GeneralListModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        GeneralListModel *_t = static_cast<GeneralListModel *>(_o);
        switch (_id) {
        case 0: { int _r = _t->rowCount((*reinterpret_cast< const QModelIndex(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 1: { int _r = _t->rowCount();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 2: _t->setRolesName((*reinterpret_cast< QList<QString>(*)>(_a[1]))); break;
        case 3: _t->setListData((*reinterpret_cast< const QList<QVariant>(*)>(_a[1]))); break;
        case 4: _t->updateData(); break;
        case 5: { bool _r = _t->setData((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QVariant(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 6: { bool _r = _t->removeData((*reinterpret_cast< const int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 7: { bool _r = _t->removeDataByKeys((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 8: { int _r = _t->getRoleByRoleName((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 9: { QVariant _r = _t->getItem((*reinterpret_cast< const int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QVariant*>(_a[0]) = _r; }  break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<QString> >(); break;
            }
            break;
        }
    }
}

const QMetaObject GeneralListModel::staticMetaObject = {
    { &QAbstractListModel::staticMetaObject, qt_meta_stringdata_GeneralListModel.data,
      qt_meta_data_GeneralListModel,  qt_static_metacall, 0, 0}
};


const QMetaObject *GeneralListModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GeneralListModel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GeneralListModel.stringdata))
        return static_cast<void*>(const_cast< GeneralListModel*>(this));
    return QAbstractListModel::qt_metacast(_clname);
}

int GeneralListModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractListModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
