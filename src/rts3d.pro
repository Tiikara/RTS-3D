#-------------------------------------------------
#
# Project created by QtCreator 2013-10-07T20:30:52
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = rts3d
TEMPLATE = app

QMAKE_CXXFLAGS += -std=gnu++0x

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
    cwater.cpp \
    cscene.cpp \
    cbaseobject.cpp \
    cselection.cpp \
    ctree.cpp \
    generateforest.cpp \
    cwindow.cpp \
    cbutton.cpp \
    canimationobject.cpp \
    ccommandviewcontroller.cpp \
    ctexture.cpp \
    cpeasant.cpp \
    cunit.cpp \
    cmouseevent.cpp \
    ccursorcontroller.cpp

HEADERS  += MainWindow.h \
    gl_ext/libExt.h \
    cnoise.h \
    clandscape.h \
    ccamera.h \
    cmodel.h \
    cwater.h \
    cscene.h \
    cbaseobject.h \
    cselection.h \
    ctree.h \
    generateforest.h \
    cwindow.h \
    cbutton.h \
    canimationobject.h \
    ccommandviewcontroller.h \
    ctexture.h \
    cpeasant.h \
    cunit.h \
    cmouseevent.h \
    ccursorcontroller.h

OTHER_FILES += \
    shaders/water.vert \
    shaders/water.frag \
    shaders/surface.frag \
    shaders/surface.vert \
    shaders/model.vert \
    shaders/model.frag

RESOURCES += \
    shaders.qrc
