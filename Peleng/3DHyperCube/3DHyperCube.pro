TARGET = 3DCubePlugin
TEMPLATE = lib
DLLDESTDIR = ../../Debug/MainApp/debug
QT           += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += plugin
CONFIG += c++11

unix {
    target.path = /usr/lib
    INSTALLS += target
}

HEADERS       = \
    CubePlugin.h \
    ../Library/Interfaces/PelengPluginsInterface.h \
    ../Library/PluginAttributes/Cube3DPluginAttributes.h \
    ../Library/HyperCube.h \
    ../Library/Types.h \
    ../Library/GenericExc.h \
    GLWidget.h \
    mainwindow.h \
    ui_MainWindow3DCube.h \
    ../Library/QCustomPlot.h \
    ../SpectrPlotter/PlotterWindow.h \
    PlotterAlongLine.h

SOURCES       = \
    CubePlugin.cpp \
    ../Library/PluginAttributes/Cube3DPluginAttributes.cpp \
    ../Library/HyperCube.cpp \
    ../Library/GenericExc.cpp \
    GLWidget.cpp \
    mainwindow.cpp \
    ../Library/QCustomPlot.cpp \
    ../SpectrPlotter/PlotterWindow.cpp \
    PlotterAlongLine.cpp

DISTFILES += \
    3DCubePlugin.json

FORMS += \
    mainwindow.ui \
    ../SpectrPlotter/PlotterWindow.ui


SUBDIRS += \
    ../SpectrPlotter/SpectrPlotter.pro

win32: LIBS += -L$$PWD/E:/Qt5.5/Tools/mingw492_32/i686-w64-mingw32/lib/ -lglu32

INCLUDEPATH += $$PWD/E:/Qt5.5/Tools/mingw492_32/i686-w64-mingw32/include
DEPENDPATH += $$PWD/E:/Qt5.5/Tools/mingw492_32/i686-w64-mingw32/include

win32: LIBS += -L$$PWD/E:/Qt5.5/Tools/mingw492_32/i686-w64-mingw32/lib/ -lopengl32

RESOURCES += \
    icons3Dcube.qrc \
    splash.qrc

