#-------------------------------------------------
#
# Project created by QtCreator 2016-06-16T13:38:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ReadDataPlugin
TEMPLATE = lib
Debug:    DLLDESTDIR = ../../Debug/MainApp/debug
Release:  DLLDESTDIR = ../../Release/MainApp/release

CONFIG += plugin

DEFINES += READDATAPLUGIN_LIBRARY

SOURCES += \
    CreaterHyperCubes.cpp \
    ReadDataPlugin.cpp \
    ../Library/Ellipsoid.cpp \
    ../Library/HyperCube.cpp \
    ../Library/GenericExc.cpp

HEADERS += \
    CreaterHyperCubes.h \
    ReadDataPlugin.h \
    ../Library/Interfaces/FileReadInterface.h \
    ../Library/convertdatacubetolittleendian.h \
    ../Library/Ellipsoid.h \
    ../Library/HyperCube.h \
    ../Library/Types.h \
    ../Library/GenericExc.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    ReadDataPlugin.json

