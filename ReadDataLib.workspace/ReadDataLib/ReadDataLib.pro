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
    ../../Library/GenericExc.cpp \
    ../../Library/HyperCube.cpp

HEADERS += ReadDataLib.h \
    ReadDataLib_global.h \
    Types.h \
    Context.h \
    FilesOperation.h \
    GenericExc.h \
    ../../Library/HyperCube.h \
    ../../Library/Types.h \
    ../../Library/GenericExc.h

INCLUDEPATH += $$PWD/../../../Library
DEPENDPATH += $$PWD/../../../Library

unix {
    target.path = /usr/lib
    INSTALLS += target
}
