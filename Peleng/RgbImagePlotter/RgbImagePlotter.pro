#-------------------------------------------------
#
# Project created by QtCreator 2015-11-03T12:32:52
#
#-------------------------------------------------

QT       += core gui printsupport

TARGET = RgbImagePlotter
TEMPLATE = lib
CONFIG += plugin

DESTDIR = ../../Debug/MainApp/debug

SOURCES += \
    rgbimageplugin.cpp \
    settingsdialog.cpp \
    ../Library/HyperCube.cpp \
    ../Library/GenericExc.cpp \
    rgbprofile.cpp \
    rgbimagewindow.cpp \
    ../Library/QCustomPlot.cpp \
    zoomimage.cpp

HEADERS += \
    rgbimageplugin.h \
    settingsdialog.h \
    cie.h \
    templeteconvolute.h \
    ../Library/HyperCube.h \
    ../Library/GenericExc.h \
    rgbprofile.h \
    rgbimagewindow.h \
    ../Library/QCustomPlot.h \
    zoomimage.h \
    ../Library/Interfaces/ProcessingPluginInterface.h
DISTFILES += \
    RgbImagePlugin.json

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    settingsdialog.ui \
    rgbimagewindow.ui

RESOURCES += \
    icons.qrc
