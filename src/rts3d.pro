#-------------------------------------------------
#
# Project created by QtCreator 2013-10-07T20:30:52
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = rts3d
TEMPLATE = app

INCLUDEPATH += ../include/
DESTDIR = ../bin/

MOC_DIR = ../build/moc/
OBJECTS_DIR = ../build/obj/
RCC_DIR = ../build/rcc/
UI_DIR = ../build/uic/

SOURCES += main.cpp\
        MainWindow.cpp \
    cNoise.cpp \
    landscape.cpp \
    camera.cpp \
    gl_ext/libExt.cpp

HEADERS  += MainWindow.h \
    cNoise.h \
    landscape.h \
    camera.h \
    gl_ext/libExt.h

OTHER_FILES += \
    surf_vert.vert \
    surf_frag.frag
