QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SpectralLibraries
TEMPLATE = lib
DLLDESTDIR = ../../Debug/MainApp/debug

DEFINES += SPECTRAL_LIBRARY
CONFIG += plugin
unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    2DCubePlugin.json \
    SpectralLibPlugin.json

HEADERS += \
    SpectralLibPlugin.h \
    ../Library/Interfaces/ProcessingPluginInterface.h

SOURCES += \
    SpectralLibPlugin.cpp
