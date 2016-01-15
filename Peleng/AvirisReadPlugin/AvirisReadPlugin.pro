#-------------------------------------------------
#
# Project created by QtCreator 2015-08-18T17:32:52
#
#-------------------------------------------------

QT       += core

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AvirisReadPlugin
TEMPLATE = lib
Debug:    DLLDESTDIR = ../../Debug/MainApp/debug
Release:  DLLDESTDIR = ../../Release/MainApp/release

CONFIG += plugin

DEFINES += AVIRISREADPLUGIN_LIBRARY

SOURCES += avirisreadplugin.cpp \
    CreaterHyperCubes.cpp \
    ../Library/HyperCube.cpp \
    ../Library/GenericExc.cpp

HEADERS += avirisreadplugin.h \
    ../Library/Interfaces/FileReadInterface.h \
    CreaterHyperCubes.h \
    ../Library/HyperCube.h \
    ../Library/Types.h \
    ../Library/GenericExc.h \
    ConvertDataCubeToLittleEndian.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    AvirisReadPlugin.json

