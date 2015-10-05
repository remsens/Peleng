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
    ../Library/QCustomPlot.cpp \
    ../Library/GenericExc.cpp \
    ../Library/HyperCube.cpp \
    ../Library/PluginAttributes/SpectrPluginAttributes.cpp \
    ../Library/PluginAttributes/Cube3DPluginAttributes.cpp \
    ../Library/PluginAttributes/ChannelPluginAttributes.cpp \
    ../Library/PluginAttributes/LinePluginAttributes.cpp \
    PluginsControl.cpp

HEADERS  += mainwindow.h \
    ../Library/QCustomPlot.h \
    ../Library/Types.h \
    ../Library/GenericExc.h \
    ../Library/HyperCube.h \
    PluginsControl.h \
    ../Library/PluginAttributes/IAttributes.h \
    ../Library/PluginAttributes/SpectrPluginAttributes.h \
    ../Library/PluginAttributes/Cube3DPluginAttributes.h \
    ../Library/PluginAttributes/ChannelPluginAttributes.h \
    ../Library/PluginAttributes/LinePluginAttributes.h \
    ../Library/Interfaces/FileReadInterface.h \
    ../Library/Interfaces/PelengPluginsInterface.h



INCLUDEPATH += $$PWD/../Library
DEPENDPATH += $$PWD/../Library

FORMS    += mainwindow.ui

RESOURCES += \
    icons.qrc
