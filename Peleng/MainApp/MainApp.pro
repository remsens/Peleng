#-------------------------------------------------
#
# Project created by QtCreator 2015-08-18T17:17:30
#
#-------------------------------------------------

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MainApp
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ../Library/GenericExc.cpp \
    ../Library/HyperCube.cpp \
    PluginsControl.cpp \
    ../Library/PluginLoader.cpp \
    ../Library/ReadPluginLoader.cpp \
    ../Library/ProcessingPluginLoader.cpp \
    ../Library/Attributes/Attributes.cpp

HEADERS  += mainwindow.h \
    ../Library/Types.h \
    ../Library/GenericExc.h \
    ../Library/HyperCube.h \
    PluginsControl.h \
    ../Library/Interfaces/FileReadInterface.h \
    ../Library/PluginLoader.h \
    ../Library/ReadPluginLoader.h \
    ../Library/ProcessingPluginLoader.h \
    ../Library/Interfaces/ProcessingPluginInterface.h \
    ../Library/Attributes/Attributes.h



INCLUDEPATH += $$PWD/../Library
DEPENDPATH += $$PWD/../Library

FORMS    += mainwindow.ui

RESOURCES += \
    icons.qrc
