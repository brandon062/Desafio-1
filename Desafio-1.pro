TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        encriptacion.cpp \
        lz78.cpp \
        main.cpp \
        rle.cpp \
        utilidades.cpp

HEADERS += \
    encriptacion.h \
    lz78.h \
    rle.h \
    utilidades.h
