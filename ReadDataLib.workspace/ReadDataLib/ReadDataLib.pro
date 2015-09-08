#-------------------------------------------------
#
# Project created by QtCreator 2015-09-07T11:17:51
#
#-------------------------------------------------

QT       -= gui

TARGET = ReadDataLib
TEMPLATE = lib

DEFINES += READDATALIB_LIBRARY

SOURCES += ReadDataLib.cpp \
    Context.cpp \
    FilesOperation.cpp \
    HyperCube.cpp \
    GenericExc.cpp

HEADERS += ReadDataLib.h \
    ReadDataLib_global.h \
    Types.h \
    HyperCube.h \
    Context.h \
    FilesOperation.h \
    GenericExc.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
