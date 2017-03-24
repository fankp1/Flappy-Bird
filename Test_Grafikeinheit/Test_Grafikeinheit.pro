#-------------------------------------------------
#
# Project created by QtCreator 2014-02-10T13:51:05
#
#-------------------------------------------------

QT       += core \
    widgets \
    gui \
    network \
    multimedia


TARGET = BasisprojektGraphikprogramm
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += \
    LibrarySource/gui_main.cpp \
    LibrarySource/main.cpp \
    LibrarySource/Networking.cpp \
    LibrarySource/window.cpp \
    LibrarySource/Worker.cpp \
    LibrarySource/communication.c \
    LibrarySource/dbgmmgt.c \
    LibrarySource/error.c \
    Test_Grafikeinheit.c

HEADERS += \
    communication.h \
    dbgmmgt.h \
    error.h \
    window.h \
    LibrarySource/gui_main.h \
    LibrarySource/networking.h \
    LibrarySource/qtguilibrary.h \
    LibrarySource/Worker.h

DISTFILES += \
    Resources/sound1.wav \
    Resources/sound2.wav \
    Resources/sound4.wav \
    Resources/TestImage.png

install_it.path = $$OUT_PWD/debug
install_it.files = Resources/*

INSTALLS += install_it

QMAKE_EXTRA_TARGETS += install
