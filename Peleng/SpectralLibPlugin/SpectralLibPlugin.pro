QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SpectralLibraries
TEMPLATE = lib
Debug:    DLLDESTDIR = ../../Debug/MainApp/debug
Release:     DLLDESTDIR = ../../Release/MainApp/release


DEFINES += SPECTRAL_LIBRARY
CONFIG += plugin
CONFIG += c++11
unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    2DCubePlugin.json \
    SpectralLibPlugin.json

HEADERS += \
    SpectralLibPlugin.h \
    ../Library/Interfaces/ProcessingPluginInterface.h \
    ../Library/HyperCube.h \
    CreateSpectr.h \
    ../Library/Attributes/Attributes.h \
    ../Library/GenericExc.h \
    ../Library/Types.h \
    AddSpectr.h \
    ../Library/MathOperations/Interpolation.h

SOURCES += \
    SpectralLibPlugin.cpp \
    CreateSpectr.cpp \
    ../Library/HyperCube.cpp \
    ../Library/Attributes/Attributes.cpp \
    ../Library/GenericExc.cpp \
    AddSpectr.cpp \
    ../Library/MathOperations/Interpolation.cpp

FORMS += \
    CreateSpectr.ui \
    AddSpectr.ui

RESOURCES += \
    resources.qrc
