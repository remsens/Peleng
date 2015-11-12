#-------------------------------------------------
#
# Project created by QtCreator 2015-10-13T14:17:11
#
#-------------------------------------------------

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 2DHyperCube
TEMPLATE = lib
DLLDESTDIR = ../../Debug/MainApp/debug

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

    Polygon.cpp


    ../Library/Attributes/Attributes.cpp


HEADERS += \
    Hyper2dPlugin.h \
    ../Library/GenericExc.h \
    ../Library/HyperCube.h \
     ../Library/QCustomPlot.h\
    ../Library/Types.h \
    Main2DWindow.h \


    Polygon.h



    ../Library/Attributes/Attributes.h


FORMS += \
    Main2DWindow.ui \

RESOURCES += \
    icons2Dcube.qrc \
