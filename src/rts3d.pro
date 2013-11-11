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
    gl_ext/libExt.cpp \
    cnoise.cpp \
    clandscape.cpp \
    ccamera.cpp \
    cmodel.cpp \
    cwater.cpp

HEADERS  += MainWindow.h \
    gl_ext/libExt.h \
    cnoise.h \
    clandscape.h \
    ccamera.h \
    cmodel.h \
    cwater.h

OTHER_FILES += \
    shaders/water.vert \
    shaders/water.frag \
    shaders/surface.frag \
    shaders/surface.vert \
    shaders/model.vert \
    shaders/model.frag

RESOURCES += \
    shaders.qrc
