#-------------------------------------------------
#
# Project created by QtCreator 2015-10-05T12:40:11
#
#-------------------------------------------------

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LinePlotter
TEMPLATE = lib
DLLDESTDIR = ../../Debug/MainApp/debug

DEFINES += LINEPLOTTER_LIBRARY

unix {
    target.path = /usr/lib
    INSTALLS += target
}
DISTFILES += LinePlugin.json

SOURCES += \
    ../Library/GenericExc.cpp \
    ../Library/HyperCube.cpp \
    ../Library/QCustomPlot.cpp \
    LinePlotterWindow.cpp \
    LinePlugin.cpp \
    ../Library/PluginAttributes/LinePluginAttributes.cpp

HEADERS +=\
    ../Library/GenericExc.h \
    ../Library/HyperCube.h \
    ../Library/QCustomPlot.h \
    ../Library/Types.h \
    LinePlotterWindow.h \
    ../Library/Interfaces/PelengPluginsInterface.h \
    LinePlugin.h
INCLUDEPATH += $$PWD/../Library\
               $$PWD/../3DHyperCube
DEPENDPATH += $$PWD/../Library\
               $$PWD/../3DHyperCube

RESOURCES += \
    iconslineplotter.qrc



