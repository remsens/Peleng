HEADERS       = \
    GetHyperCube.h \
    MainWindow.h \
    GLWidget.h \
    ui_MainWindow3DCube.h \
    ../../Library/GenericExc.h \
    ../../Library/HyperCube.h \
    ../../Library/Types.h \
    ../../Library/QCustomPlot.h
SOURCES       = \
    GetHyperCube.cpp \
    GLWidget.cpp \
    Main.cpp \
    MainWindow.cpp \
    ../../Library/GenericExc.cpp \
    ../../Library/HyperCube.cpp \
    ../../Library/QCustomPlot.cpp

INCLUDEPATH += $$PWD/../../Library
DEPENDPATH += $$PWD/../../Library


QT           += widgets
QT           += core gui printsupport

FORMS += \
    mainwindow.ui


CONFIG += c++11
