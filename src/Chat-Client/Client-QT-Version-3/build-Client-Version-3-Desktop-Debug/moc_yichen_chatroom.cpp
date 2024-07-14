/****************************************************************************
** Meta object code from reading C++ file 'yichen_chatroom.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../Client-Version-3/yichen_chatroom.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'yichen_chatroom.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_yichen_chatroom_t {
    QByteArrayData data[8];
    char stringdata0[90];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_yichen_chatroom_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_yichen_chatroom_t qt_meta_stringdata_yichen_chatroom = {
    {
QT_MOC_LITERAL(0, 0, 15), // "yichen_chatroom"
QT_MOC_LITERAL(1, 16, 9), // "slotEnter"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 13), // "slotConnected"
QT_MOC_LITERAL(4, 41, 16), // "slotDisconnected"
QT_MOC_LITERAL(5, 58, 12), // "dataReceived"
QT_MOC_LITERAL(6, 71, 8), // "slotSend"
QT_MOC_LITERAL(7, 80, 9) // "slotClear"

    },
    "yichen_chatroom\0slotEnter\0\0slotConnected\0"
    "slotDisconnected\0dataReceived\0slotSend\0"
    "slotClear"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_yichen_chatroom[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x0a /* Public */,
       3,    0,   45,    2, 0x0a /* Public */,
       4,    0,   46,    2, 0x0a /* Public */,
       5,    0,   47,    2, 0x0a /* Public */,
       6,    0,   48,    2, 0x0a /* Public */,
       7,    0,   49,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void yichen_chatroom::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<yichen_chatroom *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->slotEnter(); break;
        case 1: _t->slotConnected(); break;
        case 2: _t->slotDisconnected(); break;
        case 3: _t->dataReceived(); break;
        case 4: _t->slotSend(); break;
        case 5: _t->slotClear(); break;
        default: ;
        }
    }
    (void)_a;
}

QT_INIT_METAOBJECT const QMetaObject yichen_chatroom::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_yichen_chatroom.data,
    qt_meta_data_yichen_chatroom,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *yichen_chatroom::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *yichen_chatroom::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_yichen_chatroom.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int yichen_chatroom::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
