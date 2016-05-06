#-------------------------------------------------
#
# Project created by QtCreator 2015-10-13T14:17:11
#
#-------------------------------------------------

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 2DHyperCube
TEMPLATE = lib

Debug:    DLLDESTDIR = ../../Debug/MainApp/debug
Release:     DLLDESTDIR = ../../Release/MainApp/release

DEFINES += TWODHYPERCUBE_LIBRARY
CONFIG += plugin
unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    2DCubePlugin.json

SOURCES += \
    Hyper2dPlugin.cpp \
    ../Library/GenericExc.cpp \
    ../Library/HyperCube.cpp \
    ../Library/QCustomPlot.cpp \
    Main2DWindow.cpp \
    ../Library/Attributes/Attributes.cpp \
    PolygonManager.cpp \
    Region.cpp\
    ../Library/GeoTiff.cpp



HEADERS += \
    Hyper2dPlugin.h \
    ../Library/GenericExc.h \
    ../Library/HyperCube.h \
     ../Library/QCustomPlot.h\
    ../Library/Types.h \
    Main2DWindow.h \
    ../Library/Attributes/Attributes.h \
    PolygonManager.h \
    Region.h \
    ../Library/GeoTiff.h


FORMS += \
    Main2DWindow.ui \
    PolygonManager.ui

RESOURCES += \
    icons2Dcube.qrc \

# _______________________
LIBS += -LD:/Work/Programming/Other/GDAL/release-1800-x64-gdal-1-11-3-mapserver-6-4-2-libs/lib -lgdal_i

INCLUDEPATH += D:/Work/Programming/Other/GDAL/release-1800-x64-gdal-1-11-3-mapserver-6-4-2-libs/include
DEPENDPATH += D:/Work/Programming/Other/GDAL/release-1800-x64-gdal-1-11-3-mapserver-6-4-2-libs/include
