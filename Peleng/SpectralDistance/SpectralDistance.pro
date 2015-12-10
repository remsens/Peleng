QT       = quick widgets printsupport

TARGET = SpectralDistance
TEMPLATE = lib

DLLDESTDIR = ../../Debug/MainApp/debug
DEFINES += SPECTRALDISTANCE_LIBRARY
CONFIG += plugin
unix {
    target.path = /usr/lib
    INSTALLS += target
}

Debug:    DLLDESTDIR = ../../Debug/MainApp/debug
Release:    DLLDESTDIR = ../../Release/MainApp/release

DISTFILES += \
    SpectralDistance.json \
    sdistancewin.qml

SOURCES += spectraldistance.cpp \
    ../Library/HyperCube.cpp \
    ../Library/GenericExc.cpp \
    ../Library/Attributes/Attributes.cpp \
    ../Library/CustomPlotForPreview2D/Preview2D.cpp \
    ../Library/QCustomPlot.cpp

HEADERS += spectraldistance.h\
    ../Library/HyperCube.h \
    ../Library/Types.h \
    ../Library/Attributes/Attributes.h \
    ../Library/Interfaces/ProcessingPluginInterface.h \
    ../Library/CustomPlotForPreview2D/Preview2D.h \
    ../Library/QCustomPlot.h



RESOURCES += \
    qmldata.qrc

FORMS += \
    ../Library/CustomPlotForPreview2D/Preview2D.ui


