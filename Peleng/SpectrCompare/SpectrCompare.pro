#-------------------------------------------------
#
# Project created by QtCreator 2016-05-16T18:14:22
#
#-------------------------------------------------

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SpectrCompare
TEMPLATE = lib

DEFINES += SPECTRACOMPARE_PLUGIN

Debug:    DLLDESTDIR = ../../Debug/MainApp/debug
Release:    DLLDESTDIR = ../../Release/MainApp/release

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    SpectraCompare.json

SOURCES +=\
        SpectraCompare.cpp \
    SpectraComparePlugin.cpp \
    ../Library/QCustomPlot.cpp

HEADERS  += SpectraCompare.h \
    SpectraComparePlugin.h \
    ../Library/QCustomPlot.h

FORMS    += SpectraCompare.ui
