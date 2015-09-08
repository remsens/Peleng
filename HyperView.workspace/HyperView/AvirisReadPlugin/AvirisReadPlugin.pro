#-------------------------------------------------
#
# Project created by QtCreator 2015-08-18T17:32:52
#
#-------------------------------------------------

QT       -= gui

TARGET = AvirisReadPlugin
TEMPLATE = lib

DEFINES += AVIRISREADPLUGIN_LIBRARY

SOURCES += avirisreadplugin.cpp \
    ../../../Library/GenericExc.cpp

HEADERS += avirisreadplugin.h \
    ../MainApp/Interfaces/filereadinterface.h \
    ../../../Library/GenericExc.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    AvirisReadPlugin.json


INCLUDEPATH += $$PWD/ReadDataAviris
DEPENDPATH += $$PWD/ReadDataAviris

INCLUDEPATH += $$PWD/../MainApp/
DEPENDPATH += $$PWD/../MainApp/

INCLUDEPATH += $$PWD/../MainApp/HyperCube
DEPENDPATH += $$PWD/../MainApp/HyperCube

INCLUDEPATH += $$PWD/../../../Library
DEPENDPATH += $$PWD/../../../Library

INCLUDEPATH += $$PWD/ReadDataAviris
DEPENDPATH += $$PWD/ReadDataAviris
LIBS += $$OUT_PWD/Debug/ReadDataLib.dll
