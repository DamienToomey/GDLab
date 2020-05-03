android|ios|winrt {
    error( "This example is not supported for android, ios, or winrt." )
}

!include( ../examples.pri ) {
    error( "Couldn't find the examples.pri file!" )
}

SOURCES += src/main.cpp \
    src/gradientdescent/adagrad.cpp \
    src/gradientdescent/adam.cpp \
    src/gradientdescent/gradientdescent.cpp \
    src/gradientdescent/gradientdescentwithmomentum.cpp \
    src/gradientdescent/nesterovmomentum.cpp \
    src/gradientdescent/rmsprop.cpp \
    src/mytablewidgetitem.cpp \
    src/resultwindow.cpp \
    src/surfacegraph.cpp \
    src/mainwindow.cpp \
    src/gradientdescent/vanillagradientdescent.cpp

HEADERS += include/surfacegraph.h \
    include/gradientdescent/adagrad.h \
    include/gradientdescent/adam.h \
    include/gradientdescent/gradientdescent.h \
    include/gradientdescent/gradientdescentwithmomentum.h \
    include/gradientdescent/rmsprop.h \
    include/mainwindow.h \
    include/gradientdescent/nesterovmomentum.h \
    include/mytablewidgetitem.h \
    include/resultwindow.h \
    include/tinycolormap.hpp \
    include/gradientdescent/vanillagradientdescent.h

QT += widgets qml
requires(qtConfig(combobox))

RESOURCES += surface.qrc

OTHER_FILES += doc/src/* \
               doc/images/*
