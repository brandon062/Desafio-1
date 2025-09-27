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

DISTFILES += \
    ../../Downloads/datasetDesarrollo/Encriptado1.txt \
    ../../Downloads/datasetDesarrollo/Encriptado2.txt \
    ../../Downloads/datasetDesarrollo/Encriptado3.txt \
    ../../Downloads/datasetDesarrollo/Encriptado4.txt \
    ../../Downloads/datasetDesarrollo/pista1.txt \
    ../../Downloads/datasetDesarrollo/pista2.txt \
    ../../Downloads/datasetDesarrollo/pista3.txt \
    ../../Downloads/datasetDesarrollo/pista4.txt
