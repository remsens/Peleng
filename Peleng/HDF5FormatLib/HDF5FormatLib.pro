#-------------------------------------------------
#
# Project created by QtCreator 2015-09-09T12:01:01
#
#-------------------------------------------------

QT       -= gui

TARGET = HDF5FormatLib
TEMPLATE = lib
DLLDESTDIR = ../../Debug/MainApp/debug

DEFINES += HDF5FORMATLIB_LIBRARY

SOURCES += HDF5FormatLib.cpp \
    ../Library/GenericExc.cpp

HEADERS += HDF5FormatLib.h\
        hdf5formatlib_global.h \
    ../Library/GenericExc.h \
    ../Library/Types.h

INCLUDEPATH += $$PWD/../Library
DEPENDPATH += $$PWD/../Library

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32: LIBS += -L$$PWD/include/ -lhdf5

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include
