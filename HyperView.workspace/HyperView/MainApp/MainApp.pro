#-------------------------------------------------
#
# Project created by QtCreator 2015-08-18T17:17:30
#
#-------------------------------------------------

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MainApp
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    QCustomPlot/qcustomplot.cpp \
    ../../../Library/GenericExc.cpp \
    ../../../Library/HyperCube.cpp

HEADERS  += mainwindow.h \
    Interfaces/filereadinterface.h \
    QCustomPlot/qcustomplot.h \
    ../../../Library/Types.h \
    ../../../Library/GenericExc.h \
    ../../../Library/HyperCube.h


INCLUDEPATH += $$PWD/../../../Library
DEPENDPATH += $$PWD/../../../Library

FORMS    += mainwindow.ui

RESOURCES += \
    icons.qrc
