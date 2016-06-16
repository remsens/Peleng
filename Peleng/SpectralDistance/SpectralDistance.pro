QT       = quick widgets printsupport

TARGET = SpectralDistance
TEMPLATE = lib

DLLDESTDIR = ../../Debug/MainApp/debug
DEFINES += SPECTRALDISTANCE_LIBRARY
CONFIG += plugin
CONFIG += c++11
unix {
    target.path = /usr/lib
    INSTALLS += target
}

Debug:    DLLDESTDIR = ../../Debug/MainApp/debug
Release:    DLLDESTDIR = ../../Release/MainApp/release

DISTFILES += \
    SpectralDistance.json \


SOURCES += spectraldistance.cpp \
    ../Library/HyperCube.cpp \
    ../Library/GenericExc.cpp \
    ../Library/Attributes/Attributes.cpp \
    ../Library/CustomPlotForPreview2D/Preview2D.cpp \
    ../Library/QCustomPlot.cpp \
    SpecDistWindow.cpp \
    ../Library/Ellipsoid.cpp

HEADERS += spectraldistance.h\
    ../Library/HyperCube.h \
    ../Library/Types.h \
    ../Library/Attributes/Attributes.h \
    ../Library/Interfaces/ProcessingPluginInterface.h \
    ../Library/CustomPlotForPreview2D/Preview2D.h \
    ../Library/QCustomPlot.h \
    SpecDistWindow.h \
    ../Library/Ellipsoid.h



RESOURCES +=

FORMS += \
    ../Library/CustomPlotForPreview2D/Preview2D.ui \
    SpecDistWindow.ui


