#-------------------------------------------------
#
# Project created by QtCreator 2015-09-28T13:37:07
#
#-------------------------------------------------

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
Debug:    DLLDESTDIR = ../../Debug/MainApp/debug
Release:     DLLDESTDIR = ../../Release/MainApp/release


TARGET = HistPlotter
TEMPLATE = lib

DEFINES += HISTPLUGIN_LIBRARY

SOURCES +=\
    ../Library/GenericExc.cpp \
    ../Library/HyperCube.cpp \
    ../Library/QCustomPlot.cpp \
    histplugin.cpp \
    ../Library/Attributes/Attributes.cpp \
    histplotterwidget.cpp \
    ../Library/Ellipsoid.cpp

HEADERS  += \
    ../Library/GenericExc.h \
    ../Library/HyperCube.h \
    ../Library/QCustomPlot.h \
    ../Library/Types.h \
    histplugin.h \
    ../Library/Utils/typeconvertor.h \
    ../Library/Interfaces/ProcessingPluginInterface.h \
    ../Library/Attributes/Attributes.h \
    histplotterwidget.h \
    ../Library/Ellipsoid.h

DISTFILES += \
    HistPlugin.json

FORMS += \
    histplotterwidget.ui
