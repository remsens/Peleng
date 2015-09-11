#-------------------------------------------------
#
# Project created by QtCreator 2015-09-10T12:29:28
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = HDF5FormatTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    ../../Library/GenericExc.cpp \
    HDF5FormatLib.cpp

win32: LIBS += -L$$PWD/../HDF5FormatLib/include/ -lhdf5

INCLUDEPATH += $$PWD/../HDF5FormatLib/include
DEPENDPATH += $$PWD/../HDF5FormatLib/include

INCLUDEPATH += $$PWD/../../../Library
DEPENDPATH += $$PWD/../../../Library

HEADERS += \
    ../../Library/Types.h \
    ../../Library/GenericExc.h \
    HDF5FormatLib.h
