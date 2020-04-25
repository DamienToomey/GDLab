/****************************************************************************
** Meta object code from reading C++ file 'surfacegraph.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../surface/surfacegraph.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'surfacegraph.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SurfaceGraph_t {
    QByteArrayData data[16];
    char stringdata0[206];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SurfaceGraph_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SurfaceGraph_t qt_meta_stringdata_SurfaceGraph = {
    {
QT_MOC_LITERAL(0, 0, 12), // "SurfaceGraph"
QT_MOC_LITERAL(1, 13, 11), // "changeTheme"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 5), // "theme"
QT_MOC_LITERAL(4, 32, 14), // "changeFunction"
QT_MOC_LITERAL(5, 47, 8), // "function"
QT_MOC_LITERAL(6, 56, 19), // "changeSelectionMode"
QT_MOC_LITERAL(7, 76, 13), // "selectionMode"
QT_MOC_LITERAL(8, 90, 14), // "changeColormap"
QT_MOC_LITERAL(9, 105, 8), // "colormap"
QT_MOC_LITERAL(10, 114, 13), // "changeSurface"
QT_MOC_LITERAL(11, 128, 7), // "surface"
QT_MOC_LITERAL(12, 136, 5), // "graph"
QT_MOC_LITERAL(13, 142, 11), // "Q3DSurface*"
QT_MOC_LITERAL(14, 154, 30), // "preprocessArithmeticExpression"
QT_MOC_LITERAL(15, 185, 20) // "arithmeticexpression"

    },
    "SurfaceGraph\0changeTheme\0\0theme\0"
    "changeFunction\0function\0changeSelectionMode\0"
    "selectionMode\0changeColormap\0colormap\0"
    "changeSurface\0surface\0graph\0Q3DSurface*\0"
    "preprocessArithmeticExpression\0"
    "arithmeticexpression"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SurfaceGraph[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   49,    2, 0x0a /* Public */,
       4,    1,   52,    2, 0x0a /* Public */,
       6,    1,   55,    2, 0x0a /* Public */,
       8,    1,   58,    2, 0x0a /* Public */,
      10,    1,   61,    2, 0x0a /* Public */,
      12,    0,   64,    2, 0x0a /* Public */,
      14,    1,   65,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::Int,   11,
    0x80000000 | 13,
    QMetaType::QString, QMetaType::QString,   15,

       0        // eod
};

void SurfaceGraph::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SurfaceGraph *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->changeTheme((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->changeFunction((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->changeSelectionMode((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->changeColormap((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->changeSurface((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: { Q3DSurface* _r = _t->graph();
            if (_a[0]) *reinterpret_cast< Q3DSurface**>(_a[0]) = std::move(_r); }  break;
        case 6: { QString _r = _t->preprocessArithmeticExpression((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject SurfaceGraph::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_SurfaceGraph.data,
    qt_meta_data_SurfaceGraph,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SurfaceGraph::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SurfaceGraph::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SurfaceGraph.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int SurfaceGraph::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
