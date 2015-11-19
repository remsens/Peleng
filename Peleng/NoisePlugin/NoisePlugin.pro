QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Noise
TEMPLATE = lib
DLLDESTDIR = ../../Debug/MainApp/debug

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
    Median1DAlg.cpp \
    Utils.cpp \
    ../Library/QCustomPlot.cpp


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
    Utils.h \
    ../Library/QCustomPlot.h

RESOURCES += \
    resources.qrc


