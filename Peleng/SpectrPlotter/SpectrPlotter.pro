#-------------------------------------------------
#
# Project created by QtCreator 2015-09-08T13:12:16
#
#-------------------------------------------------

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SpectrPlotter
TEMPLATE = lib
DLLDESTDIR = ../../Debug/MainApp/debug

DEFINES += SPECTRPLUGIN_LIBRARY

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    SpectrPlugin.json

SOURCES +=\
    ../Library/GenericExc.cpp \
    ../Library/HyperCube.cpp \
    ../Library/QCustomPlot.cpp \
    PlotterWindow.cpp \
    SpectrPlugin.cpp \
    ../Library/PluginAttributes/SpectrPluginAttributes.cpp

HEADERS  += \
    ../Library/GenericExc.h \
    ../Library/HyperCube.h \
    ../Library/QCustomPlot.h \
    ../Library/Types.h \
    PlotterWindow.h \
    ../Library/Interfaces/PelengPluginsInterface.h \
    SpectrPlugin.h

INCLUDEPATH += $$PWD/../Library\
               $$PWD/../3DHyperCube
DEPENDPATH += $$PWD/../Library
              $$PWD/../3DHyperCube
FORMS    += \
    PlotterWindow.ui
