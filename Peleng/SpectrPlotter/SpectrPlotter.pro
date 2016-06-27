#-------------------------------------------------
#
# Project created by QtCreator 2015-09-08T13:12:16
#
#-------------------------------------------------

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SpectrPlotter
TEMPLATE = lib
Debug:    DLLDESTDIR = ../../Debug/MainApp/debug
Release:  DLLDESTDIR = ../../Release/MainApp/release

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
    SpectrPlugin.cpp \
    PlotterWindow.cpp \
    ../Library/Attributes/Attributes.cpp \
    ../Library/MathOperations/interpolation.cpp \
    ../Library/Spectr.cpp \
    ../Library/Ellipsoid.cpp \
    SpectrCompareAlg.cpp \
    AddSpectr.cpp \
    SpectrumComparatorFromExternalLibraries.cpp

HEADERS  += \
    ../Library/GenericExc.h \
    ../Library/HyperCube.h \
    ../Library/QCustomPlot.h \
    ../Library/Types.h \
    SpectrPlugin.h \
    PlotterWindow.h \
    ../Library/Interfaces/ProcessingPluginInterface.h \
    ../Library/Attributes/Attributes.h \
    ../Library/MathOperations/interpolation.h \
    ../Library/Spectr.h \
    ../Library/structures.h \
    ../Library/Ellipsoid.h \
    SpectrCompareAlg.h \
    AddSpectr.h \
    SpectrumComparatorFromExternalLibraries.h

INCLUDEPATH += $$PWD/../Library\
DEPENDPATH += $$PWD/../Library
FORMS    += \
    PlotterWindow.ui

RESOURCES += \
    iconsplotter.qrc
