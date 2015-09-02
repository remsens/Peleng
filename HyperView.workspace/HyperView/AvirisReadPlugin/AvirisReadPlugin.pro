#-------------------------------------------------
#
# Project created by QtCreator 2015-08-18T17:32:52
#
#-------------------------------------------------

QT       -= gui

TARGET = AvirisReadPlugin
TEMPLATE = lib

DEFINES += AVIRISREADPLUGIN_LIBRARY

SOURCES += avirisreadplugin.cpp

HEADERS += avirisreadplugin.h \
    ../MainApp/Interfaces/filereadinterface.h

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


win32: LIBS += -L$$PWD//ReadDataAviris// -lReadDataLib
INCLUDEPATH += $$PWD/ReadDataAviris
DEPENDPATH += $$PWD/ReadDataAviris
