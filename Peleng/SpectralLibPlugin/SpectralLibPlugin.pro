QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SpectralLibraries
TEMPLATE = lib
debug{
    DLLDESTDIR = ../../Debug/MainApp/debug
}
release{
    DLLDESTDIR = ../../Release/MainApp/release
}

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
    ../Library/Interfaces/ProcessingPluginInterface.h \
    ../Library/HyperCube.h \
    CreateSpectr.h \
    ../Library/Attributes/Attributes.h \
    ../Library/GenericExc.h \
    ../Library/Types.h \
    AddSpectr.h

SOURCES += \
    SpectralLibPlugin.cpp \
    CreateSpectr.cpp \
    ../Library/HyperCube.cpp \
    ../Library/Attributes/Attributes.cpp \
    ../Library/GenericExc.cpp \
    AddSpectr.cpp

FORMS += \
    CreateSpectr.ui \
    AddSpectr.ui

RESOURCES += \
    resources.qrc
