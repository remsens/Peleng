HEADERS       = \
    GetHyperCube.h \
    MainWindow.h \
    GLWidget.h \
    ui_MainWindow3DCube.h \
    ../../Library/GenericExc.h \
    ../../Library/HyperCube.h \
    ../../Library/Types.h \
    ../../Library/QCustomPlot.h \
    ../../SpectrPlotter.workspace/SpectrPlotter/PlotterWindow.h
SOURCES       = \
    GetHyperCube.cpp \
    GLWidget.cpp \
    Main.cpp \
    MainWindow.cpp \
    ../../Library/GenericExc.cpp \
    ../../Library/HyperCube.cpp \
    ../../Library/QCustomPlot.cpp \
    ../../SpectrPlotter.workspace/SpectrPlotter/PlotterWindow.cpp

INCLUDEPATH += $$PWD/../../Library\
                $$PWD/../../SpectrPlotter.workspace/SpectrPlotter\
DEPENDPATH += $$PWD/../../Library
                 $$PWD/../../SpectrPlotter.workspace/SpectrPlotter\

QT           += widgets
QT           += core gui printsupport

FORMS += \
    mainwindow.ui \
    ../../SpectrPlotter.workspace/SpectrPlotter/PlotterWindow.ui


CONFIG += c++11

SUBDIRS += \
    ../../SpectrPlotter.workspace/SpectrPlotter/SpectrPlotter.pro

