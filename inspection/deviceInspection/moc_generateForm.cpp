/****************************************************************************
** Meta object code from reading C++ file 'generateForm.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/generateForm/generateForm.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'generateForm.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_GenerateForm_t {
    QByteArrayData data[20];
    char stringdata[264];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GenerateForm_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GenerateForm_t qt_meta_stringdata_GenerateForm = {
    {
QT_MOC_LITERAL(0, 0, 12),
QT_MOC_LITERAL(1, 13, 9),
QT_MOC_LITERAL(2, 23, 0),
QT_MOC_LITERAL(3, 24, 5),
QT_MOC_LITERAL(4, 30, 23),
QT_MOC_LITERAL(5, 54, 26),
QT_MOC_LITERAL(6, 81, 5),
QT_MOC_LITERAL(7, 87, 9),
QT_MOC_LITERAL(8, 97, 8),
QT_MOC_LITERAL(9, 106, 9),
QT_MOC_LITERAL(10, 116, 14),
QT_MOC_LITERAL(11, 131, 11),
QT_MOC_LITERAL(12, 143, 10),
QT_MOC_LITERAL(13, 154, 22),
QT_MOC_LITERAL(14, 177, 11),
QT_MOC_LITERAL(15, 189, 17),
QT_MOC_LITERAL(16, 207, 23),
QT_MOC_LITERAL(17, 231, 11),
QT_MOC_LITERAL(18, 243, 11),
QT_MOC_LITERAL(19, 255, 8)
    },
    "GenerateForm\0dataError\0\0error\0"
    "createComponentFinished\0"
    "checkLegalityErrorOccurred\0label\0"
    "errorType\0dataType\0precision\0"
    "initializeForm\0getItemData\0otherParam\0"
    "checkFormInputLegality\0rootObjName\0"
    "contentColObjName\0choiceDialogItemObjName\0"
    "deviceValue\0dataPackage\0formType"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GenerateForm[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       6,   62, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,
       4,    0,   47,    2, 0x06 /* Public */,
       5,    4,   48,    2, 0x06 /* Public */,

 // methods: name, argc, parameters, tag, flags
      10,    0,   57,    2, 0x02 /* Public */,
      11,    1,   58,    2, 0x02 /* Public */,
      13,    0,   61,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::QString, QMetaType::QString,    6,    7,    8,    9,

 // methods: parameters
    QMetaType::Void,
    QMetaType::QVariantMap, QMetaType::QVariantMap,   12,
    QMetaType::Int,

 // properties: name, type, flags
      14, QMetaType::QString, 0x00095103,
      15, QMetaType::QString, 0x00095103,
      16, QMetaType::QString, 0x00095003,
      17, QMetaType::QVariantMap, 0x00095103,
      18, QMetaType::QVariantMap, 0x00095103,
      19, QMetaType::QString, 0x00095103,

       0        // eod
};

void GenerateForm::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        GenerateForm *_t = static_cast<GenerateForm *>(_o);
        switch (_id) {
        case 0: _t->dataError((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->createComponentFinished(); break;
        case 2: _t->checkLegalityErrorOccurred((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4]))); break;
        case 3: _t->initializeForm(); break;
        case 4: { QVariantMap _r = _t->getItemData((*reinterpret_cast< const QVariantMap(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QVariantMap*>(_a[0]) = _r; }  break;
        case 5: { int _r = _t->checkFormInputLegality();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (GenerateForm::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GenerateForm::dataError)) {
                *result = 0;
            }
        }
        {
            typedef void (GenerateForm::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GenerateForm::createComponentFinished)) {
                *result = 1;
            }
        }
        {
            typedef void (GenerateForm::*_t)(const QString & , const int & , const QString & , const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GenerateForm::checkLegalityErrorOccurred)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject GenerateForm::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_GenerateForm.data,
      qt_meta_data_GenerateForm,  qt_static_metacall, 0, 0}
};


const QMetaObject *GenerateForm::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GenerateForm::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GenerateForm.stringdata))
        return static_cast<void*>(const_cast< GenerateForm*>(this));
    return QObject::qt_metacast(_clname);
}

int GenerateForm::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = getRootObjName(); break;
        case 1: *reinterpret_cast< QString*>(_v) = getContentColObjName(); break;
        case 2: *reinterpret_cast< QString*>(_v) = getChoiceDialogObjName(); break;
        case 3: *reinterpret_cast< QVariantMap*>(_v) = getDeviceValue(); break;
        case 4: *reinterpret_cast< QVariantMap*>(_v) = getDataPackage(); break;
        case 5: *reinterpret_cast< QString*>(_v) = getFormType(); break;
        }
        _id -= 6;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setRootObjName(*reinterpret_cast< QString*>(_v)); break;
        case 1: setContentColObjName(*reinterpret_cast< QString*>(_v)); break;
        case 2: setChoiceDialogObjName(*reinterpret_cast< QString*>(_v)); break;
        case 3: setDeviceValue(*reinterpret_cast< QVariantMap*>(_v)); break;
        case 4: setDataPackage(*reinterpret_cast< QVariantMap*>(_v)); break;
        case 5: setFormType(*reinterpret_cast< QString*>(_v)); break;
        }
        _id -= 6;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 6;
    } else if (_c == QMetaObject::RegisterPropertyMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void GenerateForm::dataError(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void GenerateForm::createComponentFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void GenerateForm::checkLegalityErrorOccurred(const QString & _t1, const int & _t2, const QString & _t3, const QString & _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
