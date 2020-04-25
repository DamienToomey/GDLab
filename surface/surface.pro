android|ios|winrt {
    error( "This example is not supported for android, ios, or winrt." )
}

!include( ../examples.pri ) {
    error( "Couldn't find the examples.pri file!" )
}

SOURCES += main.cpp \
    gradientdescent.cpp \
           surfacegraph.cpp \
    mainwindow.cpp \
    vanillagradientdescent.cpp

HEADERS += surfacegraph.h \
    gradientdescent.h \
    mainwindow.h \
    tinycolormap.hpp \
    vanillagradientdescent.h

QT += widgets qml
requires(qtConfig(combobox))

RESOURCES += surface.qrc

OTHER_FILES += doc/src/* \
               doc/images/*
