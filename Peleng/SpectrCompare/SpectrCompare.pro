#-------------------------------------------------
#
# Project created by QtCreator 2016-05-16T18:14:22
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

Debug:    DLLDESTDIR = ../../Debug/MainApp/debug
Release:    DLLDESTDIR = ../../Release/MainApp/release

TARGET = SpectrCompare
TEMPLATE = lib

DEFINES += SPECTRACOMPARE_PLUGIN

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    SpectraCompare.json

SOURCES +=\
        SpectraCompare.cpp \
    SpectraComparePlugin.cpp

HEADERS  += SpectraCompare.h \
    SpectraComparePlugin.h

FORMS    += SpectraCompare.ui
