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
    ../Library/Interfaces/ProcessingPluginInterface.h \
    ../Library/HyperCube.h \
    ../Library/Types.h \
    ../Library/GenericExc.h \
    GLWidget.h \
    mainwindow.h \
    ui_MainWindow3DCube.h \
    ../Library/QCustomPlot.h \
    ContrastWindow.h
    ../Library/Attributes/Attributes.h


SOURCES       = \
    CubePlugin.cpp \
    ../Library/HyperCube.cpp \
    ../Library/GenericExc.cpp \
    GLWidget.cpp \
    mainwindow.cpp \
    ../Library/QCustomPlot.cpp \
    ContrastWindow.cpp
    ../Library/Attributes/Attributes.cpp


DISTFILES += \
    3DCubePlugin.json

FORMS += \
    mainwindow.ui \
    ../SpectrPlotter/PlotterWindow.ui \
    ../2DHyperCube/Main2DWindow.ui \
    ContrastWindow.ui


SUBDIRS += \
    ../SpectrPlotter/SpectrPlotter.pro


win32: LIBS += -L$$PWD/E:/Qt5.5/Tools/mingw492_32/i686-w64-mingw32/lib/ -lglu32

INCLUDEPATH += $$PWD/E:/Qt5.5/Tools/mingw492_32/i686-w64-mingw32/include
DEPENDPATH += $$PWD/E:/Qt5.5/Tools/mingw492_32/i686-w64-mingw32/include

win32: LIBS += -L$$PWD/E:/Qt5.5/Tools/mingw492_32/i686-w64-mingw32/lib/ -lopengl32

RESOURCES += \
    icons3Dcube.qrc \
    splash.qrc




#win32: LIBS += -L$$PWD/../../Debug/2DHyperCube/debug/ -l2DHyperCube

#INCLUDEPATH += $$PWD/../../Debug/2DHyperCube/debug
#DEPENDPATH += $$PWD/../../Debug/2DHyperCube/debug
