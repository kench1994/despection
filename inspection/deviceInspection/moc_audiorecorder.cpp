/****************************************************************************
** Meta object code from reading C++ file 'audiorecorder.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/imageProcess/audiorecorder.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'audiorecorder.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AudioRecorder_t {
    QByteArrayData data[22];
    char stringdata[297];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AudioRecorder_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AudioRecorder_t qt_meta_stringdata_AudioRecorder = {
    {
QT_MOC_LITERAL(0, 0, 13),
QT_MOC_LITERAL(1, 14, 21),
QT_MOC_LITERAL(2, 36, 0),
QT_MOC_LITERAL(3, 37, 8),
QT_MOC_LITERAL(4, 46, 6),
QT_MOC_LITERAL(5, 53, 11),
QT_MOC_LITERAL(6, 65, 13),
QT_MOC_LITERAL(7, 79, 6),
QT_MOC_LITERAL(8, 86, 21),
QT_MOC_LITERAL(9, 108, 5),
QT_MOC_LITERAL(10, 114, 13),
QT_MOC_LITERAL(11, 128, 12),
QT_MOC_LITERAL(12, 141, 6),
QT_MOC_LITERAL(13, 148, 14),
QT_MOC_LITERAL(14, 163, 19),
QT_MOC_LITERAL(15, 183, 18),
QT_MOC_LITERAL(16, 202, 17),
QT_MOC_LITERAL(17, 220, 19),
QT_MOC_LITERAL(18, 240, 14),
QT_MOC_LITERAL(19, 255, 14),
QT_MOC_LITERAL(20, 270, 13),
QT_MOC_LITERAL(21, 284, 12)
    },
    "AudioRecorder\0audioRecorderFinished\0"
    "\0filePath\0result\0description\0volumeChanged\0"
    "volume\0soundPlayStateChanged\0state\0"
    "processBuffer\0QAudioBuffer\0buffer\0"
    "soundPlayState\0QMediaPlayer::State\0"
    "startAudioRecorder\0stopAudioRecorder\0"
    "cancelAudioRecorder\0getAudioRecord\0"
    "startPlayAudio\0stopPlayAudio\0getPlayState"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AudioRecorder[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   74,    2, 0x06 /* Public */,
       6,    1,   81,    2, 0x06 /* Public */,
       8,    1,   84,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      10,    1,   87,    2, 0x0a /* Public */,
      13,    1,   90,    2, 0x0a /* Public */,

 // methods: name, argc, parameters, tag, flags
      15,    0,   93,    2, 0x02 /* Public */,
      16,    0,   94,    2, 0x02 /* Public */,
      17,    0,   95,    2, 0x02 /* Public */,
      18,    0,   96,    2, 0x02 /* Public */,
      19,    1,   97,    2, 0x02 /* Public */,
      20,    0,  100,    2, 0x02 /* Public */,
      21,    0,  101,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Bool, QMetaType::QString,    3,    4,    5,
    QMetaType::Void, QMetaType::QReal,    7,
    QMetaType::Void, QMetaType::QString,    9,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 11,   12,
    QMetaType::Void, 0x80000000 | 14,    9,

 // methods: parameters
    QMetaType::Bool,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Bool,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,
    QMetaType::QString,

       0        // eod
};

void AudioRecorder::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AudioRecorder *_t = static_cast<AudioRecorder *>(_o);
        switch (_id) {
        case 0: _t->audioRecorderFinished((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 1: _t->volumeChanged((*reinterpret_cast< const qreal(*)>(_a[1]))); break;
        case 2: _t->soundPlayStateChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->processBuffer((*reinterpret_cast< const QAudioBuffer(*)>(_a[1]))); break;
        case 4: _t->soundPlayState((*reinterpret_cast< QMediaPlayer::State(*)>(_a[1]))); break;
        case 5: { bool _r = _t->startAudioRecorder();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 6: _t->stopAudioRecorder(); break;
        case 7: _t->cancelAudioRecorder(); break;
        case 8: { bool _r = _t->getAudioRecord();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 9: _t->startPlayAudio((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 10: _t->stopPlayAudio(); break;
        case 11: { QString _r = _t->getPlayState();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAudioBuffer >(); break;
            }
            break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QMediaPlayer::State >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (AudioRecorder::*_t)(const QString , bool , const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AudioRecorder::audioRecorderFinished)) {
                *result = 0;
            }
        }
        {
            typedef void (AudioRecorder::*_t)(const qreal & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AudioRecorder::volumeChanged)) {
                *result = 1;
            }
        }
        {
            typedef void (AudioRecorder::*_t)(const QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AudioRecorder::soundPlayStateChanged)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject AudioRecorder::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_AudioRecorder.data,
      qt_meta_data_AudioRecorder,  qt_static_metacall, 0, 0}
};


const QMetaObject *AudioRecorder::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AudioRecorder::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AudioRecorder.stringdata))
        return static_cast<void*>(const_cast< AudioRecorder*>(this));
    return QObject::qt_metacast(_clname);
}

int AudioRecorder::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void AudioRecorder::audioRecorderFinished(const QString _t1, bool _t2, const QString & _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void AudioRecorder::volumeChanged(const qreal & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void AudioRecorder::soundPlayStateChanged(const QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
