#-------------------------------------------------
#
# Project created by QtCreator 2015-08-18T17:32:52
#
#-------------------------------------------------

QT       -= gui

TARGET = AvirisReadPlugin
TEMPLATE = lib
debug{
    DLLDESTDIR = ../../Debug/MainApp/debug
}
release{
    DLLDESTDIR = ../../Release/MainApp/release
}
CONFIG += plugin

DEFINES += AVIRISREADPLUGIN_LIBRARY

SOURCES += avirisreadplugin.cpp \
    ../Library/GenericExc.cpp

HEADERS += avirisreadplugin.h \
    ../Library/Interfaces/FileReadInterface.h \
    ../Library/GenericExc.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    AvirisReadPlugin.json

INCLUDEPATH += $$PWD/../Library
DEPENDPATH += $$PWD/../Library

INCLUDEPATH += $$PWD/ReadDataAviris
DEPENDPATH += $$PWD/ReadDataAviris
LIBS += $$OUT_PWD/ReadDataLib.dll
