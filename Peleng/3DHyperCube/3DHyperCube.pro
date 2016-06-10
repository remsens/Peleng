TARGET = 3DCubePlugin
TEMPLATE = lib
Debug:    DLLDESTDIR = ../../Debug/MainApp/debug
Release:    DLLDESTDIR = ../../Release/MainApp/release

QT           += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets xml

CONFIG += plugin
CONFIG += c++11

unix {
    target.path = /usr/lib
    INSTALLS += target
}

HEADERS       = \
    CubePlugin.h \
    ../Library/Interfaces/ProcessingPluginInterface.h \
    ../Library/HyperCube.h \
    ../Library/Types.h \
    ../Library/GenericExc.h \
    GLWidget.h \
    mainwindow.h \
    ../Library/QCustomPlot.h \
    ContrastWindow.h \
    ../Library/Attributes/Attributes.h \
    ../Library/FileProjectFeatures.h \
    ../Library/stepsdefinitions.h \
    ../Library/Utils/Compare.h \
    ../Library/Spectr.h \
    ../Library/MathOperations/interpolation.h
    ../Library/saveENVI.h

SOURCES       = \
    CubePlugin.cpp \
    ../Library/HyperCube.cpp \
    ../Library/GenericExc.cpp \
    GLWidget.cpp \
    mainwindow.cpp \
    ../Library/QCustomPlot.cpp \
    ContrastWindow.cpp \
    ../Library/Attributes/Attributes.cpp \
    ../Library/FileProjectFeatures.cpp \
    ../Library/Utils/Compare.cpp \
    ../Library/Spectr.cpp \
    ../Library/MathOperations/interpolation.cpp



DISTFILES += \
    3DCubePlugin.json

FORMS += \
    mainwindow.ui \
    ContrastWindow.ui



SUBDIRS += \
    ../SpectrPlotter/SpectrPlotter.pro


win32: LIBS += -L$$PWD/E:/Qt5.5/Tools/mingw492_32/i686-w64-mingw32/lib/ -lglu32

INCLUDEPATH += $$PWD/E:/Qt5.5/Tools/mingw492_32/i686-w64-mingw32/include
DEPENDPATH += $$PWD/E:/Qt5.5/Tools/mingw492_32/i686-w64-mingw32/include

win32: LIBS += -L$$PWD/E:/Qt5.5/Tools/mingw492_32/i686-w64-mingw32/lib/ -lopengl32

win32: LIBS += -L$$PWD/../Library/GDAL/lib/ -lgdal_i
INCLUDEPATH += $$PWD/../Library/GDAL/include
DEPENDPATH += $$PWD/../Library/GDAL/include

RESOURCES += \
    icons3Dcube.qrc \
    splash.qrc

