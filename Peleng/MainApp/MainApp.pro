#-------------------------------------------------
#
# Project created by QtCreator 2015-08-18T17:17:30
#
#-------------------------------------------------

QT       += core gui printsupport xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ThemeInfo
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ../Library/GenericExc.cpp \
    ../Library/HyperCube.cpp \
    PluginsControl.cpp \
    ../Library/PluginLoader.cpp \
    ../Library/ReadPluginLoader.cpp \
    ../Library/ProcessingPluginLoader.cpp \
    ../Library/Attributes/Attributes.cpp \
    FileProjectWindow.cpp \
    ../Library/FileProjectFeatures.cpp \
    ../Library/Ellipsoid.cpp

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
    ../Library/Attributes/Attributes.h \
    FileProjectWindow.h \
    ../Library/FileProjectFeatures.h \
    ../Library/stepsdefinitions.h \
    ../Library/Ellipsoid.h



INCLUDEPATH += $$PWD/../Library
DEPENDPATH += $$PWD/../Library

FORMS    += mainwindow.ui \
    FileProjectWindow.ui

RESOURCES += \
    icons.qrc
CONFIG += c++11

