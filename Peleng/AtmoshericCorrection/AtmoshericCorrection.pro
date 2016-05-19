#-------------------------------------------------
#
# Project created by QtCreator 2016-05-16T13:44:33
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = AtmoshericCorrection
TEMPLATE = lib
CONFIG += plugin

Debug:    DLLDESTDIR = ../../Debug/MainApp/debug
Release:    DLLDESTDIR = ../../Release/MainApp/release

SOURCES += \
    atmoshericcorrection.cpp \
    atmcorrmodule.cpp \
    ../Library/HyperCube.cpp \
    ../Library/GenericExc.cpp

HEADERS += \
    atmoshericcorrection.h \
    atmcorrmodule.h \
    ../Library/HyperCube.h \
    ../Library/GenericExc.h
DISTFILES += AtmoshericCorrection.json

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS +=
