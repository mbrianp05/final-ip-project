TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    interfaz/io.c \
    logica/logica.c

HEADERS += \
    logica/logica.h \
    logica/types.h \
    interfaz/io.h

