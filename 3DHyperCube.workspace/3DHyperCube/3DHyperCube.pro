HEADERS       = \
    ui_mainwindow.h \
    GetHyperCube.h \
    MainWindow.h \
    QCustomPlot.h \
    GLWidget.h \
    ui_MainWindow3DCube.h \
    ../../Library/GenericExc.h \
    ../../Library/HyperCube.h \
    ../../Library/Types.h
SOURCES       = \
    GetHyperCube.cpp \
    GLWidget.cpp \
    Main.cpp \
    MainWindow.cpp \
    QCustomPlot.cpp \
    ../../Library/GenericExc.cpp \
    ../../Library/HyperCube.cpp

INCLUDEPATH += $$PWD/../../Library
DEPENDPATH += $$PWD/../../Library


QT           += widgets
QT           += core gui printsupport

FORMS += \
    mainwindow.ui


CONFIG += c++11
