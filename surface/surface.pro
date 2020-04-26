android|ios|winrt {
    error( "This example is not supported for android, ios, or winrt." )
}

!include( ../examples.pri ) {
    error( "Couldn't find the examples.pri file!" )
}

SOURCES += main.cpp \
    gradientdescent/gradientdescent.cpp \
    gradientdescent/gradientdescentwithmomentum.cpp \
    gradientdescent/nesterovmomentum.cpp \
           surfacegraph.cpp \
    mainwindow.cpp \
    gradientdescent/vanillagradientdescent.cpp

HEADERS += surfacegraph.h \
    gradientdescent/gradientdescent.h \
    gradientdescent/gradientdescentwithmomentum.h \
    mainwindow.h \
    gradientdescent/nesterovmomentum.h \
    tinycolormap.hpp \
    gradientdescent/vanillagradientdescent.h

QT += widgets qml
requires(qtConfig(combobox))

RESOURCES += surface.qrc

OTHER_FILES += doc/src/* \
               doc/images/*
