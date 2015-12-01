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
    ../Library/QCustomPlot.cpp \
    ../Library/CustomPlotForPreview2D/Preview2D.cpp \
    ../Library/Utils/Compare.cpp \
    ../Library/BusyIndicator/QProgressIndicator.cpp


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
    ../Library/BusyIndicator/QProgressIndicator.h

RESOURCES += \
    resources.qrc

FORMS += \
    ../Library/CustomPlotForPreview2D/Preview2D.ui


