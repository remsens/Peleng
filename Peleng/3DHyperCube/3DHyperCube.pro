TARGET = 3DCubePlugin
TEMPLATE = lib
DEFINES += 3DCUBEPLUGIN_LIBRARY

QT           += widgets
QT           += core gui printsupport

HEADERS       = \
    MainWindow.h \
    GLWidget.h \
    ui_MainWindow3DCube.h \
    ../Library/GenericExc.h \
    ../Library/HyperCube.h \
    ../Library/Types.h \
    ../Library/QCustomPlot.h \
    ../SpectrPlotter/PlotterWindow.h \
    CubePlugin.h \
    ../Library/Interfaces/PelengPluginsInterface.h \
    ../Library/PluginAttributes/Cube3DPluginAttributes.h \
    CubePlugin_global.h

SOURCES       = \
    GLWidget.cpp \
    MainWindow.cpp \
    ../Library/GenericExc.cpp \
    ../Library/HyperCube.cpp \
    ../Library/QCustomPlot.cpp \
    ../SpectrPlotter/PlotterWindow.cpp \
    CubePlugin.cpp \
    ../Library/PluginAttributes/Cube3DPluginAttributes.cpp

INCLUDEPATH += $$PWD/../Library\
                $$PWD/../SpectrPlotter\
DEPENDPATH += $$PWD/../Library
                 $$PWD/../SpectrPlotter\



DISTFILES += \
    3DCubePlugin.json
    
FORMS += \
    mainwindow.ui \
    ../SpectrPlotter/PlotterWindow.ui


CONFIG += c++11

SUBDIRS += \
    ../SpectrPlotter/SpectrPlotter.pro

win32: LIBS += -L$$PWD/E:/Qt5.5/Tools/mingw492_32/i686-w64-mingw32/lib/ -lglu32

INCLUDEPATH += $$PWD/E:/Qt5.5/Tools/mingw492_32/i686-w64-mingw32/include
DEPENDPATH += $$PWD/E:/Qt5.5/Tools/mingw492_32/i686-w64-mingw32/include

win32: LIBS += -L$$PWD/E:/Qt5.5/Tools/mingw492_32/i686-w64-mingw32/lib/ -lopengl32
