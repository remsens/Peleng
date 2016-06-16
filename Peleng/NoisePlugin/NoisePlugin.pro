QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NoisePlugin
TEMPLATE = lib
Debug:    DLLDESTDIR = ../../Debug/MainApp/debug
Release:    DLLDESTDIR = ../../Release/MainApp/release
DEFINES += NOISE_LIBRARY
CONFIG += plugin
unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    NoisePlugin.json

SOURCES += \
    NoisePlugin.cpp \
    ../Library/GenericExc.cpp \
    ../Library/HyperCube.cpp \
    ../Library/Attributes/Attributes.cpp \
    AlgFactory.cpp \
    ../Library/QCustomPlot.cpp \
    ../Library/CustomPlotForPreview2D/Preview2D.cpp \
    ../Library/Utils/Compare.cpp \
    ../Library/Spectr.cpp \
    ../Library/MathOperations/interpolation.cpp \
    ../Library/Ellipsoid.cpp


HEADERS += \
    NoisePlugin.h \
    ../Library/GenericExc.h \
    ../Library/HyperCube.h \
    ../Library/Types.h \
    ../Library/Attributes/Attributes.h \
    ../Library/Interfaces/ProcessingPluginInterface.h \
    INoiseAlg.h \
    BaseNoiseAlg.h \
    Median2DAlg.h \
    Median1DAlg.h \
    AlgFactory.h \
    ../Library/QCustomPlot.h \
    ../Library/CustomPlotForPreview2D/Preview2D.h \
    ../Library/Utils/Compare.h \
    SavitskiGolay1DAlg.h \
    ../Library/Spectr.h \
    ../Library/structures.h \
    ../Library/MathOperations/interpolation.h \
    ../Library/Ellipsoid.h

RESOURCES += \
    resources.qrc

FORMS += \
    ../Library/CustomPlotForPreview2D/Preview2D.ui

CONFIG += c++11
